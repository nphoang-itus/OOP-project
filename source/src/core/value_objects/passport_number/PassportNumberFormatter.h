/**
 * @file PassportNumberFormatter.h
 * @brief Định nghĩa lớp PassportNumberFormatter để định dạng số hộ chiếu
 * @author Nhóm dự án OOP
 */

#ifndef PASSPORT_NUMBER_FORMATTER_H
#define PASSPORT_NUMBER_FORMATTER_H

#include <string>
#include <algorithm>

/**
 * @class PassportNumberFormatter
 * @brief Lớp tiện ích để định dạng số hộ chiếu
 * 
 * Lớp này cung cấp chức năng định dạng số hộ chiếu
 * thành biểu diễn chuỗi tiêu chuẩn.
 */
class PassportNumberFormatter {
public:
    /**
     * @brief Định dạng mã quốc gia và số thành chuỗi hộ chiếu tiêu chuẩn
     * @param countryCode Mã quốc gia (sẽ được chuyển thành chữ hoa)
     * @param number Số hộ chiếu
     * @return Chuỗi đã định dạng theo "QUỐC_GIA:SỐ" với mã quốc gia chữ hoa
     */
    static std::string toString(const std::string& countryCode, const std::string& number) {
        // Tạo bản sao của countryCode để không thay đổi tham số gốc
        std::string upperCountryCode = countryCode;
        
        // Chuyển đổi countryCode thành chữ hoa
        std::transform(upperCountryCode.begin(), upperCountryCode.end(), upperCountryCode.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        
        // Nối với number và trả về kết quả
        return upperCountryCode + ":" + number;
    }
};

#endif
