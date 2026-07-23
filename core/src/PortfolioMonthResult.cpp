#include "debtpilot/PortfolioMonthResult.hpp"

#include <utility>

namespace debtpilot
{
    PortfolioMonthResult::PortfolioMonthResult(std::size_t monthNumber, std::vector<DebtMonthlySnapshot> debtSnapshots, Money totalInterest, Money totalPaid) : monthNumber_{monthNumber}, debtSnapshots_{debtSnapshots}, totalInterest_{totalInterest}, totalPaid_{totalPaid}
    {

    }

    const std::vector<DebtMonthlySnapshot>& PortfolioMonthResult::debtSnapshots() const noexcept
    {
        return std::move(debtSnapshots_);
    }

    std::size_t PortfolioMonthResult::monthNumber() const noexcept
    {
        return monthNumber_;
    }

    Money PortfolioMonthResult::totalInterest() const noexcept
    {
        return totalInterest_;
    }

    Money PortfolioMonthResult::totalPaid() const noexcept
    {
        return totalPaid_;
    }
    
}