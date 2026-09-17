#include <doctest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <map>
#include <optional>
#include <random>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/dead_record.hpp"
#include "sim/hundredths.hpp"
#include "sim/noise.hpp"
#include "sim/opinion.hpp"
#include "sim/personal_opinion.hpp"
#include "sim/relations.hpp"
#include "sim/stance_table.hpp"

using namespace sim;

namespace {

const OpinionConfig CONFIG{};
const WorldSeed SEED{0xDEADu};
const LifecycleConfig LIFECYCLE{};
using O = LongOpinionOutcome;
using R = RelationType;

TargetId topic(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

TargetId community(std::uint32_t id) {
    return *TargetId::from(CommunityId{id});
}

CharacterRegistry make_registry(std::uint32_t count, const CharacterInit& init = {}) {
    CharacterRegistry registry;
    for (std::uint32_t i = 1; i <= count; ++i) {
        (void)registry.create(NameId{i}, Gender::Female, Date{0}, init);
    }
    return registry;
}

// Byte-wise span comparison; every entry type here has unique object representations.
template<class T>
bool same_span(std::span<const T> a, std::span<const T> b) {
    static_assert(std::has_unique_object_representations_v<T>);
    return a.size() == b.size() && (a.empty() || std::memcmp(a.data(), b.data(), a.size_bytes()) == 0);
}

bool same_character(const Character& a, const Character& b) {
    return a.id() == b.id() && a.name() == b.name() && a.gender() == b.gender() && a.birth() == b.birth()
        && a.health() == b.health() && a.stress() == b.stress() && a.capacity() == b.capacity()
        && a.strength() == b.strength() && a.intelligence() == b.intelligence() && a.stability() == b.stability()
        && a.openness() == b.openness() && a.extraversion() == b.extraversion()
        && a.conscientiousness() == b.conscientiousness() && a.agreeableness() == b.agreeableness()
        && a.attractiveness() == b.attractiveness() && a.height() == b.height() && a.shape() == b.shape()
        && a.charisma() == b.charisma() && a.reputation() == b.reputation()
        && same_span(a.nicknames(), b.nicknames()) && same_span(a.practise(), b.practise())
        && same_span(a.involvement(), b.involvement()) && same_span(a.sacred(), b.sacred())
        && same_span(a.long_people(), b.long_people()) && same_span(a.long_targets(), b.long_targets())
        && same_span(a.modifiers(), b.modifiers());
}

std::vector<CharacterId> ids_of(std::span<const Character> characters) {
    std::vector<CharacterId> out;
    for (const Character& c : characters) {
        out.push_back(c.id());
    }
    return out;
}

WorldContext ctx(const StanceTable& stances) {
    return WorldContext{stances, CONFIG, LIFECYCLE, SEED};
}

// The DeadRecord kill() will write, built independently for expected values.
DeadRecord expected_record(const Character& c, Date death) {
    return DeadRecord{.id = c.id(),
                      .name = c.name(),
                      .birth = c.birth(),
                      .death = death,
                      .main_community = c.main_community().value_or(CommunityId{}),
                      .reputation = static_cast<std::int8_t>(c.reputation()),
                      .gender = c.gender(),
                      .fame = 0};
}

} // namespace

// ---- layout --------------------------------------------------------------------------------

static_assert(sizeof(DeadRecord) == 24 && alignof(DeadRecord) == 4);
static_assert(std::has_unique_object_representations_v<DeadRecord>);
static_assert(offsetof(DeadRecord, death) == 12 && offsetof(DeadRecord, main_community) == 16
              && offsetof(DeadRecord, reputation) == 20 && offsetof(DeadRecord, gender) == 21
              && offsetof(DeadRecord, fame) == 22);
static_assert(survives_death(R::Parent) && survives_death(R::Child) && survives_death(R::Spouse));
static_assert(!survives_death(R::Liege) && !survives_death(R::Vassal) && !survives_death(R::Employer)
              && !survives_death(R::Employee) && !survives_death(R::Friend) && !survives_death(R::Rival)
              && !survives_death(R::Attraction));

TEST_CASE("death: sizeof(DeadRecord)") {
    MESSAGE("sizeof(DeadRecord) = " << sizeof(DeadRecord));
    CHECK(sizeof(DeadRecord) == 24);
}

// ---- kill ----------------------------------------------------------------------------------

TEST_CASE("kill: Invalid and NotFound; find and find_dead; DeadRecord fields") {
    const StanceTable stances;
    CharacterRegistry r;
    const CharacterId a = r.create(NameId{11}, Gender::Male, Date{100}, CharacterInit{.reputation = -20});
    const CharacterId b = r.create(NameId{12}, Gender::Female, Date{50}, CharacterInit{});
    const CharacterId rememberer = r.create(NameId{13}, Gender::Female, Date{0}, CharacterInit{});
    // Someone living remembers a and b, so their records aren't forgotten at death.
    REQUIRE(r.add_modifier(rememberer, a, ModifierId{1}, 0) == EditResult::Ok);
    REQUIRE(r.add_modifier(rememberer, b, ModifierId{1}, 0) == EditResult::Ok);
    REQUIRE(r.find(a)->set_involvement(CommunityId{4}, 2) == EditResult::Ok);
    REQUIRE(r.find(a)->set_involvement(CommunityId{9}, 7) == EditResult::Ok);
    r.find(a)->set_reputation(37); // changed after creation; frozen at death

    CHECK(r.kill(CharacterId{}, Date{200}, ctx(stances)).result == EditResult::Invalid);
    CHECK(r.kill(CharacterId{4}, Date{200}, ctx(stances)).result == EditResult::NotFound);
    CHECK(r.kill(CharacterId{4}, Date{-1000}, ctx(stances)).result == EditResult::NotFound); // NotFound first
    CHECK(r.kill(a, Date{99}, ctx(stances)).result == EditResult::Invalid);                  // death < birth
    CHECK(r.find(a) != nullptr);
    CHECK(r.find_dead(a) == nullptr);
    CHECK(r.dead_count() == 0);

    CHECK(r.kill(a, Date{100}, ctx(stances)).result == EditResult::Ok); // death == birth is allowed
    CHECK(r.find(a) == nullptr);
    CHECK(r.kill(a, Date{300}, ctx(stances)).result == EditResult::NotFound); // already dead
    const DeadRecord* d = r.find_dead(a);
    REQUIRE(d != nullptr);
    CHECK(d->id == a);
    CHECK(d->name == NameId{11});
    CHECK(d->birth == Date{100});
    CHECK(d->death == Date{100});
    CHECK(d->main_community == CommunityId{9});
    CHECK(d->reputation == 37);
    CHECK(d->gender == Gender::Male);
    CHECK(r.exists(a));
    CHECK(r.size() == 2);
    CHECK(r.dead_count() == 1);

    // No involvement: an invalid main community.
    CHECK(r.find_dead(b) == nullptr);
    CHECK(r.kill(b, Date{60}, ctx(stances)).result == EditResult::Ok);
    CHECK(!r.find_dead(b)->main_community.valid());
    CHECK(r.characters().size() == 1);
    CHECK(r.find_dead(CharacterId{}) == nullptr);
    CHECK(r.find_dead(CharacterId{4}) == nullptr);
    CHECK(r.find_dead(rememberer) == nullptr);
    // Ids are never reused.
    CHECK(r.create(NameId{14}, Gender::Male, Date{0}, CharacterInit{}) == CharacterId{4});
}

TEST_CASE("kill: characters() excludes the dead in id order; other characters are unchanged after relocation") {
    const StanceTable stances;
    CharacterRegistry r;
    std::mt19937 rng(606u);
    const auto trait = [&] { return static_cast<int>(rng() % 201) - 100; };
    for (std::uint32_t i = 1; i <= 8; ++i) {
        (void)r.create(NameId{100 + i}, i % 2 == 0 ? Gender::Male : Gender::Female, Date{static_cast<int>(i)},
                       CharacterInit{.health = 50.25f,
                                     .strength = trait(),
                                     .stability = trait(),
                                     .openness = trait(),
                                     .extraversion = 100,
                                     .charisma = trait(),
                                     .reputation = trait()});
    }
    for (std::uint32_t i = 1; i <= 8; ++i) {
        Character& c = *r.find(CharacterId{i});
        REQUIRE(c.add_nickname(NameId{500 + i}) == EditResult::Ok);
        REQUIRE(c.add_skill(SkillKind::Language, SkillId{static_cast<std::uint16_t>(i)}) == EditResult::Ok);
        REQUIRE(c.set_involvement(CommunityId{i}, i) == EditResult::Ok);
        REQUIRE(c.add_sacred(topic(i), SacredSign::Plus) == EditResult::Ok);
    }
    for (std::uint32_t i = 1; i <= 8; ++i) {
        // Opinions about each other except about 3 (the victim), so no rebase touches them.
        for (std::uint32_t k = 1; k <= 8; ++k) {
            if (k != i && k != 3) {
                REQUIRE(r.add_long_opinion(CharacterId{i}, CharacterId{k}, static_cast<int>(i * 10 + k))
                        .outcome == O::Created);
            }
        }
        REQUIRE(r.add_long_opinion(CharacterId{i}, topic(i), 5).outcome == O::Created);
        if (i != 3) {
            REQUIRE(r.add_modifier(CharacterId{i}, CharacterId{i % 8 + 1}, ModifierId{1}, -7) == EditResult::Ok);
        }
    }
    std::vector<Character> before(r.characters().begin(), r.characters().end());

    REQUIRE(r.kill(CharacterId{3}, Date{500}, ctx(stances)).result == EditResult::Ok);
    CHECK(ids_of(r.characters())
          == std::vector<CharacterId>{CharacterId{1}, CharacterId{2}, CharacterId{4}, CharacterId{5}, CharacterId{6},
                                      CharacterId{7}, CharacterId{8}});
    for (const Character& old : before) {
        CAPTURE(old.id().value);
        if (old.id() == CharacterId{3}) {
            continue;
        }
        const Character* now = r.find(old.id());
        REQUIRE(now != nullptr);
        CHECK(same_character(*now, old)); // includes the modifier of 2 about 3, which stays
    }
    REQUIRE(r.kill(CharacterId{8}, Date{500}, ctx(stances)).result == EditResult::Ok); // last slot
    REQUIRE(r.kill(CharacterId{1}, Date{500}, ctx(stances)).result == EditResult::Ok); // first slot
    CHECK(ids_of(r.characters())
          == std::vector<CharacterId>{CharacterId{2}, CharacterId{4}, CharacterId{5}, CharacterId{6}, CharacterId{7}});
    for (const Character& c : r.characters()) {
        CHECK(r.find(c.id()) == &c);
    }
    CHECK(r.find_dead(CharacterId{3})->id == CharacterId{3});
    CHECK(r.find_dead(CharacterId{8})->id == CharacterId{8});
    CHECK(r.find_dead(CharacterId{1})->id == CharacterId{1});
}

// ---- relations -----------------------------------------------------------------------------

TEST_CASE("kill: surviving links stay on both sides; others are unlinked; one-way edges are cleared") {
    const StanceTable stances;
    CharacterRegistry r = make_registry(9);
    const CharacterId a{1}, spouse{2}, child{3}, vassal{4}, employee{5}, friend_{6}, rival{7}, liege{8}, other{9};
    REQUIRE(r.link(a, R::Spouse, spouse) == EditResult::Ok);
    REQUIRE(r.link(a, R::Child, child) == EditResult::Ok);
    REQUIRE(r.link(a, R::Vassal, vassal) == EditResult::Ok);
    REQUIRE(r.link(a, R::Employee, employee) == EditResult::Ok);
    REQUIRE(r.link(a, R::Liege, liege) == EditResult::Ok);
    REQUIRE(r.link(liege, R::Employee, spouse) == EditResult::Ok); // unrelated to a
    REQUIRE(r.set_relation(a, R::Friend, friend_) == EditResult::Ok);
    REQUIRE(r.set_relation(rival, R::Rival, a) == EditResult::Ok);
    REQUIRE(r.set_relation(spouse, R::Attraction, a) == EditResult::Ok); // one-way on a surviving pair
    REQUIRE(r.link(a, R::Employer, spouse) == EditResult::Ok);            // non-surviving on a surviving pair
    REQUIRE(r.set_relation(other, R::Friend, rival) == EditResult::Ok);   // unrelated

    REQUIRE(r.kill(a, Date{10}, ctx(stances)).result == EditResult::Ok);

    CHECK(r.has_relation(a, R::Spouse, spouse));
    CHECK(r.has_relation(spouse, R::Spouse, a));
    CHECK(r.has_relation(a, R::Child, child));
    CHECK(r.has_relation(child, R::Parent, a));
    CHECK(!r.has_relation(spouse, R::Attraction, a));
    CHECK(!r.has_relation(a, R::Employer, spouse));
    CHECK(!r.has_relation(spouse, R::Employee, a));
    CHECK(r.relations(a).size() == 2);
    CHECK(r.relations(spouse).size() == 2); // a (Spouse only) and liege (Employer)
    CHECK(r.relations(vassal).empty());      // empty edges removed
    CHECK(r.relations(employee).empty());
    CHECK(r.relations(liege).size() == 1);   // the spouse link
    CHECK(r.relations(friend_).empty());
    CHECK(r.relations(rival).empty());
    CHECK(r.has_relation(other, R::Friend, rival));
    CHECK(r.parents(child).size() == 1);
    CHECK(r.parents(child)[0] == a); // may return a dead id
    CHECK(r.find(r.parents(child)[0]) == nullptr);
}

TEST_CASE("kill: siblings through a dead parent; parents() and children() of a dead character; forgotten grandparent") {
    const StanceTable stances;
    CharacterRegistry r = make_registry(6);
    const CharacterId grandparent{1}, parent{2}, other_parent{3}, x{4}, y{5}, z{6};
    REQUIRE(r.link(grandparent, R::Child, parent) == EditResult::Ok);
    for (const CharacterId child : {x, y}) {
        REQUIRE(r.link(parent, R::Child, child) == EditResult::Ok);
    }
    REQUIRE(r.link(other_parent, R::Child, x) == EditResult::Ok);
    REQUIRE(r.link(other_parent, R::Child, z) == EditResult::Ok);

    REQUIRE(r.kill(parent, Date{5}, ctx(stances)).result == EditResult::Ok);
    REQUIRE(r.kill(grandparent, Date{6}, ctx(stances)).result == EditResult::Ok);
    CHECK(r.siblings(y) == std::vector<CharacterId>{x});
    CHECK(r.siblings(x) == std::vector<CharacterId>{y, z});
    CHECK(r.shared_parents(x, y) == 1);
    CHECK(r.children(parent) == std::vector<CharacterId>{x, y});
    // Only the dead parent held the grandparent, so it was forgotten at its own death: the
    // dead parent's edge to it is gone as well.
    CHECK(!r.exists(grandparent));
    CHECK(r.parents(parent).empty());
    CHECK(r.children(grandparent).empty());
}

TEST_CASE("posthumous links: surviving types only; one-way edits with the dead are Invalid") {
    const StanceTable stances;
    CharacterRegistry r = make_registry(7);
    const CharacterId father{1}, mother{2}, child{3}, stepfather{4}, m{5}, n{6}, living{7};
    // living remembers the dead used below, so their records stay.
    for (const CharacterId remembered : {father, mother, m, n}) {
        REQUIRE(r.add_modifier(living, remembered, ModifierId{9}, 0) == EditResult::Ok);
    }
    REQUIRE(r.kill(father, Date{0}, ctx(stances)).result == EditResult::Ok);

    CHECK(r.link(father, R::Child, child) == EditResult::Ok); // born after the father's death
    CHECK(r.has_relation(child, R::Parent, father));
    CHECK(r.link(father, R::Liege, living) == EditResult::Invalid);
    CHECK(r.link(living, R::Employee, father) == EditResult::Invalid);
    CHECK(r.unlink(father, R::Liege, living) == EditResult::Invalid);
    CHECK(r.set_relation(living, R::Friend, father) == EditResult::Invalid);
    CHECK(r.set_relation(father, R::Rival, living) == EditResult::Invalid);
    CHECK(r.clear_relation(living, R::Friend, father) == EditResult::Invalid);
    // Order: static Invalid, then NotFound, then the dead participant.
    CHECK(r.link(father, R::Liege, CharacterId{99}) == EditResult::NotFound);
    CHECK(r.set_relation(father, R::Friend, CharacterId{99}) == EditResult::NotFound);
    CHECK(r.link(father, R::Friend, living) == EditResult::Invalid); // wrong kind
    CHECK(r.link(father, R::Child, child) == EditResult::Duplicate);

    // MAX_PARENTS counts dead parents.
    REQUIRE(r.link(mother, R::Child, child) == EditResult::Ok);
    CHECK(r.link(stepfather, R::Child, child) == EditResult::Full);
    REQUIRE(r.kill(mother, Date{1}, ctx(stances)).result == EditResult::Ok);
    CHECK(r.link(stepfather, R::Child, child) == EditResult::Full);
    CHECK(r.parents(child).size() == 2);

    // Two dead characters, a surviving type; Conflict still applies.
    REQUIRE(r.kill(m, Date{2}, ctx(stances)).result == EditResult::Ok);
    REQUIRE(r.kill(n, Date{3}, ctx(stances)).result == EditResult::Ok);
    CHECK(r.link(m, R::Spouse, n) == EditResult::Ok);
    CHECK(r.has_relation(n, R::Spouse, m));
    CHECK(r.link(m, R::Parent, n) == EditResult::Ok);
    CHECK(r.link(m, R::Child, n) == EditResult::Conflict);
    CHECK(r.unlink(m, R::Spouse, n) == EditResult::Ok);
    CHECK(r.unlink(father, R::Child, child) == EditResult::Ok);
}

// ---- opinions ------------------------------------------------------------------------------

TEST_CASE("kill: own opinions go; modifiers about the deceased stay; dead targets allowed, dead holders Invalid") {
    const StanceTable stances;
    CharacterRegistry r = make_registry(3, CharacterInit{.extraversion = 100});
    const CharacterId a{1}, b{2}, c{3};
    REQUIRE(r.add_long_opinion(a, b, 40).outcome == O::Created);
    REQUIRE(r.add_long_opinion(a, topic(1), 40).outcome == O::Created);
    REQUIRE(r.add_modifier(a, b, ModifierId{1}, 10) == EditResult::Ok);
    REQUIRE(r.add_modifier(b, a, ModifierId{1}, -30) == EditResult::Ok);
    REQUIRE(r.add_modifier(b, a, ModifierId{2}, -20) == EditResult::Ok);
    REQUIRE(r.add_modifier(b, c, ModifierId{2}, 5) == EditResult::Ok);

    REQUIRE(r.kill(a, Date{1}, ctx(stances)).result == EditResult::Ok);
    // a's opinions of others are gone; b's modifiers about a stay.
    CHECK(short_opinion(*r.find(b), a) == -50);
    CHECK(r.find(b)->modifiers().size() == 3);
    const OpinionBreakdown o = opinion_breakdown(*r.find(b), *r.find_dead(a), stances, CONFIG, SEED);
    CHECK(o.short_term == -50);
    CHECK(o.total == std::clamp(weak_opinion(*r.find(b), *r.find_dead(a), stances, CONFIG, SEED) - 50.0, -100.0,
                                100.0));

    // A dead target works.
    CHECK(r.add_modifier(c, a, ModifierId{3}, 12) == EditResult::Ok);
    CHECK(r.remove_modifier(b, a, ModifierId{2}) == EditResult::Ok);
    CHECK(r.add_long_opinion(c, a, 25).outcome == O::Created);
    CHECK(long_opinion(*r.find(c), a) == 25);
    // A dead holder is Invalid, after NotFound.
    CHECK(r.add_modifier(a, c, ModifierId{1}, 1) == EditResult::Invalid);
    CHECK(r.add_modifier(a, topic(1), ModifierId{1}, 1) == EditResult::Invalid);
    CHECK(r.remove_modifier(a, b, ModifierId{1}) == EditResult::Invalid);
    CHECK(r.remove_modifier(a, topic(1), ModifierId{1}) == EditResult::Invalid);
    CHECK(r.add_long_opinion(a, c, 5).outcome == O::Invalid);
    CHECK(r.add_long_opinion(a, topic(1), 5).outcome == O::Invalid);
    CHECK(r.add_modifier(a, CharacterId{9}, ModifierId{1}, 1) == EditResult::NotFound);
    CHECK(r.add_long_opinion(a, CharacterId{9}, 5).outcome == O::NotFound);
    CHECK(r.maintain() == 0); // skips the dead
}

namespace {

// A world where a's weak opinion of b moves a lot at b's death: communities, compat and
// reputation all change.
struct RebaseWorld {
    StanceTable stances;
    CharacterRegistry r;
    CharacterId a, b, c;

