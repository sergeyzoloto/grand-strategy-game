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

// The DeadRecord kill() will write, built independently for expected values.
DeadRecord expected_record(const Character& c, Date death) {
    return DeadRecord{.id = c.id(),
                      .name = c.name(),
                      .birth = c.birth(),
                      .death = death,
                      .main_community = c.main_community().value_or(CommunityId{}),
                      .reputation = static_cast<std::int8_t>(c.reputation()),
                      .gender = c.gender(),
                      .reserved = 0};
}

} // namespace

// ---- layout --------------------------------------------------------------------------------

static_assert(sizeof(DeadRecord) == 24 && alignof(DeadRecord) == 4);
static_assert(std::has_unique_object_representations_v<DeadRecord>);
static_assert(offsetof(DeadRecord, death) == 12 && offsetof(DeadRecord, main_community) == 16
              && offsetof(DeadRecord, reputation) == 20 && offsetof(DeadRecord, gender) == 21
              && offsetof(DeadRecord, reserved) == 22);
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
    REQUIRE(r.find(a)->set_involvement(CommunityId{4}, 2) == EditResult::Ok);
    REQUIRE(r.find(a)->set_involvement(CommunityId{9}, 7) == EditResult::Ok);
    r.find(a)->set_reputation(37); // changed after creation; frozen at death

    CHECK(r.kill(CharacterId{}, Date{200}, stances, CONFIG, SEED) == EditResult::Invalid);
    CHECK(r.kill(CharacterId{3}, Date{200}, stances, CONFIG, SEED) == EditResult::NotFound);
    CHECK(r.kill(CharacterId{3}, Date{-1000}, stances, CONFIG, SEED) == EditResult::NotFound); // NotFound first
    CHECK(r.kill(a, Date{99}, stances, CONFIG, SEED) == EditResult::Invalid);                  // death < birth
    CHECK(r.find(a) != nullptr);
    CHECK(r.find_dead(a) == nullptr);
    CHECK(r.dead_count() == 0);

    CHECK(r.kill(a, Date{100}, stances, CONFIG, SEED) == EditResult::Ok); // death == birth is allowed
    CHECK(r.find(a) == nullptr);
    CHECK(r.kill(a, Date{300}, stances, CONFIG, SEED) == EditResult::NotFound); // already dead
    const DeadRecord* d = r.find_dead(a);
    REQUIRE(d != nullptr);
    CHECK(d->id == a);
    CHECK(d->name == NameId{11});
    CHECK(d->birth == Date{100});
    CHECK(d->death == Date{100});
    CHECK(d->main_community == CommunityId{9});
    CHECK(d->reputation == 37);
    CHECK(d->gender == Gender::Male);
    CHECK(d->reserved == 0);
    CHECK(r.exists(a));
    CHECK(r.size() == 1);
    CHECK(r.dead_count() == 1);

    // No involvement: an invalid main community.
    CHECK(r.find_dead(b) == nullptr);
    CHECK(r.kill(b, Date{60}, stances, CONFIG, SEED) == EditResult::Ok);
    CHECK(!r.find_dead(b)->main_community.valid());
    CHECK(r.characters().empty());
    CHECK(r.find_dead(CharacterId{}) == nullptr);
    CHECK(r.find_dead(CharacterId{3}) == nullptr);
    // Ids are never reused.
    CHECK(r.create(NameId{13}, Gender::Male, Date{0}, CharacterInit{}) == CharacterId{3});
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

    REQUIRE(r.kill(CharacterId{3}, Date{500}, stances, CONFIG, SEED) == EditResult::Ok);
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
    REQUIRE(r.kill(CharacterId{8}, Date{500}, stances, CONFIG, SEED) == EditResult::Ok); // last slot
    REQUIRE(r.kill(CharacterId{1}, Date{500}, stances, CONFIG, SEED) == EditResult::Ok); // first slot
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

    REQUIRE(r.kill(a, Date{10}, stances, CONFIG, SEED) == EditResult::Ok);

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

