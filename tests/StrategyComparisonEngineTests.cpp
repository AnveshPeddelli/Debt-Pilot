#include "debtpilot/StrategyComparisonEngine.hpp"

#include <gtest/gtest.h>
#include <vector>

using debtpilot::Debt;
using debtpilot::DebtType;
using debtpilot::InterestRate;
using debtpilot::Money;
using debtpilot::RepaymentStrategy;
using debtpilot::StrategyComparisonEngine;

namespace
{
    Debt createDebt(const std::string& id, std::int64_t balancePaise, std::int32_t annualBasisPoints, std::int64_t minimumPaymentPaise)
    {
        return Debt{id, id, DebtType::CreditCard, Money::fromPaise(balancePaise), InterestRate::fromBasisPoints(annualBasisPoints), Money::fromPaise(minimumPaymentPaise)};
    }

    //Both strategies are generated
    TEST(StrategyComparisionEngineTest, GeneratesBothStrategyPlans)
    {
        const std::vector<Debt> debts{
            createDebt("small", 100'000, 1'200, 25'000),
            createDebt("large", 300'000, 2'400, 25'000)
        };

        const auto result = StrategyComparisonEngine::compare(debts, Money::fromPaise(100'000));

        EXPECT_GT(result.snowballPlan().totalMonths(), 0);
        EXPECT_GT(result.avalanchePlan().totalMonths(), 0);
    }

    //Avalanche saves interest
    TEST(StrategyComparisonEngineTest, RecommendsAvalancheWhenItCostsLessInterest)
    {
        const std::vector<Debt> debts{
            createDebt("small-low-rate", 100'000, 600, 10'000),
            createDebt("large-high-rate", 500'000, 3'600, 10'000)
        };

        const auto result = StrategyComparisonEngine::compare(debts, Money::fromPaise(100'000));
        
        EXPECT_LE(result.avalanchePlan().totalInterest(), result.snowballPlan().totalInterest());
        EXPECT_EQ(result.recommendedStrategy(), RepaymentStrategy::Avalanche);
    }

    //Savings amount is correct
    TEST(StrategyComparisonEngineTest, CalculatesAbsoluteInterestSavings)
    {
        const std::vector<Debt> debts{
            createDebt("a", 100'000, 600, 10'000),
            createDebt("b", 500'000, 3'600, 10'000)
        };

        const auto result = StrategyComparisonEngine::compare(debts, Money::fromPaise(100'000));

        const Money snowballInterest = result.snowballPlan().totalInterest();
        const Money avalancheInterest = result.avalanchePlan().totalInterest();
        const Money expectedSavings = snowballInterest>avalancheInterest ? snowballInterest-avalancheInterest : avalancheInterest-snowballInterest;

        EXPECT_EQ(result.interestSavings(), expectedSavings);
    }

    //Equal plans use Avalanche
    TEST(StrategyComparisonEngineTest, UsesAvalancheWhenStrategiesAreEqual)
    {
        const std::vector<Debt> debts{
            createDebt("only-debt", 100'000, 1'200, 25'000)
        };

        const auto result = StrategyComparisonEngine::compare(debts, Money::fromPaise(50'000));

        EXPECT_EQ(result.snowballPlan().totalInterest(), result.avalanchePlan().totalInterest());
        EXPECT_EQ(result.snowballPlan().totalMonths(), result.avalanchePlan().totalMonths());
        EXPECT_EQ(result.recommendedStrategy(), RepaymentStrategy::Avalanche);
        EXPECT_EQ(result.interestSavings().paise(), 0);
        EXPECT_EQ(result.monthsSaved(), 0);
    }

    //Empty portfolio
    TEST(StrategyComparisonEngineTest, HandlesEmptyPortfolio)
    {
        const std::vector<Debt> debts;

        const auto result = StrategyComparisonEngine::compare(debts, Money::fromPaise(0));

        EXPECT_EQ(result.snowballPlan().totalMonths(), 0);
        EXPECT_EQ(result.avalanchePlan().totalMonths(), 0);
        EXPECT_EQ(result.interestSavings().paise(), 0);
        EXPECT_EQ(result.monthsSaved(), 0);
    }

    //Add stronger consistency tests
    TEST(StrategyComparisonEngineTest, TotalPaidEqualsPrincipalPlusInterestForBothStrategies)
    {
        const std::vector<Debt> debts{
            createDebt("a", 100'000, 1'200, 25'000),
            createDebt("b", 200'000, 2'400, 25'000)
        };

        const Money originalPrincipal = Money::fromPaise(300'000);
        
        const auto result = StrategyComparisonEngine::compare(debts, Money::fromPaise(100'000));
    
        EXPECT_EQ(result.snowballPlan().totalPaid(), originalPrincipal+result.snowballPlan().totalInterest());
        EXPECT_EQ(result.avalanchePlan().totalPaid(), originalPrincipal+result.avalanchePlan().totalInterest());
    }
}