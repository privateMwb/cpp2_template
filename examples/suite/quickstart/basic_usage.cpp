// clang-format off
#include "support/framework.h"  // REGISTER_EXAMPLE_SUITE(), borderLine()
// clang-format on

namespace {

// The simplest possible use of the library: construct an instance, call
// its most basic method, print the result. This is usually the first
// file a new user reads, so keep it short and free of edge cases.
void run_examples() {
    // TODO: replace with the library's most basic construction + call.
    std::cout << "Constructed an instance and called its main method.\n";
    borderLine();
    std::cout << "Result: <replace with actual output>\n";
}

}  // namespace

REGISTER_EXAMPLE_SUITE()
