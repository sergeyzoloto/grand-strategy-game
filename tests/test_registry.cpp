#include <doctest.h>

#include <cstdint>
#include <cstring>
#include <type_traits>

#include "sim/character.hpp"
#include "sim/character_registry.hpp"
#include "sim/personal_opinion.hpp"
#include "sim/stance_table.hpp"
#include "sim/world_context.hpp"

using namespace sim;

// Only the registry creates characters (passkey). C++20: CharacterKey has a
// user-declared constructor, so it is not an aggregate and CharacterKey{} is private.
static_assert(!std::is_default_constructible_v<CharacterKey>);
static_assert(!std::is_constructible_v<Character, CharacterId, NameId, Gender, Date, const CharacterInit&>);
static_assert(std::is_copy_constructible_v<Character>);
static_assert(!std::is_copy_assignable_v<Character> && !std::is_move_assignable_v<Character>);
static_assert(std::is_trivially_copyable_v<Character>);
static_assert(sizeof(Character) == 1164);

TEST_CASE("registry: ids start at 1 and increase by one") {
    CharacterRegistry registry;
    CHECK(registry.size() == 0);
    CHECK(registry.characters().empty());
    for (std::uint32_t expected = 1; expected <= 5; ++expected) {
        CHECK(registry.create(NameId{expected}, Gender::Female, Date{0}, CharacterInit{}) == CharacterId{expected});
    }
    CHECK(registry.size() == 5);
}

