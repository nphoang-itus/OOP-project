#ifndef PRICE_VALIDATOR_H
#define PRICE_VALIDATOR_H

#include <string>
#include <regex>
#include <algorithm>
#include <sstream>
#include "../../exceptions/ValidationResult.h"
#include "PriceError.h"
#include "CurrencyRegistry.h"

class PriceValidator {
private:
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    static bool isValidCurrency(const std::string& currency) {
        return CurrencyRegistry::isValidCurrency(currency);
    }

    static bool isValidAmount(double amount) {
        return amount >= 0;
    }

    static std::string normalizeDecimalSeparator(const std::string& value) {
        std::string result = value;
        std::replace(result.begin(), result.end(), ',', '.');
        return result;
    }

public:
    // Validate combined string format (AMOUNT CURRENCY)
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            PriceErrorHelper::addError(result, PriceError::EMPTY_PRICE);
            return result;
        }

        // Validate format (AMOUNT CURRENCY)
        // Support both dot and comma as decimal separators
        std::regex pattern("^-?[0-9]+([.,][0-9]+)?\\s+[A-Za-z]+$");
        if (!std::regex_match(value, pattern)) {
            PriceErrorHelper::addError(result, PriceError::INVALID_FORMAT);
            return result;
        }

        // Split and validate components
        std::string normalizedValue = normalizeDecimalSeparator(value);
        std::istringstream iss(normalizedValue);
        double amount;
        std::string currency;
        
        if (iss >> amount >> currency) {
            if (!isValidAmount(amount)) {
                PriceErrorHelper::addError(result, PriceError::NEGATIVE_AMOUNT);
            }

            if (!isValidCurrency(currency)) {
                PriceErrorHelper::addError(result, PriceError::INVALID_CURRENCY);
            }
        } else {
            PriceErrorHelper::addError(result, PriceError::INVALID_AMOUNT);
        }
        
        return result;
    }

    // Validate separate amount and currency
    static ValidationResult validate(const std::pair<double, std::string>& input) {
        ValidationResult result;
        const auto& [amount, currency] = input;
        
        if (!isValidAmount(amount)) {
            PriceErrorHelper::addError(result, PriceError::NEGATIVE_AMOUNT);
        }

        if (!isValidCurrency(currency)) {
            PriceErrorHelper::addError(result, PriceError::INVALID_CURRENCY);
        }
        
        return result;
    }
};

#endif 