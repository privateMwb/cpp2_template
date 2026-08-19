// clang-format off
#include <gtest/gtest.h>
// clang-format on

// One test per resolved issue, added at the time the fix lands. Name
// each test after the bug it guards against, not a generic label.
// TODO: replace with a regression test for a real fixed bug —
// reproduce the exact input that used to trigger it, and assert the
// correct behavior now holds.
TEST(Regression, IssuePlaceholder) {
    EXPECT_TRUE(true);
}
