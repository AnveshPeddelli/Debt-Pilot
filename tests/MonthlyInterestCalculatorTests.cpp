#include "debtpilot/MonthlyInterestCalculator.hpp"

#include "gtest/gtest.h"
#include <stdexcept>

using debtpilot::InterestRate;
using debtpilot::Money;
using debtpilot::MonthlyInterestCalculator;

TEST(MonthlyInterestCalculator, ZeroBalanceProducesZeroInterest)
{
    const Money balance = Money::fromPaise(0);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money interest = MonthlyInterestCalculator::calculate(balance, rate);
    EXPECT_EQ(interest.paise(), 0);
}

TEST(MonthlyInterestCalcularor, ZeroRateProduceZeroInterest)
{
    const Money balance = Money::fromPaise(1'000'000);
    const InterestRate rate = InterestRate::fromBasisPoints(0);
    const Money interest = MonthlyInterestCalculator::calculate(balance, rate);
    EXPECT_EQ(interest.paise(), 0);
}

TEST(MonthlyInterestCalcularor, CalculatesTwelvePercentAnnualInterest)
{
    const Money balance = Money::fromPaise(10'000'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money interest = MonthlyInterestCalculator::calculate(balance, rate);
    EXPECT_EQ(interest.paise(), 100'000);
}

TEST(MonthlyInterestCalculator, CalculatesEighteenPercentAnnualInterest)
{
    const Money balance = Money::fromPaise(5'000'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'800);
    const Money interest = MonthlyInterestCalculator::calculate(balance, rate);
    EXPECT_EQ(interest.paise(), 75'000);
}

TEST(MonthlyInterestCalculator, RoundsHalfPaiseUp)
{
    const Money balance = Money::fromPaise(100);
    
    //6% annually:
    //100paise*600/120000 = 0.5 paise
    const InterestRate rate = InterestRate::fromBasisPoints(600);
    const Money interest = MonthlyInterestCalculator::calculate(balance, rate);
    EXPECT_EQ(interest.paise(), 1);
}

TEST(MonthlyInterestCalculator, RejectsNegativeBalance)
{
    const Money balance = Money::fromPaise(-100);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    EXPECT_THROW(MonthlyInterestCalculator::calculate(balance, rate), std::invalid_argument);
}