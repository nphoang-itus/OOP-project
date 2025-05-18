#ifndef PRICE_H
#define PRICE_H

#include <string>
#include <utility>
#include <optional>
#include <algorithm>
#include "../../exceptions/ValidationResult.h"
#include "PriceValidator.h"
#include "PriceParser.h"
#include "PriceFormatter.h"
#include "CurrencyRegistry.h"

class Price {
private:
    double _amount;
    std::string _currency;

    Price(const double& amount, const std::string& currency) 
        : _amount(amount) {
        std::string upperCurrency = currency;
        
        // Convert currency to uppercase
        std::transform(upperCurrency.begin(), upperCurrency.end(), upperCurrency.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        
        _currency = upperCurrency;
    }

    template<typename InputType>
    static Result<Price> createInternal(const InputType& input) {
        auto validationResult = PriceValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<Price>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::pair<double, std::string>>) {
            const auto& [amount, currency] = input;
            return Success(Price(amount, currency));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = PriceParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<Price>(CoreError("Failed to parse price", "PARSE_ERROR"));
            }
            const auto& [amount, currency] = parsed.value();
            return Success(Price(amount, currency));
        }
        return Failure<Price>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    static Result<Price> create(const std::string& value) {
        return createInternal(value);    
    }

    static Result<Price> create(const double& amount, const std::string& currency) {
        return createInternal(std::make_pair(amount, currency));
    }

    const double& getAmount() const {
        return _amount;
    }

    const std::string& getCurrency() const {
        return _currency;
    }

    std::optional<std::string> getCurrencyName() const {
        return CurrencyRegistry::getName(_currency);
    }

    std::string toString() const {
        return PriceFormatter::toString(_amount, _currency);
    }

    bool operator==(const Price& other) const {
        return _amount == other._amount && _currency == other._currency;
    }

    bool operator!=(const Price& other) const {
        return !(*this == other);
    }
};

#endif
