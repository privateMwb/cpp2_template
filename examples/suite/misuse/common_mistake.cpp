// clang-format off
#include "support/framework.h"  // REGISTER_EXAMPLE_SUITE(), borderLine()
// clang-format on

namespace {

// A common mistake and the exception or undefined behavior it leads to,
// shown next to the correct pattern. If the mistake would actually
// trigger UB, show it as a comment rather than executing it — the
// reader should see what to avoid without the program invoking it.
void run_examples() {
    std::cout << "Correct pattern:\n";
    // TODO: replace with the correct usage.
    std::cout << "  Result: <replace with actual output>\n";

    borderLine();

    std::cout << "Common mistake (not executed — would be UB / throw):\n";
    // TODO: replace with the mistake, commented out, e.g.:
    //   auto value = container.at(out_of_range_index);  // throws std::out_of_range
    std::cout << "  See comment above for the mistake and its consequence.\n";
}

}  // namespace

REGISTER_EXAMPLE_SUITE()
