#include "debtpilot/InterestRate.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

using debtpilot::InterestRate;

TEST(InterestRateTests, ZeroRate)
{
    const InterestRate rate = InterestRate::fromBasisPoints(0);
    EXPECT_TRUE(rate.isZero());
    EXPECT_EQ(rate.basisPoints(), 0);
}

TEST(InterestRateTests, PositiveRate)
{
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    EXPECT_FALSE(rate.isZero());
    EXPECT_EQ(rate.basisPoints(), 1'200);
}

TEST(InterestRateTests, RejectsNegativeRate)
{
    EXPECT_THROW((void)InterestRate::fromBasisPoints(-1), std::invalid_argument);
}

TEST(InterestRateTests, StoresIntegerBoundary)
{
    const auto maximum = std::numeric_limits<InterestRate::BasisPoints>::max();
    EXPECT_EQ(InterestRate::fromBasisPoints(maximum).basisPoints(), maximum);
}

TEST(InterestRateTests, SupportsComparison)
{
    EXPECT_LT(InterestRate::fromBasisPoints(500),
              InterestRate::fromBasisPoints(1'200));
}
