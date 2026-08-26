#pragma once

// Single include that pulls in the entire regression-tool framework —
// the tool's main file includes only this to get benchmark-snapshot
// parsing, comparison output, and result export.

// clang-format off
#include "helpers.h"            // benchmark/baseline data types, snapshot loading, comparison output
#include "export.h"             // exportJson(), exportMarkdown()
// clang-format on

// ── Run drivers ─────────────────────────────────────────────────────
//
// These are the entry points regression_main.cpp calls directly. Kept
// here rather than in helpers.h so helpers.h stays a set of reusable
// building blocks, and the top-level "run the whole comparison" /
// "list what's available" orchestration lives in one place.

// The one function to edit when retargeting this skeleton at a
// different library: sets the display label used in the exported
// markdown report's title. Called once at startup, before the tool
// runs.
inline void setProjectLabels() {
    setCustom("PROJECT_NAME"); // TODO: replace with the library's display name
}

// Prints/records a comparison row against current results for each
// baseline entry. Matched by benchmark name alone -- unlike the custom
// suite there's no suite/operation split and no fixed iteration tiers
// to group or re-header by, since Google Benchmark auto-tunes
// iteration count per run rather than running fixed SMALL/MEDIUM/LARGE
// passes.
inline void printRegression(const std::vector<BenchmarkResult>& baseline,
                            const std::vector<BenchmarkResult>& current) {
    setHeader("Google Benchmarks");

    for (const auto& entry : baseline) {
        double cns = getCns(current, entry.name);
        printComparisonRow(entry.name, entry.real_time, cns);
    }
}

// Prints every available baseline snapshot's name, sorted oldest to
// newest. Only considers this tool's "gv"-prefixed snapshots -- the
// custom suite's "v"-prefixed snapshots live in the same directory but
// use an incompatible JSON schema, so they're skipped rather than
// parsed.
inline void printList() {
    std::vector<Baseline> baselines;

    for (const auto& entry : fs::directory_iterator("benchmarks/baselines")) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        if (!isGoogleBaselineName(entry.path()))
            continue;

        baselines.push_back(parseBaseline(entry.path()));
    }

    std::sort(baselines.begin(), baselines.end());

    std::cout << "Available baselines:\n\n";

    for (const auto& baseline : baselines)
        std::cout << "  " << baseline.name << '\n';
}