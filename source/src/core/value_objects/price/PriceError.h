#ifndef PRICE_ERROR_H
#define PRICE_ERROR_H

#include <string>
#include <unordered_map>
#include "../../exceptions/ValidationResult.h"
#include "CurrencyRegistry.h"

enum class PriceError {
    EMPTY_PRICE,
    INVALID_FORMAT,
    INVALID_CURRENCY,
    INVALID_AMOUNT,
    NEGATIVE_AMOUNT
};

struct PriceErrorHelper {
    static std::string toString(PriceError error) {
        static const std::unordered_map<PriceError, std::string> errorMap = {
            {PriceError::EMPTY_PRICE, "EMPTY_PRICE"},
            {PriceError::INVALID_FORMAT, "INVALID_FORMAT"},
            {PriceError::INVALID_CURRENCY, "INVALID_CURRENCY"},
            {PriceError::INVALID_AMOUNT, "INVALID_AMOUNT"},
            {PriceError::NEGATIVE_AMOUNT, "NEGATIVE_AMOUNT"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    static std::string getMessage(PriceError error) {
        static const std::unordered_map<PriceError, std::string> messageMap = {
            {PriceError::EMPTY_PRICE, "Price cannot be empty"},
            {PriceError::INVALID_FORMAT, "Price must be in format 'AMOUNT CURRENCY' (amount can use . or , as decimal separator)"},
            {PriceError::INVALID_CURRENCY, "Currency must be one of: " + getSupportedCurrencies()},
            {PriceError::INVALID_AMOUNT, "Amount must be a valid number"},
            {PriceError::NEGATIVE_AMOUNT, "Amount cannot be negative"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Unknown error occurred";
    }

    static void addError(ValidationResult& result, PriceError error) {
        result.addError(
            "price",
            getMessage(error),
            toString(error)
        );
    }

private:
    static std::string getSupportedCurrencies() {
        auto codes = CurrencyRegistry::getAllCodes();
        std::string result;
        for (size_t i = 0; i < codes.size(); ++i) {
            if (i > 0) result += ", ";
            result += codes[i];
        }
        return result;
    }
};

#endif 