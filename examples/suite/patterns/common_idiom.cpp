// clang-format off
#include "support/framework.h"  // REGISTER_EXAMPLE_SUITE(), borderLine()
// clang-format on

namespace {

// A usage idiom built on top of the core API — something a caller would
// reach for repeatedly in real code, rather than a one-off basic call.
void run_examples() {
    // TODO: replace with a real idiom, e.g. lazy initialization, a
    // read-modify-write loop, or a helper wrapped around the core API.
    std::cout << "Demonstrating a common usage idiom.\n";
    borderLine();
    std::cout << "Result: <replace with actual output>\n";
}

}  // namespace

REGISTER_EXAMPLE_SUITE()
