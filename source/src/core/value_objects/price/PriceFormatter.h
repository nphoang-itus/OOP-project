#ifndef PRICE_FORMATTER_H
#define PRICE_FORMATTER_H

#include <string>
#include <iomanip>
#include <sstream>

class PriceFormatter {
public:
    static std::string toString(double amount, const std::string& currency) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << amount << ":" << currency;
        return ss.str();
    }
};

#endif 