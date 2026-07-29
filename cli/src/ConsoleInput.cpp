#include "debtpilot/cli/ConsoleInput.hpp"

#include "debtpilot/DebtType.hpp"
#include "debtpilot/InterestRate.hpp"

#include <charconv>
#include <cctype>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>


namespace debtpilot::cli
{
    namespace
    {
        std::string trim(std::string value)
        {
            const auto first = value.find_first_not_of("\t\r\n");
            if(first==std::string::npos)
            {
                return {};
            }

            const auto last = value.find_last_not_of("\t\r\n");

            return value.substr(first, last-first+1);
        }

        std::int64_t parseInteger(const std::string& value)
        {
            std::int64_t result{};

            const char* begin = value.data();
            const char* end = begin + value.size();

            const auto [position, error] = std::from_chars(begin, end, result);
            if(error!=std::errc{} || position!=end)
            {
                throw std::invalid_argument{"Value is not a valid integer"};
            }
            
            return result;
        }
    }

    std::vector<Debt> ConsoleInput::readDebts() const
    {
        const std::size_t debtCount = readPositiveCount("Enter number of debts: ");
        std::vector<Debt> debts;
        debts.reserve(debtCount);

        for(std::size_t index = 0; index < debtCount; ++index)
        {
            std::cout<< "\nDebt "<< index+1<<"\n";
            const std::string id = readRequiredText(" ID: ");
            const std::string name = readRequiredText( " Name: ");
            const std::int64_t balancePaise = readNonNegativePaise(" Outstanding balance (rupees): ");
            const std::int32_t annualBasisPoints = readNonNegativeBasisPoints(" Annual interest rate in basis points (example: 1800 = 18%): ");
            const std::int64_t minimumPaymentPaise = readNonNegativePaise(" Minimum monthly payment (rupees): ");

            debts.emplace_back(id, name, DebtType::CreditCard, Money::fromPaise(balancePaise), InterestRate::fromBasisPoints(annualBasisPoints), Money::fromPaise(minimumPaymentPaise));
        }
        return debts;
    }

    Money ConsoleInput::readMonthlyBudget() const
    {
        return Money::fromPaise(readNonNegativePaise("\nEnter total monthly debt budget (rupees): "));
    }

    std::string ConsoleInput::readRequiredText(const std::string& prompt)
    {
        while(true)
        {
            std::cout<< prompt;
            std::string value;
            if(!std::getline(std::cin, value))
            {
                throw std::runtime_error{"Input stream was closed"};
            }
            value = trim(std::move(value));
            if(!value.empty())
            {
                return value;
            }
            std::cout<< " value cannot be empty. Please try again.\n";
        }
    }

    std::size_t ConsoleInput::readPositiveCount(const std::string& prompt)
    {
        while(true)
        {
            const std::string input = readRequiredText(prompt);

            try
            {
                const std::int64_t value = parseInteger(input);
                if(value <= 0)
                {
                    throw std::invalid_argument{"Count must be positive"};
                }
                if(static_cast<std::uint64_t>(value) > std::numeric_limits<std::size_t>::max())
                {
                    throw std::out_of_range("Count is too large");
                }

                return static_cast<std::size_t>(value);
            }
            catch(const std::exception&)
            {
                std::cout<< " Enter a positive whole number.\n";
            }
        }
    }

    std::int64_t ConsoleInput::readNonNegativePaise(const std::string& prompt)
    {
        while (true)
        {
            const std::string input = readRequiredText(prompt);

            try
            {
                const std::int64_t paise = parseRupeesToPaise(input);
                if(paise < 0)
                {
                    throw std::invalid_argument{"Money cannot be negative"};
                }
                return paise;
            }
            catch(const std::exception&)
            {
                std::cout<< " Enter a valid non-negative amount, such as 1500 or 1500.50.\n";
            }
        }
        
    }

    std::int32_t ConsoleInput::readNonNegativeBasisPoints(const std::string& prompt)
    {
        while(true)
        {
            const std::string input = readRequiredText(prompt);

            try
            {
                const std::int64_t value = parseInteger(input);
                if(value<0 || value>std::numeric_limits<std::int32_t>::max())
                {
                    throw std::out_of_range("Basis points are outside range");
                }
                return static_cast<std::int32_t>(value);
            }
            catch(const std::exception&)
            {
                std::cout<< " Enter a valid non-negative whole number of basis points.\n";
            }
        }
    }

    std::int64_t ConsoleInput::parseRupeesToPaise(const std::string& input)
    {
        const std::string value = trim(input);

        if(value.empty())
        {
            throw std::invalid_argument{"Amount cannot be empty"};
        }
        if(value.front() == '-')
        {
            throw std::invalid_argument{"Amount cannot be negative"};
        }
        const std::size_t decimalPosition = value.find('.');
        if(decimalPosition != std::string::npos && value.find('.', decimalPosition+1)!=std::string::npos)
        {
            throw std::invalid_argument{"Amount has multiple decimal points"};
        }
        const std::string rupeesText = decimalPosition == std::string::npos ? value : value.substr(0, decimalPosition);
        std::string paiseText = decimalPosition == std::string::npos ? "" : value.substr(decimalPosition+1);

        if(rupeesText.empty())
        {
            throw std::invalid_argument{"Rupee amount is missing"};
        }
        if(paiseText.size() > 2)
        {
            throw  std::invalid_argument{"Amount can have at most two decimals"};
        }

        for(const char character : rupeesText)
        {
            if(!std::isdigit(static_cast<unsigned char>(character)))
            {
                throw std::invalid_argument{"Invalid rupee amount"};
            }
        }

        for(const char character: paiseText)
        {
            if(!std::isdigit(static_cast<unsigned char>(character)))
            {
                throw std::invalid_argument("Invalid paise amount");
            }
        }

        if(paiseText.empty())
        {
            paiseText = "00";
        }
        else if(paiseText.size() == 1)
        {
            paiseText.push_back('0');
        }

        const std::int64_t rupees = parseInteger(rupeesText);
        const std::int64_t paise = parseInteger(paiseText);

        if(rupees>(std::numeric_limits<std::int64_t>::max()-paise)/100)
        {
            throw std::overflow_error("Amount is too large");
        }

        return rupees*100 + paise;
    }

}