TEST_CASE("registry: find returns nullptr for id 0 and unknown ids") {
    CharacterRegistry registry;
    CHECK(registry.find(CharacterId{}) == nullptr);
    CHECK(registry.find(CharacterId{1}) == nullptr);
    const CharacterId id = registry.create(NameId{1}, Gender::Male, Date{0}, CharacterInit{});
    const CharacterRegistry& const_registry = registry;
    CHECK(registry.find(id) != nullptr);
    CHECK(const_registry.find(id) != nullptr);
    CHECK(registry.find(CharacterId{}) == nullptr);
    CHECK(const_registry.find(CharacterId{2}) == nullptr);
    CHECK(registry.find(CharacterId{0xFFFF'FFFFu}) == nullptr);
}

TEST_CASE("registry: init values arrive and edits through find persist") {
    CharacterRegistry registry;
    const CharacterId id = registry.create(NameId{42}, Gender::Male, Date{-5000},
                                           CharacterInit{.health = 70.5f, .strength = 30, .charisma = -150});
    Character* c = registry.find(id);
    REQUIRE(c != nullptr);
    CHECK(c->id() == id);
    CHECK(c->name() == NameId{42});
    CHECK(c->gender() == Gender::Male);
    CHECK(c->birth() == Date{-5000});
    CHECK(c->health() == 70.5f);
    CHECK(c->capacity() == 100.0f);
    CHECK(c->strength() == 30);
    CHECK(c->charisma() == -100);

    c->set_openness(12);
    CHECK(c->add_nickname(NameId{7}) == EditResult::Ok);
    const Character* again = registry.find(id);
    CHECK(again->openness() == 12);
    CHECK(again->nicknames().size() == 1);
}

TEST_CASE("registry: iteration follows creation order") {
    CharacterRegistry registry;
    for (std::uint32_t i = 1; i <= 40; ++i) {
        REQUIRE(registry.create(NameId{1000 + i}, Gender::Female, Date{static_cast<std::int32_t>(i)}, CharacterInit{})
                == CharacterId{i});
    }
    std::uint32_t expected = 1;
    for (const Character& c : registry.characters()) {
        CHECK(c.id() == CharacterId{expected});
        CHECK(c.name() == NameId{1000 + expected});
        ++expected;
    }
    CHECK(expected == 41);
}

TEST_CASE("registry: early characters keep their data after many creates") {
    CharacterRegistry registry;
    const CharacterId first = registry.create(NameId{11}, Gender::Female, Date{-123}, CharacterInit{.stress = 12.34f});
    REQUIRE(registry.find(first)->add_skill(SkillKind::Language, SkillId{9}) == EditResult::Ok);
    REQUIRE(registry.find(first)->set_involvement(CommunityId{3}, 17) == EditResult::Ok);
    const CharacterId second = registry.create(NameId{22}, Gender::Male, Date{0}, CharacterInit{});
    REQUIRE(registry.set_relation(first, RelationType::Friend, second) == EditResult::Ok);

    for (int i = 0; i < 5000; ++i) { // many reallocations
        (void)registry.create(NameId{99}, Gender::Male, Date{i}, CharacterInit{});
    }
    CHECK(registry.size() == 5002);

    const Character* c = registry.find(first);
    REQUIRE(c != nullptr);
    CHECK(c->id() == first);
    CHECK(c->name() == NameId{11});
    CHECK(c->birth() == Date{-123});
    CHECK(c->stress() == 12.34f);
    CHECK(c->has_skill(SkillKind::Language, SkillId{9}));
    CHECK(c->involvement_total() == 17);
    CHECK(registry.has_relation(first, RelationType::Friend, second));
    CHECK(registry.find(CharacterId{5002})->birth() == Date{4999});
}

// ---- byte determinism ----------------------------------------------------------------------

namespace {

TargetId topic_id(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

} // namespace

static_assert(std::has_unique_object_representations_v<Character>);
static_assert(!std::is_default_constructible_v<Character>);

TEST_CASE("byte determinism: the same logical Character through two edit histories is byte-identical") {
    const StanceTable stances;
    const OpinionConfig opinion_config{};
    const LifecycleConfig lifecycle{};
    constexpr std::uint32_t COUNT = 14;
    const CharacterId t{2}; // the character compared; id 1 dies in the second registry, relocating it

    // Direct: every value set once, in its final form.
    CharacterRegistry direct;
    for (std::uint32_t i = 1; i <= COUNT; ++i) {
        (void)direct.create(NameId{i}, Gender::Male, Date{-40},
                            i == t.value ? CharacterInit{.health = 55.5f, .strength = 20, .extraversion = -100,
                                                         .reputation = 15}
                                         : CharacterInit{});
    }
    {
        Character& c = *direct.find(t);
        REQUIRE(c.add_nickname(NameId{51}) == EditResult::Ok);
        REQUIRE(c.add_nickname(NameId{52}) == EditResult::Ok);
        REQUIRE(c.add_nickname(NameId{53}) == EditResult::Ok);
        REQUIRE(c.add_skill(SkillKind::Education, SkillId{2}) == EditResult::Ok);
        REQUIRE(c.add_skill(SkillKind::Language, SkillId{9}) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{3}, 10) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{7}, 40) == EditResult::Ok);
        REQUIRE(c.add_sacred(topic_id(6), SacredSign::Minus) == EditResult::Ok);
    }
    for (std::uint32_t k = 3; k <= 10; ++k) {
        REQUIRE(direct.add_long_opinion(t, CharacterId{k}, static_cast<int>(100 + k)).outcome
                == LongOpinionOutcome::Created);
    }
    for (std::uint32_t k = 1; k <= 15; ++k) {
        REQUIRE(direct.add_long_opinion(t, topic_id(k), 50).outcome == LongOpinionOutcome::Created);
    }
    REQUIRE(direct.add_long_opinion(t, topic_id(20), 60).outcome == LongOpinionOutcome::Created);
    REQUIRE(direct.add_modifier(t, CharacterId{5}, ModifierId{2}, -20) == EditResult::Ok);
    REQUIRE(direct.add_modifier(t, topic_id(8), ModifierId{3}, 7) == EditResult::Ok);

    // Detours: inserts and erases, a kill that relocates t, a maintain trim and evictions.
    CharacterRegistry detour;
    for (std::uint32_t i = 1; i <= COUNT; ++i) {
        (void)detour.create(NameId{i}, Gender::Male, Date{-40},
                            i == t.value ? CharacterInit{.health = 30.0f, .extraversion = 100} : CharacterInit{});
    }
    REQUIRE(detour.kill(CharacterId{1}, Date{0}, WorldContext{stances, opinion_config, lifecycle, WorldSeed{9}}).result
            == EditResult::Ok);
    REQUIRE(detour.characters()[0].id() == t); // relocated from slot 1 to slot 0
    {
        Character& c = *detour.find(t);
        c.set_health(55.5f);
        c.add_strength(30);
        c.add_strength(-10);
        c.set_reputation(5);
        c.add_reputation(10);
        REQUIRE(c.add_nickname(NameId{50}) == EditResult::Ok);
        REQUIRE(c.add_nickname(NameId{51}) == EditResult::Ok);
        REQUIRE(c.add_nickname(NameId{52}) == EditResult::Ok);
        REQUIRE(c.remove_nickname(NameId{50}) == EditResult::Ok);
        REQUIRE(c.add_nickname(NameId{53}) == EditResult::Ok);
        REQUIRE(c.add_skill(SkillKind::Language, SkillId{9}) == EditResult::Ok);
        REQUIRE(c.add_skill(SkillKind::Ability, SkillId{4}) == EditResult::Ok);
        REQUIRE(c.add_skill(SkillKind::Education, SkillId{2}) == EditResult::Ok);
        REQUIRE(c.remove_skill(SkillKind::Ability, SkillId{4}) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{7}, 30) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{3}, 10) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{9}, 200) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{9}, 0) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{7}, 40) == EditResult::Ok);
        REQUIRE(c.add_sacred(topic_id(5), SacredSign::Plus) == EditResult::Ok);
        REQUIRE(c.add_sacred(topic_id(6), SacredSign::Minus) == EditResult::Ok);
        REQUIRE(c.remove_sacred(topic_id(5)) == EditResult::Ok);
    }
    // People list: the final entries in reverse order plus two weak ones, then a trim to limit 8
    // (removes 11), then an eviction by 10 (removes 12), then a removal and re-creation of 3.
    for (std::uint32_t k = 9; k >= 3; --k) {
        REQUIRE(detour.add_long_opinion(t, CharacterId{k}, static_cast<int>(100 + k)).outcome
                == LongOpinionOutcome::Created);
    }
    REQUIRE(detour.add_long_opinion(t, CharacterId{11}, 5).outcome == LongOpinionOutcome::Created);
    REQUIRE(detour.add_long_opinion(t, CharacterId{12}, 6).outcome == LongOpinionOutcome::Created);
    detour.find(t)->set_extraversion(-100);
    REQUIRE(detour.maintain() == 1);
    const LongOpinionResult<CharacterId> evicting = detour.add_long_opinion(t, CharacterId{10}, 110);
    REQUIRE(evicting.outcome == LongOpinionOutcome::CreatedWithEviction);
    REQUIRE(evicting.evicted == CharacterId{12});
    REQUIRE(detour.add_long_opinion(t, CharacterId{3}, -103).outcome == LongOpinionOutcome::Removed);
    REQUIRE(detour.add_long_opinion(t, CharacterId{3}, 103).outcome == LongOpinionOutcome::Created);
    // Target list: full at TARGET_LIMIT with one weak entry, evicted by topic 20.
    for (std::uint32_t k = 16; k >= 1; --k) {
        REQUIRE(detour.add_long_opinion(t, topic_id(k), k == 16 ? 2 : 50).outcome == LongOpinionOutcome::Created);
    }
    REQUIRE(detour.add_long_opinion(t, topic_id(20), 60).outcome == LongOpinionOutcome::CreatedWithEviction);
    // Modifiers: an extra one added and removed.
    REQUIRE(detour.add_modifier(t, topic_id(8), ModifierId{3}, 7) == EditResult::Ok);
    REQUIRE(detour.add_modifier(t, CharacterId{4}, ModifierId{1}, 10) == EditResult::Ok);
    REQUIRE(detour.add_modifier(t, CharacterId{5}, ModifierId{2}, -20) == EditResult::Ok);
    REQUIRE(detour.remove_modifier(t, CharacterId{4}, ModifierId{1}) == EditResult::Ok);

    const Character& a = *direct.find(t);
    const Character& b = *detour.find(t);
    // Logical equality first (a clearer failure), then the bytes.
    REQUIRE(a.health() == b.health());
    REQUIRE((a.strength() == b.strength() && a.reputation() == b.reputation() && a.extraversion() == b.extraversion()));
    REQUIRE(a.long_people().size() == b.long_people().size());
    REQUIRE(a.long_targets().size() == b.long_targets().size());
    REQUIRE(a.modifiers().size() == b.modifiers().size());
    CHECK(std::memcmp(&a, &b, sizeof(Character)) == 0);
}
