#include <doctest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <random>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/relation_graph.hpp"
#include "sim/relations.hpp"

using namespace sim;

namespace {

CharacterRegistry make_registry(std::uint32_t count) {
    CharacterRegistry registry;
    for (std::uint32_t i = 0; i < count; ++i) {
        (void)registry.create(NameId{i + 1}, Gender::Female, Date{0}, CharacterInit{});
    }
    return registry;
}

const CharacterId A{1};
const CharacterId B{2};
const CharacterId C{3};
const CharacterId D{4};
const CharacterId E{5};

std::vector<RelationEdge> edges_of(const CharacterRegistry& registry, CharacterId id) {
    const auto span = registry.relations(id);
    return {span.begin(), span.end()};
}

bool same_edges(const std::vector<RelationEdge>& a, const std::vector<RelationEdge>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i].other != b[i].other || a[i].mask != b[i].mask) {
            return false;
        }
    }
    return true;
}

std::vector<CharacterId> to_vector(const FixedVector<CharacterId, MAX_PARENTS>& v) {
    return {v.begin(), v.end()};
}

} // namespace

// ---- relation_info -------------------------------------------------------------

static_assert(relation_info(RelationType::Friend)->kind == RelationKind::OneWay);
static_assert(!relation_info(RelationType::Friend)->complement.has_value());
static_assert(!relation_info(RelationType::Rival)->complement.has_value());
static_assert(!relation_info(RelationType::Attraction)->complement.has_value());
static_assert(relation_info(RelationType::Parent)->complement == RelationType::Child);
static_assert(relation_info(RelationType::Child)->complement == RelationType::Parent);
static_assert(relation_info(RelationType::Liege)->complement == RelationType::Vassal);
static_assert(relation_info(RelationType::Vassal)->complement == RelationType::Liege);
static_assert(relation_info(RelationType::Employer)->complement == RelationType::Employee);
static_assert(relation_info(RelationType::Employee)->complement == RelationType::Employer);
static_assert(relation_info(RelationType::Spouse)->complement == RelationType::Spouse);
static_assert(!relation_info(static_cast<RelationType>(10)).has_value());
static_assert(sizeof(RelationEdge) == 8);

TEST_CASE("sizeof(RelationEdge)") {
    MESSAGE("sizeof(RelationEdge) = " << sizeof(RelationEdge));
    CHECK(sizeof(RelationEdge) == 8);
}

// ---- one-way ---------------------------------------------------------------------

TEST_CASE("one-way: only the source edge changes; Duplicate and NotFound") {
    CharacterRegistry r = make_registry(3);
    CHECK(r.set_relation(A, RelationType::Friend, B) == EditResult::Ok);
    CHECK(r.has_relation(A, RelationType::Friend, B));
    CHECK(!r.has_relation(B, RelationType::Friend, A));
    CHECK(r.relations(B).empty());
    REQUIRE(r.relations(A).size() == 1);
    CHECK(r.relations(A)[0].other == B);
    CHECK(r.relations(A)[0].mask == relation_bit(RelationType::Friend));

    CHECK(r.set_relation(A, RelationType::Friend, B) == EditResult::Duplicate);
    CHECK(r.set_relation(A, RelationType::Rival, B) == EditResult::Ok); // same edge, second bit
    CHECK(r.relations(A).size() == 1);

    CHECK(r.clear_relation(A, RelationType::Attraction, B) == EditResult::NotFound);
    CHECK(r.clear_relation(B, RelationType::Friend, A) == EditResult::NotFound);
    CHECK(r.set_relation(A, RelationType::Friend, CharacterId{99}) == EditResult::NotFound);
    CHECK(r.clear_relation(CharacterId{99}, RelationType::Friend, A) == EditResult::NotFound);

    CHECK(r.clear_relation(A, RelationType::Friend, B) == EditResult::Ok);
    CHECK(r.clear_relation(A, RelationType::Rival, B) == EditResult::Ok);
    CHECK(r.relations(A).empty()); // empty edge removed
}

