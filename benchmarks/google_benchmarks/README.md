# Google Benchmark Suite

Benchmarks the library using the Google Benchmark library instead of
the project's own framework. Organized into categories under `suite/`,
same layout as `../custom/suite/` — each `BENCHMARK(...)` registers
itself automatically, no manual wiring needed when a new suite file is
added.

## Building

Configure with benchmarks enabled and build:

```
cmake -S . -B build_benchmarks -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_BENCHMARKS=ON -DBUILD_REGRESSION=OFF
cmake --build build_benchmarks
```

This produces a `benchmarks_google` executable under `build_benchmarks/`,
and fetches Google Benchmark at configure time.

## Usage

Run every registered benchmark:

```
./build_benchmarks/benchmarks_google
```

List all benchmark names without running them — useful when you don't
remember one's exact name:

```
./build_benchmarks/benchmarks_google --benchmark_list_tests
```

Run only the benchmarks matching a regex, e.g. everything with
"Search" in its name:

```
./build_benchmarks/benchmarks_google --benchmark_filter=Search
```

Print results as JSON instead of the default table — pipe this into a
file or another tool:

```
./build_benchmarks/benchmarks_google --benchmark_format=json
```

Write results straight to a file, in a chosen format, without touching
stdout:

```
./build_benchmarks/benchmarks_google --benchmark_out=results.json --benchmark_out_format=json
```

These flags combine freely — e.g. filter down to one category and
write just that subset to a file. Run `--help` for the full flag list.
