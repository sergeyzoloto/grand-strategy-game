#include <doctest.h>

#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "sim/character.hpp"

#include "test_support.hpp"

using namespace sim;

namespace {

Character make_character() {
    return ::make_character(NameId{1}, Gender::Female, Date{0}, CharacterInit{});
}

TargetId community_target(std::uint32_t id) {
    return *TargetId::from(CommunityId{id});
}

TargetId topic_target(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

const SkillKind ALL_SKILL_KINDS[] = {SkillKind::Ability, SkillKind::Education, SkillKind::Language};

template<class T>
concept CanSetInvolvement = requires(Character& c, T w) { c.set_involvement(CommunityId{1}, w); };

} // namespace

// ---- compile-time ----------------------------------------------------------------

static_assert(std::is_trivially_copyable_v<Character>);
static_assert(sizeof(PractiseEntry) == 4);
static_assert(sizeof(InvolvementEntry) == 8);
static_assert(sizeof(SacredEntry) == 8);

static_assert(CanSetInvolvement<int> && CanSetInvolvement<std::int64_t> && CanSetInvolvement<unsigned>);
static_assert(CanSetInvolvement<std::uint8_t> && CanSetInvolvement<std::uint64_t>);
static_assert(!CanSetInvolvement<float> && !CanSetInvolvement<double> && !CanSetInvolvement<long double>);
static_assert(!CanSetInvolvement<bool> && !CanSetInvolvement<char>);

TEST_CASE("sizeof of Character and list entries") {
    MESSAGE("sizeof(Character) = " << sizeof(Character) << ", alignof = " << alignof(Character)
            << "; PractiseEntry = " << sizeof(PractiseEntry) << ", InvolvementEntry = " << sizeof(InvolvementEntry)
            << ", SacredEntry = " << sizeof(SacredEntry) << ", NameId = " << sizeof(NameId));
    CHECK(sizeof(Character) == 444);
}

TEST_CASE("lists start empty") {
    const Character c = make_character();
    CHECK(c.nicknames().empty());
    CHECK(c.practise().empty());
    CHECK(c.involvement().empty());
    CHECK(c.sacred().empty());
    CHECK(c.involvement_total() == 0);
    CHECK(!c.main_community().has_value());
}

// ---- nicknames ---------------------------------------------------------------------

TEST_CASE("nicknames: add keeps insertion order, rejects duplicates and invalid ids") {
    Character c = make_character();
    CHECK(c.add_nickname(NameId{30}) == EditResult::Ok);
    CHECK(c.add_nickname(NameId{10}) == EditResult::Ok);
    CHECK(c.add_nickname(NameId{20}) == EditResult::Ok);
    CHECK(c.add_nickname(NameId{10}) == EditResult::Duplicate);
    CHECK(c.add_nickname(NameId{}) == EditResult::Invalid);
    CHECK(c.remove_nickname(NameId{}) == EditResult::Invalid);

    const auto names = c.nicknames();
    REQUIRE(names.size() == 3);
    CHECK(names[0] == NameId{30});
    CHECK(names[1] == NameId{10});
    CHECK(names[2] == NameId{20});
}

TEST_CASE("nicknames: full, remove keeps order, NotFound") {
    Character c = make_character();
    for (std::uint32_t i = 1; i <= NICKNAME_CAP; ++i) {
        CHECK(c.add_nickname(NameId{i}) == EditResult::Ok);
    }
    CHECK(c.add_nickname(NameId{99}) == EditResult::Full);
    CHECK(c.add_nickname(NameId{2}) == EditResult::Duplicate); // duplicate is reported before full
    CHECK(c.nicknames().size() == NICKNAME_CAP);

    CHECK(c.remove_nickname(NameId{2}) == EditResult::Ok);
    CHECK(c.remove_nickname(NameId{2}) == EditResult::NotFound);
    const auto names = c.nicknames();
    REQUIRE(names.size() == 3);
    CHECK(names[0] == NameId{1});
    CHECK(names[1] == NameId{3});
    CHECK(names[2] == NameId{4});

    CHECK(c.add_nickname(NameId{99}) == EditResult::Ok);
    CHECK(c.nicknames().back() == NameId{99});
}

// ---- practise ----------------------------------------------------------------------

TEST_CASE("practise: add, duplicate, remove and has_skill on every kind") {
    for (const SkillKind kind : ALL_SKILL_KINDS) {
        CAPTURE(static_cast<int>(kind));
        Character c = make_character();
        CHECK(!c.has_skill(kind, SkillId{7}));
        CHECK(c.add_skill(kind, SkillId{7}) == EditResult::Ok);
        CHECK(c.has_skill(kind, SkillId{7}));
        CHECK(c.add_skill(kind, SkillId{7}) == EditResult::Duplicate);
        CHECK(c.practise().size() == 1);
        CHECK(c.practise()[0].reserved == 0);

        CHECK(c.add_skill(kind, SkillId{}) == EditResult::Invalid);
        CHECK(c.remove_skill(kind, SkillId{}) == EditResult::Invalid);
        CHECK(!c.has_skill(kind, SkillId{}));

        CHECK(c.remove_skill(kind, SkillId{7}) == EditResult::Ok);
        CHECK(!c.has_skill(kind, SkillId{7}));
        CHECK(c.remove_skill(kind, SkillId{7}) == EditResult::NotFound);
        CHECK(c.practise().empty());
    }
}

TEST_CASE("practise: the same id under different kinds is a different skill") {
    Character c = make_character();
    CHECK(c.add_skill(SkillKind::Language, SkillId{3}) == EditResult::Ok);
    CHECK(!c.has_skill(SkillKind::Ability, SkillId{3}));
    CHECK(c.add_skill(SkillKind::Ability, SkillId{3}) == EditResult::Ok);
    CHECK(c.remove_skill(SkillKind::Education, SkillId{3}) == EditResult::NotFound);
    CHECK(c.practise().size() == 2);
}

TEST_CASE("practise: an out-of-range kind is Invalid") {
    Character c = make_character();
    const auto bogus = static_cast<SkillKind>(200);
    CHECK(c.add_skill(bogus, SkillId{1}) == EditResult::Invalid);
    CHECK(c.remove_skill(bogus, SkillId{1}) == EditResult::Invalid);
    CHECK(!c.has_skill(bogus, SkillId{1}));
    CHECK(c.practise().empty());
}

TEST_CASE("practise: sorted by (kind, id) regardless of insertion order") {
    Character c = make_character();
    CHECK(c.add_skill(SkillKind::Language, SkillId{2}) == EditResult::Ok);
    CHECK(c.add_skill(SkillKind::Ability, SkillId{900}) == EditResult::Ok);
    CHECK(c.add_skill(SkillKind::Education, SkillId{5}) == EditResult::Ok);
    CHECK(c.add_skill(SkillKind::Ability, SkillId{1}) == EditResult::Ok);
    CHECK(c.add_skill(SkillKind::Language, SkillId{1}) == EditResult::Ok);

    const auto skills = c.practise();
    REQUIRE(skills.size() == 5);
    CHECK((skills[0].kind == SkillKind::Ability && skills[0].skill == SkillId{1}));
    CHECK((skills[1].kind == SkillKind::Ability && skills[1].skill == SkillId{900}));
    CHECK((skills[2].kind == SkillKind::Education && skills[2].skill == SkillId{5}));
    CHECK((skills[3].kind == SkillKind::Language && skills[3].skill == SkillId{1}));
    CHECK((skills[4].kind == SkillKind::Language && skills[4].skill == SkillId{2}));
}

TEST_CASE("practise: a full list returns Full without changing state and still reports existing skills") {
    Character c = make_character();
    // Fill in descending id order across kinds to exercise front inserts.
    std::size_t added = 0;
    for (std::uint16_t id = 1000; added < PRACTISE_CAP; --id) {
        const SkillKind kind = ALL_SKILL_KINDS[id % 3];
        REQUIRE(c.add_skill(kind, SkillId{id}) == EditResult::Ok);
        ++added;
    }
    REQUIRE(c.practise().size() == PRACTISE_CAP);
    const std::uint16_t lowest_id = static_cast<std::uint16_t>(1000 - PRACTISE_CAP + 1);

    const Character before = c;
    for (const SkillKind kind : ALL_SKILL_KINDS) {
        CHECK(c.add_skill(kind, SkillId{1}) == EditResult::Full);
    }
    CHECK(c.practise().size() == PRACTISE_CAP);
    for (std::size_t i = 0; i < PRACTISE_CAP; ++i) {
        CHECK((c.practise()[i].kind == before.practise()[i].kind && c.practise()[i].skill == before.practise()[i].skill));
    }

    // Existing skills are still reported, duplicates still detected, and nothing was evicted.
    for (std::uint16_t id = lowest_id; id <= 1000; ++id) {
        const SkillKind kind = ALL_SKILL_KINDS[id % 3];
        if (!c.has_skill(kind, SkillId{id}) || c.add_skill(kind, SkillId{id}) != EditResult::Duplicate) {
            FAIL_CHECK("existing skill not reported at id " << id);
        }
    }

    // Removing one frees exactly one slot.
    CHECK(c.remove_skill(ALL_SKILL_KINDS[1000 % 3], SkillId{1000}) == EditResult::Ok);
    CHECK(c.add_skill(SkillKind::Ability, SkillId{1}) == EditResult::Ok);
    CHECK(c.add_skill(SkillKind::Education, SkillId{1}) == EditResult::Full);
}

// ---- involvement -------------------------------------------------------------------

TEST_CASE("involvement: shares sum to 1, absent community has share 0, total is exact") {
    Character c = make_character();
    CHECK(c.set_involvement(CommunityId{3}, 50) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{1}, 30) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{2}, 7) == EditResult::Ok);
    CHECK(c.involvement_total() == 87);

    double sum = 0.0;
    for (const InvolvementEntry& e : c.involvement()) {
        sum += static_cast<double>(c.involvement_share(e.community));
    }
    CHECK(std::abs(sum - 1.0) <= 1e-6);
    CHECK(c.involvement_share(CommunityId{3}) == static_cast<float>(50.0 / 87.0));
    CHECK(c.involvement_share(CommunityId{99}) == 0.0f);
    CHECK(c.involvement_share(CommunityId{}) == 0.0f);

    const auto entries = c.involvement();
    REQUIRE(entries.size() == 3);
    CHECK(entries[0].community == CommunityId{1});
    CHECK(entries[1].community == CommunityId{2});
    CHECK(entries[2].community == CommunityId{3});
}