    RebaseWorld() {
        REQUIRE(stances.set_stance(CommunityId{1}, community(2), 60) == EditResult::Ok);
        REQUIRE(stances.set_stance(CommunityId{1}, community(3), -45) == EditResult::Ok);
        a = r.create(NameId{1}, Gender::Female, Date{0},
                     CharacterInit{.stability = 80, .openness = 70, .extraversion = 90, .agreeableness = 60});
        b = r.create(NameId{2}, Gender::Male, Date{0},
                     CharacterInit{.stability = 75, .openness = 65, .extraversion = 85, .agreeableness = 50,
                                   .reputation = -30});
        c = r.create(NameId{3}, Gender::Female, Date{0}, CharacterInit{.stability = -60});
        REQUIRE(r.find(a)->set_involvement(CommunityId{1}, 3) == EditResult::Ok);
        REQUIRE(r.find(c)->set_involvement(CommunityId{1}, 1) == EditResult::Ok);
        REQUIRE(r.find(b)->set_involvement(CommunityId{2}, 5) == EditResult::Ok);
        REQUIRE(r.find(b)->set_involvement(CommunityId{3}, 4) == EditResult::Ok);
    }

    // round(weak_before - weak_after) for holder h about b, computed without kill().
    int expected_delta(CharacterId h) {
        const double before = weak_opinion(*r.find(h), *r.find(b), stances, CONFIG, SEED);
        const double after = weak_opinion(*r.find(h), expected_record(*r.find(b), Date{50}), stances, CONFIG, SEED);
        return static_cast<int>(std::round(before - after));
    }
};

} // namespace

TEST_CASE("rebase: opinions move by at most 0.5 unless long saturates; 0 removes; no entry means the base") {
    SUBCASE("rebased entry") {
        RebaseWorld w;
        REQUIRE(w.r.add_long_opinion(w.a, w.b, 30).outcome == O::Created);
        REQUIRE(w.r.add_modifier(w.a, w.b, ModifierId{1}, -15) == EditResult::Ok);
        const int delta = w.expected_delta(w.a);
        const double before = opinion(*w.r.find(w.a), *w.r.find(w.b), w.stances, CONFIG, SEED);
        MESSAGE("rebase delta = " << delta << ", opinion before = " << before);
        REQUIRE(std::abs(delta) >= 5); // the setup really moves the weak opinion
        REQUIRE(w.r.kill(w.b, Date{50}, ctx(w.stances)).result == EditResult::Ok);
        CHECK(long_opinion(*w.r.find(w.a), w.b) == 30 + delta);
        const double after = opinion(*w.r.find(w.a), *w.r.find_dead(w.b), w.stances, CONFIG, SEED);
        CHECK(std::abs(after - before) <= 0.5);
    }
    SUBCASE("a rebase to 0 removes the entry") {
        RebaseWorld w;
        const int delta = w.expected_delta(w.a);
        REQUIRE(delta != 0);
        REQUIRE(w.r.add_long_opinion(w.a, w.b, -delta).outcome == O::Created);
        REQUIRE(w.r.kill(w.b, Date{50}, ctx(w.stances)).result == EditResult::Ok);
        CHECK(long_opinion(*w.r.find(w.a), w.b) == 0);
        CHECK(w.r.find(w.a)->long_people().empty());
        CHECK(!w.r.exists(w.b)); // that entry was its last holder
    }
    SUBCASE("saturated long") {
        RebaseWorld w;
        const int delta = w.expected_delta(w.a);
        REQUIRE(delta != 0);
        const int saturated = delta > 0 ? 200 : -200;
        REQUIRE(w.r.add_long_opinion(w.a, w.b, saturated).outcome == O::Created);
        REQUIRE(w.r.kill(w.b, Date{50}, ctx(w.stances)).result == EditResult::Ok);
        CHECK(long_opinion(*w.r.find(w.a), w.b) == saturated); // clamped, so the opinion may move
    }
    SUBCASE("without a long entry the opinion is the dead-record base") {
        RebaseWorld w;
        REQUIRE(w.r.add_long_opinion(w.a, w.c, 20).outcome == O::Created); // about someone else
        REQUIRE(w.r.add_modifier(w.c, w.b, ModifierId{1}, 0) == EditResult::Ok); // keeps b remembered, adds 0
        REQUIRE(w.r.kill(w.b, Date{50}, ctx(w.stances)).result == EditResult::Ok);
        const DeadRecord& d = *w.r.find_dead(w.b);
        CHECK(long_opinion(*w.r.find(w.c), w.b) == 0);
        CHECK(opinion(*w.r.find(w.c), d, w.stances, CONFIG, SEED) == weak_opinion(*w.r.find(w.c), d, w.stances, CONFIG, SEED));
        CHECK(long_opinion(*w.r.find(w.a), w.c) == 20);
    }
}

// ---- weak opinion of a dead character --------------------------------------------------------

TEST_CASE("weak opinion of a dead character: hand-computed; noise is identical before and after death") {
    StanceTable stances;
    // D's main community is 6 (weight 9), whose parent is 5. A is in 1 (weight 3) and 2 (weight 1),
    // 2's parent is 3. Stances: 1 -> 5 = 40 (inherited through 6's chain), 3 -> 6 = -80.
    REQUIRE(stances.set_parent(CommunityId{6}, CommunityId{5}) == EditResult::Ok);
    REQUIRE(stances.set_parent(CommunityId{2}, CommunityId{3}) == EditResult::Ok);
    REQUIRE(stances.set_stance(CommunityId{1}, community(5), 40) == EditResult::Ok);
    REQUIRE(stances.set_stance(CommunityId{3}, community(6), -80) == EditResult::Ok);
    CharacterRegistry r;
    const CharacterId a = r.create(NameId{1}, Gender::Female, Date{0}, CharacterInit{.openness = -100});
    const CharacterId d = r.create(NameId{2}, Gender::Male, Date{0}, CharacterInit{.reputation = 20});
    REQUIRE(r.find(a)->set_involvement(CommunityId{1}, 3) == EditResult::Ok);
    REQUIRE(r.find(a)->set_involvement(CommunityId{2}, 1) == EditResult::Ok);
    REQUIRE(r.find(d)->set_involvement(CommunityId{6}, 9) == EditResult::Ok);
    REQUIRE(r.find(d)->set_involvement(CommunityId{8}, 2) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, d, ModifierId{1}, 0) == EditResult::Ok); // a remembers d; weak unaffected
    const WeakOpinionBreakdown alive = weak_opinion_breakdown(*r.find(a), *r.find(d), stances, CONFIG, SEED);

