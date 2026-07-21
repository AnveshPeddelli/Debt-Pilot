#include "debtpilot/MonthlyRepaymentCalculator.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

using debtpilot::InterestRate;
using debtpilot::Money;
using debtpilot::MonthlyRepaymentCalculator;

TEST(MonthlyRepaymentCalculatorTest, AppliesInterestAndPayment)
{
    const Money openingBalance = Money::fromPaise(10'000'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money payment = Money::fromPaise(1'000'000);

    const auto result = MonthlyRepaymentCalculator::calculate(openingBalance, rate, payment);
    EXPECT_EQ(result.interestCharged().paise(), 100'000);
    EXPECT_EQ(result.principalRepaid().paise(), 900'000);
    EXPECT_EQ(result.closingBalance().paise(), 9'100'000);
    EXPECT_FALSE(result.isFullyRepaid());
}

//Zero-interest debt
TEST(MonthlyRepaymentCalculatorTest, AppliesEntirePaymentToPrincipalAtZeroInterest)
{
    const Money openingBalance = Money::fromPaise(100'000);
    const InterestRate rate = InterestRate::fromBasisPoints(0);
    const Money payment = Money::fromPaise(25'000);
    
    const auto result = MonthlyRepaymentCalculator::calculate(openingBalance, rate, payment);
    EXPECT_EQ(result.interestCharged().paise(), 0);
    EXPECT_EQ(result.principalRepaid().paise(), 25'000);
    EXPECT_EQ(result.closingBalance().paise(), 75'000);
}

//OverPayment
TEST(MonthlyRepaymentCalculatorTest, LimitsFinalPaymentToAmountDue)
{
    const Money openingBalance = Money::fromPaise(100'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money requestedPayment = Money::fromPaise(200'000);

    const auto result = MonthlyRepaymentCalculator::calculate(openingBalance, rate, requestedPayment);


    //Rs.10'000 balance plus Rs.10 interest
    EXPECT_EQ(result.interestCharged().paise(), 1'000);
    EXPECT_EQ(result.requestedPayment().paise(), 200'000);
    EXPECT_EQ(result.actualPayment().paise(), 101'000);
    EXPECT_EQ(result.closingBalance().paise(), 0);
    EXPECT_TRUE(result.isFullyRepaid());
}

//Payment smaller than interest
TEST(MonthlyRepaymentCalculatorTest, BalanceIncreasesWhenPaymentIsBelowInterest)
{
    const Money openingBalance = Money::fromPaise(10'000'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money payment = Money::fromPaise(50'000);

    const auto result = MonthlyRepaymentCalculator::calculate(openingBalance, rate, payment);

    EXPECT_EQ(result.interestCharged().paise(), 100'000);
    EXPECT_EQ(result.principalRepaid().paise(), 0);
    EXPECT_EQ(result.closingBalance().paise(), 10'050'000);
}

//Zero opening balance
TEST(MonthlyRepaymentCalculatorTest, ZeroBalanceRequiresNoPayment)
{
    const Money openingBalance = Money::fromPaise(0);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money payment = Money::fromPaise(100'000);

    const auto result = MonthlyRepaymentCalculator::calculate(openingBalance, rate, payment);

    EXPECT_EQ(result.interestCharged().paise(), 0);
    EXPECT_EQ(result.actualPayment().paise(), 0);
    EXPECT_EQ(result.closingBalance().paise(), 0);
    EXPECT_TRUE(result.isFullyRepaid());
}

//Negative payment
TEST(MonthlyRepaymentCalculatorTest, RejectsNegativePayment)
{
    const Money openingBalance = Money::fromPaise(100'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money payment = Money::fromPaise(-1);

    EXPECT_THROW(MonthlyRepaymentCalculator::calculate(openingBalance, rate, payment), std::invalid_argument);
}

//Negative opening balance
TEST(MonthlyRepaymentCalculatorTest, RejectNegativeOpeningBalance)
{
    const Money openingBalance = Money::fromPaise(-1);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money payment = Money::fromPaise(100);

    EXPECT_THROW(MonthlyRepaymentCalculator::calculate(openingBalance, rate, payment), std::invalid_argument);
}