TEST_CASE("involvement: three equal weights give three identical shares") {
    Character c = make_character();
    CHECK(c.set_involvement(CommunityId{4}, 85) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{8}, 85) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{2}, 85) == EditResult::Ok);
    const float a = c.involvement_share(CommunityId{2});
    const float b = c.involvement_share(CommunityId{4});
    const float d = c.involvement_share(CommunityId{8});
    CHECK(a == b);
    CHECK(b == d);
    CHECK(a == static_cast<float>(1.0 / 3.0));
}

TEST_CASE("involvement: weight 0 removes, update in place, clamping") {
    Character c = make_character();
    CHECK(c.set_involvement(CommunityId{5}, 0) == EditResult::Ok); // absent: no change
    CHECK(c.involvement().empty());

    CHECK(c.set_involvement(CommunityId{5}, 10) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{5}, 20) == EditResult::Ok);
    REQUIRE(c.involvement().size() == 1);
    CHECK(c.involvement()[0].weight == 20);

    CHECK(c.set_involvement(CommunityId{5}, 1000) == EditResult::Ok);
    CHECK(c.involvement()[0].weight == 255);
    CHECK(c.set_involvement(CommunityId{5}, std::numeric_limits<std::uint64_t>::max()) == EditResult::Ok);
    CHECK(c.involvement()[0].weight == 255);

    CHECK(c.set_involvement(CommunityId{5}, -40) == EditResult::Ok); // clamps to 0: removes
    CHECK(c.involvement().empty());

    CHECK(c.set_involvement(CommunityId{5}, 9) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{5}, 0) == EditResult::Ok);
    CHECK(c.involvement().empty());
    CHECK(c.involvement_total() == 0);

    CHECK(c.set_involvement(CommunityId{}, 10) == EditResult::Invalid);
    CHECK(c.set_involvement(CommunityId{}, 0) == EditResult::Invalid);
}