TEST_CASE("kill: siblings through a dead parent; parents() and children() of a dead character") {
    const StanceTable stances;
    CharacterRegistry r = make_registry(6);
    const CharacterId grandparent{1}, parent{2}, other_parent{3}, x{4}, y{5}, z{6};
    REQUIRE(r.link(grandparent, R::Child, parent) == EditResult::Ok);
    for (const CharacterId child : {x, y}) {
        REQUIRE(r.link(parent, R::Child, child) == EditResult::Ok);
    }
    REQUIRE(r.link(other_parent, R::Child, x) == EditResult::Ok);
    REQUIRE(r.link(other_parent, R::Child, z) == EditResult::Ok);

    REQUIRE(r.kill(parent, Date{5}, stances, CONFIG, SEED) == EditResult::Ok);
    REQUIRE(r.kill(grandparent, Date{6}, stances, CONFIG, SEED) == EditResult::Ok);
    CHECK(r.siblings(y) == std::vector<CharacterId>{x});
    CHECK(r.siblings(x) == std::vector<CharacterId>{y, z});
    CHECK(r.shared_parents(x, y) == 1);
    CHECK(r.children(parent) == std::vector<CharacterId>{x, y});
    CHECK(r.parents(parent).size() == 1);
    CHECK(r.parents(parent)[0] == grandparent);
    CHECK(r.children(grandparent) == std::vector<CharacterId>{parent});
}

TEST_CASE("posthumous links: surviving types only; one-way edits with the dead are Invalid") {
    const StanceTable stances;
    CharacterRegistry r = make_registry(7);
    const CharacterId father{1}, mother{2}, child{3}, stepfather{4}, m{5}, n{6}, living{7};
    REQUIRE(r.kill(father, Date{0}, stances, CONFIG, SEED) == EditResult::Ok);

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
    REQUIRE(r.kill(mother, Date{1}, stances, CONFIG, SEED) == EditResult::Ok);
    CHECK(r.link(stepfather, R::Child, child) == EditResult::Full);
    CHECK(r.parents(child).size() == 2);

    // Two dead characters, a surviving type; Conflict still applies.
    REQUIRE(r.kill(m, Date{2}, stances, CONFIG, SEED) == EditResult::Ok);
    REQUIRE(r.kill(n, Date{3}, stances, CONFIG, SEED) == EditResult::Ok);
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

    REQUIRE(r.kill(a, Date{1}, stances, CONFIG, SEED) == EditResult::Ok);
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
        REQUIRE(w.r.kill(w.b, Date{50}, w.stances, CONFIG, SEED) == EditResult::Ok);
        CHECK(long_opinion(*w.r.find(w.a), w.b) == 30 + delta);
        const double after = opinion(*w.r.find(w.a), *w.r.find_dead(w.b), w.stances, CONFIG, SEED);
        CHECK(std::abs(after - before) <= 0.5);
    }
    SUBCASE("a rebase to 0 removes the entry") {
        RebaseWorld w;
        const int delta = w.expected_delta(w.a);
        REQUIRE(delta != 0);
        REQUIRE(w.r.add_long_opinion(w.a, w.b, -delta).outcome == O::Created);
        REQUIRE(w.r.kill(w.b, Date{50}, w.stances, CONFIG, SEED) == EditResult::Ok);
        CHECK(long_opinion(*w.r.find(w.a), w.b) == 0);
        CHECK(w.r.find(w.a)->long_people().empty());
    }
    SUBCASE("saturated long") {
        RebaseWorld w;
        const int delta = w.expected_delta(w.a);
        REQUIRE(delta != 0);
        const int saturated = delta > 0 ? 200 : -200;
        REQUIRE(w.r.add_long_opinion(w.a, w.b, saturated).outcome == O::Created);
        REQUIRE(w.r.kill(w.b, Date{50}, w.stances, CONFIG, SEED) == EditResult::Ok);
        CHECK(long_opinion(*w.r.find(w.a), w.b) == saturated); // clamped, so the opinion may move
    }
    SUBCASE("without a long entry the opinion is the dead-record base") {
        RebaseWorld w;
        REQUIRE(w.r.add_long_opinion(w.a, w.c, 20).outcome == O::Created); // about someone else
        REQUIRE(w.r.kill(w.b, Date{50}, w.stances, CONFIG, SEED) == EditResult::Ok);
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
    const WeakOpinionBreakdown alive = weak_opinion_breakdown(*r.find(a), *r.find(d), stances, CONFIG, SEED);

    REQUIRE(r.kill(d, Date{9}, stances, CONFIG, SEED) == EditResult::Ok);
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
    REQUIRE(r.kill(e, Date{9}, stances, CONFIG, SEED) == EditResult::Ok);
    CHECK(weak_opinion_breakdown(*r.find(a), *r.find_dead(e), stances, CONFIG, SEED).community == 0.0);
}

// ---- property test against a reference model ----------------------------------------------

