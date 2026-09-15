add_library(doctest INTERFACE)
target_include_directories(doctest SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/third_party/doctest)

add_executable(sim_tests
    main.cpp
    test_ids.cpp
    test_quantize.cpp
    test_date.cpp
    test_character.cpp
    test_mortality.cpp
)

target_link_libraries(sim_tests PRIVATE sim_core doctest)
sim_configure_target(sim_tests)

add_test(NAME sim_tests COMMAND sim_tests)
