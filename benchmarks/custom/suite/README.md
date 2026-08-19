# Custom Benchmark Suite

This document describes the benchmark categories under `suite/` — what
each one measures, and the individual benchmarks it contains.

Every benchmark compares the library's core type against a reference
implementation — the conventional way the same functionality is
typically built without this library. A category can support more
than one reference for comparison, but for now each category is
benchmarked against a single reference.

Every `BENCH()` call, in every category below, is automatically repeated
at three iteration tiers — SMALL (10K), MEDIUM (100K), and LARGE (1M) —
to smooth out timing noise and show whether relative performance holds
steady as call volume increases. This applies uniformly across the whole
suite; it is not specific to any one category. The **Scaling** category
below measures something different: how per-operation cost changes as
capacity itself grows or shrinks, independent of iteration count.

Some benchmarks have no meaningful reference equivalent — the
reference implementation may lack certain introspection or statistics
the library's type provides. Those run through `BENCH_SOLO()` instead
of `BENCH()`, timing the library's type alone.

---

## Access

Benchmarks read and lookup operations on already-constructed data —
retrieving values by key, indexed access, and existence checks.

### Benchmarks



---

## Core

Benchmarks the fundamental, most frequently exercised operations —
parsing raw input into the in-memory structure, serializing it back
to text, and equality comparison between instances.

### Benchmarks



---

## Lifecycle

Benchmarks object lifetime operations — construction, destruction,
copying, and moving — across the different states or value kinds the
library's core type can hold.

### Benchmarks



---

## Scaling

Benchmarks how per-operation cost changes as the *size of the input
data* grows — for example, a container with an increasing number of
elements. This is a separate axis from the SMALL/MEDIUM/LARGE
iteration tiers described above: those repeat the same fixed-size
operation more times, while Scaling grows the operation itself and
observes the resulting cost.

### Benchmarks



---

## Utility

Benchmarks helper and miscellaneous operations that don't belong to
any of the categories above — pretty-printing, string formatting,
and similar non-core utilities.

### Benchmarks


