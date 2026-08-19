// clang-format off
#include "support/framework.h"  // REGISTER_EXAMPLE_SUITE(), borderLine()

#include <utility>                // std::move
// clang-format on

namespace {

// Deeper mechanics of the library — move semantics, exception safety,
// and other implementation-level behavior worth calling out explicitly
// rather than leaving the reader to discover by accident.
void run_examples() {
    // TODO: replace with a real move-construction / move-assignment
    // demo, e.g. constructing an instance, moving it, and showing the
    // moved-from state is left valid but unspecified.
    std::cout << "Demonstrating move semantics.\n";
    borderLine();
    std::cout << "Result: <replace with actual output>\n";
}

}  // namespace

REGISTER_EXAMPLE_SUITE()
