/**
 * @file PriceParser.h
 * @brief Định nghĩa lớp PriceParser để phân tích chuỗi giá tiền
 * @author Nhóm dự án OOP
 */

#ifndef PRICE_PARSER_H
#define PRICE_PARSER_H

#include <string>
#include <utility>
#include <optional>
#include <sstream>
#include <algorithm>

/**
 * @class PriceParser
 * @brief Lớp tiện ích để phân tích chuỗi giá tiền
 * 
 * Lớp này cung cấp chức năng phân tích chuỗi giá tiền
 * có định dạng "SỐ_TIỀN TIỀN_TỆ" thành các thành phần riêng biệt.
 */
class PriceParser {
private:
    /**
     * @brief Chuẩn hóa dấu phân cách thập phân
     * @param value Chuỗi cần chuẩn hóa
     * @return Chuỗi với dấu chấm làm phân cách thập phân
     */
    static std::string normalizeDecimalSeparator(const std::string& value) {
        std::string result = value;
        // Replace comma with dot for consistent parsing
        std::replace(result.begin(), result.end(), ',', '.');
        return result;
    }

public:
    /**
     * @brief Phân tích chuỗi giá tiền thành số tiền và loại tiền tệ
     * @param value Chuỗi có định dạng "SỐ_TIỀN TIỀN_TỆ"
     * @return Optional pair của (số_tiền, tiền_tệ) nếu phân tích thành công, nullopt nếu không
     */
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