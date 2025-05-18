#ifndef PASSPORT_NUMBER_FORMATTER_H
#define PASSPORT_NUMBER_FORMATTER_H

#include <string>
#include <algorithm>

class PassportNumberFormatter {
public:
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
