#include <doctest.h>

#include <cstdint>
#include <type_traits>

#include "sim/character_lists.hpp"
#include "sim/fixed_vector.hpp"

using sim::FixedVector;

namespace {

// Non-zero defaults show that freed slots are value-initialized, not zeroed.
struct Probe {
    int key = 7;
    int payload = -1;
    friend bool operator==(const Probe&, const Probe&) = default;
};

template<class T, std::size_t N>
concept FixedVectorAccepted = requires { typename FixedVector<T, N>; };

} // namespace

static_assert(std::is_trivially_copyable_v<FixedVector<int, 4>>);
static_assert(std::is_trivially_copyable_v<FixedVector<Probe, 8>>);
static_assert(std::is_trivially_copyable_v<FixedVector<sim::PractiseEntry, sim::PRACTISE_CAP>>);
static_assert(FixedVectorAccepted<int, 1> && FixedVectorAccepted<int, 255>);
static_assert(!FixedVectorAccepted<int, 0> && !FixedVectorAccepted<int, 256>);
static_assert(FixedVector<int, 255>::capacity() == 255);

TEST_CASE("FixedVector: bounds") {
    FixedVector<int, 3> v;
    CHECK(v.empty());
    CHECK(!v.full());
    CHECK(v.size() == 0);
    CHECK(v.capacity() == 3);
    CHECK(v.begin() == v.end());

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    CHECK(v.full());
    CHECK(v.size() == 3);
    CHECK(v[0] == 10);
    CHECK(v[2] == 30);
    CHECK(v.end() - v.begin() == 3);
}

TEST_CASE("FixedVector: ordered insert at front, middle and end") {
    FixedVector<int, 5> v;
    v.insert(0, 30); // [30]
    v.insert(0, 10); // [10 30]
    v.insert(1, 20); // [10 20 30]
    v.insert(3, 50); // [10 20 30 50]
    v.insert(3, 40); // [10 20 30 40 50]
    REQUIRE(v.size() == 5);
    for (std::size_t i = 0; i < v.size(); ++i) {
        CHECK(v[i] == static_cast<int>((i + 1) * 10));
    }
}

TEST_CASE("FixedVector: erase shifts left and value-initializes freed slots") {
    FixedVector<Probe, 4> v;
    for (int i = 1; i <= 4; ++i) {
        v.push_back(Probe{.key = i, .payload = i * 100});
    }

    v.erase(1); // middle
    REQUIRE(v.size() == 3);
    CHECK(v[0].key == 1);
    CHECK(v[1].key == 3);
    CHECK(v[2].key == 4);
    CHECK(v.data()[3] == Probe{});

    v.erase(0); // front
    v.erase(1); // back
    REQUIRE(v.size() == 1);
    CHECK(v[0].key == 3);
    for (std::size_t i = 1; i < v.capacity(); ++i) {
        CAPTURE(i);
        CHECK(v.data()[i] == Probe{});
    }

    v.erase(0);
    CHECK(v.empty());
    CHECK(v.data()[0] == Probe{});
}

TEST_CASE("FixedVector: unused slots start value-initialized") {
    const FixedVector<Probe, 4> v;
    for (std::size_t i = 0; i < v.capacity(); ++i) {
        CHECK(v.data()[i] == Probe{});
    }
}

TEST_CASE("FixedVector: copies are independent") {
    FixedVector<int, 4> a;
    a.push_back(1);
    FixedVector<int, 4> b = a;
    a.push_back(2);
    a[0] = 9;
    CHECK(b.size() == 1);
    CHECK(b[0] == 1);
}
