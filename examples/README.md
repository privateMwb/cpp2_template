# Examples

Runnable, self-contained demonstrations of the library, organized into
categories under `suite/` (see `suite/README.md` for what each category
covers). Every suite is registered automatically at startup — no manual
wiring needed when a new example file is added.

## Building

Configure with examples enabled and build:

```
cmake -S . -B build_examples -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_BENCHMARKS=OFF -DBUILD_REGRESSION=OFF -DBUILD_EXAMPLES=ON
cmake --build build_examples
```

This produces a single `examples` executable under `build_examples/`.

## Usage

```
./build_examples/examples                # run every registered suite
./build_examples/examples list           # list all suites, grouped by category
./build_examples/examples <category>     # run every suite in a category, e.g. `advanced`
./build_examples/examples <id>           # run one suite by id, e.g. `A1`
./build_examples/examples <name>         # run one suite by file name, e.g. `basic_usage`
```

Matching is case-insensitive. Category and file-name arguments are
matched against the same `list` output, so run `./build_examples/examples list`
first if you're not sure what's available.
