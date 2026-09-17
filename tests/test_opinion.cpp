#include <doctest.h>

#include <algorithm>
#include <cstdint>

#include "sim/character_registry.hpp"
#include "sim/opinion.hpp"
#include "sim/stance_table.hpp"

using namespace sim;

namespace {

TargetId ct(std::uint32_t id) {
    return *TargetId::from(CommunityId{id});
}

TargetId tt(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

CharacterInit personality(int stability, int openness, int extraversion, int conscientiousness, int agreeableness) {
    return CharacterInit{.stability = stability, .openness = openness, .extraversion = extraversion,
                         .conscientiousness = conscientiousness, .agreeableness = agreeableness};
}

OpinionConfig without_noise() {
    OpinionConfig config;
    config.k_noise = 0.0;
    return config;
}

} // namespace

// ---- compat and openness factor --------------------------------------------------------

TEST_CASE("compat: identical gives +k_compat, opposite extremes give -k_compat, symmetric") {
    // Create everything first: references into the registry are valid only until the next create.
    CharacterRegistry r;
    const CharacterId a_id = r.create(NameId{1}, Gender::Female, Date{0}, personality(10, -20, 30, -40, 50));
    const CharacterId b_id = r.create(NameId{2}, Gender::Male, Date{0}, personality(10, -20, 30, -40, 50));
    const CharacterId high_id = r.create(NameId{3}, Gender::Male, Date{0}, personality(100, 100, 100, 100, 100));
    const CharacterId low_id = r.create(NameId{4}, Gender::Male, Date{0}, personality(-100, -100, -100, -100, -100));
    const CharacterId mixed_id = r.create(NameId{5}, Gender::Female, Date{0}, personality(-7, 64, -100, 3, 81));
    const CharacterId open_id = r.create(NameId{6}, Gender::Female, Date{0}, personality(0, 100, 0, 0, 0));
    const CharacterId closed_id = r.create(NameId{7}, Gender::Female, Date{0}, personality(0, -100, 0, 0, 0));
    const Character& a = *r.find(a_id);
    const Character& b = *r.find(b_id);
    const Character& high = *r.find(high_id);
    const Character& low = *r.find(low_id);
    const Character& mixed = *r.find(mixed_id);
    const Character& open = *r.find(open_id);
    const Character& closed = *r.find(closed_id);
    const OpinionConfig config{};

    CHECK(compat(a, b, config) == config.k_compat);
    CHECK(compat(high, low, config) == -config.k_compat);
    CHECK(compat(low, high, config) == -config.k_compat);
    CHECK(compat(a, mixed, config) == compat(mixed, a, config));
    CHECK(compat(high, mixed, config) == compat(mixed, high, config));

    OpinionConfig weighted = config;
    weighted.w_openness = 2.0;
    weighted.w_agreeableness = 0.0;
    CHECK(compat(a, mixed, weighted) == compat(mixed, a, weighted));
    // Only openness differs: distance 200 at weight 2 = 400, weight sum 5:
    // 20 * (1 - 2 * 400 / (200 * 5)) = 4
    CHECK(compat(open, closed, weighted) == doctest::Approx(4.0));
}

#ifdef NDEBUG
// Zero weights are asserted in debug builds.
TEST_CASE("compat: a zero weight sum returns 0") {
    CharacterRegistry r;
    OpinionConfig config;
    config.w_stability = config.w_openness = config.w_extraversion = config.w_conscientiousness =
        config.w_agreeableness = 0.0;
    const CharacterId a_id = r.create(NameId{1}, Gender::Female, Date{0}, personality(10, 20, 30, 40, 50));
    const CharacterId b_id = r.create(NameId{2}, Gender::Female, Date{0}, CharacterInit{});
    CHECK(compat(*r.find(a_id), *r.find(b_id), config) == 0.0);
}
#endif

TEST_CASE("openness factor is linear from 0.5 to 1.5") {
    CharacterRegistry r;
    const OpinionConfig config{};
    CHECK(openness_factor(*r.find(r.create(NameId{1}, Gender::Male, Date{0}, CharacterInit{.openness = -100})), config) == 0.5);
    CHECK(openness_factor(*r.find(r.create(NameId{1}, Gender::Male, Date{0}, CharacterInit{.openness = 0})), config) == 1.0);
    CHECK(openness_factor(*r.find(r.create(NameId{1}, Gender::Male, Date{0}, CharacterInit{.openness = 100})), config) == 1.5);
    CHECK(openness_factor(*r.find(r.create(NameId{1}, Gender::Male, Date{0}, CharacterInit{.openness = 50})), config) == 1.25);
}

// ---- weak opinions ---------------------------------------------------------------------

TEST_CASE("weak opinion: hand-computed example with two communities each and inherited stances") {
    // A: community 1 (weight 3, parent 5) and 2 (weight 1). B: community 3 (weight 1) and 4 (weight 1, parent 6).
    StanceTable t;
    REQUIRE(t.set_parent(CommunityId{1}, CommunityId{5}) == EditResult::Ok);
    REQUIRE(t.set_parent(CommunityId{4}, CommunityId{6}) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{5}, ct(3), 40) == EditResult::Ok);  // 1 -> 3 via source parent 5
    REQUIRE(t.set_stance(CommunityId{1}, ct(6), -20) == EditResult::Ok); // 1 -> 4 via target parent 6
    REQUIRE(t.set_stance(CommunityId{5}, ct(6), 80) == EditResult::Ok);  // shadowed by 1 -> 6 (source side first)
    REQUIRE(t.set_stance(CommunityId{2}, ct(3), 10) == EditResult::Ok);  // 2 -> 3 explicit
    REQUIRE(t.set_stance(CommunityId{2}, ct(6), 30) == EditResult::Ok);  // 2 -> 4 via target parent 6
    REQUIRE(t.set_stance(CommunityId{5}, tt(7), 50) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{2}, tt(7), -10) == EditResult::Ok);

    CharacterRegistry r;
    const CharacterId a_id = r.create(NameId{1}, Gender::Female, Date{0}, CharacterInit{});
    const CharacterId b_id = r.create(NameId{2}, Gender::Male, Date{0}, CharacterInit{.reputation = 40});
    REQUIRE(r.find(a_id)->set_involvement(CommunityId{1}, 3) == EditResult::Ok);
    REQUIRE(r.find(a_id)->set_involvement(CommunityId{2}, 1) == EditResult::Ok);
    REQUIRE(r.find(b_id)->set_involvement(CommunityId{3}, 1) == EditResult::Ok);
    REQUIRE(r.find(b_id)->set_involvement(CommunityId{4}, 1) == EditResult::Ok);
    const Character& a = *r.find(a_id);
    const Character& b = *r.find(b_id);

    // (3*1*40 + 3*1*(-20) + 1*1*10 + 1*1*30) / (4 * 2) = 100 / 8 = 12.5
    const WeakOpinionBreakdown w = weak_opinion_breakdown(a, b, t, without_noise(), WorldSeed{1});
    CHECK(w.community == 12.5);
    CHECK(w.reputation == 10.0); // 0.25 * 40
    CHECK(w.compat == 20.0);     // identical (all-zero) personalities
    CHECK(w.noise == 0.0);
    CHECK(w.total == 42.5);
    CHECK(weak_opinion(a, b, t, without_noise(), WorldSeed{1}) == 42.5);

    // Topic: (3 * 50 + 1 * (-10)) / 4 = 35
    const WeakOpinionBreakdown topic = weak_opinion_breakdown(a, tt(7), t, without_noise(), WorldSeed{1});
    CHECK(topic.community == 35.0);
    CHECK(topic.reputation == 0.0);
    CHECK(topic.compat == 0.0);
    CHECK(topic.total == 35.0);
    // Community target 4 walks its chain: (3 * (-20) + 1 * 30) / 4 = -7.5
    CHECK(weak_opinion(a, ct(4), t, without_noise(), WorldSeed{1}) == -7.5);
    CHECK(weak_opinion(a, TargetId{}, t, OpinionConfig{}, WorldSeed{1}) == 0.0);
}

