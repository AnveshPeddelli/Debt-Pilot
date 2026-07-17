#include "debtpilot/InterestRate.hpp"
#include <gtest/gtest.h>

using debtpilot::InterestRate;

TEST(InterestTest, zeroRate)
{
    const InterestRate Irr = InterestRate::fromBasisPoints(0);
    EXPECT_TRUE(Irr.isZero());
    EXPECT_EQ(Irr.basisPoints(), 0);
}

TEST(InterestTest, PositiveRate)
{
    const InterestRate Irr = InterestRate::fromBasisPoints(1'200);
    EXPECT_GT(Irr.basisPoints(), 0);
    EXPECT_EQ(Irr.basisPoints(), 1'200);
}

TEST(InterestTest, NegativeRate)
{
    EXPECT_THROW(InterestRate::fromBasisPoints(-1), std::invalid_argument);
}

TEST(InterestTest, StoredBasisPoint)
{
    const InterestRate Irr = InterestRate::fromBasisPoints(12);
    EXPECT_EQ(Irr.basisPoints(), 12);
}