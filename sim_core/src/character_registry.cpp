#include "sim/character_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>

#include "sim/hundredths.hpp"
#include "sim/opinion.hpp"
#include "sim/personal_opinion.hpp"
#include "sim/vector_growth.hpp"

namespace sim {

namespace {
constexpr std::size_t MIN_CHARACTER_CAPACITY = 16;
constexpr std::size_t MIN_DEAD_CAPACITY = 16;
constexpr std::size_t MIN_SLOT_CAPACITY = 16;
} // namespace

CharacterId CharacterRegistry::create(NameId name, Gender gender, Date birth, const CharacterInit& init) {
    // Slot values use the top bit as the dead flag, so both living and dead indices stay below it.
    if (slots_.size() >= DEAD_FLAG - 1) {
        return CharacterId{};
    }
    // Reserve in every container before the first write, so bad_alloc creates nothing.
    detail::reserve_one_more(living_, MIN_CHARACTER_CAPACITY);
    detail::reserve_one_more(slots_, MIN_SLOT_CAPACITY);
    relations_.reserve_node();

    const CharacterId id{static_cast<std::uint32_t>(slots_.size() + 1)};
    living_.emplace_back(CharacterKey{}, id, name, gender, birth, init);
    slots_.push_back(static_cast<std::uint32_t>(living_.size() - 1));
    relations_.add_node();
    assert(slots_.size() == relations_.node_count());
    return id;
}

EditResult CharacterRegistry::kill(CharacterId id, Date death, const StanceTable& stances,
                                   const OpinionConfig& config, WorldSeed seed) {
    if (!id.valid()) {
        return EditResult::Invalid;
    }
    if (!exists(id) || is_dead(id)) {
        return EditResult::NotFound;
    }
    const std::uint32_t slot = slots_[id.value - 1];
    if (death < living_[slot].birth()) {
        return EditResult::Invalid;
    }
    // The only container a kill grows. Nothing below allocates.
    detail::reserve_one_more(dead_, MIN_DEAD_CAPACITY);

    const Character& victim = living_[slot];
    const DeadRecord record{.id = id,
                            .name = victim.name(),
                            .birth = victim.birth(),
                            .death = death,
                            .main_community = victim.main_community().value_or(CommunityId{}),
                            .reputation = static_cast<std::int8_t>(victim.reputation()),
                            .gender = victim.gender(),
                            .reserved = 0};

    // 1. Rebase existing long-term entries about the deceased so opinions don't jump.
    for (Character& holder : living_) {
        if (holder.id() == id || long_opinion(holder, id) == 0) {
            continue;
        }
        const double before = weak_opinion(holder, victim, stances, config, seed);
        const double after = weak_opinion(holder, record, stances, config, seed);
        const auto delta = static_cast<int>(detail::clamp_round(before - after, -LONG_DELTA_MAX, LONG_DELTA_MAX));
        const LongOpinionResult<CharacterId> result = holder.add_long_opinion(CharacterKey{}, id, delta);
        assert(result.outcome == LongOpinionOutcome::Updated || result.outcome == LongOpinionOutcome::Removed
               || result.outcome == LongOpinionOutcome::Unchanged);
        (void)result;
    }

    // 2. Relations: non-surviving types on the deceased's own edges (paired on both sides),
    //    then one-way edges of the living towards it. The dead hold no one-way edges; kill
    //    never adds any, so checking the dead neighbours before the removals is enough
    //    (the property test checks every dead node).
#ifndef NDEBUG
    for (const RelationEdge& e : relations_.relations(id)) {
        assert(!is_dead(e.other) || !relations_.has_one_way_edges(e.other));
    }
#endif
    relations_.remove_non_surviving(id);
    for (const Character& holder : living_) {
        if (holder.id() != id) {
            relations_.clear_one_way(holder.id(), id);
        }
    }
    assert(!relations_.has_one_way_edges(id));

    // 3 and 4. The Character (with its own opinions and modifiers) is replaced by the record;
    // later living slots move down by one. Characters are not assignable, so each slot is
    // destroyed and copy-constructed in place.
    for (std::size_t i = slot; i + 1 < living_.size(); ++i) {
        std::destroy_at(&living_[i]);
        std::construct_at(&living_[i], living_[i + 1]);
        slots_[living_[i].id().value - 1] = static_cast<std::uint32_t>(i);
    }
    living_.pop_back();
    dead_.push_back(record);
    slots_[id.value - 1] = DEAD_FLAG | static_cast<std::uint32_t>(dead_.size() - 1);
    return EditResult::Ok;
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
    return check != EditResult::Ok ? check : relations_.set_relation(a, type, b);
}

EditResult CharacterRegistry::clear_relation(CharacterId a, RelationType type, CharacterId b) noexcept {
    const EditResult check = check_relation_edit(a, type, b, RelationKind::OneWay);
    return check != EditResult::Ok ? check : relations_.clear_relation(a, type, b);
}

EditResult CharacterRegistry::link(CharacterId a, RelationType type, CharacterId b) {
    const EditResult check = check_relation_edit(a, type, b, RelationKind::Paired);
    return check != EditResult::Ok ? check : relations_.link(a, type, b);
}

EditResult CharacterRegistry::unlink(CharacterId a, RelationType type, CharacterId b) noexcept {
    const EditResult check = check_relation_edit(a, type, b, RelationKind::Paired);
    return check != EditResult::Ok ? check : relations_.unlink(a, type, b);
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
    return source->add_modifier(CharacterKey{}, b, modifier, effect);
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
    return source->add_modifier(CharacterKey{}, target, modifier, effect);
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
    return source->remove_modifier(CharacterKey{}, b, modifier);
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
    return source->add_long_opinion(CharacterKey{}, b, delta);
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
    return source->add_long_opinion(CharacterKey{}, target, delta);
}

std::size_t CharacterRegistry::maintain() noexcept {
    std::size_t evicted = 0;
    for (Character& c : living_) { // id order; the dead hold no opinions
        evicted += c.trim_long_opinions(CharacterKey{});
    }
    return evicted;
}

std::size_t CharacterRegistry::allocated_bytes() const noexcept {
    return sizeof(*this) + living_.capacity() * sizeof(Character) + dead_record_bytes() + slot_bytes()
         + relations_.allocated_bytes();
}

} // namespace sim
