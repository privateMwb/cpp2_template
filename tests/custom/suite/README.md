# Test Suite

This document describes the test categories under `suite/` — what each
one verifies, and the individual test files it contains.

Unlike the benchmark suite, tests validate the library's own
correctness directly — there is no reference implementation to compare
against, so results are simply pass or fail.

Every test suite registers itself automatically via
`REGISTER_TEST_SUITE()` at startup, and is assigned a sequential id
within its category (e.g. `U1`, `U2` for Unit; `L1`, `L2` for
Lifecycle) — there's no suite list to maintain by hand. This applies
uniformly across every category below.

---

## Concurrency

Verifies thread-safety — concurrent reads and writes from multiple
threads, and correctness under simultaneous access.

### Tests



---

## Integration

Verifies multiple components working together end-to-end — for
example, a full parse-modify-serialize round trip — rather than a
single function in isolation.

### Tests



---

## Lifecycle

Verifies object lifetime operations — construction, destruction,
copying, and moving — across the different states or value kinds the
library's core type can hold.

### Tests



---

## Regression

Verifies that a specific, previously fixed bug stays fixed. One test
per resolved issue, added at the time the fix lands.

### Tests



---

## Unit

Verifies individual functions or methods in isolation — the smallest
testable unit of behavior, independent of the categories above.

### Tests


