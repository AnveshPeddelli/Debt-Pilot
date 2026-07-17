#pragma once

#include <cstdint>
#include <stdexcept>

namespace debtpilot
{
    class InterestRate
    {
        public:
            using BasisPoints = std::int32_t;

            static InterestRate fromBasisPoints(BasisPoints value)
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
            
            auto operator <=>(const InterestRate&) const = default;


        private:
            explicit constexpr InterestRate(BasisPoints value) noexcept : basisPoints_{value}{}

            BasisPoints basisPoints_{0};
    };
}