#include <doctest.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/dead_record.hpp"
#include "sim/lifecycle_config.hpp"
#include "sim/personal_opinion.hpp"
#include "sim/relations.hpp"
#include "sim/stance_table.hpp"
#include "sim/world_context.hpp"

using namespace sim;

namespace {

using O = LongOpinionOutcome;
using R = RelationType;

const OpinionConfig CONFIG{};
const LifecycleConfig LIFECYCLE{};
const WorldSeed SEED{0x3E3Eu};

TargetId topic(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

CharacterRegistry make_registry(std::uint32_t count, const CharacterInit& init = {}) {
    CharacterRegistry registry;
    for (std::uint32_t i = 1; i <= count; ++i) {
        (void)registry.create(NameId{i}, Gender::Female, Date{0}, init);
    }
    return registry;
}

// Kills with an empty stance table and the default configs.
KillResult kill(CharacterRegistry& r, CharacterId id, const LifecycleConfig& lifecycle = LIFECYCLE) {
    const StanceTable stances;
    return r.kill(id, Date{10}, WorldContext{stances, CONFIG, lifecycle, SEED});
}

} // namespace

// ---- layout --------------------------------------------------------------------------------

static_assert(sizeof(DeadRecord) == 24 && offsetof(DeadRecord, fame) == 22);
static_assert(std::is_same_v<decltype(DeadRecord::fame), std::uint16_t>);
static_assert(std::has_unique_object_representations_v<DeadRecord>);

// ---- holders -------------------------------------------------------------------------------

TEST_CASE("holders: long entries, modifiers and edges; several references from one holder count once") {
    CharacterRegistry r = make_registry(4, CharacterInit{.extraversion = 100});
    const CharacterId a{1}, b{2}, c{3}, x{4};
    CHECK(r.holders(x) == 0);

    REQUIRE(r.add_long_opinion(a, x, 30).outcome == O::Created);
    CHECK(r.holders(x) == 1);
    REQUIRE(r.add_modifier(a, x, ModifierId{1}, 0) == EditResult::Ok); // a zero effect is a reference too
    REQUIRE(r.add_modifier(a, x, ModifierId{2}, 5) == EditResult::Ok);
    REQUIRE(r.set_relation(a, R::Friend, x) == EditResult::Ok);
    CHECK(r.holders(x) == 1); // still one distinct holder
    REQUIRE(r.add_modifier(b, x, ModifierId{1}, 5) == EditResult::Ok);
    CHECK(r.holders(x) == 2);
    REQUIRE(r.link(c, R::Employee, x) == EditResult::Ok); // both edges: c holds x, x holds c
    CHECK(r.holders(x) == 3);
    CHECK(r.holders(c) == 1);

    // Removing some of a's references keeps a counted until the last one goes.
    REQUIRE(r.add_long_opinion(a, x, -30).outcome == O::Removed);
    REQUIRE(r.remove_modifier(a, x, ModifierId{1}) == EditResult::Ok);
    REQUIRE(r.remove_modifier(a, x, ModifierId{2}) == EditResult::Ok);
    CHECK(r.holders(x) == 3);
    REQUIRE(r.clear_relation(a, R::Friend, x) == EditResult::Ok);
    CHECK(r.holders(x) == 2);
    REQUIRE(r.unlink(c, R::Employee, x) == EditResult::Ok);
    CHECK(r.holders(x) == 1);
    CHECK(r.holders(c) == 0);
    REQUIRE(r.remove_modifier(b, x, ModifierId{1}) == EditResult::Ok);
    CHECK(r.holders(x) == 0);

    // Target-domain opinions are not references to characters.
    REQUIRE(r.add_long_opinion(a, topic(4), 50).outcome == O::Created);
    REQUIRE(r.add_modifier(a, topic(4), ModifierId{1}, 9) == EditResult::Ok);
    CHECK(r.holders(x) == 0);
    // Failed edits change nothing.
    CHECK(r.add_modifier(a, CharacterId{9}, ModifierId{1}, 9) == EditResult::NotFound);
    CHECK(r.add_long_opinion(a, a, 5).outcome == O::Invalid);
    CHECK(r.holders(CharacterId{}) == 0);
    CHECK(r.holders(CharacterId{9}) == 0);
}

TEST_CASE("holders: eviction and maintain trims release references") {
    CharacterRegistry r = make_registry(20, CharacterInit{.extraversion = -100}); // limit 8
    const CharacterId a{1};
    for (std::uint32_t k = 2; k <= 9; ++k) {
        REQUIRE(r.add_long_opinion(a, CharacterId{k}, static_cast<int>(k) * 10).outcome == O::Created);
    }
    CHECK(r.holders(CharacterId{2}) == 1);
    const LongOpinionResult<CharacterId> evicting = r.add_long_opinion(a, CharacterId{10}, 150);
    REQUIRE(evicting.outcome == O::CreatedWithEviction);
    REQUIRE(evicting.evicted == CharacterId{2});
    CHECK(r.holders(CharacterId{2}) == 0);
    CHECK(r.holders(CharacterId{10}) == 1);

    // An evicted target that is still referenced otherwise keeps its holder.
    REQUIRE(r.add_modifier(a, CharacterId{3}, ModifierId{1}, 1) == EditResult::Ok);
    REQUIRE(r.add_long_opinion(a, CharacterId{11}, 160).outcome == O::CreatedWithEviction); // evicts 3
    CHECK(r.holders(CharacterId{3}) == 1);

    // maintain trims: raise the limit, add weak entries, then lower it again.
    r.find(a)->set_extraversion(100);
    for (std::uint32_t k = 12; k <= 20; ++k) {
        REQUIRE(r.add_long_opinion(a, CharacterId{k}, 5).outcome == O::Created);
    }
    CHECK(r.holders(CharacterId{12}) == 1);
    r.find(a)->set_extraversion(-100);
    CHECK(r.maintain() == 9);
    for (std::uint32_t k = 12; k <= 20; ++k) {
        CHECK(r.holders(CharacterId{k}) == 0);
    }
}

TEST_CASE("holders: the dead hold no references; a holder's death releases its targets") {
    CharacterRegistry r = make_registry(4, CharacterInit{.extraversion = 100});
    const CharacterId parent{1}, child{2}, friend_{3}, other{4};
    REQUIRE(r.link(parent, R::Child, child) == EditResult::Ok);
    REQUIRE(r.add_long_opinion(parent, friend_, 40).outcome == O::Created);
    REQUIRE(r.add_modifier(parent, other, ModifierId{1}, 3) == EditResult::Ok);
    REQUIRE(r.add_modifier(child, other, ModifierId{1}, 3) == EditResult::Ok);
    CHECK(r.holders(child) == 1);
    CHECK(r.holders(parent) == 1);
    CHECK(r.holders(friend_) == 1);
    CHECK(r.holders(other) == 2);

    REQUIRE(kill(r, parent).result == EditResult::Ok);
    CHECK(r.holders(child) == 0);   // the dead parent's edge doesn't count
    CHECK(r.holders(friend_) == 0); // its long entry is gone
    CHECK(r.holders(other) == 1);   // its modifier is gone
    CHECK(r.holders(parent) == 1);  // the child's edge to it
    CHECK(r.find_dead(parent) != nullptr);

    // A posthumous link counts only on the living side.
    REQUIRE(r.link(other, R::Parent, parent) == EditResult::Ok); // parent becomes other's parent
    CHECK(r.holders(parent) == 2);
    CHECK(r.holders(other) == 1);
}

// ---- forgetting ----------------------------------------------------------------------------

TEST_CASE("forgetting: immediately at death when nobody remembers the deceased") {
    CharacterRegistry r = make_registry(3);
    const CharacterId lonely{1}, b{2}, c{3};
    REQUIRE(r.add_modifier(lonely, b, ModifierId{1}, 1) == EditResult::Ok); // lonely holds b, not the reverse
    const KillResult result = kill(r, lonely);
    CHECK(result.result == EditResult::Ok);
    CHECK(result.fame == 0);
    CHECK(!r.exists(lonely));
    CHECK(r.find_dead(lonely) == nullptr);
    CHECK(r.dead_count() == 0);
    CHECK(r.holders(b) == 0);
    CHECK(r.characters().size() == 2);
    CHECK(r.find(c) != nullptr);
}

TEST_CASE("forgetting: deferred until the last living child dies; siblings work while a child lives") {
    CharacterRegistry r = make_registry(4);
    const CharacterId parent{1}, x{2}, y{3}, outsider{4};
    REQUIRE(r.link(parent, R::Child, x) == EditResult::Ok);
    REQUIRE(r.link(parent, R::Child, y) == EditResult::Ok);
    REQUIRE(kill(r, parent).result == EditResult::Ok);
    CHECK(r.holders(parent) == 2);
    CHECK(r.siblings(x) == std::vector<CharacterId>{y});

    REQUIRE(kill(r, x).result == EditResult::Ok);
    // x is held by nobody living (y references only the parent), so x is forgotten at once;
    // the parent is still remembered by y.
    CHECK(!r.exists(x));
    CHECK(r.exists(parent));
    CHECK(r.holders(parent) == 1);
    CHECK(r.children(parent) == std::vector<CharacterId>{y});
    CHECK(r.siblings(y).empty()); // x is forgotten
    CHECK(r.parents(y)[0] == parent);

    REQUIRE(kill(r, y).result == EditResult::Ok);
    CHECK(!r.exists(y));
    CHECK(!r.exists(parent)); // its last living child died
    CHECK(r.dead_count() == 0);
    CHECK(r.find(outsider) != nullptr);
}

TEST_CASE("forgetting: triggered by removing the last long entry, the last modifier and the last edge") {
    SUBCASE("long entry") {
        CharacterRegistry r = make_registry(2);
        REQUIRE(r.add_long_opinion(CharacterId{1}, CharacterId{2}, 25).outcome == O::Created);
        REQUIRE(kill(r, CharacterId{2}).result == EditResult::Ok);
        REQUIRE(r.exists(CharacterId{2}));
        const int value = long_opinion(*r.find(CharacterId{1}), CharacterId{2}); // rebased at death
        REQUIRE(std::abs(value) >= 2);
        const int step = value > 0 ? 1 : -1;
        CHECK(r.add_long_opinion(CharacterId{1}, CharacterId{2}, -(value - step)).outcome == O::Updated);
        CHECK(r.exists(CharacterId{2}));
        CHECK(r.add_long_opinion(CharacterId{1}, CharacterId{2}, -step).outcome == O::Removed);
        CHECK(!r.exists(CharacterId{2}));
    }
    SUBCASE("modifier") {
        CharacterRegistry r = make_registry(2);
        REQUIRE(r.add_modifier(CharacterId{1}, CharacterId{2}, ModifierId{1}, -40) == EditResult::Ok);
        REQUIRE(r.add_modifier(CharacterId{1}, CharacterId{2}, ModifierId{2}, 0) == EditResult::Ok);
        REQUIRE(kill(r, CharacterId{2}).result == EditResult::Ok);
        CHECK(r.remove_modifier(CharacterId{1}, CharacterId{2}, ModifierId{2}) == EditResult::Ok);
        CHECK(r.exists(CharacterId{2}));
        CHECK(r.remove_modifier(CharacterId{1}, CharacterId{2}, ModifierId{1}) == EditResult::Ok);
        CHECK(!r.exists(CharacterId{2}));
    }
    SUBCASE("edge") {
        CharacterRegistry r = make_registry(2);
        REQUIRE(r.link(CharacterId{1}, R::Spouse, CharacterId{2}) == EditResult::Ok);
        REQUIRE(kill(r, CharacterId{2}).result == EditResult::Ok);
        CHECK(r.holders(CharacterId{2}) == 1);
        CHECK(r.unlink(CharacterId{2}, R::Spouse, CharacterId{1}) == EditResult::Ok); // from the dead side
        CHECK(!r.exists(CharacterId{2}));
        CHECK(r.relations(CharacterId{1}).empty());
    }
    SUBCASE("eviction") {
        CharacterRegistry r = make_registry(12, CharacterInit{.extraversion = -100}); // limit 8
        const CharacterId a{1};
        for (std::uint32_t k = 2; k <= 9; ++k) {
            REQUIRE(r.add_long_opinion(a, CharacterId{k}, k == 5 ? 3 : 90).outcome == O::Created);
        }
        REQUIRE(kill(r, CharacterId{5}).result == EditResult::Ok);
        REQUIRE(r.exists(CharacterId{5}));
        const LongOpinionResult<CharacterId> result = r.add_long_opinion(a, CharacterId{10}, 50);
        CHECK(result.outcome == O::CreatedWithEviction);
        CHECK(result.evicted == CharacterId{5}); // typed id of a now forgotten character
        CHECK(!r.exists(CharacterId{5}));
    }
    SUBCASE("maintain trim, several at once") {
        CharacterRegistry r = make_registry(12, CharacterInit{.extraversion = 100});
        const CharacterId a{1};
        for (std::uint32_t k = 2; k <= 12; ++k) {
            // 2..4 stay weak even after the rebase at their death (at most +-40 here).
            REQUIRE(r.add_long_opinion(a, CharacterId{k}, k <= 4 ? static_cast<int>(k) : 150).outcome == O::Created);
        }
        for (const std::uint32_t k : {3u, 2u, 4u}) {
            REQUIRE(kill(r, CharacterId{k}).result == EditResult::Ok);
        }
        CHECK(r.dead_count() == 3);
        r.find(a)->set_extraversion(-100); // limit 8: the three weakest (2, 3, 4) go
        CHECK(r.maintain() == 3);
        CHECK(r.dead_count() == 0);
        for (const std::uint32_t k : {2u, 3u, 4u}) {
            CHECK(!r.exists(CharacterId{k}));
        }
        CHECK(r.maintain() == 0);
    }
}

TEST_CASE("forgetting: the record, its edges on both sides and its id are gone; edits return NotFound") {
    CharacterRegistry r = make_registry(5);
    const CharacterId keeper{1}, m{2}, n{3}, child{4}, other{5};
    // m and n are spouses; child is n's child; keeper remembers only m.
    REQUIRE(r.link(m, R::Spouse, n) == EditResult::Ok);
    REQUIRE(r.link(n, R::Child, child) == EditResult::Ok);
    REQUIRE(r.add_modifier(keeper, m, ModifierId{1}, 7) == EditResult::Ok);
    REQUIRE(kill(r, m).result == EditResult::Ok);
    REQUIRE(kill(r, n).result == EditResult::Ok); // n is held by its child
    CHECK(r.has_relation(n, R::Spouse, m));
    CHECK(r.dead_count() == 2);

    REQUIRE(r.remove_modifier(keeper, m, ModifierId{1}) == EditResult::Ok); // m forgotten
    CHECK(!r.exists(m));
    CHECK(r.find_dead(m) == nullptr);
    CHECK(r.find(m) == nullptr);
    CHECK(r.holders(m) == 0);
    CHECK(r.dead_count() == 1);
    CHECK(r.find_dead(n)->id == n); // the swapped record still resolves
    // Edges with another dead character are removed on both sides.
    CHECK(r.relations(m).empty());
    CHECK(!r.has_relation(n, R::Spouse, m));
    CHECK(r.relations(n).size() == 1); // the child
    CHECK(r.parents(m).empty());
    CHECK(r.children(m).empty());
    CHECK(r.siblings(m).empty());
    CHECK(r.shared_parents(m, n) == 0);
    CHECK(r.holders(n) == 1);

    // Edits: NotFound, as for an id never created.
    CHECK(kill(r, m).result == EditResult::NotFound);
    CHECK(r.link(m, R::Spouse, other) == EditResult::NotFound);
    CHECK(r.unlink(m, R::Spouse, n) == EditResult::NotFound);
    CHECK(r.set_relation(other, R::Friend, m) == EditResult::NotFound);
    CHECK(r.clear_relation(other, R::Friend, m) == EditResult::NotFound);
    CHECK(r.add_modifier(other, m, ModifierId{1}, 1) == EditResult::NotFound);
    CHECK(r.remove_modifier(other, m, ModifierId{1}) == EditResult::NotFound);
    CHECK(r.add_long_opinion(other, m, 5).outcome == O::NotFound);
    // Ids are never reused.
    CHECK(r.create(NameId{6}, Gender::Male, Date{0}, CharacterInit{}) == CharacterId{6});
    CHECK(!r.exists(m));
}

// ---- fame ----------------------------------------------------------------------------------

TEST_CASE("fame: holders before the kill plus |reputation|; stored and returned; legendary threshold") {
    CharacterRegistry r = make_registry(6);
    const CharacterId hero{1}, villain{2}, nobody{3};
    for (const CharacterId holder : {CharacterId{4}, CharacterId{5}, CharacterId{6}}) {
        REQUIRE(r.add_modifier(holder, hero, ModifierId{1}, 10) == EditResult::Ok);
        REQUIRE(r.add_modifier(holder, villain, ModifierId{1}, -10) == EditResult::Ok);
    }
    r.find(hero)->set_reputation(97);
    r.find(villain)->set_reputation(-96);
    REQUIRE(r.link(hero, R::Employee, villain) == EditResult::Ok);
    CHECK(r.holders(hero) == 4);    // three modifiers and the villain's edge
    CHECK(r.holders(villain) == 4);

    const KillResult h = kill(r, hero); // 4 + 97 = 101
    CHECK(h.result == EditResult::Ok);
    CHECK(h.fame == 101);
    CHECK(h.legendary);
    CHECK(r.find_dead(hero)->fame == 101);
    const KillResult v = kill(r, villain); // 3 (the hero's edge ended at its death) + 96 = 99
    CHECK(v.fame == 99);
    CHECK(!v.legendary);
    CHECK(r.find_dead(villain)->fame == 99);

    const LifecycleConfig exact{.fame_per_reputation = 1, .legendary_fame = 0};
    const KillResult n = kill(r, nobody, exact); // fame 0 >= 0; forgotten at once, still reported
    CHECK(n.result == EditResult::Ok);
    CHECK(n.fame == 0);
    CHECK(n.legendary);
    CHECK(!r.exists(nobody));

    // Saturation at 65535.
    CharacterRegistry big = make_registry(2);
    big.find(CharacterId{1})->set_reputation(-100);
    REQUIRE(big.add_modifier(CharacterId{2}, CharacterId{1}, ModifierId{1}, 1) == EditResult::Ok);
    const LifecycleConfig heavy{.fame_per_reputation = 1000, .legendary_fame = 65535};
    const KillResult s = kill(big, CharacterId{1}, heavy);
    CHECK(s.fame == 65535);
    CHECK(s.legendary);
    CHECK(big.find_dead(CharacterId{1})->fame == 65535);

    // Failed kills report no fame.
    const KillResult failed = kill(r, CharacterId{99});
    CHECK(failed.result == EditResult::NotFound);
    CHECK(failed.fame == 0);
    CHECK(!failed.legendary);
}