    REQUIRE(r.kill(d, Date{9}, ctx(stances)).result == EditResult::Ok);
    const WeakOpinionBreakdown dead = weak_opinion_breakdown(*r.find(a), *r.find_dead(d), stances, CONFIG, SEED);
    CHECK(dead.community == (3.0 * 40.0 + 1.0 * -80.0) / 4.0); // 10
    CHECK(dead.reputation == 5.0);                             // 0.25 * 20
    CHECK(dead.compat == 0.0);
    CHECK(dead.noise == CONFIG.k_noise * noise(SEED, a, NoiseSubject::Person, d.value));
    CHECK(dead.noise == alive.noise); // same inputs; no openness factor for persons
    CHECK(dead.total == std::clamp(15.0 + dead.noise, -100.0, 100.0));
    CHECK(weak_opinion(*r.find(a), *r.find_dead(d), stances, CONFIG, SEED) == dead.total);

    // Without a main community the community term is 0.
    const CharacterId e = r.create(NameId{3}, Gender::Male, Date{0}, CharacterInit{});
    REQUIRE(r.add_modifier(a, e, ModifierId{1}, 0) == EditResult::Ok);
    REQUIRE(r.kill(e, Date{9}, ctx(stances)).result == EditResult::Ok);
    CHECK(weak_opinion_breakdown(*r.find(a), *r.find_dead(e), stances, CONFIG, SEED).community == 0.0);
}


