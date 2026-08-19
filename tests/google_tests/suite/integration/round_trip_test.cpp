// clang-format off
#include <gtest/gtest.h>
// clang-format on

// Multiple components working together end-to-end, rather than a
// single function in isolation.
// TODO: replace with a real end-to-end round trip, e.g.
// parse-modify-serialize, and check the result matches expectations.
TEST(Integration, RoundTrip) {
    EXPECT_TRUE(true);
}
