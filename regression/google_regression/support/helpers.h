#pragma once

// clang-format off
#include <nlohmann/json.hpp>   // nlohmann::json (loadResults — parses benchmark snapshots)

#include <algorithm>           // std::sort (printList)
#include <filesystem>          // std::filesystem::path, std::filesystem::directory_iterator
#include <fstream>             // std::ifstream (loadResults)
#include <iomanip>             // std::setw, std::setprecision, std::fixed, std::left, std::showpos
#include <iostream>            // std::cout
#include <sstream>             // std::ostringstream (printComparisonRow), std::stringstream (parseBaseline)
#include <stdexcept>           // std::runtime_error
#include <string>              // std::string, std::string_view
#include <vector>              // std::vector
// clang-format on

using json = nlohmann::json;

namespace fs = std::filesystem;

// ── Constants ───────────────────────────────────────────────────────

// ANSI terminal color codes.
inline constexpr const char* RESET = "\033[0m";
inline constexpr const char* GREEN = "\033[92m";
inline constexpr const char* RED = "\033[91m";
inline constexpr const char* YELLOW = "\033[93m";
inline constexpr const char* CYAN = "\033[96m";
inline constexpr const char* GRAY = "\033[37m";
inline constexpr const char* BLUE = "\033[94m";

// ── Data structures ─────────────────────────────────────────────────

// Shape of one entry loaded from a Google Benchmark JSON snapshot
// (--benchmark_out_format=json). Unlike the custom suite's snapshot,
// there's no "suite"/"operation" split and no total_ns to divide down
// -- Google Benchmark already reports per-iteration time directly via
// real_time/cpu_time, in whatever unit time_unit names (normally "ns").
// The top-level "context" object in the snapshot (host info, library
// version, etc.) is ignored here; only the "benchmarks" array matters.
struct BenchmarkResult {
    std::string name;
    std::size_t iterations;
    double real_time;
    double cpu_time;
    std::string time_unit;
};

// Shape of one row this tool produces: a baseline-vs-current comparison,
// not a raw benchmark result. Kept separate from BenchmarkResult since
// they're different data (input vs. output of this tool).
//
// Matched by name alone -- unlike the custom suite, Google Benchmark
// auto-tunes iteration count per run (via --benchmark_min_time), so the
// same benchmark can legitimately run a different number of iterations
// between the baseline snapshot and the current run. Matching on
// (name, iterations) the way the custom suite does would silently drop
// comparisons whenever that count drifts.
struct RegressionRow {
    std::string name;
    double baseline_ns;
    double current_ns;
    double pct_change;
};

// One parsed baseline snapshot filename (e.g. "v1.2.0.json" -> 1.2.0),
// ordered by semantic version so the newest baseline can be found.
struct Baseline {
    int major{};
    int minor{};
    int patch{};
    std::string name;

    auto operator<=>(const Baseline&) const = default;
};

// ── Run state ───────────────────────────────────────────────────────

// Display label used in the exported markdown report's title. Set via
// setCustom() before the tool runs.
inline std::string custom;

// Sets the display label used in the exported markdown report's title.
inline void setCustom(std::string name) {
    custom = name;
}

// Process-wide accumulator of comparison rows, populated by
// printComparisonRow() and read back by exportJson().
inline std::vector<RegressionRow>& regression_results() {
    static std::vector<RegressionRow> results;
    return results;
}

// Accumulates a markdown-formatted transcript of the run, built up by
// setHeader()/printComparisonRow() alongside their normal stdout printing.
inline std::string& markdown_buffer() {
    static std::string buffer;
    return buffer;
}

// ── Loading / parsing ───────────────────────────────────────────────

// Loads a Google Benchmark JSON snapshot (see BenchmarkResult) from
// disk. Reads only the "benchmarks" array; the "context" object
// (host/library metadata) is not needed for comparison.
inline std::vector<BenchmarkResult> loadResults(const std::string& file) {
    std::ifstream in(file);

    if (!in) {
        throw std::runtime_error("Failed to open: " + file);
    }

    json data;
    in >> data;

    std::vector<BenchmarkResult> results;

    for (const auto& entry : data.at("benchmarks")) {
        results.push_back({entry["name"].get<std::string>(), entry["iterations"].get<std::size_t>(),
                           entry["real_time"].get<double>(), entry["cpu_time"].get<double>(),
                           entry["time_unit"].get<std::string>()});
    }

    return results;
}