// ---- property test against a reference model ----------------------------------------------

namespace {

constexpr std::uint32_t MAX_IDS = 160;

// Naive lifecycle model: status per id (living, dead, forgotten), relation masks per ordered
// pair, long-term people entries and person modifiers per holder. Holders are recounted from
// scratch and dead ids without holders are forgotten by a global sweep after every change.
// Weak opinions come from the tested functions.
struct LifecycleModel {
    struct Person {
        bool dead = false;
        bool forgotten = false;
        int extraversion = 100;
        int reputation = 0;
        std::map<std::uint32_t, int> longs;                                // target -> value
        std::map<std::pair<std::uint32_t, std::uint16_t>, int> modifiers; // (target, modifier) -> effect
    };
    std::vector<Person> persons; // index = id - 1
    std::map<std::pair<std::uint32_t, std::uint32_t>, std::uint32_t> masks;

    bool exists(CharacterId id) const {
        return id.valid() && id.value <= persons.size() && !persons[id.value - 1].forgotten;
    }
    bool dead(CharacterId id) const { return persons[id.value - 1].dead; }
    bool living(CharacterId id) const { return exists(id) && !dead(id); }
    std::uint32_t mask(std::uint32_t a, std::uint32_t b) const {
        const auto it = masks.find({a, b});
        return it == masks.end() ? 0 : it->second;
    }
    void set_bits(std::uint32_t a, std::uint32_t b, std::uint32_t bits) { masks[{a, b}] |= bits; }
    void clear_bits(std::uint32_t a, std::uint32_t b, std::uint32_t bits) {
        const auto it = masks.find({a, b});
        if (it == masks.end()) {
            return;
        }
        it->second &= ~bits;
        if (it->second == 0) {
            masks.erase(it);
        }
    }
    std::size_t parent_count(std::uint32_t child) const {
        std::size_t count = 0;
        for (const auto& [key, m] : masks) {
            count += key.first == child && (m & relation_bit(R::Parent)) != 0;
        }
        return count;
    }
    static std::size_t limit(int extraversion) {
        return 8 + static_cast<std::size_t>((32 * (extraversion + 100) + 100) / 200);
    }

