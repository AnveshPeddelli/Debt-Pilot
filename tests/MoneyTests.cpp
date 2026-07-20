#include "debtpilot/Money.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <limits>

using debtpilot::Money;

TEST(MoneyTests, DefaultValueIsZero)
{
    const Money amount;

    EXPECT_EQ(amount.paise(), 0);
    EXPECT_TRUE(amount.isZero());
}

TEST(MoneyTests, StoresPaise)
{
    const Money amount = Money::fromPaise(125'050);
    EXPECT_EQ(amount.paise(), 125'050);
}

TEST(MoneyTests, StoresSigned64BitBoundaries)
{
    EXPECT_EQ(Money::fromPaise(std::numeric_limits<std::int64_t>::min()).paise(),
              std::numeric_limits<std::int64_t>::min());
    EXPECT_EQ(Money::fromPaise(std::numeric_limits<std::int64_t>::max()).paise(),
              std::numeric_limits<std::int64_t>::max());
}

TEST(MoneyTests, StoresNegativePaise)
{
    const Money amount = Money::fromPaise(-1);

    EXPECT_TRUE(amount.isNegative());
    EXPECT_FALSE(amount.isZero());
}

TEST(MoneyTests, AddsAmounts)
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

TEST(MoneyTests, SupportsComparison)
{
    EXPECT_GT(Money::fromPaise(20'000), Money::fromPaise(10'000));
}
