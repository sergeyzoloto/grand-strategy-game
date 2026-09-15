cmake_minimum_required(VERSION 3.20)

project(grand_strategy_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SIM_BUILD_TESTS "Build the sim_core test executable" ON)
option(SIM_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(SIM_SANITIZE "Build with AddressSanitizer and UndefinedBehaviorSanitizer (use with Debug)" OFF)

# Applies warning, floating-point and sanitizer flags to one of our own targets.
# Third-party code (doctest) never receives these flags.
function(sim_configure_target target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /fp:precise)
        if(SIM_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
        if(SIM_SANITIZE)
            # MSVC has no UBSan; only ASan is available.
            target_compile_options(${target} PRIVATE /fsanitize=address)
        endif()
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wconversion -Wsign-conversion -Wshadow
            # Deterministic floating point: no FMA contraction. Never use -ffast-math.
            -ffp-contract=off)
        if(SIM_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
        if(SIM_SANITIZE)
            set(sanitize_flags -fsanitize=address,undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer)
            target_compile_options(${target} PRIVATE ${sanitize_flags})
            target_link_options(${target} PRIVATE ${sanitize_flags})
        endif()
    endif()
endfunction()

add_subdirectory(sim_core)

if(SIM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
