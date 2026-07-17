#include "debtpilot/Money.hpp"
#include <gtest/gtest.h>

using debtpilot::Money;

TEST(MoneyTests, DefaultValueisZero)
{
    const Money amount;

    EXPECT_EQ(amount.paise(), 0);
    EXPECT_TRUE(amount.isZero());
}

TEST(MoneyTests, StorePaise)
{
    const Money amount = Money::fromPaise(125050);
    EXPECT_EQ(amount.paise(), 125050);
}

TEST(MoneyTests, AddAmounts)
{
    const Money first = Money::fromPaise(10'000);
    const Money second = Money::fromPaise(5'000);

    EXPECT_EQ((first + second).paise(), 15'000);
}

TEST(MoneyTests, SubtractsAmounts)
{
    const Money first = Money::fromPaise(10'000);
    const Money second = Money::fromPaise(2'500);

    EXPECT_EQ((first - second).paise(), 7'500);
}

TEST(MoneyTests, SupportComparison)
{
    EXPECT_GT(Money::fromPaise(20'000), Money::fromPaise(10'000));
}