// Looks up the current run's real_time for a given benchmark name.
// Returns 0.0 if no matching benchmark was found in the current run
// (e.g. it was renamed, removed, or filtered out via --benchmark_filter).
inline double getCns(const std::vector<BenchmarkResult>& results, const std::string& name) {
    for (const auto& result : results) {
        if (result.name == name) {
            return result.real_time;
        }
    }

    return 0.0;
}

// Parses a baseline snapshot's filename (e.g. "gv1.2.0.json") into a
// comparable Baseline. Google snapshots are prefixed "gv"/"GV" to
// distinguish them from the custom suite's "v"-prefixed snapshots when
// both live in the same benchmarks/baselines directory.
inline Baseline parseBaseline(const fs::path& path) {
    std::string name = path.stem().string();

    std::string version = name;
    if (version.size() >= 2 && (version[0] == 'g' || version[0] == 'G') &&
        (version[1] == 'v' || version[1] == 'V'))
        version.erase(0, 2);

    Baseline baseline;
    baseline.name = name;

    char dot1, dot2;
    std::stringstream ss(version);

    if (!(ss >> baseline.major >> dot1 >> baseline.minor >> dot2 >> baseline.patch) ||
        dot1 != '.' || dot2 != '.') {
        throw std::runtime_error("Invalid version: " + name);
    }

    return baseline;
}

// True if a baselines-directory filename belongs to this tool -- i.e.
// it's a "gv"/"GV"-prefixed Google Benchmark snapshot, not one of the
// custom suite's "v"-prefixed snapshots sitting in the same directory.
inline bool isGoogleBaselineName(const fs::path& path) {
    std::string stem = path.stem().string();
    return stem.size() >= 2 && (stem[0] == 'g' || stem[0] == 'G') &&
           (stem[1] == 'v' || stem[1] == 'V');
}

// Returns the path to the newest baseline snapshot in
// benchmarks/baselines, considering only this tool's "gv"-prefixed
// snapshots.
inline std::string latestBaseline() {
    bool found = false;
    Baseline latest;
    fs::path latestPath;

    for (const auto& entry : fs::directory_iterator("benchmarks/baselines")) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        if (!isGoogleBaselineName(entry.path()))
            continue;

        Baseline current = parseBaseline(entry.path());

        if (!found || current > latest) {
            latest = current;
            latestPath = entry.path();
            found = true;
        }
    }

    if (!found)
        throw std::runtime_error("No baseline snapshots found.");

    return latestPath.string();
}

// ── Output / printing ───────────────────────────────────────────────

// Prints a horizontal separator line.
inline void borderLine() {
    std::cout << GRAY << std::string(90, '-') << RESET << "\n";
}

// Prints a section header row and starts the matching markdown table.
// Unlike the custom suite, there's no "suite" grouping in Google
// Benchmark's own output, so this is called once at the start of a run
// rather than once per suite.
inline void setHeader(std::string_view header) {
    borderLine();
    std::cout << std::left << CYAN << std::setw(40) << header << std::setw(15) << "Current"
              << std::setw(15) << "Baseline" << std::setw(15) << "  Δ" << RESET << "\n";
    borderLine();

    markdown_buffer() += "\n## " + std::string(header) + "\n\n";
    markdown_buffer() += "| Benchmark | Current | Baseline | Δ |\n";
    markdown_buffer() += "|---|---|---|---|\n";
}

// Prints (and records, for export) one baseline-vs-current comparison
// row. bNs is the baseline real_time, cNs is the current run's
// real_time -- printed under the "Current"/"Baseline" columns set up
// by setHeader(), in that order.
inline void printComparisonRow(std::string_view name, double bNs, double cNs) {
    const double pct = bNs == 0 ? 0.0 : (bNs - cNs) / cNs * 100.0;

    // 0% (no measured difference) is treated as neutral; anything else
    // colors green (faster) or red (slower) by direction.
    const char* deltaColor = (pct > 0.0) ? GREEN : (pct < 0.0) ? RED : GRAY;

    std::ostringstream deltaStream;
    deltaStream << std::showpos << std::fixed << std::setprecision(1) << pct << "%";

    std::ostringstream currentStream;
    currentStream << cNs << " ns";

    std::ostringstream baselineStream;
    baselineStream << bNs << " ns";

    // clang-format off
    std::cout << std::left
              << std::setw(40) << name
              << std::setw(15) << currentStream.str()
              << std::setw(15) << baselineStream.str()
              << deltaColor << deltaStream.str()
              << RESET << "\n";
    // clang-format on

    markdown_buffer() += "| " + std::string(name) + " | " + currentStream.str() + " | " +
                         baselineStream.str() + " | " + deltaStream.str() + " |\n";

    regression_results().push_back({std::string(name), bNs, cNs, pct});
}