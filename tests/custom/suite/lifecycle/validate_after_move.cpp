// clang-format off
#include "support/framework.h"  // REGISTER_TEST_SUITE(), RUN(), CHK()

#include <utility>                // std::move
// clang-format on

namespace {

// Construction, destruction, copying, and moving of the core type.
void move_leaves_source_valid() {
    // TODO: replace with a real construct-then-move check.
    int source = 1;
    int dest = std::move(source);
    CHK(dest == 1);
}

void run_tests() {
    RUN(move_leaves_source_valid);
}

}  // namespace

REGISTER_TEST_SUITE()
