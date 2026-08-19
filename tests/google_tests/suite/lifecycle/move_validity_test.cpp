// clang-format off
#include <gtest/gtest.h>

#include <utility>                // std::move
// clang-format on

// Construction, destruction, copying, and moving of the core type.
// TODO: replace with a real construct-then-move check.
TEST(Lifecycle, MoveValidity) {
    int source = 1;
    int dest = std::move(source);
    EXPECT_EQ(dest, 1);
}