namespace {

constexpr std::uint32_t MAX_IDS = 160;

// Naive model: status per id, relation masks per ordered pair, long-term people entries
// and person modifiers per holder. Weak opinions come from the tested functions.
struct DeathModel {
    struct Person {
        bool dead = false;
        std::map<std::uint32_t, int> longs;                                 // target -> value
        std::map<std::pair<std::uint32_t, std::uint16_t>, int> modifiers;  // (target, modifier) -> effect
    };
    std::vector<Person> persons; // index = id - 1
    std::map<std::pair<std::uint32_t, std::uint32_t>, std::uint32_t> masks;

    bool exists(CharacterId id) const { return id.valid() && id.value <= persons.size(); }
    bool dead(CharacterId id) const { return persons[id.value - 1].dead; }
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

    static O add_long(std::map<std::uint32_t, int>& longs, std::uint32_t target, int delta) {
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
        if (longs.size() >= PERSON_LIMIT_MAX) { // extraversion +100 everywhere
            auto weakest = longs.begin();
            for (auto e = longs.begin(); e != longs.end(); ++e) {
                if (std::abs(e->second) < std::abs(weakest->second)) {
                    weakest = e;
                }
            }
            if (std::abs(value) <= std::abs(weakest->second)) {
                return O::Dropped;
            }
            longs.erase(weakest);
            longs[target] = value;
            return O::CreatedWithEviction;
        }
        longs[target] = value;
        return O::Created;
    }
};

} // namespace