TEST_CASE("weak opinion: empty involvement gives a zero community term") {
    StanceTable t;
    REQUIRE(t.set_stance(CommunityId{1}, ct(1), 100) == EditResult::Ok);
    CharacterRegistry r;
    const CharacterId a_id = r.create(NameId{1}, Gender::Female, Date{0}, CharacterInit{});
    const CharacterId b_id = r.create(NameId{2}, Gender::Male, Date{0}, CharacterInit{});
    REQUIRE(r.find(b_id)->set_involvement(CommunityId{1}, 10) == EditResult::Ok);
    const Character& a = *r.find(a_id);
    const Character& b = *r.find(b_id);
    CHECK(weak_opinion_breakdown(a, b, t, OpinionConfig{}, WorldSeed{3}).community == 0.0);
    CHECK(weak_opinion_breakdown(b, a, t, OpinionConfig{}, WorldSeed{3}).community == 0.0);
    CHECK(weak_opinion_breakdown(a, ct(1), t, OpinionConfig{}, WorldSeed{3}).community == 0.0);
    CHECK(weak_opinion(a, b, t, without_noise(), WorldSeed{3}) == 20.0); // only compat remains
}

TEST_CASE("weak opinion: clamping at both ends; breakdown terms sum to the unclamped total") {
    StanceTable t;
    REQUIRE(t.set_stance(CommunityId{1}, ct(1), 100) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{2}, ct(2), -100) == EditResult::Ok);
    CharacterRegistry r;
    const CharacterId fan = r.create(NameId{1}, Gender::Female, Date{0}, personality(100, 100, 100, 100, 100));
    const CharacterId idol = r.create(NameId{2}, Gender::Male, Date{0},
                                      CharacterInit{.stability = 100, .openness = 100, .extraversion = 100,
                                                    .conscientiousness = 100, .agreeableness = 100, .reputation = 100});
    const CharacterId hater = r.create(NameId{3}, Gender::Female, Date{0}, personality(-100, -100, -100, -100, -100));
    const CharacterId villain = r.create(NameId{4}, Gender::Male, Date{0},
                                         CharacterInit{.stability = 100, .openness = 100, .extraversion = 100,
                                                       .conscientiousness = 100, .agreeableness = 100, .reputation = -100});
    REQUIRE(r.find(fan)->set_involvement(CommunityId{1}, 50) == EditResult::Ok);
    REQUIRE(r.find(idol)->set_involvement(CommunityId{1}, 7) == EditResult::Ok);
    REQUIRE(r.find(hater)->set_involvement(CommunityId{2}, 50) == EditResult::Ok);
    REQUIRE(r.find(villain)->set_involvement(CommunityId{2}, 7) == EditResult::Ok);

    const OpinionConfig config{};
    for (std::uint64_t seed = 0; seed < 50; ++seed) {
        CAPTURE(seed);
        const WeakOpinionBreakdown up = weak_opinion_breakdown(*r.find(fan), *r.find(idol), t, config, WorldSeed{seed});
        CHECK(up.community + up.reputation + up.compat + up.noise > 100.0);
        CHECK(up.total == 100.0);
        const WeakOpinionBreakdown down =
            weak_opinion_breakdown(*r.find(hater), *r.find(villain), t, config, WorldSeed{seed});
        CHECK(down.community + down.reputation + down.compat + down.noise < -100.0);
        CHECK(down.total == -100.0);
    }

    // Unclamped cases: the total equals the sum of the terms exactly.
    for (std::uint64_t seed = 0; seed < 50; ++seed) {
        const WeakOpinionBreakdown w = weak_opinion_breakdown(*r.find(idol), *r.find(hater), t, config, WorldSeed{seed});
        const double sum = w.community + w.reputation + w.compat + w.noise;
        CHECK(w.total == std::clamp(sum, -100.0, 100.0));
        CHECK(sum > -100.0);
        CHECK(sum < 100.0);
        CHECK(w.total == sum);
    }
}

