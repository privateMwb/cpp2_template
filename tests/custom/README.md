# Custom Test Suite

Correctness tests for the library, using the project's own lightweight
framework (`RUN`, `CHK`, `CHK_THROWS`) instead of a third-party library.
Organized into categories under `suite/` (see `suite/README.md` for
what each category covers). Every suite registers itself automatically
at startup — no manual wiring needed when a new test file is added.

## Building

Configure with tests enabled and build:

```
cmake -S . -B build_tests -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF -DBUILD_BENCHMARKS=OFF -DBUILD_REGRESSION=OFF
cmake --build build_tests
```

This produces a single `tests` executable under `build_tests/`.

## Usage

Run every registered suite, then print pass/fail stats:

```
./build_tests/tests
```

List all suites, grouped by category — useful when you don't remember
a suite's id or exact file name:

```
./build_tests/tests list
```

Run every suite in one category, e.g. everything under `unit`:

```
./build_tests/tests unit
```

Run a single suite by its id, e.g. the first Unit suite:

```
./build_tests/tests U1
```

Run a single suite by its file name instead of its id:

```
./build_tests/tests basic_behavior
```

Matching is case-insensitive. Category and file-name arguments are
matched against the same `list` output, so run `./build_tests/tests
list` first if you're not sure what's available.

The build also registers a `ctest` target, so `ctest --test-dir
build_tests` runs the same executable and reports pass/fail through
CTest instead.
