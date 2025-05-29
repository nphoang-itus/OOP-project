/**
 * @file ContactInfo.h
 * @brief Value object đại diện cho thông tin liên lạc của hành khách
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef CONTACT_INFO_H
#define CONTACT_INFO_H

#include <string>
#include <tuple>
#include "../../exceptions/Result.h"
#include "ContactInfoValidator.h"
#include "ContactInfoParser.h"
#include "ContactInfoFormatter.h"

/**
 * @brief Value object đại diện cho thông tin liên lạc đầy đủ của hành khách
 * 
 * ContactInfo đóng gói email, số điện thoại và địa chỉ của hành khách.
 * Value object này đảm bảo tính hợp lệ của từng thành phần theo các chuẩn quốc tế:
 * - Email theo RFC 5321 (tối đa 254 ký tự)
 * - Số điện thoại theo E.164 (tối đa 15 chữ số)
 * - Địa chỉ tùy chọn (tối đa 100 ký tự)
 */
class ContactInfo {
private:
    std::string _email;   ///< Địa chỉ email của hành khách
    std::string _phone;   ///< Số điện thoại của hành khách  
    std::string _address; ///< Địa chỉ của hành khách (tùy chọn)

    /**
     * @brief Constructor riêng tư để đảm bảo tạo instance chỉ thông qua factory method
     * @param email Địa chỉ email đã được validate
     * @param phone Số điện thoại đã được validate
     * @param address Địa chỉ đã được validate (có thể rỗng)
     */
    ContactInfo(std::string email, std::string phone, std::string address)
        : _email(std::move(email)), _phone(std::move(phone)), _address(std::move(address)) {}

    /**
     * @brief Template method cho quy trình tạo ContactInfo
     * @tparam InputType Kiểu đầu vào (tuple hoặc string)
     * @param input Dữ liệu đầu vào cần xử lý
     * @return Result chứa ContactInfo hợp lệ hoặc lỗi validation
     */
    template<typename InputType>
    static Result<ContactInfo> createInternal(const InputType& input) {
        auto validationResult = ContactInfoValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<ContactInfo>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::tuple<std::string, std::string, std::string>>) {
            const auto& [email, phone, address] = input;
            return Success(ContactInfo(email, phone, address));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = ContactInfoParser::parse(input);
            if (!parsed) {
                return Failure<ContactInfo>(CoreError("Failed to parse contact info", "PARSE_ERROR"));
            }
            const auto& [email, phone, address] = *parsed;
            return Success(ContactInfo(email, phone, address));
        }
        return Failure<ContactInfo>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    /**
     * @brief Constructor mặc định tạo ContactInfo rỗng
     */
    ContactInfo() = default;

    /**
     * @brief Tạo ContactInfo từ các thành phần riêng biệt
     * @param email Địa chỉ email (bắt buộc)
     * @param phone Số điện thoại (bắt buộc)
     * @param address Địa chỉ (tùy chọn, mặc định rỗng)
     * @return Result chứa ContactInfo hợp lệ hoặc lỗi validation
     * 
     * Phương thức này validate từng thành phần:
     * - Email: định dạng hợp lệ, tối đa 254 ký tự
     * - Phone: định dạng E.164, tối đa 15 chữ số, có thể có tiền tố '+'
     * - Address: tùy chọn, tối đa 100 ký tự nếu có
     */
    static Result<ContactInfo> create(const std::string& email, const std::string& phone, const std::string& address = "") {
        return createInternal(std::make_tuple(email, phone, address));
    }

    /**
     * @brief Tạo ContactInfo từ chuỗi định dạng
     * @param value Chuỗi theo định dạng "email|phone|address"
     * @return Result chứa ContactInfo hợp lệ hoặc lỗi validation/parsing
     * 
     * Chuỗi đầu vào phải theo định dạng: "email|phone|address"
     * Ví dụ: "john@example.com|+84123456789|123 Main St"
     */
    static Result<ContactInfo> create(const std::string& value) {
        return createInternal(value);
    }

    /**
     * @brief Lấy địa chỉ email
     * @return Tham chiếu const đến chuỗi email
     */
    const std::string& getEmail() const { return _email; }

    /**
     * @brief Lấy số điện thoại
     * @return Tham chiếu const đến chuỗi số điện thoại
     */
    const std::string& getPhone() const { return _phone; }

    /**
     * @brief Lấy địa chỉ
     * @return Tham chiếu const đến chuỗi địa chỉ
     */
    const std::string& getAddress() const { return _address; }

    /**
     * @brief So sánh bằng với ContactInfo khác
     * @param other ContactInfo để so sánh
     * @return true nếu tất cả thành phần giống nhau
     */
    bool operator==(const ContactInfo& other) const {
        return _email == other._email && 
               _phone == other._phone && 
               _address == other._address;
    }

    /**
     * @brief So sánh khác với ContactInfo khác
     * @param other ContactInfo để so sánh
     * @return true nếu có ít nhất một thành phần khác nhau
     */
    bool operator!=(const ContactInfo& other) const {
        return !(*this == other);
    }

    /**
     * @brief Chuyển đổi thành chuỗi định dạng
     * @return Chuỗi theo định dạng "email|phone|address"
     */
    std::string toString() const {
        return ContactInfoFormatter::toString(_email, _phone, _address);
    }
};

#endif