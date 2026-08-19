// clang-format off
#include "support/framework.h"  // REGISTER_TEST_SUITE(), RUN(), CHK()
// clang-format on

namespace {

// One test per resolved issue, added at the time the fix lands. Name
// each function after the bug it guards against, not a generic label.
void issue_placeholder() {
    // TODO: replace with a regression test for a real fixed bug —
    // reproduce the exact input that used to trigger it, and assert
    // the correct behavior now holds.
    CHK(true);
}

void run_tests() {
    RUN(issue_placeholder);
}

}  // namespace

REGISTER_TEST_SUITE()
