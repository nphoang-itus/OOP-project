/**
 * @file ContactInfoParser.h
 * @brief Parser để phân tích chuỗi thông tin liên lạc thành các thành phần riêng biệt
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef CONTACT_INFO_PARSER_H
#define CONTACT_INFO_PARSER_H

#include <string>
#include <tuple>
#include <optional>
#include <regex>

/**
 * @brief Lớp parser để phân tích chuỗi thông tin liên lạc
 * 
 * ContactInfoParser chuyển đổi chuỗi định dạng "email|phone|address"
 * thành tuple các thành phần riêng biệt để xử lý và validation.
 * 
 * Định dạng đầu vào: "email|phone|address"
 * - email: Bắt buộc, không được chứa ký tự '|'
 * - phone: Bắt buộc, không được chứa ký tự '|'
 * - address: Tùy chọn, có thể rỗng, không được chứa ký tự '|'
 */
class ContactInfoParser {
private:
    static const std::regex CONTACT_INFO_REGEX; ///< Regex pattern để parse chuỗi thông tin liên lạc

public:
    /**
     * @brief Parse chuỗi thông tin liên lạc thành tuple
     * @param value Chuỗi theo định dạng "email|phone|address"
     * @return Optional chứa tuple (email, phone, address) nếu parse thành công, nullopt nếu thất bại
     * 
     * Phương thức này sử dụng regex để tách chuỗi đầu vào thành 3 phần:
     * 1. Email (phần trước dấu '|' đầu tiên)
     * 2. Phone (phần giữa hai dấu '|')
     * 3. Address (phần sau dấu '|' cuối, có thể rỗng)
     * 
     * Ví dụ:
     * - "john@email.com|+84123456789|123 Main St" → ("john@email.com", "+84123456789", "123 Main St")
     * - "jane@email.com|0987654321|" → ("jane@email.com", "0987654321", "")
     */
    static std::optional<std::tuple<std::string, std::string, std::string>> parse(const std::string& value) {
        std::smatch matches;
        if (std::regex_match(value, matches, CONTACT_INFO_REGEX)) {
            return std::make_tuple(matches[1].str(), matches[2].str(), matches[3].str());
        }
        return std::nullopt;
    }
};

/**
 * @brief Khởi tạo static regex pattern
 * 
 * Pattern: ^([^|]+)\\|([^|]+)\\|(.*)$
 * - ([^|]+): Nhóm 1 - email (ít nhất 1 ký tự không phải '|')
 * - \\|: Dấu phân cách '|' đầu tiên
 * - ([^|]+): Nhóm 2 - phone (ít nhất 1 ký tự không phải '|')
 * - \\|: Dấu phân cách '|' thứ hai
 * - (.*): Nhóm 3 - address (bất kỳ ký tự nào, có thể rỗng)
 */
inline const std::regex ContactInfoParser::CONTACT_INFO_REGEX(
    "^([^|]+)\\|([^|]+)\\|(.*)$"
);

#endif