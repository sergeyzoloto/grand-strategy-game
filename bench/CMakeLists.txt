# Manual benchmarks: build in Release, run by hand, not registered with ctest.
add_executable(sim_bench bench_main.cpp)
target_link_libraries(sim_bench PRIVATE sim_core)
sim_configure_target(sim_bench)