TEST_CASE("one-way: paired types, a == b, id 0 and unknown enum values are Invalid") {
    CharacterRegistry r = make_registry(2);
    CHECK(r.set_relation(A, RelationType::Parent, B) == EditResult::Invalid);
    CHECK(r.set_relation(A, RelationType::Spouse, B) == EditResult::Invalid);
    CHECK(r.clear_relation(A, RelationType::Liege, B) == EditResult::Invalid);
    CHECK(r.set_relation(A, RelationType::Friend, A) == EditResult::Invalid);
    CHECK(r.set_relation(CharacterId{}, RelationType::Friend, B) == EditResult::Invalid);
    CHECK(r.set_relation(A, static_cast<RelationType>(31), B) == EditResult::Invalid);
    CHECK(r.clear_relation(A, RelationType::Friend, A) == EditResult::Invalid);
    // Invalid is reported before NotFound.
    CHECK(r.set_relation(CharacterId{99}, RelationType::Friend, CharacterId{99}) == EditResult::Invalid);
    CHECK(r.relations(A).empty());
    CHECK(r.relations(B).empty());
}

// ---- paired ----------------------------------------------------------------------

TEST_CASE("paired: both sides get the right complement") {
    CharacterRegistry r = make_registry(5);
    CHECK(r.link(A, RelationType::Child, B) == EditResult::Ok); // B is A's child
    CHECK(r.has_relation(A, RelationType::Child, B));
    CHECK(r.has_relation(B, RelationType::Parent, A));
    CHECK(!r.has_relation(A, RelationType::Parent, B));

    CHECK(r.link(C, RelationType::Liege, D) == EditResult::Ok); // D is C's liege
    CHECK(r.has_relation(C, RelationType::Liege, D));
    CHECK(r.has_relation(D, RelationType::Vassal, C));

    CHECK(r.link(D, RelationType::Employee, E) == EditResult::Ok);
    CHECK(r.has_relation(E, RelationType::Employer, D));

    CHECK(r.link(A, RelationType::Spouse, E) == EditResult::Ok);
    CHECK(r.has_relation(A, RelationType::Spouse, E));
    CHECK(r.has_relation(E, RelationType::Spouse, A));
    CHECK(r.link(E, RelationType::Spouse, A) == EditResult::Duplicate);
    CHECK(r.link(A, RelationType::Child, B) == EditResult::Duplicate);
    CHECK(r.link(B, RelationType::Parent, A) == EditResult::Duplicate); // same link from the other side
}

TEST_CASE("paired: unlink removes both sides; one-way bits survive; empty edges disappear") {
    CharacterRegistry r = make_registry(2);
    REQUIRE(r.link(A, RelationType::Employer, B) == EditResult::Ok);
    REQUIRE(r.set_relation(A, RelationType::Rival, B) == EditResult::Ok);

    CHECK(r.unlink(A, RelationType::Employer, B) == EditResult::Ok);
    CHECK(!r.has_relation(A, RelationType::Employer, B));
    CHECK(!r.has_relation(B, RelationType::Employee, A));
    CHECK(r.has_relation(A, RelationType::Rival, B)); // one-way bit on the same edge survives
    REQUIRE(r.relations(A).size() == 1);
    CHECK(r.relations(A)[0].mask == relation_bit(RelationType::Rival));
    CHECK(r.relations(B).empty()); // reverse edge had only Employee

    CHECK(r.unlink(A, RelationType::Employer, B) == EditResult::NotFound);
    CHECK(r.link(A, RelationType::Spouse, B) == EditResult::Ok);
    CHECK(r.unlink(B, RelationType::Spouse, A) == EditResult::Ok);
    CHECK(r.clear_relation(A, RelationType::Rival, B) == EditResult::Ok);
    CHECK(r.relations(A).empty());
    CHECK(r.relations(B).empty());
}

TEST_CASE("paired: one-way types, a == b and unknown ids are rejected by link and unlink") {
    CharacterRegistry r = make_registry(2);
    CHECK(r.link(A, RelationType::Friend, B) == EditResult::Invalid);
    CHECK(r.unlink(A, RelationType::Attraction, B) == EditResult::Invalid);
    CHECK(r.link(A, RelationType::Spouse, A) == EditResult::Invalid);
    CHECK(r.link(A, static_cast<RelationType>(12), B) == EditResult::Invalid);
    CHECK(r.link(A, RelationType::Spouse, CharacterId{3}) == EditResult::NotFound);
    CHECK(r.unlink(CharacterId{3}, RelationType::Spouse, A) == EditResult::NotFound);
    CHECK(r.relations(A).empty());
    CHECK(r.relations(B).empty());
}

