#include "debtpilot/RepaymentScheduleGenerator.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

using debtpilot::InterestRate;
using debtpilot::Money;
using debtpilot::RepaymentScheduleGenerator;


TEST(RepaymentScheduleGeneratorTest, GenerateScheduleUntilDebtIsRepaid)
{
    const Money balance = Money::fromPaise(100'000);
    const InterestRate rate = InterestRate::fromBasisPoints(0);
    const Money monthlyPayment = Money::fromPaise(25'000);

    const auto schedule = RepaymentScheduleGenerator::generate(balance, rate, monthlyPayment);

    EXPECT_EQ(schedule.payoffMonths(), 4);
    EXPECT_EQ(schedule.totalInterest().paise(), 0);
    EXPECT_EQ(schedule.totalPaid().paise(), 100'000);
}

//Final payment adjustment test
TEST(RepaymentScheduleGeneratorTest, AdjustsFinalPaymentToRemainingBalance)
{
    const Money balance = Money::fromPaise(100'000);
    const InterestRate rate = InterestRate::fromBasisPoints(0);
    const Money monthlyPayment = Money::fromPaise(30'000);

    const auto schedule = RepaymentScheduleGenerator::generate(balance, rate, monthlyPayment);

    ASSERT_EQ(schedule.payoffMonths(), 4);
    const auto& finalMonth = schedule.monthlyResults().back();

    EXPECT_EQ(finalMonth.requestedPayment().paise(), 30'000);
    EXPECT_EQ(finalMonth.actualPayment().paise(), 10'000);
    EXPECT_EQ(finalMonth.closingBalance().paise(), 0);
    EXPECT_TRUE(finalMonth.isFullyRepaid());
}

//Interest accumulation test
TEST(RepaymentScheduleGeneratorTest, AccumulatesTotalInterest)
{
    const Money balance = Money::fromPaise(100'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money monthlyPayment = Money::fromPaise(50'000);

    const auto schedule = RepaymentScheduleGenerator::generate(balance, rate, monthlyPayment);

    EXPECT_GT(schedule.totalInterest().paise(), 0);
    EXPECT_EQ(schedule.totalPaid(), balance + schedule.totalInterest());
}

//Payment too small test
TEST(RepaymentScheduleGeneratorTest, RejectsPaymentThatDoesNotReduceBalance)
{
    const Money balance = Money::fromPaise(10'000'000);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money monthlyPayment = Money::fromPaise(50'000);

    EXPECT_THROW(RepaymentScheduleGenerator::generate(balance, rate, monthlyPayment), std::runtime_error);
}

//Zero payment test
TEST(RepaymentScheduleGeneratorTest, RejectZeroPaymentForOutstandingDebt)
{
    const Money balance = Money::fromPaise(100'000);
    const InterestRate rate = InterestRate::fromBasisPoints(0);
    const Money monthlyPayment = Money::fromPaise(0);

    EXPECT_THROW(RepaymentScheduleGenerator::generate(balance, rate, monthlyPayment), std::invalid_argument);
}

//Already-repaid debt test
TEST(RepaymentScheduleGeneratorTest, ReturnEmptyScheduleForZeroBalance)
{
    const Money balance = Money::fromPaise(0);
    const InterestRate rate = InterestRate::fromBasisPoints(1'200);
    const Money monthlyPayment = Money::fromPaise(0);

    const auto schedule = RepaymentScheduleGenerator::generate(balance, rate, monthlyPayment);

    EXPECT_EQ(schedule.payoffMonths(), 0);
    EXPECT_TRUE(schedule.monthlyResults().empty());
    EXPECT_EQ(schedule.totalInterest().paise(), 0);
    EXPECT_EQ(schedule.totalPaid().paise(), 0);
}

//Maximum-month test
TEST(RepaymentScheduleGeneratorTest, StopsAtMaximumMonthLimit)
{
    const Money balance = Money::fromPaise(1'000'000);
    const InterestRate rate = InterestRate::fromBasisPoints(0);
    const Money monthlyPayment = Money::fromPaise(1);

    EXPECT_THROW(RepaymentScheduleGenerator::generate(balance, rate, monthlyPayment, 12), std::invalid_argument);
}