# Example Suite

This document describes the example categories under `suite/` — what
each one demonstrates, and the individual example files it contains.

Unlike the test suite, an example doesn't assert correctness — it
demonstrates real usage of the library, including deliberate misuse
where instructive (see Misuse), so the reader sees both the correct
pattern and the mistake it guards against.

Every example file ends with `REGISTER_EXAMPLE_SUITE()`, which derives
the suite's category from its containing directory and assigns it a
sequential id within that category. This applies uniformly across
every category below.

---

## Advanced

Demonstrates deeper mechanics of the library — move semantics,
exception safety, and other implementation-level behavior worth
calling out explicitly.

### Examples

---

## Integration

Demonstrates interoperability with the rest of a codebase — custom
types, standard library algorithms, and embedding the library inside a
larger class.

### Examples

---

## Misuse

Demonstrates common mistakes and the exceptions or undefined behavior
they lead to, alongside the correct pattern — including examples shown
but not executed, so the reader can see what to avoid without the
program actually invoking undefined behavior.

### Examples

---

## Patterns

Demonstrates common usage idioms built on top of the core API.

### Examples

---

## Quickstart

Demonstrates fundamental, everyday usage — construction, and the
basic operations most callers will reach for first.

### Examples