TEST_CASE("involvement: full list rejects new communities but updates and removes existing ones") {
    Character c = make_character();
    for (std::uint32_t i = 1; i <= INVOLVEMENT_CAP; ++i) {
        CHECK(c.set_involvement(CommunityId{i * 10}, 1) == EditResult::Ok);
    }
    CHECK(c.set_involvement(CommunityId{5}, 1) == EditResult::Full);
    CHECK(c.set_involvement(CommunityId{5}, 0) == EditResult::Ok); // absent, weight 0: no change
    CHECK(c.set_involvement(CommunityId{10}, 200) == EditResult::Ok);
    CHECK(c.involvement().size() == INVOLVEMENT_CAP);
    CHECK(c.involvement_total() == 200 + static_cast<int>(INVOLVEMENT_CAP) - 1);
    CHECK(c.set_involvement(CommunityId{20}, 0) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{5}, 1) == EditResult::Ok);
}

TEST_CASE("involvement: main community tie-break and empty involvement") {
    Character c = make_character();
    CHECK(!c.main_community().has_value());

    CHECK(c.set_involvement(CommunityId{9}, 40) == EditResult::Ok);
    CHECK(c.main_community() == CommunityId{9});
    CHECK(c.set_involvement(CommunityId{12}, 60) == EditResult::Ok);
    CHECK(c.main_community() == CommunityId{12});
    CHECK(c.set_involvement(CommunityId{3}, 60) == EditResult::Ok); // tie: smaller id wins
    CHECK(c.main_community() == CommunityId{3});
    CHECK(c.set_involvement(CommunityId{15}, 60) == EditResult::Ok); // tie with larger id: no change
    CHECK(c.main_community() == CommunityId{3});

    CHECK(c.set_involvement(CommunityId{3}, 0) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{12}, 0) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{15}, 0) == EditResult::Ok);
    CHECK(c.set_involvement(CommunityId{9}, 0) == EditResult::Ok);
    CHECK(!c.main_community().has_value());
}

