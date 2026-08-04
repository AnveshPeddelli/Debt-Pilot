add_library(debtpilot_core STATIC 
    src/Debt.cpp 
    src/InterestRate.cpp 
    src/Money.cpp
    src/MonthlyBudget.cpp
    src/MonthlyInterestCalculator.cpp
    src/MonthlyRepaymentResult.cpp
    src/MonthlyRepaymentCalculator.cpp
    src/RepaymentSchedule.cpp
    src/RepaymentScheduleGenerator.cpp
    src/DebtPrioritizer.cpp
    src/DebtPaymentAllocation.cpp
    src/PortfolioPaymentAllocation.cpp
    src/PortfolioPaymentAllocator.cpp
    src/PortfolioMonthResult.cpp
    src/PortfolioRepaymentPlan.cpp
    src/PortfolioRepaymentSimulator.cpp
    src/DebtMonthlySnapshot.cpp
    src/StrategyComparisonEngine.cpp
    src/StrategyComparisonResult.cpp
    src/DebtPlanningRequest.cpp
    src/DebtPlanningService.cpp
    src/MinimumBudgetCalculator.cpp
    src/PlannedPaymentPayoffResult.cpp
    src/PlannedPaymentPayoffSimulator.cpp
)

target_include_directories(debtpilot_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(debtpilot_core PUBLIC 
    cxx_std_20
)

debtpilot_enable_strict_warnings(debtpilot_core)