TEST_CASE("Conflict leaves both sides unchanged") {
    CharacterRegistry r = make_registry(3);
    REQUIRE(r.link(A, RelationType::Child, B) == EditResult::Ok);     // B is A's child
    REQUIRE(r.set_relation(B, RelationType::Friend, A) == EditResult::Ok);
    const auto before_a = edges_of(r, A);
    const auto before_b = edges_of(r, B);

    CHECK(r.link(A, RelationType::Parent, B) == EditResult::Conflict); // B also A's parent
    CHECK(r.link(B, RelationType::Child, A) == EditResult::Conflict);  // same, from B's side
    CHECK(same_edges(edges_of(r, A), before_a));
    CHECK(same_edges(edges_of(r, B), before_b));

    REQUIRE(r.link(A, RelationType::Liege, C) == EditResult::Ok);
    CHECK(r.link(A, RelationType::Vassal, C) == EditResult::Conflict);
    CHECK(r.link(C, RelationType::Liege, A) == EditResult::Conflict);
    CHECK(r.has_relation(A, RelationType::Liege, C));
    CHECK(!r.has_relation(A, RelationType::Vassal, C));
}

TEST_CASE("parent limit: a third parent is Full and leaves both sides unchanged") {
    CharacterRegistry r = make_registry(5);
    REQUIRE(r.link(A, RelationType::Child, C) == EditResult::Ok);
    REQUIRE(r.link(C, RelationType::Parent, B) == EditResult::Ok);
    REQUIRE(r.set_relation(D, RelationType::Friend, C) == EditResult::Ok);
    const auto before_c = edges_of(r, C);
    const auto before_d = edges_of(r, D);

    CHECK(r.link(D, RelationType::Child, C) == EditResult::Full);
    CHECK(r.link(C, RelationType::Parent, D) == EditResult::Full);
    CHECK(same_edges(edges_of(r, C), before_c));
    CHECK(same_edges(edges_of(r, D), before_d));
    CHECK(to_vector(r.parents(C)) == std::vector<CharacterId>{A, B});

    // A parent may have any number of children.
    CHECK(r.link(A, RelationType::Child, D) == EditResult::Ok);
    CHECK(r.link(A, RelationType::Child, E) == EditResult::Ok);
    CHECK(r.children(A) == std::vector<CharacterId>{C, D, E});
}

// ---- family queries ---------------------------------------------------------------

TEST_CASE("siblings: derived from shared parents; full and half siblings") {
    // P1 + P2 -> K1, K2 (full siblings); P2 + P3 -> K3 (half sibling of K1 and K2); X has no parents.
    CharacterRegistry r = make_registry(7);
    const CharacterId P1{1}, P2{2}, P3{3}, K1{4}, K2{5}, K3{6}, X{7};
    REQUIRE(r.link(P1, RelationType::Child, K2) == EditResult::Ok);
    REQUIRE(r.link(P2, RelationType::Child, K2) == EditResult::Ok);
    REQUIRE(r.link(K1, RelationType::Parent, P2) == EditResult::Ok);
    REQUIRE(r.link(K1, RelationType::Parent, P1) == EditResult::Ok);
    REQUIRE(r.link(P3, RelationType::Child, K3) == EditResult::Ok);
    REQUIRE(r.link(P2, RelationType::Child, K3) == EditResult::Ok);

    CHECK(to_vector(r.parents(K1)) == std::vector<CharacterId>{P1, P2});
    CHECK(r.children(P2) == std::vector<CharacterId>{K1, K2, K3});

    CHECK(r.siblings(K1) == std::vector<CharacterId>{K2, K3}); // no self, no duplicates, id order
    CHECK(r.siblings(K2) == std::vector<CharacterId>{K1, K3});
    CHECK(r.siblings(K3) == std::vector<CharacterId>{K1, K2});
    CHECK(r.shared_parents(K1, K2) == 2);
    CHECK(r.shared_parents(K2, K1) == 2);
    CHECK(r.shared_parents(K1, K3) == 1);
    CHECK(r.shared_parents(K3, K2) == 1);

    CHECK(r.siblings(X).empty());
    CHECK(r.parents(X).empty());
    CHECK(r.shared_parents(X, K1) == 0);
    CHECK(r.shared_parents(K1, K1) == 0);
    CHECK(r.siblings(P1).empty());
    CHECK(r.siblings(CharacterId{}).empty());
    CHECK(r.siblings(CharacterId{99}).empty());
    CHECK(r.shared_parents(K1, CharacterId{99}) == 0);

    REQUIRE(r.unlink(K1, RelationType::Parent, P2) == EditResult::Ok);
    CHECK(r.shared_parents(K1, K2) == 1);
    CHECK(r.siblings(K1) == std::vector<CharacterId>{K2});
}

