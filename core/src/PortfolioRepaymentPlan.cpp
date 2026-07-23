#include "debtpilot/PortfolioRepaymentPlan.hpp"

#include <utility>


namespace debtpilot
{
    PortfolioRepaymentPlan::PortfolioRepaymentPlan(std::vector<PortfolioMonthResult> monthlyResults, std::unordered_map<std::string, std::size_t> payoffMonths, Money totalInterest, Money totalPaid) : monthlyResults_{std::move(monthlyResults)}, payoffMonths_{std::move(payoffMonths)}, totalInterest_{totalInterest}, totalPaid_{totalPaid}
    {

    }

    const std::vector<PortfolioMonthResult>& PortfolioRepaymentPlan::monthlyResults() const noexcept
    {
        return monthlyResults_;
    }

    const std::unordered_map<std::string, std::size_t>& PortfolioRepaymentPlan::payoffMonths() const noexcept
    {
        return payoffMonths_;
    }

    Money PortfolioRepaymentPlan::totalInterest() const noexcept
    {
        return totalInterest_;
    }

    Money PortfolioRepaymentPlan::totalPaid() const noexcept
    {
        return totalPaid_;
    }

    std::size_t PortfolioRepaymentPlan::totalMonths() const noexcept
    {
        return monthlyResults_.size();
    }
    
}
