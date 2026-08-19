# Custom Benchmark Suite

Benchmarks the library using the project's own lightweight framework
(`BENCH`, `BENCH_SOLO`, `BENCH_CUSTOM`) instead of a third-party
library, comparing against a reference implementation under `../shared/`.
Organized into categories under `suite/` — every suite registers
itself automatically at startup via `REGISTER_BENCH_SUITE()`, no
manual wiring needed when a new suite file is added.

## Building

Configure with benchmarks enabled and build:

```
cmake -S . -B build_benchmarks -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_BENCHMARKS=ON -DBUILD_REGRESSION=OFF
cmake --build build_benchmarks
```

This produces a `benchmarks` executable under `build_benchmarks/`.

## Usage

Run every registered suite, print each table, then export the results:

```
./build_benchmarks/benchmarks
```

List all suites, grouped by category — useful when you don't remember
a suite's id or exact file name:

```
./build_benchmarks/benchmarks list
```

Run every suite in one category, e.g. everything under `search`:

```
./build_benchmarks/benchmarks search
```

Run a single suite by its id, e.g. the first Search suite:

```
./build_benchmarks/benchmarks S1
```

Run a single suite by its file name instead of its id:

```
./build_benchmarks/benchmarks binary_search
```

Matching is case-insensitive. Category and file-name arguments are
matched against the same `list` output, so run `./build_benchmarks/benchmarks
list` first if you're not sure what's available.

Every run except `list` writes `benchmark_results.json` and
`benchmark_results.md` to `benchmarks/results/`, overwriting any
previous report. The regression tool reads the JSON to compare against
saved baselines.