// ---- property test against a reference model -----------------------------------------

namespace {

// Naive model: masks per ordered pair (source -> map of target -> mask), applying the
// rules directly.
class ReferenceModel {
public:
    explicit ReferenceModel(std::uint32_t count) : count_(count), masks_(count + 1) {}

    EditResult apply(int op, std::uint32_t a, std::uint8_t type, std::uint32_t b) {
        // Independent table: -2 unknown, -1 one-way, otherwise the complement.
        static constexpr std::array<int, 10> complement{-1, -1, -1, 4, 3, 6, 5, 8, 7, 9};
        const int comp = type < complement.size() ? complement[type] : -2;
        const bool paired_op = op >= 2;
        if (a == 0 || b == 0 || a == b || comp == -2 || (paired_op ? comp < 0 : comp >= 0)) {
            return EditResult::Invalid;
        }
        if (a > count_ || b > count_) {
            return EditResult::NotFound;
        }
        const std::uint32_t bit = std::uint32_t{1} << type;
        switch (op) {
        case 0: // set_relation
            if (mask(a, b) & bit) {
                return EditResult::Duplicate;
            }
            set_mask(a, b, mask(a, b) | bit);
            return EditResult::Ok;
        case 1: // clear_relation
            if (!(mask(a, b) & bit)) {
                return EditResult::NotFound;
            }
            set_mask(a, b, mask(a, b) & ~bit);
            return EditResult::Ok;
        case 2: { // link
            const std::uint32_t comp_bit = std::uint32_t{1} << comp;
            if (mask(a, b) & bit) {
                return EditResult::Duplicate;
            }
            if (comp != type && ((mask(a, b) & comp_bit) || (mask(b, a) & bit))) {
                return EditResult::Conflict;
            }
            if ((type == 4 && parent_count(b) >= 2) || (type == 3 && parent_count(a) >= 2)) {
                return EditResult::Full;
            }
            set_mask(a, b, mask(a, b) | bit);
            set_mask(b, a, mask(b, a) | comp_bit);
            return EditResult::Ok;
        }
        default: { // unlink
            const std::uint32_t comp_bit = std::uint32_t{1} << comp;
            if (!(mask(a, b) & bit)) {
                return EditResult::NotFound;
            }
            set_mask(a, b, mask(a, b) & ~bit);
            set_mask(b, a, mask(b, a) & ~comp_bit);
            return EditResult::Ok;
        }
        }
    }

    // Target id -> mask for edges from a, in id order.
    const std::map<std::uint32_t, std::uint32_t>& edges(std::uint32_t a) const { return masks_[a]; }

private:
    std::uint32_t mask(std::uint32_t a, std::uint32_t b) const {
        const auto it = masks_[a].find(b);
        return it == masks_[a].end() ? 0 : it->second;
    }
    void set_mask(std::uint32_t a, std::uint32_t b, std::uint32_t m) {
        if (m == 0) {
            masks_[a].erase(b);
        } else {
            masks_[a][b] = m;
        }
    }
    int parent_count(std::uint32_t child) const {
        int n = 0;
        for (const auto& [other, m] : masks_[child]) {
            if (m & (std::uint32_t{1} << 3)) {
                ++n;
            }
        }
        return n;
    }