#ifdef NDEBUG
// a == b is asserted in debug builds.
TEST_CASE("weak opinion: a == b returns 0") {
    CharacterRegistry r;
    const Character& a = *r.find(r.create(NameId{1}, Gender::Female, Date{0}, CharacterInit{.reputation = 90}));
    CHECK(weak_opinion(a, a, StanceTable{}, OpinionConfig{}, WorldSeed{1}) == 0.0);
}
#endif

// ---- golden values -----------------------------------------------------------------------

TEST_CASE("weak opinion: golden values (identical in Debug and Release)") {
    CharacterRegistry r;
    const CharacterId a_id = r.create(NameId{1}, Gender::Female, Date{0},
                                      CharacterInit{.stability = 40, .openness = 70, .extraversion = -20,
                                                    .conscientiousness = 10, .agreeableness = 55, .reputation = -30});
    const CharacterId b_id = r.create(NameId{2}, Gender::Male, Date{0},
                                      CharacterInit{.stability = -35, .openness = 15, .extraversion = 90,
                                                    .conscientiousness = -60, .agreeableness = 5, .reputation = 64});
    const CharacterId c_id = r.create(NameId{3}, Gender::Female, Date{0},
                                      CharacterInit{.stability = 100, .openness = -100, .extraversion = 0,
                                                    .conscientiousness = 33, .agreeableness = -77, .reputation = 7});
    StanceTable t;
    REQUIRE(t.set_parent(CommunityId{11}, CommunityId{10}) == EditResult::Ok);
    REQUIRE(t.set_parent(CommunityId{21}, CommunityId{20}) == EditResult::Ok);
    REQUIRE(t.set_parent(CommunityId{20}, CommunityId{2}) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{10}, ct(20), -45) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{11}, ct(21), 30) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{12}, ct(2), 17) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{10}, ct(10), 60) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{21}, ct(11), -8) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{2}, tt(5), 90) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{11}, tt(5), -25) == EditResult::Ok);
    REQUIRE(r.find(a_id)->set_involvement(CommunityId{11}, 120) == EditResult::Ok);
    REQUIRE(r.find(a_id)->set_involvement(CommunityId{12}, 45) == EditResult::Ok);
    REQUIRE(r.find(a_id)->set_involvement(CommunityId{10}, 9) == EditResult::Ok);
    REQUIRE(r.find(b_id)->set_involvement(CommunityId{21}, 200) == EditResult::Ok);
    REQUIRE(r.find(b_id)->set_involvement(CommunityId{11}, 31) == EditResult::Ok);
    REQUIRE(r.find(c_id)->set_involvement(CommunityId{20}, 77) == EditResult::Ok);
    const Character& a = *r.find(a_id);
    const Character& b = *r.find(b_id);
    const Character& c = *r.find(c_id);
    const OpinionConfig config{};
    const WorldSeed seed{0x5EEDu};

    const WeakOpinionBreakdown ab = weak_opinion_breakdown(a, b, t, config, seed);
    CHECK(ab.community == 0x1.9ac8a08066b89p+4); // 1031940 / 40194 = 25.67398119122257
    CHECK(ab.reputation == 16.0);
    CHECK(ab.compat == 0x1.6666666666667p+2);   // 5.6
    CHECK(ab.noise == -0x1.1e623471c485ep+1);   // -2.2373719745165053
    CHECK(ab.total == 0x1.684af9c5e3e0bp+5);    // 45.036609216706061

    const WeakOpinionBreakdown ba = weak_opinion_breakdown(b, a, t, config, seed);
    CHECK(ba.community == 0x1.3155caba906p+0);  // 1.1927153306463651
    CHECK(ba.noise == 0x1.4401f8d993e9ap+0);    // 1.2656550913997422
    CHECK(ba.total == 0x1.1de2ba5b7bc6cp-1);    // 0.55837042204610787

    const WeakOpinionBreakdown ca = weak_opinion_breakdown(c, a, t, config, seed);
    CHECK(ca.community == 0.0);
    CHECK(ca.compat == 0x1.e666666666664p+1);   // 3.7999999999999989
    CHECK(ca.total == -0x1.3fd1b247aa958p+3);   // -9.9943477058156276

    const WeakOpinionBreakdown a_topic = weak_opinion_breakdown(a, tt(5), t, config, seed);
    CHECK(a_topic.community == -0x1.13dcb08d3dcb1p+4); // -3000 / 174
    CHECK(a_topic.total == -0x1.3da3f0fedaa03p+4);     // -19.852524753116096

    CHECK(weak_opinion(c, ct(21), t, config, seed) == 0x1.2f8136df2c231p+2); // 4.742261617605025
}

