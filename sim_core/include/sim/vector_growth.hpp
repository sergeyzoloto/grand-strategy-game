#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

namespace sim::detail {

// Ensures v has room for one more element, growing capacity geometrically (doubling,
// at least `min_capacity`). Call it on every container an edit will grow before the
// first write, so a bad_alloc leaves state unchanged; after it, one push_back or
// insert cannot reallocate. Never reserve(size() + 1): that reallocates on every insert.
template<class T>
void reserve_one_more(std::vector<T>& v, std::size_t min_capacity) {
    if (v.size() == v.capacity()) {
        v.reserve(std::max(min_capacity, v.capacity() * 2));
    }
}

} // namespace sim::detail
