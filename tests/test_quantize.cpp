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

TEST_CASE("signed quantization: bounds and saturation") {
    CHECK(quantize_signed<std::int8_t>(1.0, 1.0) == 127);
    CHECK(quantize_signed<std::int8_t>(-1.0, 1.0) == -127);
    CHECK(quantize_signed<std::int8_t>(0.0, 1.0) == 0);
    CHECK(quantize_signed<std::int8_t>(5.0, 1.0) == 127);
    CHECK(quantize_signed<std::int8_t>(-5.0, 1.0) == -127);
    CHECK(quantize_signed<std::int8_t>(INF, 1.0) == 127);
    CHECK(quantize_signed<std::int8_t>(-INF, 1.0) == -127);
    CHECK(quantize_signed<std::int16_t>(-1e30, 1.0) == -32767);

    // Non-unit range R.
    CHECK(quantize_signed<std::int8_t>(2.0, 2.0) == 127);
    CHECK(quantize_signed<std::int8_t>(1.0, 2.0) == 64); // 63.5 rounds away from zero
    CHECK(dequantize_signed<std::int8_t>(127, 2.0) == doctest::Approx(2.0));

    CHECK(dequantize_signed<std::int8_t>(127, 1.0) == 1.0f);
    CHECK(dequantize_signed<std::int8_t>(-127, 1.0) == -1.0f);
}

TEST_CASE("signed quantization: -x and +x are symmetric") {
    for (int i = 0; i <= 2000; ++i) {
        const double v = static_cast<double>(i) / 1000.0;
        CHECK(quantize_signed<std::int8_t>(-v, 1.0) == -quantize_signed<std::int8_t>(v, 1.0));
    }
    for (int raw = -127; raw <= 127; ++raw) {
        const auto r = static_cast<std::int8_t>(raw);
        const auto neg = static_cast<std::int8_t>(-raw);
        CHECK(dequantize_signed(neg, 1.0) == -dequantize_signed(r, 1.0));
    }
}

TEST_CASE("signed quantization: round trip of every representable value") {
    for (int raw = -127; raw <= 127; ++raw) {
        const auto r = static_cast<std::int8_t>(raw);
        CHECK(quantize_signed<std::int8_t>(static_cast<double>(dequantize_signed(r, 1.0)), 1.0) == r);
    }
    for (int raw = -32767; raw <= 32767; ++raw) {
        const auto r = static_cast<std::int16_t>(raw);
        const auto back = quantize_signed<std::int16_t>(static_cast<double>(dequantize_signed(r, 1.0)), 1.0);
        if (back != r) {
            FAIL_CHECK("int16 round trip failed at raw " << raw);
        }
    }
}

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
    CHECK(add_signed<std::int8_t>(120, 0.5, 1.0) == 127);
    CHECK(add_signed<std::int8_t>(-120, -0.5, 1.0) == -127);
    CHECK(add_signed<std::int8_t>(0, INF, 1.0) == 127);
    CHECK(add_unit<std::uint16_t>(65000, 0.5) == 65535);
    CHECK(add_unit<std::uint16_t>(100, -0.5) == 0);
    CHECK(add_unit<std::uint16_t>(100, -INF) == 0);
}

TEST_CASE("NaN leaves the stored value unchanged") {
    CHECK(store_signed<std::int8_t>(42, NaN, 1.0) == 42);
    CHECK(add_signed<std::int8_t>(-42, NaN, 1.0) == -42);
    CHECK(store_unit<std::uint16_t>(1234, NaN) == 1234);
    CHECK(add_unit<std::uint16_t>(1234, NaN) == 1234);
}

TEST_CASE("resolution rule: sub-half-step adds on int8 are no-ops") {
    std::int8_t raw = 0;
    for (int i = 0; i < 1000; ++i) {
        raw = add_signed(raw, 0.001, 1.0);
    }
    CHECK(raw == 0);
    CHECK(add_signed<std::int8_t>(0, 1.0 / 127.0, 1.0) == 1);
}

TEST_CASE("500 increments of 0.001 on uint16 give 0.5 +- 0.01") {
    std::uint16_t raw = 0;
    for (int i = 0; i < 500; ++i) {
        raw = add_unit(raw, 0.001);
    }
    CHECK(std::abs(static_cast<double>(dequantize_unit(raw)) - 0.5) <= 0.01);
}