TEST_CASE("weak opinion: golden values with non-dyadic coefficients (FMA-contraction sensitive)") {
    // The default config's products are exact (weights 1.0, k_rep 0.25, integer distances), so
    // FMA contraction would not change the goldens above. These coefficients make the products
    // inexact: a build that loses -ffp-contract=off and targets FMA (-mfma) changes compat and
    // the topic noise in their last bits, and this test fails. See the FMA check in CLAUDE.md.
    CharacterRegistry r;
    const CharacterId a_id = r.create(NameId{1}, Gender::Female, Date{0},
                                      CharacterInit{.stability = 37, .openness = -61, .extraversion = 13,
                                                    .conscientiousness = 88, .agreeableness = -29, .reputation = -47});
    const CharacterId b_id = r.create(NameId{2}, Gender::Male, Date{0},
                                      CharacterInit{.stability = -52, .openness = 71, .extraversion = -9,
                                                    .conscientiousness = 3, .agreeableness = 66, .reputation = 83});
    StanceTable t;
    REQUIRE(t.set_stance(CommunityId{1}, ct(2), 37) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{1}, tt(3), -71) == EditResult::Ok);
    REQUIRE(r.find(a_id)->set_involvement(CommunityId{1}, 7) == EditResult::Ok);
    REQUIRE(r.find(a_id)->set_involvement(CommunityId{4}, 3) == EditResult::Ok);
    REQUIRE(r.find(b_id)->set_involvement(CommunityId{2}, 11) == EditResult::Ok);
    OpinionConfig config;
    config.k_rep = 0.137;
    config.k_compat = 17.3;
    config.w_stability = 0.9;
    config.w_openness = 1.3;
    config.w_extraversion = 0.7;
    config.w_conscientiousness = 1.1;
    config.w_agreeableness = 0.3;
    config.k_noise = 7.9;
    config.openness_factor_min = 0.37;
    config.openness_factor_max = 1.83;
    const WorldSeed seed{0xF0Au};
    const Character& a = *r.find(a_id);
    const Character& b = *r.find(b_id);

    const WeakOpinionBreakdown ab = weak_opinion_breakdown(a, b, t, config, seed);
    CHECK(ab.community == 0x1.9e66666666666p+4);
    CHECK(ab.reputation == 0x1.6bdf3b645a1cbp+3);
    CHECK(ab.compat == 0x1.a54040030c563p+0);
    CHECK(ab.noise == -0x1.7ad2ae286edacp+1);
    CHECK(ab.total == 0x1.1fa7d929db2f6p+5);

    const WeakOpinionBreakdown at = weak_opinion_breakdown(a, tt(3), t, config, seed);
    CHECK(at.noise == -0x1.b3ec388369a27p+1);
    CHECK(at.total == -0x1.a8d85d21d033cp+5);
}