    std::uint32_t count_;
    std::vector<std::map<std::uint32_t, std::uint32_t>> masks_; // index = source id
};

struct PairedBits {
    std::uint32_t bit;
    std::uint32_t complement_bit;
};

// {bit, complement bit} for every paired type, computed at compile time from relation_info.
constexpr std::array<PairedBits, 7> PAIRED_BITS = [] {
    std::array<PairedBits, 7> result{};
    std::size_t n = 0;
    for (std::uint8_t t = 0; t < 32; ++t) {
        const auto type = static_cast<RelationType>(t);
        const auto info = relation_info(type);
        if (info && info->kind == RelationKind::Paired) {
            result[n++] = PairedBits{relation_bit(type), relation_bit(*info->complement)};
        }
    }
    return result;
}();
static_assert(PAIRED_BITS[6].bit != 0 && PAIRED_BITS[6].complement_bit != 0); // all 7 paired types found

constexpr std::array<std::uint8_t, 3> ONE_WAY_RAW{0, 1, 2};
constexpr std::array<std::uint8_t, 7> PAIRED_RAW{3, 4, 5, 6, 7, 8, 9};

const char* op_name(int op) {
    static constexpr std::array<const char*, 4> names{"set_relation", "clear_relation", "link", "unlink"};
    return names[static_cast<std::size_t>(op)];
}

} // namespace

TEST_CASE("property: random edits match a reference model and keep paired bits symmetric") {
    constexpr std::uint32_t COUNT = 12;
    constexpr int STEPS = 20000;
    CharacterRegistry r = make_registry(COUNT);
    ReferenceModel model(COUNT);
    // Raw mt19937 output only: standard distributions differ between standard libraries.
    std::mt19937 rng(20260916u);

    std::array<int, 6> result_counts{}; // indexed by EditResult
    for (int step = 0; step < STEPS; ++step) {
        const int op = static_cast<int>(rng() % 4);

        // a: a real id 96% of the time, otherwise 0 (invalid) or COUNT + 1 (unknown).
        std::uint32_t a = 0;
        const auto a_roll = rng() % 100;
        if (a_roll < 96) {
            a = 1 + static_cast<std::uint32_t>(rng() % COUNT);
        } else {
            a = a_roll < 98 ? 0 : COUNT + 1;
        }
        // b: differs from a 95% of the time.
        std::uint32_t b = a;
        if (rng() % 100 < 95) {
            if (a >= 1 && a <= COUNT) {
                b = 1 + static_cast<std::uint32_t>(rng() % (COUNT - 1));
                if (b >= a) {
                    ++b;
                }
            } else {
                b = 1 + static_cast<std::uint32_t>(rng() % COUNT);
            }
        }
        // type: of the right kind for the call 90% of the time, otherwise any raw value
        // 0..10 (wrong kind or unknown).
        std::uint8_t type_raw = 0;
        if (rng() % 100 < 90) {
            type_raw = op < 2 ? ONE_WAY_RAW[rng() % ONE_WAY_RAW.size()] : PAIRED_RAW[rng() % PAIRED_RAW.size()];
        } else {
            type_raw = static_cast<std::uint8_t>(rng() % 11); // 10 = unknown type
        }
        const auto type = static_cast<RelationType>(type_raw);

        EditResult actual = EditResult::Invalid;
        switch (op) {
        case 0: actual = r.set_relation(CharacterId{a}, type, CharacterId{b}); break;
        case 1: actual = r.clear_relation(CharacterId{a}, type, CharacterId{b}); break;
        case 2: actual = r.link(CharacterId{a}, type, CharacterId{b}); break;
        default: actual = r.unlink(CharacterId{a}, type, CharacterId{b}); break;
        }
        const EditResult expected = model.apply(op, a, type_raw, b);
        ++result_counts[static_cast<std::size_t>(actual)];
        if (actual != expected) {
            FAIL("step " << step << ": " << op_name(op) << "(" << a << ", " << int(type_raw) << ", " << b
                         << ") returned " << int(actual) << ", model " << int(expected));
        }

        // Full edge sets after every operation, compared with the model. The spans stay
        // valid while checking: nothing mutates the registry here.
        for (std::uint32_t id = 1; id <= COUNT; ++id) {
            const auto edges = r.relations(CharacterId{id});
            const auto& expected_edges = model.edges(id);
            bool same = edges.size() == expected_edges.size();
            auto it = expected_edges.begin();
            for (std::size_t i = 0; same && i < edges.size(); ++i, ++it) {
                same = edges[i].other.value == it->first && edges[i].mask == it->second;
            }
            if (!same) {
                FAIL("step " << step << ": edge set of " << id << " differs from the model");
            }
        }
        // Invariants: no empty edges, at most MAX_PARENTS parents, paired bits symmetric.
        const auto mask_of = [&](CharacterId from, CharacterId to) -> std::uint32_t {
            for (const RelationEdge& e : r.relations(from)) {
                if (e.other == to) {
                    return e.mask;
                }
            }
            return 0;
        };
        for (std::uint32_t id = 1; id <= COUNT; ++id) {
            std::size_t parents = 0;
            for (const RelationEdge& e : r.relations(CharacterId{id})) {
                if (e.mask == 0) {
                    FAIL("step " << step << ": empty edge stored");
                }
                if (e.mask & relation_bit(RelationType::Parent)) {
                    ++parents;
                }
                const std::uint32_t reverse = mask_of(e.other, CharacterId{id});
                for (const PairedBits& paired : PAIRED_BITS) {
                    if ((e.mask & paired.bit) && !(reverse & paired.complement_bit)) {
                        FAIL("step " << step << ": paired bit " << paired.bit << " on " << id << " -> "
                                     << e.other.value << " has no complement");
                    }
                }
            }
            if (parents > MAX_PARENTS) {
                FAIL("step " << step << ": " << id << " has " << parents << " parents");
            }
        }
    }
    MESSAGE("random edits: " << STEPS << " steps; Ok " << result_counts[0] << ", Full " << result_counts[1]
                              << ", Duplicate " << result_counts[2] << ", Conflict " << result_counts[3]
                              << ", NotFound " << result_counts[4] << ", Invalid " << result_counts[5]);
    // Successful edits are the most frequent result, and every rule stays reachable.
    for (std::size_t i = 1; i < result_counts.size(); ++i) {
        CHECK(result_counts[0] > result_counts[i]);
    }
    for (std::size_t i = 0; i < result_counts.size(); ++i) {
        CAPTURE(i);
        CHECK(result_counts[i] > 0); // every rule is exercised at least once
    }
}

