/**
 * @file PriceFormatter.h
 * @brief Định nghĩa lớp PriceFormatter để định dạng giá tiền
 * @author Nguyễn Phúc Hoàng
 */

#ifndef PRICE_FORMATTER_H
#define PRICE_FORMATTER_H

#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>

/**
 * @class PriceFormatter
 * @brief Lớp tiện ích để định dạng giá tiền
 * 
 * Lớp này cung cấp chức năng định dạng giá tiền
 * thành biểu diễn chuỗi tiêu chuẩn.
 */
class PriceFormatter {
public:
    /**
     * @brief Định dạng số tiền và loại tiền tệ thành chuỗi giá tiền tiêu chuẩn
     * @param amount Số tiền (sẽ được làm tròn đến 2 chữ số thập phân)
     * @param currency Loại tiền tệ
     * @return Chuỗi đã định dạng theo "SỐ_TIỀN TIỀN_TỆ"
     */
    static std::string toString(double amount, const std::string& currency) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << amount << " " << currency;
        return ss.str();
    }
};

#endif