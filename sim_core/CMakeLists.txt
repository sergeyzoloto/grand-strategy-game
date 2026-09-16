add_library(sim_core STATIC
    src/character.cpp
    src/character_lists.cpp
    src/character_registry.cpp
    src/mortality.cpp
    src/opinion.cpp
    src/relation_graph.cpp
    src/stance_table.cpp
)

target_include_directories(sim_core PUBLIC include)
target_compile_features(sim_core PUBLIC cxx_std_20)
sim_configure_target(sim_core)
