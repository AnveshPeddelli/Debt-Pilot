#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/Money.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace debtpilot::cli
{
    class ConsoleInput
    {
        public:
            [[nodiscard]] std::vector<Debt> readDebts() const;
            [[nodiscard]] Money readMonthlyBudget() const;

        private:
            [[nodiscard]] static std::string readRequiredText(const std::string& prompt);
            [[nodiscard]] static std::size_t readPositiveCount(const std::string& prompt);
            [[nodiscard]] static std::int64_t readNonNegativePaise(const std::string& prompt);
            [[nodiscard]] static std::int32_t readNonNegativeBasisPoints(const std::string& promt);
            [[nodiscard]] static std::int64_t parseRupeesToPaise(const std::string& input);
    
       };
}