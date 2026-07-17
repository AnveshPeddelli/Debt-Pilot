#pragma once

#include <iostream>
#include <compare>


namespace debtpilot
{
    class Money
    {
        public:
            using ValueType = std::int64_t;
            constexpr Money() noexcept = default;

            explicit constexpr Money(ValueType paise) noexcept : paise_{paise}
            {

            }

            [[nodiscard]] static constexpr Money fromPaise(ValueType paise) noexcept
            {
                return Money(paise);
            }

            [[nodiscard]] constexpr ValueType paise() const noexcept
            {
                return paise_;
            }
            
            [[nodiscard]] constexpr bool isZero() const noexcept
            {
                return paise_ == 0;
            }

            [[nodiscard]] constexpr bool isNegative() const noexcept
            {
                return paise_ < 0;
            }

            constexpr Money& operator+=(Money other) noexcept
            {
                paise_ += other.paise_;
                return *this;
            }

            constexpr Money& operator-=(Money other) noexcept
            {
                paise_ -= other.paise_;
                return *this;
            }

            friend constexpr Money operator+(Money left, Money right) noexcept
            {
                left += right;
                return left;
            }

            friend constexpr Money operator-(Money left, Money right) noexcept
            {
                left -= right;
                return left;
            }

            std::strong_ordering operator<=>(const Money&) const = default;    

        private:
            ValueType paise_{0};
    };
}