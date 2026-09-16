#include <doctest.h>

#include <cstdint>
#include <limits>
#include <type_traits>

#include "sim/stance_table.hpp"

using namespace sim;

namespace {

TargetId ct(std::uint32_t id) {
    return *TargetId::from(CommunityId{id});
}

TargetId tt(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

template<class T>
concept CanSetStance = requires(StanceTable& table, T v) { table.set_stance(CommunityId{1}, TargetId{}, v); };

// Builds a straight chain: ids[0] is the root, each next id is a child of the previous one.
void make_line(StanceTable& table, std::initializer_list<std::uint32_t> ids) {
    std::uint32_t previous = 0;
    for (const std::uint32_t id : ids) {
        if (previous != 0) {
            REQUIRE(table.set_parent(CommunityId{id}, CommunityId{previous}) == EditResult::Ok);
        }
        previous = id;
    }
}

} // namespace

static_assert(CanSetStance<int> && CanSetStance<std::int64_t> && CanSetStance<unsigned>);
static_assert(!CanSetStance<float> && !CanSetStance<double> && !CanSetStance<bool> && !CanSetStance<char>);
static_assert(sizeof(StanceEntry) == 12);

TEST_CASE("stances: an explicit 0 differs from absent; clear restores inheritance") {
    StanceTable t;
    REQUIRE(t.set_parent(CommunityId{2}, CommunityId{1}) == EditResult::Ok);
    REQUIRE(t.set_stance(CommunityId{1}, ct(9), 70) == EditResult::Ok);
    CHECK(t.stance(CommunityId{2}, ct(9)) == 70); // inherited

    CHECK(t.set_stance(CommunityId{2}, ct(9), 0) == EditResult::Ok);
    CHECK(t.explicit_stance(CommunityId{2}, ct(9)) == 0);
    CHECK(t.stance(CommunityId{2}, ct(9)) == 0); // explicit 0 blocks inheritance
    CHECK(!t.explicit_stance(CommunityId{3}, ct(9)).has_value());

    CHECK(t.clear_stance(CommunityId{2}, ct(9)) == EditResult::Ok);
    CHECK(t.stance(CommunityId{2}, ct(9)) == 70);
    CHECK(t.clear_stance(CommunityId{2}, ct(9)) == EditResult::NotFound);
}

TEST_CASE("stances: set clamps, overwrites and rejects invalid ids") {
    StanceTable t;
    CHECK(t.set_stance(CommunityId{1}, tt(1), 150) == EditResult::Ok);
    CHECK(t.stance(CommunityId{1}, tt(1)) == 100);
    CHECK(t.set_stance(CommunityId{1}, tt(1), std::numeric_limits<std::int64_t>::min()) == EditResult::Ok);
    CHECK(t.stance(CommunityId{1}, tt(1)) == -100);
    CHECK(t.set_stance(CommunityId{1}, tt(1), -100) == EditResult::Ok); // equal value: Ok
    CHECK(t.stance_count() == 1);

    CHECK(t.set_stance(CommunityId{}, tt(1), 5) == EditResult::Invalid);
    CHECK(t.set_stance(CommunityId{1}, TargetId{}, 5) == EditResult::Invalid);
    CHECK(t.clear_stance(CommunityId{}, tt(1)) == EditResult::Invalid);
    CHECK(t.stance(CommunityId{}, tt(1)) == 0);
    CHECK(t.stance(CommunityId{1}, TargetId{}) == 0);
    CHECK(t.stance_count() == 1);
}

TEST_CASE("stances: inheritance on the source side, the target side and both") {
    StanceTable t;
    make_line(t, {1, 2, 3});    // tribe 1 > clan 2 > family 3
    make_line(t, {10, 20, 30}); // tribe 10 > clan 20 > family 30

    REQUIRE(t.set_stance(CommunityId{1}, ct(30), -40) == EditResult::Ok); // source side
    CHECK(t.stance(CommunityId{3}, ct(30)) == -40);
    CHECK(t.stance(CommunityId{2}, ct(30)) == -40);

    REQUIRE(t.set_stance(CommunityId{2}, ct(10), 25) == EditResult::Ok); // target side
    CHECK(t.stance(CommunityId{2}, ct(20)) == 25);

    REQUIRE(t.set_stance(CommunityId{1}, ct(10), 60) == EditResult::Ok); // both sides
    CHECK(t.stance(CommunityId{3}, ct(20)) == 25); // clan 2 -> tribe 10 found before tribe 1 -> tribe 10
    CHECK(t.stance(CommunityId{1}, ct(20)) == 60);
    CHECK(t.stance(CommunityId{1}, ct(30)) == -40);
    CHECK(t.stance(CommunityId{10}, ct(1)) == 0);  // stances are directed

    CHECK(t.chain(CommunityId{3}).size() == 3);
    CHECK(t.chain(CommunityId{3})[2] == CommunityId{1});
    CHECK(t.target_chain(ct(30)).size() == 3);
    CHECK(t.target_chain(ct(30))[1] == ct(20));
}

TEST_CASE("stances: source-side priority") {
    StanceTable t;
    make_line(t, {1, 2});   // tribe 1 > family 2
    make_line(t, {10, 20}); // tribe 10 > family 20
    REQUIRE(t.set_stance(CommunityId{2}, ct(10), -90) == EditResult::Ok); // the family's own feud with tribe 10
    REQUIRE(t.set_stance(CommunityId{1}, ct(20), 80) == EditResult::Ok);  // its tribe likes family 20
    // Source side first: family 2's entries (towards 20, then 10) win over tribe 1's.
    CHECK(t.stance(CommunityId{2}, ct(20)) == -90);
    CHECK(t.stance(CommunityId{1}, ct(20)) == 80);
}

TEST_CASE("stances: a topic target walks no chain; self-stance") {
    StanceTable t;
    make_line(t, {1, 2});
    REQUIRE(t.set_stance(CommunityId{1}, tt(2), 33) == EditResult::Ok);
    CHECK(t.stance(CommunityId{2}, tt(2)) == 33); // source chain still applies
    CHECK(t.target_chain(tt(2)).size() == 1);
    // A topic with the number of a community's parent does not pick up the parent's entries.
    REQUIRE(t.set_stance(CommunityId{5}, ct(1), 44) == EditResult::Ok);
    CHECK(t.stance(CommunityId{5}, tt(1)) == 0);
    CHECK(t.stance(CommunityId{5}, ct(2)) == 44);

    CHECK(t.set_stance(CommunityId{7}, ct(7), 90) == EditResult::Ok); // in-group cohesion
    CHECK(t.stance(CommunityId{7}, ct(7)) == 90);
}

TEST_CASE("hierarchy: set, re-set, clear, invalid and cycles") {
    StanceTable t;
    CHECK(t.set_parent(CommunityId{2}, CommunityId{1}) == EditResult::Ok);
    CHECK(t.set_parent(CommunityId{2}, CommunityId{1}) == EditResult::Ok); // current parent: no change
    CHECK(t.parent_count() == 1);
    CHECK(t.parent(CommunityId{2}) == CommunityId{1});

    CHECK(t.set_parent(CommunityId{}, CommunityId{1}) == EditResult::Invalid);
    CHECK(t.set_parent(CommunityId{2}, CommunityId{}) == EditResult::Invalid);
    CHECK(t.set_parent(CommunityId{2}, CommunityId{2}) == EditResult::Invalid);
    CHECK(t.clear_parent(CommunityId{}) == EditResult::Invalid);

    REQUIRE(t.set_parent(CommunityId{3}, CommunityId{2}) == EditResult::Ok);
    CHECK(t.set_parent(CommunityId{1}, CommunityId{3}) == EditResult::Conflict); // 1 > 2 > 3 > 1
    CHECK(t.set_parent(CommunityId{1}, CommunityId{2}) == EditResult::Conflict);
    CHECK(!t.parent(CommunityId{1}).has_value());

    CHECK(t.set_parent(CommunityId{3}, CommunityId{1}) == EditResult::Ok); // re-parent
    CHECK(t.chain(CommunityId{3}).size() == 2);
    CHECK(t.clear_parent(CommunityId{3}) == EditResult::Ok);
    CHECK(t.clear_parent(CommunityId{3}) == EditResult::NotFound);
    CHECK(t.chain(CommunityId{3}).size() == 1);
}

TEST_CASE("hierarchy: the depth limit returns Full, including when re-parenting a subtree") {
    StanceTable t;
    make_line(t, {1, 2, 3, 4, 5, 6}); // depth 6: the maximum
    CHECK(t.chain(CommunityId{6}).size() == MAX_COMMUNITY_DEPTH);
    CHECK(t.set_parent(CommunityId{7}, CommunityId{6}) == EditResult::Full);
    CHECK(t.set_parent(CommunityId{7}, CommunityId{5}) == EditResult::Ok);

    // Subtree 10 > 11 > 12 (height 3) fits under 3 (depth 3) but not under 4 (depth 4).
    make_line(t, {10, 11, 12});
    CHECK(t.set_parent(CommunityId{10}, CommunityId{4}) == EditResult::Full);
    CHECK(!t.parent(CommunityId{10}).has_value());
    CHECK(t.set_parent(CommunityId{10}, CommunityId{3}) == EditResult::Ok);
    CHECK(t.chain(CommunityId{12}).size() == 6);
    // Moving the subtree deeper fails and leaves it where it was.
    CHECK(t.set_parent(CommunityId{10}, CommunityId{5}) == EditResult::Full);
    CHECK(t.parent(CommunityId{10}) == CommunityId{3});
    // Growing the subtree at its deepest node fails too.
    CHECK(t.set_parent(CommunityId{13}, CommunityId{12}) == EditResult::Full);
    // A wide subtree only counts its tallest branch.
    CHECK(t.set_parent(CommunityId{14}, CommunityId{10}) == EditResult::Ok);
    CHECK(t.set_parent(CommunityId{15}, CommunityId{11}) == EditResult::Ok);
}
