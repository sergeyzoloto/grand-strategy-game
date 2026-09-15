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
