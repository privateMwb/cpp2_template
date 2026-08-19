// clang-format off
#include "support/framework.h"  // REGISTER_TEST_SUITE(), RUN(), CHK()
// clang-format on

namespace {

// Multiple components working together end-to-end, rather than a
// single function in isolation.
void round_trip() {
    // TODO: replace with a real end-to-end round trip, e.g.
    // parse-modify-serialize, and check the result matches expectations.
    CHK(true);
}

void run_tests() {
    RUN(round_trip);
}

}  // namespace

REGISTER_TEST_SUITE()