TEST_CASE("property: creates, kills, links, one-way relations and opinions match a reference model") {
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
    CharacterRegistry reg;
    DeathModel model;
    const auto create = [&] {
        const auto trait = [&] { return static_cast<int>(pick(201)) - 100; };
        const CharacterId id = reg.create(NameId{static_cast<std::uint32_t>(model.persons.size() + 1)}, Gender::Female,
                                          Date{0},
                                          CharacterInit{.stability = trait(), .openness = trait(), .extraversion = 100,
                                                        .conscientiousness = trait(), .agreeableness = trait(),
                                                        .reputation = trait()});
        REQUIRE(id.value == model.persons.size() + 1);
        model.persons.emplace_back();
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
    const auto check_all = [&] {
        std::vector<CharacterId> living;
        for (std::uint32_t id = 1; id <= model.persons.size(); ++id) {
            CAPTURE(id);
            const DeathModel::Person& p = model.persons[id - 1];
            REQUIRE((reg.find(CharacterId{id}) == nullptr) == p.dead);
            REQUIRE((reg.find_dead(CharacterId{id}) != nullptr) == p.dead);
            std::vector<std::pair<std::uint32_t, std::uint32_t>> expected;
            for (auto it = model.masks.lower_bound({id, 0}); it != model.masks.end() && it->first.first == id; ++it) {
                expected.emplace_back(it->first.second, it->second);
            }
            const std::span<const RelationEdge> edges = reg.relations(CharacterId{id});
            REQUIRE(edges.size() == expected.size());
            for (std::size_t i = 0; i < edges.size(); ++i) {
                REQUIRE(edges[i].other.value == expected[i].first);
                REQUIRE(edges[i].mask == expected[i].second);
            }
            if (p.dead) {
                continue;
            }
            living.push_back(CharacterId{id});
            const Character& c = *reg.find(CharacterId{id});
            REQUIRE(c.long_people().size() == p.longs.size());
            auto it = p.longs.begin();
            for (const LongOpinion& e : c.long_people()) {
                REQUIRE((e.target == it->first && e.value == it->second));
                ++it;
            }
            REQUIRE(c.modifiers().size() == p.modifiers.size());
            auto mit = p.modifiers.begin();
            for (const OpinionModifier& m : c.modifiers()) {
                REQUIRE((m.target == mit->first.first && m.modifier.value == mit->first.second
                         && m.effect == mit->second));
                ++mit;
            }
        }
        REQUIRE(ids_of(reg.characters()) == living);
    };

    std::size_t kills = 0, kills_with_surviving = 0, kills_unlinking = 0, kills_clearing_one_way = 0;
    std::size_t rebased = 0, rebase_removed = 0, posthumous_links = 0, dead_rejections = 0;

    for (int step = 0; step < STEPS; ++step) {
        CAPTURE(step);
        const std::uint32_t op = pick(100);
        const CharacterId a = random_id();
        const CharacterId b = random_id();
        if (op < 4) {
            if (model.persons.size() < MAX_IDS) {
                create();
            }
        } else if (op < 8) { // kill
            if (!model.exists(a) || model.dead(a)) {
                REQUIRE(reg.kill(a, Date{10}, stances, CONFIG, SEED)
                        == (a.valid() ? EditResult::NotFound : EditResult::Invalid));
                continue;
            }
            const Character& victim = *reg.find(a);
            const DeadRecord record = expected_record(victim, Date{10});
            // Sometimes make one holder's entry cancel exactly, so rebases reach 0.
            std::vector<std::pair<CharacterId, int>> deltas;
            for (const Character& holder : reg.characters()) {
                if (holder.id() == a) {
                    continue;
                }
                const double before = weak_opinion(holder, victim, stances, CONFIG, SEED);
                const double after = weak_opinion(holder, record, stances, CONFIG, SEED);
                deltas.emplace_back(holder.id(), static_cast<int>(detail::clamp_round(before - after, -400, 400)));
            }
            if (!deltas.empty() && pick(2) == 0) {
                const auto [holder, delta] = deltas[pick(static_cast<std::uint32_t>(deltas.size()))];
                if (delta != 0) {
                    auto& longs = model.persons[holder.value - 1].longs;
                    const int current = longs.count(a.value) != 0 ? longs[a.value] : 0;
                    REQUIRE(reg.add_long_opinion(holder, a, -delta - current).outcome
                            == DeathModel::add_long(longs, a.value, -delta - current));
                }
            }
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
                auto& longs = model.persons[holder.value - 1].longs;
                if (longs.count(a.value) != 0) {
                    ++rebased;
                    rebase_removed += DeathModel::add_long(longs, a.value, delta) == O::Removed;
                }
            }
            model.persons[a.value - 1].dead = true;
            model.persons[a.value - 1].longs.clear();
            model.persons[a.value - 1].modifiers.clear();
            REQUIRE(reg.kill(a, Date{10}, stances, CONFIG, SEED) == EditResult::Ok);
            ++kills;
            kills_with_surviving += surviving;
            kills_unlinking += unlinking;
            kills_clearing_one_way += clearing;
            check_all();
            continue;
        } else if (op < 35) { // link or unlink, mostly surviving types
            const std::array<R, 7> types{R::Child, R::Parent, R::Spouse, R::Spouse, R::Liege, R::Employer, R::Friend};
            const R type = types[pick(7)];
            if (pick(5) == 0) {
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
        } else if (op < 55) { // one-way relations
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
        } else if (op < 70) { // modifiers
            const ModifierId m{static_cast<std::uint16_t>(1 + pick(2))};
            const int effect = static_cast<int>(pick(201)) - 100;
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
                    if (pick(3) == 0) {
                        expected = mods.erase(key) != 0 ? EditResult::Ok : EditResult::NotFound;
                        REQUIRE(reg.remove_modifier(a, b, m) == expected);
                        continue;
                    }
                    if (mods.count(key) != 0) {
                        expected = EditResult::Duplicate;
                    } else if (mods.size() >= MODIFIER_CAP) {
                        expected = EditResult::Full;
                    } else {
                        mods[key] = effect;
                        expected = EditResult::Ok;
                    }
                }
            }
            REQUIRE(reg.add_modifier(a, b, m, effect) == expected);
        } else { // long opinions
            const int delta = pick(10) == 0 ? 0 : static_cast<int>(pick(161)) - 80;
            O expected = O::Invalid;
            if (a.valid() && b.valid() && a != b) {
                if (!model.exists(a) || !model.exists(b)) {
                    expected = O::NotFound;
                } else if (model.dead(a)) {
                    expected = O::Invalid;
                    ++dead_rejections;
                } else {
                    expected = DeathModel::add_long(model.persons[a.value - 1].longs, b.value, delta);
                }
            }
            REQUIRE(reg.add_long_opinion(a, b, delta).outcome == expected);
        }
        if (step % 50 == 0) {
            check_all();
        }
    }
    check_all();

    MESSAGE("death property: " << STEPS << " steps, " << model.persons.size() << " ids; kills " << kills
                               << " (with surviving links " << kills_with_surviving << ", unlinking "
                               << kills_unlinking << ", clearing one-way " << kills_clearing_one_way
                               << "); rebased " << rebased << " (removed " << rebase_removed
                               << "); posthumous links " << posthumous_links << "; dead rejections "
                               << dead_rejections);
    CHECK(kills_with_surviving > 0);
    CHECK(kills_unlinking > 0);
    CHECK(kills_clearing_one_way > 0);
    CHECK(rebased > 0);
    CHECK(rebase_removed > 0);
    CHECK(posthumous_links > 0);
    CHECK(dead_rejections > 0);
}
