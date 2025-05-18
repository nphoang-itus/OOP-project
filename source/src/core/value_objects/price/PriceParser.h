#ifndef PRICE_PARSER_H
#define PRICE_PARSER_H

#include <string>
#include <utility>
#include <optional>

class PriceParser {
public:
    static std::optional<std::pair<double, std::string>> parse(const std::string& value) {
        size_t colonPos = value.find(':');
        if (colonPos != std::string::npos) {
            try {
                std::string amountStr = value.substr(0, colonPos);
                std::string currency = value.substr(colonPos + 1);
                double amount = std::stod(amountStr);
                return std::make_pair(amount, currency);
            } catch (...) {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }
};

#endif 