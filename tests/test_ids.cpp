#include <doctest.h>

#include <cstdint>
#include <type_traits>
#include <unordered_set>

#include "sim/ids.hpp"

using sim::CharacterId;
using sim::NameId;

static_assert(sizeof(CharacterId) == 4);
static_assert(std::is_trivially_copyable_v<CharacterId>);
static_assert(!std::is_convertible_v<std::uint32_t, CharacterId>);
static_assert(!std::is_convertible_v<CharacterId, std::uint32_t>);
static_assert(!std::is_convertible_v<NameId, CharacterId>);
static_assert(!std::is_constructible_v<CharacterId, NameId>);
static_assert(!CharacterId{}.valid());
static_assert(CharacterId{7}.valid());
static_assert(CharacterId{1} < CharacterId{2});

TEST_CASE("ids are comparable and hashable") {
    std::unordered_set<CharacterId> set;
    set.insert(CharacterId{1});
    set.insert(CharacterId{2});
    set.insert(CharacterId{1});
    CHECK(set.size() == 2);
    CHECK(set.contains(CharacterId{2}));
    CHECK(CharacterId{3} == CharacterId{3});
    CHECK(CharacterId{3} != CharacterId{4});
}

// ---- id Rep, new id types ---------------------------------------------------

namespace {
struct RepProbeTag;
template<class Rep>
concept IdRepAccepted = requires { typename sim::Id<RepProbeTag, Rep>; };
} // namespace

static_assert(IdRepAccepted<std::uint16_t> && IdRepAccepted<std::uint32_t> && IdRepAccepted<std::uint64_t>);
static_assert(!IdRepAccepted<bool>);
static_assert(!IdRepAccepted<char16_t>);
static_assert(!IdRepAccepted<char8_t> && !IdRepAccepted<char32_t>);
static_assert(!IdRepAccepted<int> && !IdRepAccepted<float>);

static_assert(sizeof(sim::SkillId) == 2);
static_assert(sizeof(sim::CommunityId) == 4 && sizeof(sim::TopicId) == 4 && sizeof(sim::TargetId) == 4);
static_assert(!std::is_convertible_v<std::uint16_t, sim::SkillId>);
static_assert(!std::is_constructible_v<sim::CommunityId, sim::TopicId>);
static_assert(!std::is_constructible_v<sim::TargetId, std::uint32_t>);
static_assert(!sim::SkillId{}.valid() && sim::SkillId{1}.valid());
static_assert(std::is_same_v<sim::CharacterId, sim::Id<sim::CharacterIdTag, std::uint32_t>>);

TEST_CASE("u16 and u32 ids are hashable") {
    std::unordered_set<sim::SkillId> skills{sim::SkillId{1}, sim::SkillId{65535}, sim::SkillId{1}};
    CHECK(skills.size() == 2);
    std::unordered_set<sim::TargetId> targets{*sim::TargetId::from(sim::CommunityId{5}),
                                              *sim::TargetId::from(sim::TopicId{5})};
    CHECK(targets.size() == 2);
}

// ---- TargetId ---------------------------------------------------------------

using sim::CommunityId;
using sim::TargetId;
using sim::TargetKind;
using sim::TopicId;

static_assert(!TargetId{}.valid());
static_assert(TargetId{}.raw() == 0);
static_assert(TargetId::INDEX_LIMIT == (std::uint32_t{1} << 30));

TEST_CASE("TargetId round trip for both kinds") {
    for (const std::uint32_t index : {std::uint32_t{1}, std::uint32_t{12345}, TargetId::INDEX_LIMIT - 1}) {
        CAPTURE(index);
        const auto community = TargetId::from(CommunityId{index});
        REQUIRE(community.has_value());
        CHECK(community->valid());
        CHECK(community->kind() == TargetKind::Community);
        CHECK(community->index() == index);
        CHECK(community->as_community() == CommunityId{index});
        CHECK(!community->as_topic().has_value());

        const auto topic = TargetId::from(TopicId{index});
        REQUIRE(topic.has_value());
        CHECK(topic->valid());
        CHECK(topic->kind() == TargetKind::Topic);
        CHECK(topic->index() == index);
        CHECK(topic->as_topic() == TopicId{index});
        CHECK(!topic->as_community().has_value());

        CHECK(*community != *topic);
    }
}

TEST_CASE("TargetId rejects invalid ids and indices of 2^30 or more") {
    CHECK(!TargetId::from(CommunityId{}).has_value());
    CHECK(!TargetId::from(TopicId{}).has_value());
    CHECK(!TargetId::from(CommunityId{TargetId::INDEX_LIMIT}).has_value());
    CHECK(!TargetId::from(TopicId{TargetId::INDEX_LIMIT}).has_value());
    CHECK(!TargetId::from(CommunityId{0xFFFF'FFFFu}).has_value());
    CHECK(!TargetId::from(TopicId{TargetId::INDEX_LIMIT + 1}).has_value());
}

TEST_CASE("TargetId orders by kind, then index") {
    const TargetId c_small = *TargetId::from(CommunityId{2});
    const TargetId c_large = *TargetId::from(CommunityId{TargetId::INDEX_LIMIT - 1});
    const TargetId t_small = *TargetId::from(TopicId{1});
    CHECK(c_small < c_large);
    CHECK(c_large < t_small);
}

static_assert(TargetId::from_raw(TargetId::from(TopicId{9})->raw()) == TargetId::from(TopicId{9}));
static_assert(!TargetId::from_raw(0).has_value());

TEST_CASE("TargetId::from_raw decodes raw() values and rejects index 0 and unknown kinds") {
    for (const std::uint32_t index : {std::uint32_t{1}, std::uint32_t{777}, TargetId::INDEX_LIMIT - 1}) {
        CAPTURE(index);
        const TargetId community = *TargetId::from(CommunityId{index});
        const TargetId topic = *TargetId::from(TopicId{index});
        CHECK(TargetId::from_raw(community.raw()) == community);
        CHECK(TargetId::from_raw(topic.raw()) == topic);
    }
    CHECK(!TargetId::from_raw(0).has_value());                                          // community kind, index 0
    CHECK(!TargetId::from_raw(std::uint32_t{1} << TargetId::INDEX_BITS).has_value());   // topic kind, index 0
    CHECK(!TargetId::from_raw((std::uint32_t{2} << TargetId::INDEX_BITS) | 5).has_value()); // kind 2 (reserved)
    CHECK(!TargetId::from_raw((std::uint32_t{3} << TargetId::INDEX_BITS) | 5).has_value()); // kind 3 (free)
}