// ---- sacred ------------------------------------------------------------------------

TEST_CASE("sacred: Duplicate, Conflict, remove then re-add with the other sign") {
    Character c = make_character();
    const TargetId t = topic_target(42);
    CHECK(!c.sacred_sign(t).has_value());
    CHECK(c.add_sacred(t, SacredSign::Plus) == EditResult::Ok);
    CHECK(c.sacred_sign(t) == SacredSign::Plus);
    CHECK(c.add_sacred(t, SacredSign::Plus) == EditResult::Duplicate);
    CHECK(c.add_sacred(t, SacredSign::Minus) == EditResult::Conflict);
    CHECK(c.sacred_sign(t) == SacredSign::Plus);
    CHECK(c.sacred().size() == 1);

    CHECK(c.remove_sacred(t) == EditResult::Ok);
    CHECK(c.remove_sacred(t) == EditResult::NotFound);
    CHECK(c.add_sacred(t, SacredSign::Minus) == EditResult::Ok);
    CHECK(c.sacred_sign(t) == SacredSign::Minus);
}

TEST_CASE("sacred: invalid target or sign, sorted by target, full") {
    Character c = make_character();
    CHECK(c.add_sacred(TargetId{}, SacredSign::Plus) == EditResult::Invalid);
    CHECK(c.remove_sacred(TargetId{}) == EditResult::Invalid);
    CHECK(!c.sacred_sign(TargetId{}).has_value());
    CHECK(c.add_sacred(community_target(1), static_cast<SacredSign>(7)) == EditResult::Invalid);
    CHECK(c.sacred().empty());

    // Community and topic with the same index are different targets.
    CHECK(c.add_sacred(topic_target(5), SacredSign::Minus) == EditResult::Ok);
    CHECK(c.add_sacred(community_target(5), SacredSign::Plus) == EditResult::Ok);
    CHECK(c.add_sacred(community_target(2), SacredSign::Plus) == EditResult::Ok);
    CHECK(c.add_sacred(topic_target(1), SacredSign::Plus) == EditResult::Ok);
    const auto entries = c.sacred();
    REQUIRE(entries.size() == 4);
    CHECK(entries[0].target == community_target(2));
    CHECK(entries[1].target == community_target(5));
    CHECK(entries[2].target == topic_target(1));
    CHECK(entries[3].target == topic_target(5));

    for (std::uint32_t i = 100; c.sacred().size() < SACRED_CAP; ++i) {
        REQUIRE(c.add_sacred(community_target(i), SacredSign::Plus) == EditResult::Ok);
    }
    CHECK(c.add_sacred(topic_target(999), SacredSign::Plus) == EditResult::Full);
    CHECK(c.add_sacred(topic_target(5), SacredSign::Minus) == EditResult::Duplicate);
    CHECK(c.add_sacred(topic_target(5), SacredSign::Plus) == EditResult::Conflict);
    CHECK(c.sacred().size() == SACRED_CAP);
}

// ---- copies ------------------------------------------------------------------------

TEST_CASE("a copied Character carries independent copies of all lists") {
    Character original = make_character();
    REQUIRE(original.add_nickname(NameId{11}) == EditResult::Ok);
    REQUIRE(original.add_skill(SkillKind::Language, SkillId{4}) == EditResult::Ok);
    REQUIRE(original.set_involvement(CommunityId{6}, 100) == EditResult::Ok);
    REQUIRE(original.add_sacred(topic_target(8), SacredSign::Minus) == EditResult::Ok);

    const Character copy = original;

    CHECK(original.add_nickname(NameId{12}) == EditResult::Ok);
    CHECK(original.remove_nickname(NameId{11}) == EditResult::Ok);
    CHECK(original.remove_skill(SkillKind::Language, SkillId{4}) == EditResult::Ok);
    CHECK(original.add_skill(SkillKind::Ability, SkillId{9}) == EditResult::Ok);
    CHECK(original.set_involvement(CommunityId{6}, 1) == EditResult::Ok);
    CHECK(original.set_involvement(CommunityId{7}, 50) == EditResult::Ok);
    CHECK(original.remove_sacred(topic_target(8)) == EditResult::Ok);

    REQUIRE(copy.nicknames().size() == 1);
    CHECK(copy.nicknames()[0] == NameId{11});
    CHECK(copy.has_skill(SkillKind::Language, SkillId{4}));
    CHECK(!copy.has_skill(SkillKind::Ability, SkillId{9}));
    REQUIRE(copy.involvement().size() == 1);
    CHECK(copy.involvement()[0].weight == 100);
    CHECK(copy.involvement_share(CommunityId{6}) == 1.0f);
    CHECK(copy.sacred_sign(topic_target(8)) == SacredSign::Minus);
}
