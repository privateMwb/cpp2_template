// clang-format off
#include "support/framework.h"  // REGISTER_TEST_SUITE(), RUN(), CHK()
// clang-format on

namespace {

// A single function or method in isolation — the smallest testable
// unit of behavior.
void basic_behavior() {
    // TODO: replace with a real call and assertion.
    CHK(1 + 1 == 2);
}

void run_tests() {
    RUN(basic_behavior);
}

}  // namespace

REGISTER_TEST_SUITE()