// ---- memory ---------------------------------------------------------------------------

TEST_CASE("memory: 1,500 characters with 20 edges each") {
    constexpr std::uint32_t COUNT = 1500;
    constexpr std::uint32_t EDGES = 20;
    CharacterRegistry r = make_registry(COUNT);
    for (std::uint32_t id = 1; id <= COUNT; ++id) {
        for (std::uint32_t k = 1; k <= EDGES; ++k) {
            const CharacterId other{(id - 1 + k) % COUNT + 1};
            REQUIRE(r.set_relation(CharacterId{id}, RelationType::Friend, other) == EditResult::Ok);
        }
    }
    // Geometric doubling from a minimum capacity guarantees capacity <= max(minimum, 2 * size)
    // for every vector that only grew. Bound relation storage by that, independent of
    // sizeof(Character).
    std::size_t edge_count = 0;
    std::size_t edge_blocks = 0;
    std::size_t bound = std::max(RelationGraph::MIN_NODE_CAPACITY, std::size_t{2} * COUNT)
                        * sizeof(std::vector<RelationEdge>);
    for (std::uint32_t id = 1; id <= COUNT; ++id) {
        const std::size_t size = r.relations(CharacterId{id}).size();
        edge_count += size;
        edge_blocks += size == 0 ? 0u : 1u;
        if (size != 0) {
            bound += std::max(RelationGraph::MIN_EDGE_CAPACITY, 2 * size) * sizeof(RelationEdge);
        }
    }
    CHECK(edge_count == COUNT * EDGES);
    const std::size_t relation_bytes = r.relation_bytes();
    // Allocator headers are not counted; glibc adds about 16 bytes per block (one per
    // non-empty edge list, plus the outer vector).
    MESSAGE("relation_bytes = " << relation_bytes << " (bound " << bound << "); with ~16 B allocator headers: "
                                << relation_bytes + (edge_blocks + 1) * 16);
    MESSAGE("registry allocated_bytes = " << r.allocated_bytes() << " (includes Character storage; not asserted)");
    CHECK(relation_bytes <= bound);
    CHECK(relation_bytes >= edge_count * sizeof(RelationEdge));
}
