# Regression

Compares a benchmark run against a saved baseline snapshot and reports
the change per operation — how much faster or slower the current code
is. Reads from `benchmarks/baselines/*.json` and
`benchmarks/results/benchmark_results.json`, and writes a JSON and a
markdown report back out to `benchmarks/results/`.

Set the tool's display label in `framework.h`'s `setProjectLabels()`
before use — it's what shows up in the exported markdown report's
title.

## Building

Configure with the regression tool enabled and build:

```
cmake -S . -B build_regression -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_BENCHMARKS=OFF -DBUILD_REGRESSION=ON
cmake --build build_regression
```

This produces a single `regression` executable under
`build_regression/`, and fetches `nlohmann_json` at configure time.

## Usage

Compare the newest baseline snapshot against the current benchmark
results, then export both reports:

```
./build_regression/regression
```

List every available baseline snapshot, oldest to newest — useful
when you don't remember a baseline's exact name:

```
./build_regression/google_regressions list
```

Compare one named baseline against the current benchmark results:

```
./build_regression/google_regressions v1.2.0
```

Compare two named baselines directly against each other, instead of
against the current run:

```
./build_regression/google_regressions v1.2.0 v1.3.0
```

Every run (except `list`) writes `google_regression_results.json` and
`google_regression_results.md` to `benchmarks/results/`, overwriting any
previous report.
