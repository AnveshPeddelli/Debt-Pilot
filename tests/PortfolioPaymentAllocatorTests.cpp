#include "debtpilot/PortfolioPaymentAllocator.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

using debtpilot::Money;
using debtpilot::Debt;
using debtpilot::InterestRate;
using debtpilot::DebtType;
using debtpilot::PortfolioPaymentAllocator;
using debtpilot::RepaymentStrategy;

namespace
{
    Debt createDebt(const std::string& id, std::int64_t balancePaise, std::int32_t annualBasisPoints, std::int64_t minimumPaymentPaise)
    {
        return Debt{id, id, DebtType::CreditCard, Money::fromPaise(balancePaise), InterestRate::fromBasisPoints(annualBasisPoints), Money::fromPaise(minimumPaymentPaise)};
    }
}

//Pays minimum to all debts
TEST(PortfolioPaymentAllocatorTest, PaysMinimumPaymentToEveryActiveDebt)
{
    const std::vector<Debt> debts{
        createDebt("a", 500'000, 0, 100'000),
        createDebt("b", 300'000, 0, 50'000)
    };

    const auto result = PortfolioPaymentAllocator::allocate(debts, Money::fromPaise(150'000), RepaymentStrategy::Snowball);

    ASSERT_EQ(result.allocations().size(), 2);

    EXPECT_EQ(result.allocations()[0].totalPayment().paise(), 100'000);
    EXPECT_EQ(result.allocations()[1].totalPayment().paise(), 50'000);
    EXPECT_EQ(result.unallocatedAmount().paise(), 0);
}

//Snowball sends extra to lowest balance
TEST(PortfolioPaymentAllocatorTest, SnowballSendsExtraToLowestBalanceDept)
{
    const std::vector<Debt> debts{
        createDebt("large", 500'000, 1'200, 50'000),
        createDebt("small", 200'000, 3'600, 50'000)
    };
    
    const auto result = PortfolioPaymentAllocator::allocate(debts, Money::fromPaise(200'000), RepaymentStrategy::Snowball);

    ASSERT_EQ(result.allocations().size(), 2);

    EXPECT_EQ(result.allocations()[0].extraPayment().paise(), 0);
    EXPECT_EQ(result.allocations()[1].extraPayment().paise(), 100'000);
}

//Avalanche sends extra to highest rate
TEST(PortfolioPaymentAllocatorTest, AvalancheSendsExtraToHighestRateDebt)
{
    const std::vector<Debt> debts{
        createDebt("low-rate", 200'000, 1'200, 50'000),
        createDebt("high-rate", 500'000, 3'600, 50'000)
    };

    const auto result = PortfolioPaymentAllocator::allocate(debts, Money::fromPaise(200'000), RepaymentStrategy::Avalanche);

    ASSERT_EQ(result.allocations().size(), 2);

    EXPECT_EQ(result.allocations()[0].extraPayment().paise(), 0);
    EXPECT_EQ(result.allocations()[1].extraPayment().paise(), 100'000);
}

//Reject insufficient budget
TEST(PortfolioPaymentAllocatorTest, RejectsBudgetBelowTotalMinimumPayments)
{
    const std::vector<Debt> debts{
        createDebt("a", 500'000, 0, 100'000),
        createDebt("b", 300'000, 0, 50'000)
    };

    EXPECT_THROW(PortfolioPaymentAllocator::allocate(debts, Money::fromPaise(149'999), RepaymentStrategy::Snowball), std::invalid_argument);
}

//Caps payment at amount due
TEST(PortfolioPaymentAllocatorTest, DoesNotAllocateMoreThanAmoundDue)
{
    const std::vector<Debt> debts{
        createDebt("small", 100'000, 0, 20'000)
    };

    const auto result = PortfolioPaymentAllocator::allocate(debts, Money::fromPaise(200'000), RepaymentStrategy::Snowball);

    ASSERT_EQ(result.allocations().size(), 1);

    EXPECT_EQ(result.allocations()[0].totalPayment().paise(), 100'000);
    EXPECT_EQ(result.unallocatedAmount().paise(), 100'000);
}

//Zero-balance debt gets no allocation
TEST(PortfolioPaymentAllocatorTest, SkipsFullyRepaidDebt)
{
    const std::vector<Debt> debts{
        createDebt("closed", 0, 1'200, 50'000),
        createDebt("active", 100'000, 0, 25'000)
    };

    const auto result = PortfolioPaymentAllocator::allocate(debts, Money::fromPaise(25'000), RepaymentStrategy::Snowball);

    EXPECT_EQ(result.allocations()[0].totalPayment().paise(), 0);
    EXPECT_EQ(result.allocations()[1].totalPayment().paise(), 25'000);
}