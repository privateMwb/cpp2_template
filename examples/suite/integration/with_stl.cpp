// clang-format off
#include "support/framework.h"  // REGISTER_EXAMPLE_SUITE(), borderLine()

#include <algorithm>             // std::for_each
// clang-format on

namespace {

// Interoperability with the rest of a codebase — standard library
// algorithms, custom types, or embedding the library inside a larger
// class. Shows the library playing well with code the reader already
// knows, not just its own API in isolation.
void run_examples() {
    // TODO: replace with real interop, e.g. iterating the library's
    // container with std::for_each, or using it as a member of a class.
    std::cout << "Demonstrating interop with the standard library.\n";
    borderLine();
    std::cout << "Result: <replace with actual output>\n";
}

}  // namespace

REGISTER_EXAMPLE_SUITE()
