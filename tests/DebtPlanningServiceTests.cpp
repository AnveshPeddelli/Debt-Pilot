#include "debtpilot/DebtPlanningService.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <vector>

using debtpilot::Debt;
using debtpilot::DebtPlanningRequest;
using debtpilot::DebtPlanningService;
using debtpilot::DebtType;
using debtpilot::InterestRate;
using debtpilot::Money;
using debtpilot::RepaymentStrategy;

namespace
{
    Debt createDebt(const std::string& id, std::int64_t balancePaise, std::int32_t annualBasisPaise, std::int64_t minimumPaymentPaise)
    {
        return Debt{id, id, DebtType::CreditCard, Money::fromPaise(balancePaise), InterestRate::fromBasisPoints(annualBasisPaise), Money::fromPaise(minimumPaymentPaise)};
    }
}

//Generate Snowball plan
TEST(DebtPlanningServiceTest, GenerateSnowballPlan)
{
    const DebtPlanningRequest request{{createDebt("small", 100'000, 0, 25'000), createDebt("large", 200'000, 0, 25'000)}, Money::fromPaise(100'000)};

    const DebtPlanningService service;
    const auto plan = service.generatePlan(request, RepaymentStrategy::Snowball);

    EXPECT_EQ(plan.totalMonths(), 3);
    EXPECT_EQ(plan.totalPaid().paise(), 300'000);
    EXPECT_EQ(plan.totalInterest().paise(), 0);
}

//Generate Avalanche plan
TEST(DebtPlanningServiceTest, GeneratesAvalanchePlan)
{
    const DebtPlanningRequest request{{createDebt("low-rate", 200'000, 600, 25'000), createDebt("high-rate", 200'000, 3'600, 25'000)}, Money::fromPaise(100'000)};

    const DebtPlanningService service;
    const auto plan = service.generatePlan(request, RepaymentStrategy::Avalanche);

    EXPECT_GT(plan.totalMonths(), 0);
    EXPECT_GT(plan.totalInterest().paise(), 0);
    EXPECT_LE(plan.payoffMonths().at("high-rate"), plan.payoffMonths().at("low-rate"));
}

//Compare strategies
TEST(DebtPlanningServiceTest, ComparesSnowballAndAvalanche)
{
    const DebtPlanningRequest request{{createDebt("small-low-rate", 100'000, 600, 10'000), createDebt("large-high-rate", 500'000, 3'600, 10'000)}, Money::fromPaise(100'000)};

    const DebtPlanningService service;

    const auto result = service.compareStrategies(request);

    EXPECT_GT(result.snowballPlan().totalMonths(), 0);
    EXPECT_GT(result.avalanchePlan().totalMonths(), 0);
    EXPECT_LE(result.avalanchePlan().totalInterest(), result.snowballPlan().totalInterest());
}

//Reject negative budget
TEST(DebtPlanningRequestTest, RejectsNegativeMonthlyBudget)
{
    EXPECT_THROW(DebtPlanningRequest({}, Money::fromPaise(-1)), std::invalid_argument);
}

//Reject zero maximum months
TEST(DebtPlanningRequestTest, RejectsZeroMaximumMonths)
{
    EXPECT_THROW(DebtPlanningRequest({}, Money::fromPaise(0), 0), std::invalid_argument);
}

//Preserves request values
TEST(DebtPlanningRequestTest, PreservesPlanningInput)
{
    const DebtPlanningRequest request{{createDebt("debt", 100'000, 1'200, 25'000)}, Money::fromPaise(50'000), 240};

    ASSERT_EQ(request.debts().size(), 1);
    EXPECT_EQ(request.debts()[0].id(), "debt");
    EXPECT_EQ(request.monthlyBudget().paise(), 50'000);
    EXPECT_EQ(request.maximumMonths(), 240);
}

