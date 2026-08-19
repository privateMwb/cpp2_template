/**
 * @file reference.h
 * @brief Baseline implementation used as the comparison target for
 * BENCH() calls in your benchmark suites.
 *
 * Replace the contents of this file with whatever the conventional way
 * of doing your library's job looks like — e.g. std::list +
 * std::unordered_map for an LRU cache, std::vector for a growable
 * array, std::mutex + std::queue for a thread pool. Expose the same
 * operations your library exposes, so both sides of BENCH() can be
 * driven from identical call sites.
 *
 * No natural standard-library equivalent to compare against? You don't
 * need this file — use BENCH_SOLO() instead of BENCH() in your suites,
 * and drop the #include "reference.h" from framework.h.
 */

#pragma once
