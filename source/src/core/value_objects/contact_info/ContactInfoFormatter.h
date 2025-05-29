/**
 * @file ContactInfoFormatter.h
 * @brief Formatter để chuyển đổi thông tin liên lạc thành chuỗi định dạng
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef CONTACT_INFO_FORMATTER_H
#define CONTACT_INFO_FORMATTER_H

#include <string>

/**
 * @brief Lớp formatter để chuyển đổi thông tin liên lạc thành chuỗi
 * 
 * ContactInfoFormatter cung cấp các phương thức tĩnh để kết hợp
 * email, phone và address thành chuỗi định dạng với các separator khác nhau.
 * 
 * Hỗ trợ cả separator mặc định ('|') và separator tùy chỉnh.
 */
class ContactInfoFormatter {
public:
    /**
     * @brief Chuyển đổi thông tin liên lạc thành chuỗi với separator mặc định
     * @param email Địa chỉ email
     * @param phone Số điện thoại
     * @param address Địa chỉ
     * @return Chuỗi định dạng "email|phone|address"
     * 
     * Sử dụng dấu '|' làm separator mặc định để tạo chuỗi chuẩn
     * cho việc lưu trữ và parsing sau này.
     * 
     * Ví dụ: toString("john@email.com", "+84123456789", "123 Main St")
     * → "john@email.com|+84123456789|123 Main St"
     */
    static std::string toString(
        std::string email,
        std::string phone,
        std::string address
    ) {
        return (
                email   + '|'
            +   phone   + '|'
            +   address
        );
    }

    /**
     * @brief Chuyển đổi thông tin liên lạc thành chuỗi với separator tùy chỉnh
     * @param email Địa chỉ email
     * @param phone Số điện thoại
     * @param address Địa chỉ
     * @param separator Chuỗi phân cách tùy chỉnh
     * @return Chuỗi định dạng "email{separator}phone{separator}address"
     * 
     * Cho phép sử dụng separator tùy chỉnh để hiển thị hoặc export
     * dữ liệu theo định dạng khác nhau.
     * 
     * Ví dụ: toString("john@email.com", "+84123456789", "123 Main St", " - ")
     * → "john@email.com - +84123456789 - 123 Main St"
     */
    static std::string toString(
        std::string email,
        std::string phone,
        std::string address,
        std::string separator
    ) {
        return (
                email   + separator
            +   phone   + separator
            +   address
        );
    }
};

#endif