    // holders[id - 1]: distinct living characters referencing id.
    std::vector<std::uint32_t> recount() const {
        std::vector<std::vector<std::uint32_t>> targets(persons.size());
        for (std::uint32_t h = 1; h <= persons.size(); ++h) {
            const Person& p = persons[h - 1];
            if (p.dead || p.forgotten) {
                continue;
            }
            for (const auto& [t, v] : p.longs) {
                targets[h - 1].push_back(t);
            }
            for (const auto& [key, e] : p.modifiers) {
                targets[h - 1].push_back(key.first);
            }
        }
        for (const auto& [key, m] : masks) {
            const Person& p = persons[key.first - 1];
            if (!p.dead && !p.forgotten) {
                targets[key.first - 1].push_back(key.second);
            }
        }
        std::vector<std::uint32_t> counts(persons.size(), 0);
        for (auto& list : targets) {
            std::sort(list.begin(), list.end());
            list.erase(std::unique(list.begin(), list.end()), list.end());
            for (const std::uint32_t t : list) {
                ++counts[t - 1];
            }
        }
        return counts;
    }

    // Forgets every dead id without holders; returns them in id order. `counts` receives the
    // recount (forgetting changes no living holder's references, so it stays valid).
    std::vector<std::uint32_t> sweep(std::vector<std::uint32_t>& counts) {
        counts = recount();
        std::vector<std::uint32_t> forgotten;
        for (std::uint32_t id = 1; id <= persons.size(); ++id) {
            Person& p = persons[id - 1];
            if (p.dead && !p.forgotten && counts[id - 1] == 0) {
                p.forgotten = true;
                forgotten.push_back(id);
                for (auto it = masks.begin(); it != masks.end();) {
                    it = (it->first.first == id || it->first.second == id) ? masks.erase(it) : std::next(it);
                }
            }
        }
        return forgotten;
    }

    EditResult precheck(CharacterId a, R type, CharacterId b, RelationKind kind) const {
        const std::optional<RelationInfo> info = relation_info(type);
        if (!a.valid() || !b.valid() || a == b || !info || info->kind != kind) {
            return EditResult::Invalid;
        }
        if (!exists(a) || !exists(b)) {
            return EditResult::NotFound;
        }
        if ((dead(a) || dead(b)) && !(kind == RelationKind::Paired && survives_death(type))) {
            return EditResult::Invalid;
        }
        return EditResult::Ok;
    }

    EditResult link(CharacterId a, R type, CharacterId b) {
        const EditResult pre = precheck(a, type, b, RelationKind::Paired);
        if (pre != EditResult::Ok) {
            return pre;
        }
        const R complement = *relation_info(type)->complement;
        const std::uint32_t forward = mask(a.value, b.value);
        const std::uint32_t backward = mask(b.value, a.value);
        if ((forward & relation_bit(type)) != 0) {
            return EditResult::Duplicate;
        }
        if (complement != type
            && ((forward & relation_bit(complement)) != 0 || (backward & relation_bit(type)) != 0)) {
            return EditResult::Conflict;
        }
        if ((type == R::Child && parent_count(b.value) >= MAX_PARENTS)
            || (type == R::Parent && parent_count(a.value) >= MAX_PARENTS)) {
            return EditResult::Full;
        }
        set_bits(a.value, b.value, relation_bit(type));
        set_bits(b.value, a.value, relation_bit(complement));
        return EditResult::Ok;
    }

    EditResult set_relation(CharacterId a, R type, CharacterId b) {
        const EditResult pre = precheck(a, type, b, RelationKind::OneWay);
        if (pre != EditResult::Ok) {
            return pre;
        }
        if ((mask(a.value, b.value) & relation_bit(type)) != 0) {
            return EditResult::Duplicate;
        }
        set_bits(a.value, b.value, relation_bit(type));
        return EditResult::Ok;
    }

    static O add_long(std::map<std::uint32_t, int>& longs, std::size_t lim, std::uint32_t target, int delta,
                      std::uint32_t& evicted) {
        evicted = 0;
        delta = std::clamp(delta, -400, 400);
        const auto it = longs.find(target);
        if (it != longs.end()) {
            const int next = std::clamp(it->second + delta, -200, 200);
            if (next == it->second) {
                return O::Unchanged;
            }
            if (next == 0) {
                longs.erase(it);
                return O::Removed;
            }
            it->second = next;
            return O::Updated;
        }
        if (delta == 0) {
            return O::Unchanged;
        }
        const int value = std::clamp(delta, -200, 200);
        if (longs.size() >= lim) {
            auto weakest = longs.begin();
            for (auto e = longs.begin(); e != longs.end(); ++e) {
                if (std::abs(e->second) < std::abs(weakest->second)) {
                    weakest = e;
                }
            }
            if (std::abs(value) <= std::abs(weakest->second)) {
                return O::Dropped;
            }
            evicted = weakest->first;
            longs.erase(weakest);
            longs[target] = value;
            return O::CreatedWithEviction;
        }
        longs[target] = value;
        return O::Created;
    }

