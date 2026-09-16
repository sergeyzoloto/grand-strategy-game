#include <doctest.h>

#include <cmath>
#include <cstdint>
#include <limits>

#include "sim/quantize.hpp"

using namespace sim;

namespace {
constexpr double INF = std::numeric_limits<double>::infinity();
constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
} // namespace

TEST_CASE("unit quantization: bounds and saturation") {
    CHECK(quantize_unit<std::uint16_t>(0.0) == 0);
    CHECK(quantize_unit<std::uint16_t>(1.0) == 65535);
    CHECK(quantize_unit<std::uint16_t>(-0.5) == 0);
    CHECK(quantize_unit<std::uint16_t>(1.5) == 65535);
    CHECK(quantize_unit<std::uint16_t>(INF) == 65535);
    CHECK(quantize_unit<std::uint16_t>(-INF) == 0);
    CHECK(quantize_unit<std::uint8_t>(0.5) == 128); // 127.5 rounds up
    CHECK(dequantize_unit<std::uint16_t>(65535) == 1.0f);
    CHECK(dequantize_unit<std::uint16_t>(0) == 0.0f);
}

TEST_CASE("unit quantization: round trip of every representable value") {
    for (int raw = 0; raw <= 65535; ++raw) {
        const auto r = static_cast<std::uint16_t>(raw);
        const auto back = quantize_unit<std::uint16_t>(static_cast<double>(dequantize_unit(r)));
        if (back != r) {
            FAIL_CHECK("uint16 round trip failed at raw " << raw);
        }
    }
    for (int raw = 0; raw <= 255; ++raw) {
        const auto r = static_cast<std::uint8_t>(raw);
        CHECK(quantize_unit<std::uint8_t>(static_cast<double>(dequantize_unit(r))) == r);
    }
}

TEST_CASE("add saturates at the bounds") {
    CHECK(add_unit<std::uint16_t>(65000, 0.5) == 65535);
    CHECK(add_unit<std::uint16_t>(100, -0.5) == 0);
    CHECK(add_unit<std::uint16_t>(100, -INF) == 0);
}

TEST_CASE("NaN leaves the stored value unchanged") {
    CHECK(store_unit<std::uint16_t>(1234, NaN) == 1234);
    CHECK(add_unit<std::uint16_t>(1234, NaN) == 1234);
}

TEST_CASE("500 increments of 0.001 on uint16 give 0.5 +- 0.01") {
    std::uint16_t raw = 0;
    for (int i = 0; i < 500; ++i) {
        raw = add_unit(raw, 0.001);
    }
    CHECK(std::abs(static_cast<double>(dequantize_unit(raw)) - 0.5) <= 0.01);
}
