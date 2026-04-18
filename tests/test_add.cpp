#include <gtest/gtest.h>
#include "add.hpp"

// ── Example test suite ───────────────────────────────────────────────────────
// Replace or extend these tests with tests for your own code.

TEST(AddTest, PositiveNumbers)
{
    EXPECT_EQ(add(2, 3), 5);
}

TEST(AddTest, NegativeNumbers)
{
    EXPECT_EQ(add(-1, -1), -2);
}

TEST(AddTest, MixedNumbers)
{
    EXPECT_EQ(add(-3, 5), 2);
}

TEST(AddTest, Zeros)
{
    EXPECT_EQ(add(0, 0), 0);
}
