#include "sim/character_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>
#include <utility>

#include "sim/hundredths.hpp"
#include "sim/opinion.hpp"
#include "sim/personal_opinion.hpp"
#include "sim/vector_growth.hpp"

namespace sim {

namespace {
constexpr std::size_t MIN_CHARACTER_CAPACITY = 16;
constexpr std::size_t MIN_DEAD_CAPACITY = 16;
constexpr std::size_t MIN_SLOT_CAPACITY = 16;
constexpr std::size_t MIN_SCRATCH_CAPACITY = 128;
} // namespace

CharacterId CharacterRegistry::create(NameId name, Gender gender, Date birth, const CharacterInit& init) {
    // Slot values use the top bit as the dead flag, so both living and dead indices stay below it.
    if (slots_.size() >= DEAD_FLAG - 1) {
        return CharacterId{};
    }
    // Reserve in every container before the first write, so bad_alloc creates nothing.
    detail::reserve_one_more(living_, MIN_CHARACTER_CAPACITY);
    detail::reserve_one_more(slots_, MIN_SLOT_CAPACITY);
    detail::reserve_one_more(holders_, MIN_SLOT_CAPACITY);
    relations_.reserve_node();

    const CharacterId id{static_cast<std::uint32_t>(slots_.size() + 1)};
    living_.emplace_back(CharacterKey{}, id, name, gender, birth, init);
    slots_.push_back(static_cast<std::uint32_t>(living_.size() - 1));
    holders_.push_back(0);
    relations_.add_node();
    assert(slots_.size() == relations_.node_count() && holders_.size() == slots_.size());
    return id;
}

// ---- memory -------------------------------------------------------------------------------

bool CharacterRegistry::references_except(const Character& holder, CharacterId x,
                                          ReferenceKind except) const noexcept {
    return (except != ReferenceKind::LongEntry && long_opinion(holder, x) != 0)
        || (except != ReferenceKind::Modifier && has_modifier(holder, x))
        || (except != ReferenceKind::Edge && relations_.has_edge(holder.id(), x));
}

bool CharacterRegistry::reference_flipped(const Character& holder, CharacterId x, ReferenceKind kind,
                                          bool now_present) noexcept {
    if (references_except(holder, x, kind)) {
        return false; // another kind keeps "holder references x" unchanged
    }
    std::uint32_t& count = holders_[x.value - 1];
    if (now_present) {
        ++count;
        return false;
    }
    assert(count > 0);
    --count;
    return count == 0 && is_dead(x);
}

void CharacterRegistry::forget_in_order(CharacterId first, CharacterId second) noexcept {
    if (second.valid() && (!first.valid() || second < first)) {
        std::swap(first, second);
    }
    for (const CharacterId id : {first, second}) {
        if (id.valid()) {
            forget(id);
        }
    }
}

void CharacterRegistry::forget(CharacterId id) noexcept {
    assert(exists(id) && is_dead(id) && holders_[id.value - 1] == 0);
    relations_.forget_node(id);
    const std::uint32_t index = slots_[id.value - 1] & ~DEAD_FLAG;
    const std::size_t last = dead_.size() - 1;
    if (index != last) {
        dead_[index] = dead_[last];
        slots_[dead_[index].id.value - 1] = DEAD_FLAG | index; // the moved record names its slot
    }
    dead_.pop_back();
    slots_[id.value - 1] = FORGOTTEN;
}

void CharacterRegistry::reserve_scratch(std::size_t count) {
    if (scratch_.capacity() < count) {
        scratch_.reserve(std::max({count, 2 * scratch_.capacity(), MIN_SCRATCH_CAPACITY}));
    }
}

// ---- kill ---------------------------------------------------------------------------------

KillResult CharacterRegistry::kill(CharacterId id, Date death, const WorldContext& context) {
    if (!id.valid()) {
        return {};
    }
    if (!exists(id) || is_dead(id)) {
        return {.result = EditResult::NotFound};
    }
    const std::uint32_t slot = slots_[id.value - 1];
    if (death < living_[slot].birth()) {
        return {};
    }
    // Reserve everything a kill grows: one dead record, and scratch for the living characters
    // referencing the deceased (holders), its own distinct targets and itself. Nothing below
    // allocates.
    const std::uint32_t held = holders_[id.value - 1];
    const std::size_t own_references = living_[slot].long_people().size() + living_[slot].modifiers().size()
                                     + relations_.relations(id).size();
    detail::reserve_one_more(dead_, MIN_DEAD_CAPACITY);
    reserve_scratch(held + own_references + 1);

    const Character& victim = living_[slot];
    const std::int64_t fame = static_cast<std::int64_t>(held)
                            + static_cast<std::int64_t>(context.lifecycle.fame_per_reputation)
                                  * std::abs(victim.reputation());
    const DeadRecord record{.id = id,
                            .name = victim.name(),
                            .birth = victim.birth(),
                            .death = death,
                            .main_community = victim.main_community().value_or(CommunityId{}),
                            .reputation = static_cast<std::int8_t>(victim.reputation()),
                            .gender = victim.gender(),
                            .fame = static_cast<std::uint16_t>(std::min<std::int64_t>(fame, 0xFFFF))};

    // Before any write: the living characters that reference the deceased, in id order. The
    // scan stops once all `held` of them are found and is skipped when nobody holds it.
    scratch_.clear();
    for (std::size_t i = 0; scratch_.size() < held && i < living_.size(); ++i) {
        const Character& c = living_[i];
        if (c.id() != id
            && (long_opinion(c, id) != 0 || has_modifier(c, id) || relations_.has_edge(c.id(), id))) {
            scratch_.push_back(c.id());
        }
    }
    assert(scratch_.size() == held);
    const std::size_t holder_end = scratch_.size();

    // The deceased's references stop counting: its distinct targets, sorted by id, after the holders.
    for (const LongOpinion& e : victim.long_people()) {
        scratch_.push_back(CharacterId{e.target});
    }
    for (const OpinionModifier& m : victim.modifiers()) {
        if (m.domain == ModifierDomain::Person) {
            scratch_.push_back(CharacterId{m.target});
        }
    }
    for (const RelationEdge& e : relations_.relations(id)) {
        scratch_.push_back(e.other);
    }
    const auto targets_begin = scratch_.begin() + static_cast<std::ptrdiff_t>(holder_end);
    std::sort(targets_begin, scratch_.end());
    scratch_.erase(std::unique(targets_begin, scratch_.end()), scratch_.end());
    for (auto it = targets_begin; it != scratch_.end(); ++it) {
        assert(holders_[it->value - 1] > 0);
        --holders_[it->value - 1];
    }

    // Relations: non-surviving types through the deceased's own edge list (paired links on both
    // sides). The dead hold no one-way edges; kill never adds any, so checking the dead
    // neighbours before the removals is enough (the property test checks every dead node).
#ifndef NDEBUG
    for (const RelationEdge& e : relations_.relations(id)) {
        assert(!is_dead(e.other) || !relations_.has_one_way_edges(e.other));
    }
#endif
    relations_.remove_non_surviving(id);

    // Only the characters that referenced the deceased: rebase their long-term entries about
    // it so opinions don't jump, clear their one-way edges towards it, and recount which of
    // them still reference it (every removal above counted once).
    std::uint32_t remaining = 0;
    for (std::size_t i = 0; i < holder_end; ++i) {
        Character& holder = living_[slots_[scratch_[i].value - 1]];
        bool has_long = long_opinion(holder, id) != 0;
        if (has_long) {
            const double before = weak_opinion(holder, victim, context.stances, context.opinion, context.seed);
            const double after = weak_opinion(holder, record, context.stances, context.opinion, context.seed);
            const auto delta =
                static_cast<int>(detail::clamp_round(before - after, -LONG_DELTA_MAX, LONG_DELTA_MAX));
            const LongOpinionResult<CharacterId> result = holder.add_long_opinion(CharacterKey{}, id, delta);
            assert(result.outcome == LongOpinionOutcome::Updated || result.outcome == LongOpinionOutcome::Removed
                   || result.outcome == LongOpinionOutcome::Unchanged);
            has_long = result.outcome != LongOpinionOutcome::Removed;
        }
        const bool edge = relations_.clear_one_way(holder.id(), id);
        remaining += edge || has_long || has_modifier(holder, id) ? 1u : 0u;
    }
    assert(!relations_.has_one_way_edges(id));
    holders_[id.value - 1] = remaining;

    // The Character (with its own opinions and modifiers) is replaced by the record; later
    // living slots move down by one. Characters are not assignable, so each slot is destroyed
    // and copy-constructed in place.
    for (std::size_t i = slot; i + 1 < living_.size(); ++i) {
        std::destroy_at(&living_[i]);
        std::construct_at(&living_[i], living_[i + 1]);
        slots_[living_[i].id().value - 1] = static_cast<std::uint32_t>(i);
    }
    living_.pop_back();
    dead_.push_back(record);
    slots_[id.value - 1] = DEAD_FLAG | static_cast<std::uint32_t>(dead_.size() - 1);

    // Forget, in id order, the deceased if nobody holds it and dead targets only it held.
    scratch_.erase(scratch_.begin(), scratch_.begin() + static_cast<std::ptrdiff_t>(holder_end)); // no allocation
    scratch_.insert(std::upper_bound(scratch_.begin(), scratch_.end(), id), id);                   // capacity reserved
    for (const CharacterId candidate : scratch_) {
        if (is_dead(candidate) && holders_[candidate.value - 1] == 0) {
            forget(candidate);
        }
    }
    return {.result = EditResult::Ok,
            .fame = record.fame,
            .legendary = record.fame >= context.lifecycle.legendary_fame};
}

Character* CharacterRegistry::find(CharacterId id) noexcept {
    if (!exists(id) || is_dead(id)) {
        return nullptr;
    }
    return &living_[slots_[id.value - 1]];
}

const Character* CharacterRegistry::find(CharacterId id) const noexcept {
    if (!exists(id) || is_dead(id)) {
        return nullptr;
    }
    return &living_[slots_[id.value - 1]];
}

const DeadRecord* CharacterRegistry::find_dead(CharacterId id) const noexcept {
    if (!exists(id) || !is_dead(id)) {
        return nullptr;
    }
    return &dead_[slots_[id.value - 1] & ~DEAD_FLAG];
}

// ---- relations ----------------------------------------------------------------------

EditResult CharacterRegistry::check_relation_edit(CharacterId a, RelationType type, CharacterId b,
                                                  RelationKind kind) const noexcept {
    const std::optional<RelationInfo> info = relation_info(type);
    if (!a.valid() || !b.valid() || a == b || !info || info->kind != kind) {
        return EditResult::Invalid;
    }
    if (!exists(a) || !exists(b)) {
        return EditResult::NotFound;
    }
    const bool dead_allowed = kind == RelationKind::Paired && survives_death(type);
    if (!dead_allowed && (is_dead(a) || is_dead(b))) {
        return EditResult::Invalid;
    }
    return EditResult::Ok;
}

EditResult CharacterRegistry::set_relation(CharacterId a, RelationType type, CharacterId b) {
    const EditResult check = check_relation_edit(a, type, b, RelationKind::OneWay);
    if (check != EditResult::Ok) {
        return check;
    }
    RelationGraph::EdgeFlips flips;
    const EditResult result = relations_.set_relation(a, type, b, flips);
    if (flips.forward) {
        const bool forget_b = reference_flipped(*find(a), b, ReferenceKind::Edge, true);
        assert(!forget_b); // adding never forgets
        (void)forget_b;
    }
    return result;
}

EditResult CharacterRegistry::clear_relation(CharacterId a, RelationType type, CharacterId b) noexcept {
    const EditResult check = check_relation_edit(a, type, b, RelationKind::OneWay);
    if (check != EditResult::Ok) {
        return check;
    }
    RelationGraph::EdgeFlips flips;
    const EditResult result = relations_.clear_relation(a, type, b, flips);
    if (flips.forward) {
        const bool forget_b = reference_flipped(*find(a), b, ReferenceKind::Edge, false);
        assert(!forget_b); // one-way edits involve the living only
        (void)forget_b;
    }
    return result;
}

EditResult CharacterRegistry::link(CharacterId a, RelationType type, CharacterId b) {
    const EditResult check = check_relation_edit(a, type, b, RelationKind::Paired);
    if (check != EditResult::Ok) {
        return check;
    }
    RelationGraph::EdgeFlips flips;
    const EditResult result = relations_.link(a, type, b, flips);
    // Only living sides hold references; linking only adds them, so nothing is forgotten.
    if (const Character* holder_a = flips.forward ? find(a) : nullptr) {
        (void)reference_flipped(*holder_a, b, ReferenceKind::Edge, true);
    }
    if (const Character* holder_b = flips.backward ? find(b) : nullptr) {
        (void)reference_flipped(*holder_b, a, ReferenceKind::Edge, true);
    }
    return result;
}

EditResult CharacterRegistry::unlink(CharacterId a, RelationType type, CharacterId b) noexcept {
    const EditResult check = check_relation_edit(a, type, b, RelationKind::Paired);
    if (check != EditResult::Ok) {
        return check;
    }
    RelationGraph::EdgeFlips flips;
    const EditResult result = relations_.unlink(a, type, b, flips);
    const Character* holder_a = flips.forward ? find(a) : nullptr;
    const Character* holder_b = flips.backward ? find(b) : nullptr;
    const bool forget_b = holder_a != nullptr && reference_flipped(*holder_a, b, ReferenceKind::Edge, false);
    const bool forget_a = holder_b != nullptr && reference_flipped(*holder_b, a, ReferenceKind::Edge, false);
    forget_in_order(forget_a ? a : CharacterId{}, forget_b ? b : CharacterId{});
    return result;
}

// ---- personal opinions ------------------------------------------------------------------

EditResult CharacterRegistry::add_person_modifier(CharacterId a, CharacterId b, ModifierId modifier,
                                                  int effect) noexcept {
    if (!a.valid() || !b.valid() || a == b || !modifier.valid()) {
        return EditResult::Invalid;
    }
    if (!exists(a) || !exists(b)) {
        return EditResult::NotFound;
    }
    Character* source = find(a);
    if (source == nullptr) {
        return EditResult::Invalid; // a dead holder
    }
    bool first_on_target = false;
    const EditResult result = source->add_modifier(CharacterKey{}, b, modifier, effect, first_on_target);
    if (first_on_target) {
        (void)reference_flipped(*source, b, ReferenceKind::Modifier, true); // adding never forgets
    }
    return result;
}

EditResult CharacterRegistry::add_target_modifier(CharacterId a, TargetId target, ModifierId modifier,
                                                  int effect) noexcept {
    if (!a.valid() || !target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    if (!exists(a)) {
        return EditResult::NotFound;
    }
    Character* source = find(a);
    if (source == nullptr) {
        return EditResult::Invalid; // a dead holder
    }
    return source->add_modifier(CharacterKey{}, target, modifier, effect); // not a person reference
}

EditResult CharacterRegistry::remove_modifier(CharacterId a, CharacterId b, ModifierId modifier) noexcept {
    if (!a.valid() || !b.valid() || a == b || !modifier.valid()) {
        return EditResult::Invalid;
    }
    if (!exists(a) || !exists(b)) {
        return EditResult::NotFound;
    }
    Character* source = find(a);
    if (source == nullptr) {
        return EditResult::Invalid; // a dead holder
    }
    bool last_on_target = false;
    const EditResult result = source->remove_modifier(CharacterKey{}, b, modifier, last_on_target);
    if (last_on_target && reference_flipped(*source, b, ReferenceKind::Modifier, false)) {
        forget(b);
    }
    return result;
}

EditResult CharacterRegistry::remove_modifier(CharacterId a, TargetId target, ModifierId modifier) noexcept {
    if (!a.valid() || !target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    if (!exists(a)) {
        return EditResult::NotFound;
    }
    Character* source = find(a);
    if (source == nullptr) {
        return EditResult::Invalid; // a dead holder
    }
    return source->remove_modifier(CharacterKey{}, target, modifier);
}

LongOpinionResult<CharacterId> CharacterRegistry::add_person_long(CharacterId a, CharacterId b, int delta) noexcept {
    if (!a.valid() || !b.valid() || a == b) {
        return {LongOpinionOutcome::Invalid, CharacterId{}};
    }
    if (!exists(a) || !exists(b)) {
        return {LongOpinionOutcome::NotFound, CharacterId{}};
    }
    Character* source = find(a);
    if (source == nullptr) {
        return {LongOpinionOutcome::Invalid, CharacterId{}}; // a dead holder
    }
    const LongOpinionResult<CharacterId> result = source->add_long_opinion(CharacterKey{}, b, delta);
    // The entry about b exists afterwards exactly when it was Created (with or without an
    // eviction) and ceased to exist exactly when Removed; other outcomes flip nothing.
    bool forget_b = false;
    switch (result.outcome) {
    case LongOpinionOutcome::Created:
    case LongOpinionOutcome::CreatedWithEviction:
        (void)reference_flipped(*source, b, ReferenceKind::LongEntry, true);
        break;
    case LongOpinionOutcome::Removed:
        forget_b = reference_flipped(*source, b, ReferenceKind::LongEntry, false);
        break;
    case LongOpinionOutcome::Updated:
    case LongOpinionOutcome::Unchanged:
    case LongOpinionOutcome::Dropped:
    case LongOpinionOutcome::Invalid:
    case LongOpinionOutcome::NotFound:
        break;
    }
    const bool forget_evicted = result.evicted.valid()
                             && reference_flipped(*source, result.evicted, ReferenceKind::LongEntry, false);
    assert(!(forget_b && forget_evicted)); // an eviction means b was created
    forget_in_order(forget_b ? b : CharacterId{}, forget_evicted ? result.evicted : CharacterId{});
    return result;
}

LongOpinionResult<TargetId> CharacterRegistry::add_target_long(CharacterId a, TargetId target, int delta) noexcept {
    if (!a.valid() || !target.valid()) {
        return {LongOpinionOutcome::Invalid, TargetId{}};
    }
    if (!exists(a)) {
        return {LongOpinionOutcome::NotFound, TargetId{}};
    }
    Character* source = find(a);
    if (source == nullptr) {
        return {LongOpinionOutcome::Invalid, TargetId{}}; // a dead holder
    }
    return source->add_long_opinion(CharacterKey{}, target, delta); // not a person reference
}

std::size_t CharacterRegistry::maintain() {
    // Reserve scratch for every entry that can be evicted, before the first write.
    std::size_t overflow = 0;
    for (const Character& c : living_) {
        const std::size_t limit = person_limit(c);
        overflow += c.long_people().size() > limit ? c.long_people().size() - limit : 0;
    }
    if (overflow == 0) {
        return 0;
    }
    reserve_scratch(overflow);
    scratch_.clear();

    std::size_t evicted_total = 0;
    for (Character& c : living_) { // id order; the dead hold no opinions
        FixedVector<CharacterId, PERSON_LIMIT_MAX> evicted;
        evicted_total += c.trim_long_opinions(CharacterKey{}, evicted);
        for (const CharacterId target : evicted) {
            if (reference_flipped(c, target, ReferenceKind::LongEntry, false)) {
                scratch_.push_back(target);
            }
        }
    }
    std::sort(scratch_.begin(), scratch_.end()); // each id reaches 0 at most once
    for (const CharacterId id : scratch_) {
        forget(id);
    }
    return evicted_total;
}

std::size_t CharacterRegistry::allocated_bytes() const noexcept {
    return sizeof(*this) + living_.capacity() * sizeof(Character) + dead_record_bytes() + slot_bytes()
         + scratch_.capacity() * sizeof(CharacterId) + relations_.allocated_bytes();
}

} // namespace sim
