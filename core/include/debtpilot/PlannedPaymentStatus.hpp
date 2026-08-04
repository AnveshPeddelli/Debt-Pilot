#pragma once

namespace debtpilot
{
    enum class PlannedPaymentStatus
    {
        Repaid,
        BelowRequiredPayment,
        PaymentDoesNotCoverInterest
    };
}