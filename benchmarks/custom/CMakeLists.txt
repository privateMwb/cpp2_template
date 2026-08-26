# ============================================================
# Build configuration for the custom benchmark suite.
# ============================================================

# Debug builds skew every timing — default to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Collect all benchmark suite source files.
file(GLOB_RECURSE BENCH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/suite/*.cpp"
)

# Build the benchmark executable.
add_executable(
    benchmarks
    bench_main.cpp
    ${BENCH_SOURCES}
)

# Allow includes from support/, the shared reference baseline, and the
# library's public headers.
target_include_directories(
    benchmarks
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared
        ${CMAKE_SOURCE_DIR}/include
)

# Link the library under test into the benchmark executable.
target_link_libraries(
    benchmarks
    PRIVATE
        ${PROJECT_NAME}
)

# Require C++17.
target_compile_features(benchmarks PRIVATE cxx_std_17)