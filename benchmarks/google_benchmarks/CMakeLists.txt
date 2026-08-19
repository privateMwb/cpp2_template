# ============================================================
# Build configuration for the Google Benchmark suite.
# ============================================================

# Fetch Google Benchmark.
include(FetchContent)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.1
)

# Skip Google Benchmark's own tests/install — only the library is needed.
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# Collect all benchmark suite source files.
file(GLOB_RECURSE BENCH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/suite/*.cpp"
)

# Build the benchmark executable.
add_executable(
    benchmarks_google
    bench_main.cpp
    ${BENCH_SOURCES}
)

# Allow includes from the shared reference baseline and the library's
# public headers.
target_include_directories(
    benchmarks_google
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared
        ${CMAKE_SOURCE_DIR}/include
)

# Link the library under test and Google Benchmark into the executable.
target_link_libraries(
    benchmarks_google
    PRIVATE
        ${PROJECT_NAME}
        benchmark::benchmark
)

# Require C++17.
target_compile_features(benchmarks_google PRIVATE cxx_std_17)
