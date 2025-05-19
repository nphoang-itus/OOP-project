#ifndef PRICE_PARSER_H
#define PRICE_PARSER_H

#include <string>
#include <utility>
#include <optional>
#include <sstream>
#include <algorithm>

class PriceParser {
private:
    static std::string normalizeDecimalSeparator(const std::string& value) {
        std::string result = value;
        // Replace comma with dot for consistent parsing
        std::replace(result.begin(), result.end(), ',', '.');
        return result;
    }

public:
    static std::optional<std::pair<double, std::string>> parse(const std::string& value) {
        // First normalize the decimal separator
        std::string normalizedValue = normalizeDecimalSeparator(value);
        std::istringstream iss(normalizedValue);
        double amount;
        std::string currency;
        
        if (iss >> amount >> currency) {
            return std::make_pair(amount, currency);
        }
        return std::nullopt;
    }
};

#endif 