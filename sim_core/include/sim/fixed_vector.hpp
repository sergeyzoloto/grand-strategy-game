#pragma once

#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace sim {

// Inline-storage vector with a fixed capacity N (1..255) and a uint8 size.
// Never allocates; trivially copyable when T is. Every slot not in use holds a
// value-initialized T, including slots freed by erase, so no stale data remains.
// Preconditions (capacity, index bounds) are asserted; callers report input errors.
template<class T, std::size_t N>
    requires(N >= 1 && N <= 255 && std::default_initializable<T> && std::is_nothrow_copy_assignable_v<T>)
class FixedVector {
public:
    using value_type = T;

    [[nodiscard]] constexpr std::size_t size() const noexcept { return size_; }
    [[nodiscard]] static constexpr std::size_t capacity() noexcept { return N; }
    [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] constexpr bool full() const noexcept { return size_ == N; }

    [[nodiscard]] constexpr T& operator[](std::size_t i) noexcept {
        assert(i < size_);
        return items_[i];
    }
    [[nodiscard]] constexpr const T& operator[](std::size_t i) const noexcept {
        assert(i < size_);
        return items_[i];
    }

    [[nodiscard]] constexpr T* data() noexcept { return items_; }
    [[nodiscard]] constexpr const T* data() const noexcept { return items_; }
    [[nodiscard]] constexpr T* begin() noexcept { return items_; }
    [[nodiscard]] constexpr const T* begin() const noexcept { return items_; }
    [[nodiscard]] constexpr T* end() noexcept { return items_ + size_; }
    [[nodiscard]] constexpr const T* end() const noexcept { return items_ + size_; }

    constexpr void push_back(const T& value) noexcept {
        assert(!full());
        items_[size_] = value;
        size_ = static_cast<std::uint8_t>(size_ + 1);
    }

    // Inserts before position `index` (0..size), shifting later items right.
    constexpr void insert(std::size_t index, const T& value) noexcept {
        assert(!full());
        assert(index <= size_);
        for (std::size_t i = size_; i > index; --i) {
            items_[i] = items_[i - 1];
        }
        items_[index] = value;
        size_ = static_cast<std::uint8_t>(size_ + 1);
    }

    // Removes the item at `index`, shifting later items left and value-initializing
    // the freed last slot.
    constexpr void erase(std::size_t index) noexcept {
        assert(index < size_);
        for (std::size_t i = index; i + 1 < size_; ++i) {
            items_[i] = items_[i + 1];
        }
        size_ = static_cast<std::uint8_t>(size_ - 1);
        items_[size_] = T{};
    }

private:
    T items_[N]{};            // slots [0, size) in use; the rest value-initialized
    std::uint8_t size_ = 0;   // number of items in use, 0..N
};

} // namespace sim
