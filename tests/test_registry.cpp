#include <doctest.h>

#include <cstdint>
#include <type_traits>

#include "sim/character.hpp"
#include "sim/character_registry.hpp"

using namespace sim;

// Only the registry creates characters (passkey). C++20: CharacterKey has a
// user-declared constructor, so it is not an aggregate and CharacterKey{} is private.
static_assert(!std::is_default_constructible_v<CharacterKey>);
static_assert(!std::is_constructible_v<Character, CharacterId, NameId, Gender, Date, const CharacterInit&>);
static_assert(std::is_copy_constructible_v<Character>);
static_assert(!std::is_copy_assignable_v<Character> && !std::is_move_assignable_v<Character>);
static_assert(std::is_trivially_copyable_v<Character>);
static_assert(sizeof(Character) == 1352);

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