    std::size_t maintain() {
        std::size_t evicted = 0;
        for (Person& p : persons) {
            if (p.dead || p.forgotten) {
                continue;
            }
            while (p.longs.size() > limit(p.extraversion)) {
                auto weakest = p.longs.begin();
                for (auto e = p.longs.begin(); e != p.longs.end(); ++e) {
                    if (std::abs(e->second) < std::abs(weakest->second)) {
                        weakest = e;
                    }
                }
                p.longs.erase(weakest);
                ++evicted;
            }
        }
        return evicted;
    }
};

const LifecycleConfig PROPERTY_LIFECYCLE{.fame_per_reputation = 1, .legendary_fame = 60};

} // namespace

TEST_CASE("property: lifecycle (creates, kills, links, opinions, holders, forgetting) matches a reference model") {
    constexpr int STEPS = 5000;
    std::mt19937 rng(20260917u);
    const auto pick = [&](std::uint32_t n) { return static_cast<std::uint32_t>(rng() % n); };

    StanceTable stances;
    for (std::uint32_t from = 1; from <= 4; ++from) {
        for (std::uint32_t to = 1; to <= 4; ++to) {
            REQUIRE(stances.set_stance(CommunityId{from}, community(to), static_cast<int>(pick(201)) - 100)
                    == EditResult::Ok);
        }
    }
    const WorldContext context{stances, CONFIG, PROPERTY_LIFECYCLE, SEED};
    CharacterRegistry reg;
    LifecycleModel model;
    const auto create = [&] {
        const auto trait = [&] { return static_cast<int>(pick(201)) - 100; };
        const int reputation = trait();
        const CharacterId id = reg.create(NameId{static_cast<std::uint32_t>(model.persons.size() + 1)}, Gender::Female,
                                          Date{0},
                                          CharacterInit{.stability = trait(), .openness = trait(), .extraversion = 100,
                                                        .conscientiousness = trait(), .agreeableness = trait(),
                                                        .reputation = reputation});
        REQUIRE(id.value == model.persons.size() + 1);
        model.persons.emplace_back();
        model.persons.back().reputation = reputation;
        Character& c = *reg.find(id);
        for (std::uint32_t k = 0, n = pick(3); k < n; ++k) {
            (void)c.set_involvement(CommunityId{1 + pick(4)}, 1 + pick(255));
        }
    };
    for (int i = 0; i < 12; ++i) {
        create();
    }
    const auto random_id = [&] {
        const std::uint32_t r = pick(40);
        if (r == 0) {
            return CharacterId{};
        }
        if (r == 1) {
            return CharacterId{static_cast<std::uint32_t>(model.persons.size() + 1)};
        }
        return CharacterId{1 + pick(static_cast<std::uint32_t>(model.persons.size()))};
    };
    const std::uint32_t one_way_bits = relation_bit(R::Friend) | relation_bit(R::Rival) | relation_bit(R::Attraction);
    // Compares the registry with the model; one assertion per id (the property test runs
    // under ASan, so per-field assertions would dominate its time).
    const auto id_matches = [&](std::uint32_t id, const std::vector<std::uint32_t>& counts) {
        const CharacterId cid{id};
        const LifecycleModel::Person& p = model.persons[id - 1];
        if (reg.exists(cid) != !p.forgotten || reg.holders(cid) != (p.forgotten ? 0u : counts[id - 1])
            || p.forgotten != (p.dead && counts[id - 1] == 0) // forgotten exactly when dead and unheld
            || (reg.find(cid) != nullptr) != (!p.dead && !p.forgotten)
            || (reg.find_dead(cid) != nullptr) != (p.dead && !p.forgotten)) {
            return false;
        }
        const std::span<const RelationEdge> edges = reg.relations(cid);
        auto it = model.masks.lower_bound({id, 0});
        for (const RelationEdge& e : edges) {
            if (it == model.masks.end() || it->first.first != id || e.other.value != it->first.second
                || e.mask != it->second) {
                return false;
            }
            if ((p.dead || p.forgotten) && (e.mask & one_way_bits) != 0) {
                return false; // the dead never hold one-way edges
            }
            ++it;
        }
        if (it != model.masks.end() && it->first.first == id) {
            return false;
        }
        if (p.dead || p.forgotten) {
            return !p.forgotten || edges.empty();
        }
        const Character& c = *reg.find(cid);
        if (c.long_people().size() != p.longs.size() || c.modifiers().size() != p.modifiers.size()) {
            return false;
        }
        auto lit = p.longs.begin();
        for (const LongOpinion& e : c.long_people()) {
            if (e.target != lit->first || e.value != lit->second) {
                return false;
            }
            ++lit;
        }
        auto mit = p.modifiers.begin();
        for (const OpinionModifier& m : c.modifiers()) {
            if (m.target != mit->first.first || m.modifier.value != mit->first.second || m.effect != mit->second) {
                return false;
            }
            ++mit;
        }
        return true;
    };
    const auto check_all = [&](const std::vector<std::uint32_t>& counts) {
        std::vector<CharacterId> living;
        bool all = true;
        for (std::uint32_t id = 1; id <= model.persons.size(); ++id) {
            if (!id_matches(id, counts)) {
                CAPTURE(id);
                CAPTURE(reg.holders(CharacterId{id}));
                CAPTURE(counts[id - 1]);
                all = false;
                CHECK(id_matches(id, counts));
            }
            if (model.living(CharacterId{id})) {
                living.push_back(CharacterId{id});
            }
        }
        REQUIRE(all);
        REQUIRE(ids_of(reg.characters()) == living);
    };

    std::size_t kills = 0, kills_with_surviving = 0, kills_unlinking = 0, kills_clearing_one_way = 0;
    std::size_t rebased = 0, rebase_removed = 0, posthumous_links = 0, dead_rejections = 0, legendary = 0;
    std::size_t forgotten_immediately = 0, forgotten_by_kill = 0, forgotten_by_long = 0, forgotten_by_eviction = 0,
                forgotten_by_trim = 0, forgotten_by_modifier = 0, forgotten_by_unlink = 0;

    for (int step = 0; step < STEPS; ++step) {
        CAPTURE(step);
        const std::uint32_t op = pick(100);
        const CharacterId a = random_id();
        const CharacterId b = random_id();
        std::size_t* forget_counter = nullptr; // what a forgetting in this step is attributed to
        CharacterId killed{};
        if (op < 4) {
            if (model.persons.size() < MAX_IDS) {
                create();
            }
        } else if (op < 9) { // kill
            if (!model.living(a)) {
                REQUIRE(reg.kill(a, Date{10}, context).result
                        == (a.valid() ? EditResult::NotFound : EditResult::Invalid));
                continue;
            }
            const Character& victim = *reg.find(a);
            const DeadRecord record = expected_record(victim, Date{10});
            std::vector<std::pair<CharacterId, int>> deltas;
            for (const Character& holder : reg.characters()) {
                if (holder.id() == a) {
                    continue;
                }
                const double before = weak_opinion(holder, victim, stances, CONFIG, SEED);
                const double after = weak_opinion(holder, record, stances, CONFIG, SEED);
                deltas.emplace_back(holder.id(), static_cast<int>(detail::clamp_round(before - after, -400, 400)));
            }
            // Sometimes make one holder's entry cancel exactly, so rebases reach 0.
            if (!deltas.empty() && pick(2) == 0) {
                const auto [holder, delta] = deltas[pick(static_cast<std::uint32_t>(deltas.size()))];
                if (delta != 0) {
                    LifecycleModel::Person& h = model.persons[holder.value - 1];
                    const int current = h.longs.count(a.value) != 0 ? h.longs[a.value] : 0;
                    std::uint32_t evicted = 0;
                    const O expected = LifecycleModel::add_long(h.longs, LifecycleModel::limit(h.extraversion),
                                                                a.value, -delta - current, evicted);
                    const LongOpinionResult<CharacterId> actual = reg.add_long_opinion(holder, a, -delta - current);
                    REQUIRE(actual.outcome == expected);
                    REQUIRE(actual.evicted.value == evicted);
                    std::vector<std::uint32_t> counts;
                    forgotten_by_eviction += model.sweep(counts).size();
                    check_all(counts);
                }
            }
            const std::uint32_t holders_before = model.recount()[a.value - 1];
            REQUIRE(reg.holders(a) == holders_before);
            const auto expected_fame = static_cast<std::uint16_t>(
                std::min<std::int64_t>(std::int64_t{holders_before} + std::abs(model.persons[a.value - 1].reputation), 0xFFFF));
            // Expected relation fates.
            bool surviving = false, unlinking = false, clearing = false;
            for (auto it = model.masks.begin(); it != model.masks.end();) {
                const auto [from, to] = it->first;
                std::uint32_t& m = it->second;
                if (from == a.value || to == a.value) {
                    std::uint32_t ending = 0;
                    for (unsigned t = 0; t < 32; ++t) {
                        const auto type = static_cast<R>(t);
                        if ((m & (std::uint32_t{1} << t)) == 0) {
                            continue;
                        }
                        if (survives_death(type)) {
                            surviving = true;
                            continue;
                        }
                        ending |= std::uint32_t{1} << t;
                        (relation_info(type)->kind == RelationKind::Paired ? unlinking : clearing) = true;
                    }
                    m &= ~ending;
                    if (m == 0) {
                        it = model.masks.erase(it);
                        continue;
                    }
                }
                ++it;
            }
            for (const auto& [holder, delta] : deltas) {
                LifecycleModel::Person& h = model.persons[holder.value - 1];
                if (h.longs.count(a.value) != 0) {
                    ++rebased;
                    std::uint32_t evicted = 0;
                    rebase_removed +=
                        LifecycleModel::add_long(h.longs, LifecycleModel::limit(h.extraversion), a.value, delta,
                                                 evicted)
                        == O::Removed;
                }
            }
            model.persons[a.value - 1].dead = true;
            model.persons[a.value - 1].longs.clear();
            model.persons[a.value - 1].modifiers.clear();
            const KillResult result = reg.kill(a, Date{10}, context);
            REQUIRE(result.result == EditResult::Ok);
            REQUIRE(result.fame == expected_fame);
            REQUIRE(result.legendary == (expected_fame >= PROPERTY_LIFECYCLE.legendary_fame));
            legendary += result.legendary;
            if (const DeadRecord* d = reg.find_dead(a)) {
                REQUIRE(d->fame == expected_fame);
            }
            ++kills;
            kills_with_surviving += surviving;
            kills_unlinking += unlinking;
            kills_clearing_one_way += clearing;
            killed = a;
            forget_counter = &forgotten_by_kill;
        } else if (op < 36) { // link or unlink, mostly surviving types
            const std::array<R, 7> types{R::Child, R::Parent, R::Spouse, R::Spouse, R::Liege, R::Employer, R::Friend};
            const R type = types[pick(7)];
            if (pick(4) == 0) {
                EditResult expected = model.precheck(a, type, b, RelationKind::Paired);
                if (expected == EditResult::Ok) {
                    const std::uint32_t bit = relation_bit(type);
                    if ((model.mask(a.value, b.value) & bit) == 0) {
                        expected = EditResult::NotFound;
                    } else {
                        model.clear_bits(a.value, b.value, bit);
                        model.clear_bits(b.value, a.value, relation_bit(*relation_info(type)->complement));
                    }
                }
                REQUIRE(reg.unlink(a, type, b) == expected);
                forget_counter = &forgotten_by_unlink;
            } else {
                const EditResult expected = model.link(a, type, b);
                REQUIRE(reg.link(a, type, b) == expected);
                if (expected == EditResult::Ok && (model.dead(a) || model.dead(b))) {
                    ++posthumous_links;
                }
                if (expected == EditResult::Invalid && model.exists(a) && model.exists(b)
                    && (model.dead(a) || model.dead(b)) && relation_info(type)
                    && relation_info(type)->kind == RelationKind::Paired && a != b) {
                    ++dead_rejections;
                }
            }
        } else if (op < 52) { // one-way relations
            const R type = static_cast<R>(pick(3));
            if (pick(4) == 0) {
                EditResult expected = model.precheck(a, type, b, RelationKind::OneWay);
                if (expected == EditResult::Ok) {
                    if ((model.mask(a.value, b.value) & relation_bit(type)) == 0) {
                        expected = EditResult::NotFound;
                    } else {
                        model.clear_bits(a.value, b.value, relation_bit(type));
                    }
                }
                REQUIRE(reg.clear_relation(a, type, b) == expected);
            } else {
                const EditResult expected = model.set_relation(a, type, b);
                REQUIRE(reg.set_relation(a, type, b) == expected);
                if (expected == EditResult::Invalid && a.valid() && b.valid() && a != b && model.exists(a)
                    && model.exists(b)) {
                    ++dead_rejections;
                }
            }
        } else if (op < 68) { // modifiers
            const ModifierId m{static_cast<std::uint16_t>(1 + pick(2))};
            const int effect = static_cast<int>(pick(201)) - 100;
            const bool remove = pick(3) == 0;
            EditResult expected = EditResult::Invalid;
            if (a.valid() && b.valid() && a != b) {
                if (!model.exists(a) || !model.exists(b)) {
                    expected = EditResult::NotFound;
                } else if (model.dead(a)) {
                    expected = EditResult::Invalid;
                    ++dead_rejections;
                } else {
                    auto& mods = model.persons[a.value - 1].modifiers;
                    const auto key = std::pair{b.value, m.value};
                    if (remove) {
                        expected = mods.erase(key) != 0 ? EditResult::Ok : EditResult::NotFound;
                    } else if (mods.count(key) != 0) {
                        expected = EditResult::Duplicate;
                    } else if (mods.size() >= MODIFIER_CAP) {
                        expected = EditResult::Full;
                    } else {
                        mods[key] = effect;
                        expected = EditResult::Ok;
                    }
                }
            }
            if (remove) {
                REQUIRE(reg.remove_modifier(a, b, m) == expected);
                forget_counter = &forgotten_by_modifier;
            } else {
                REQUIRE(reg.add_modifier(a, b, m, effect) == expected);
            }
        } else if (op < 92) { // long opinions
            const int delta = pick(10) == 0 ? 0 : static_cast<int>(pick(161)) - 80;
            O expected = O::Invalid;
            std::uint32_t evicted = 0;
            if (a.valid() && b.valid() && a != b) {
                if (!model.exists(a) || !model.exists(b)) {
                    expected = O::NotFound;
                } else if (model.dead(a)) {
                    expected = O::Invalid;
                    ++dead_rejections;
                } else {
                    LifecycleModel::Person& p = model.persons[a.value - 1];
                    expected = LifecycleModel::add_long(p.longs, LifecycleModel::limit(p.extraversion), b.value, delta,
                                                        evicted);
                }
            }
            const LongOpinionResult<CharacterId> actual = reg.add_long_opinion(a, b, delta);
            REQUIRE(actual.outcome == expected);
            REQUIRE(actual.evicted.value == evicted);
            forget_counter = expected == O::CreatedWithEviction ? &forgotten_by_eviction : &forgotten_by_long;
        } else if (op < 96) { // remove every reference to a remembered dead character, kind by kind
            const std::vector<std::uint32_t> counts = model.recount();
            std::vector<std::uint32_t> candidates;
            for (std::uint32_t id = 1; id <= model.persons.size(); ++id) {
                const LifecycleModel::Person& p = model.persons[id - 1];
                if (p.dead && !p.forgotten && counts[id - 1] == 1) {
                    candidates.push_back(id);
                }
            }
            if (!candidates.empty()) {
                const CharacterId dead_id{candidates[pick(static_cast<std::uint32_t>(candidates.size()))]};
                CharacterId holder{};
                for (const Character& c : reg.characters()) {
                    const LifecycleModel::Person& h = model.persons[c.id().value - 1];
                    const bool refs = h.longs.count(dead_id.value) != 0 || model.mask(c.id().value, dead_id.value) != 0
                                   || std::any_of(h.modifiers.begin(), h.modifiers.end(),
                                                  [&](const auto& kv) { return kv.first.first == dead_id.value; });
                    if (refs) {
                        holder = c.id();
                    }
                }
                REQUIRE(holder.valid());
                LifecycleModel::Person& h = model.persons[holder.value - 1];
                std::array<int, 3> kinds{0, 1, 2}; // long entry, modifiers, edges; random order
                for (std::size_t i = kinds.size() - 1; i > 0; --i) {
                    std::swap(kinds[i], kinds[pick(static_cast<std::uint32_t>(i + 1))]);
                }
                for (const int kind : kinds) {
                    std::size_t* counter = nullptr;
                    if (kind == 0 && h.longs.count(dead_id.value) != 0) {
                        const int value = h.longs[dead_id.value];
                        std::uint32_t evicted = 0;
                        REQUIRE(LifecycleModel::add_long(h.longs, 40, dead_id.value, -value, evicted) == O::Removed);
                        REQUIRE(reg.add_long_opinion(holder, dead_id, -value).outcome == O::Removed);
                        counter = &forgotten_by_long;
                    } else if (kind == 1) {
                        for (auto it = h.modifiers.begin(); it != h.modifiers.end();) {
                            if (it->first.first == dead_id.value) {
                                REQUIRE(reg.remove_modifier(holder, dead_id, ModifierId{it->first.second})
                                        == EditResult::Ok);
                                it = h.modifiers.erase(it);
                                counter = &forgotten_by_modifier;
                            } else {
                                ++it;
                            }
                        }
                    } else if (kind == 2) {
                        const std::uint32_t bits = model.mask(holder.value, dead_id.value);
                        for (unsigned t = 0; t < 32; ++t) {
                            if ((bits & (std::uint32_t{1} << t)) != 0) {
                                const auto type = static_cast<R>(t);
                                REQUIRE(reg.unlink(holder, type, dead_id) == EditResult::Ok);
                                model.clear_bits(holder.value, dead_id.value, relation_bit(type));
                                model.clear_bits(dead_id.value, holder.value,
                                                 relation_bit(*relation_info(type)->complement));
                                counter = &forgotten_by_unlink;
                            }
                        }
                    }
                    std::vector<std::uint32_t> after;
                    const std::vector<std::uint32_t> gone = model.sweep(after);
                    if (!gone.empty()) {
                        REQUIRE(counter != nullptr);
                        *counter += gone.size();
                    }
                }
                REQUIRE(!reg.exists(dead_id));
            }
        } else if (op < 97) { // extraversion change
            if (model.living(a)) {
                const int extraversion = static_cast<int>(pick(201)) - 100;
                reg.find(a)->set_extraversion(extraversion);
                model.persons[a.value - 1].extraversion = extraversion;
            }
        } else { // maintain
            REQUIRE(reg.maintain() == model.maintain());
            forget_counter = &forgotten_by_trim;
        }

        std::vector<std::uint32_t> counts;
        const std::vector<std::uint32_t> gone = model.sweep(counts);
        if (!gone.empty()) {
            REQUIRE(forget_counter != nullptr); // only these changes can forget
            for (const std::uint32_t id : gone) {
                if (CharacterId{id} == killed) {
                    ++forgotten_immediately;
                } else {
                    ++*forget_counter;
                }
            }
        }
        check_all(counts); // holders against a full recount after every step
    }

    MESSAGE("lifecycle property: " << STEPS << " steps, " << model.persons.size() << " ids; kills " << kills
                                   << " (with surviving links " << kills_with_surviving << ", unlinking "
                                   << kills_unlinking << ", clearing one-way " << kills_clearing_one_way
                                   << ", legendary " << legendary << "); rebased " << rebased << " (removed "
                                   << rebase_removed << "); posthumous links " << posthumous_links
                                   << "; dead rejections " << dead_rejections << "; forgotten: immediately "
                                   << forgotten_immediately << ", by kill " << forgotten_by_kill << ", by long entry "
                                   << forgotten_by_long << ", by eviction " << forgotten_by_eviction << ", by trim "
                                   << forgotten_by_trim << ", by modifier " << forgotten_by_modifier << ", by unlink "
                                   << forgotten_by_unlink);
    CHECK(kills_with_surviving > 0);
    CHECK(kills_unlinking > 0);
    CHECK(kills_clearing_one_way > 0);
    CHECK(rebased > 0);
    CHECK(rebase_removed > 0);
    CHECK(posthumous_links > 0);
    CHECK(dead_rejections > 0);
    CHECK(legendary > 0);
    CHECK(forgotten_immediately > 0);
    const std::size_t deferred = forgotten_by_kill + forgotten_by_long + forgotten_by_eviction + forgotten_by_trim
                               + forgotten_by_modifier + forgotten_by_unlink;
    CHECK(deferred > 0);
    CHECK(forgotten_by_long + forgotten_by_eviction + forgotten_by_trim > 0); // long entries
    CHECK(forgotten_by_modifier > 0);                                          // modifiers
    CHECK(forgotten_by_unlink + forgotten_by_kill > 0);                        // edges
}
