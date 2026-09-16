add_library(doctest INTERFACE)
target_include_directories(doctest SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/third_party/doctest)

add_executable(sim_tests
    main.cpp
    test_ids.cpp
    test_date.cpp
    test_character.cpp
    test_character_lists.cpp
    test_fixed_vector.cpp
    test_mortality.cpp
    test_noise.cpp
    test_opinion.cpp
    test_registry.cpp
    test_relations.cpp
    test_stances.cpp
    test_strong_opinion.cpp
)

target_link_libraries(sim_tests PRIVATE sim_core doctest)
sim_configure_target(sim_tests)

add_test(NAME sim_tests COMMAND sim_tests)
