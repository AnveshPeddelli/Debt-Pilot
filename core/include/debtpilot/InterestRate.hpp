#pragma once

#include <compare>
#include <cstdint>
#include <stdexcept>

namespace debtpilot
{
    class InterestRate
    {
        public:
            using BasisPoints = std::int32_t;

            [[nodiscard]] static constexpr InterestRate fromBasisPoints(BasisPoints value)
            {
                if(value < 0)
                    throw std::invalid_argument{"Interest rate cannot be negative"};

                return InterestRate(value);
            }

            [[nodiscard]] constexpr BasisPoints basisPoints() const noexcept
            {
                return basisPoints_;
            }

            [[nodiscard]] constexpr bool isZero() const noexcept
            {
                return basisPoints_ == 0;
            }
            
            constexpr auto operator<=>(const InterestRate&) const noexcept = default;


        private:
            explicit constexpr InterestRate(BasisPoints value) noexcept : basisPoints_{value}{}

            BasisPoints basisPoints_{0};
    };
}
