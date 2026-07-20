#pragma once

#include <cstdint>

namespace debtpilot
{
    enum class DebtType : std::uint8_t
    {
        CreditCard,
        PersonalLoan,
        EducationLoan,
        VehicleLoan,
        HomeLoan,
        BuyNowPayLater,
        InformalLoan,
        Other
    };
}
