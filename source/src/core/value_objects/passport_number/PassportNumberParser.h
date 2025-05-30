/**
 * @file PassportNumberParser.h
 * @brief Định nghĩa lớp PassportNumberParser để phân tích chuỗi số hộ chiếu
 * @author Nguyễn Phúc Hoàng
 */

#ifndef PASSPORT_NUMBER_PARSER_H
#define PASSPORT_NUMBER_PARSER_H

#include <string>
#include <utility>
#include <optional>

/**
 * @class PassportNumberParser
 * @brief Lớp tiện ích để phân tích chuỗi số hộ chiếu
 * 
 * Lớp này cung cấp chức năng phân tích chuỗi số hộ chiếu
 * có định dạng "QUỐC_GIA:SỐ" thành các thành phần riêng biệt.
 */
class PassportNumberParser {
public:
    /**
     * @brief Phân tích chuỗi số hộ chiếu thành mã quốc gia và số
     * @param value Chuỗi có định dạng "QUỐC_GIA:SỐ"
     * @return Optional pair của (mã_quốc_gia, số) nếu phân tích thành công, nullopt nếu không
     */
    static std::optional<std::pair<std::string, std::string>> parse(const std::string& value) {
        size_t colonPos = value.find(':');
        if (colonPos != std::string::npos) {
            std::string countryCode = value.substr(0, colonPos);
            std::string number = value.substr(colonPos + 1);
            return std::make_pair(countryCode, number);
        }
        return std::nullopt;
    }
};

#endif