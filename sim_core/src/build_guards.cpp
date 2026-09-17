// Build guards: sim_core must be compiled as ISO C++20 without fast-math. This file lives
// in sim_core's sources, not in a header, because other targets include the headers with
// their own flags. It defines nothing.
//
// FMA contraction (-ffp-contract=off) cannot be detected by the preprocessor; see the
// "-mfma" check in CLAUDE.md.

#if defined(_MSC_VER)
// MSVC and clang-cl report __cplusplus as 199711L unless /Zc:__cplusplus is given.
#    if !defined(_MSVC_LANG) || _MSVC_LANG < 202002L
#        error "sim_core requires C++20 (/std:c++20 or later)"
#    endif
#else
#    if __cplusplus < 202002L
#        error "sim_core requires C++20 (-std=c++20 or later)"
#    endif
// GCC and Clang define __STRICT_ANSI__ only in ISO modes (-std=c++20, not -std=gnu++20).
#    if (defined(__GNUC__) || defined(__clang__)) && !defined(__STRICT_ANSI__)
#        error "sim_core must not be compiled in a GNU dialect: set CMAKE_CXX_EXTENSIONS OFF (-std=c++20)"
#    endif
#endif

#if defined(__FAST_MATH__)
#    error "sim_core must not be compiled with -ffast-math (or any flag that defines __FAST_MATH__)"
#endif
