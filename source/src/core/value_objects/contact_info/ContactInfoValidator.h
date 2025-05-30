/**
 * @file ContactInfoValidator.h
 * @brief Validator cho thông tin liên lạc theo các chuẩn quốc tế
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef CONTACT_INFO_VALIDATOR_H
#define CONTACT_INFO_VALIDATOR_H

#include <string>
#include <regex>
#include <tuple>
#include <unordered_map>
#include "ContactInfoParser.h"
#include "../../exceptions/ValidationResult.h"

/**
 * @brief Enum định nghĩa các loại lỗi validation có thể xảy ra với thông tin liên lạc
 */
enum class ContactInfoError {
    EMPTY_EMAIL,            ///< Email rỗng
    INVALID_EMAIL_FORMAT,   ///< Định dạng email không hợp lệ
    EMAIL_TOO_LONG,         ///< Email quá dài (>254 ký tự)
    EMPTY_PHONE,            ///< Số điện thoại rỗng
    INVALID_PHONE_FORMAT,   ///< Định dạng số điện thoại không hợp lệ
    PHONE_TOO_LONG,         ///< Số điện thoại quá dài (>15 chữ số)
    INVALID_ADDRESS_FORMAT, ///< Định dạng địa chỉ không hợp lệ
    ADDRESS_TOO_LONG        ///< Địa chỉ quá dài (>100 ký tự)
};

/**
 * @brief Helper struct cung cấp thông tin chi tiết về các lỗi ContactInfo
 * 
 * Struct này chuyển đổi enum error thành chuỗi mã lỗi và thông báo
 * có thể đọc được, đồng thời cung cấp phương thức tiện lợi để thêm lỗi.
 */
struct ContactInfoErrorHelper {
    /**
     * @brief Chuyển đổi mã lỗi thành chuỗi để lưu trữ
     * @param error Mã lỗi ContactInfoError
     * @return Chuỗi đại diện cho mã lỗi
     */
    static std::string toString(ContactInfoError error) {
        static const std::unordered_map<ContactInfoError, std::string> errorMap = {
            {ContactInfoError::EMPTY_EMAIL, "EMPTY_EMAIL"},
            {ContactInfoError::INVALID_EMAIL_FORMAT, "INVALID_EMAIL_FORMAT"},
            {ContactInfoError::EMAIL_TOO_LONG, "EMAIL_TOO_LONG"},
            {ContactInfoError::EMPTY_PHONE, "EMPTY_PHONE"},
            {ContactInfoError::INVALID_PHONE_FORMAT, "INVALID_PHONE_FORMAT"},
            {ContactInfoError::PHONE_TOO_LONG, "PHONE_TOO_LONG"},
            {ContactInfoError::INVALID_ADDRESS_FORMAT, "INVALID_ADDRESS_FORMAT"},
            {ContactInfoError::ADDRESS_TOO_LONG, "ADDRESS_TOO_LONG"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    /**
     * @brief Lấy thông báo lỗi có thể đọc được
     * @param error Mã lỗi ContactInfoError
     * @return Thông báo lỗi bằng tiếng Anh cho người dùng
     */
    static std::string getMessage(ContactInfoError error) {
        static const std::unordered_map<ContactInfoError, std::string> messageMap = {
            {ContactInfoError::EMPTY_EMAIL, "Email cannot be empty"},
            {ContactInfoError::INVALID_EMAIL_FORMAT, "Invalid email format"},
            {ContactInfoError::EMAIL_TOO_LONG, "Email must not exceed 254 characters"},
            {ContactInfoError::EMPTY_PHONE, "Phone number cannot be empty"},
            {ContactInfoError::INVALID_PHONE_FORMAT, "Invalid phone number format"},
            {ContactInfoError::PHONE_TOO_LONG, "Phone number must not exceed 15 digits"},
            {ContactInfoError::INVALID_ADDRESS_FORMAT, "Invalid address format"},
            {ContactInfoError::ADDRESS_TOO_LONG, "Address must not exceed 100 characters"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Unknown error occurred";
    }

    /**
     * @brief Thêm lỗi vào ValidationResult
     * @param result ValidationResult để thêm lỗi vào
     * @param error Mã lỗi cần thêm
     */
    static void addError(ValidationResult& result, ContactInfoError error) {
        result.addError(
            "contactInfo",
            getMessage(error),
            toString(error)
        );
    }
};

/**
 * @brief Lớp validator để kiểm tra tính hợp lệ của thông tin liên lạc
 * 
 * ContactInfoValidator thực hiện validation theo các chuẩn quốc tế:
 * - Email: RFC 5321 (tối đa 254 ký tự)
 * - Phone: E.164 (10-15 chữ số, có thể có tiền tố '+')
 * - Address: Tùy chọn (tối đa 100 ký tự)
 * 
 * Validator hỗ trợ hai kiểu đầu vào:
 * - Tuple của (email, phone, address)
 * - Chuỗi định dạng "email|phone|address"
 */
class ContactInfoValidator {
private:
    static const std::regex EMAIL_REGEX;   ///< Regex pattern cho email
    static const std::regex PHONE_REGEX;   ///< Regex pattern cho số điện thoại
    static const std::regex ADDRESS_REGEX; ///< Regex pattern cho địa chỉ

    // Hằng số độ dài tối đa
    static constexpr size_t MAX_EMAIL_LENGTH = 254;   ///< Độ dài tối đa email theo RFC 5321
    static constexpr size_t MAX_PHONE_LENGTH = 15;    ///< Độ dài tối đa số điện thoại theo E.164
    static constexpr size_t MAX_ADDRESS_LENGTH = 100; ///< Độ dài tối đa địa chỉ

    /**
     * @brief Template method để validation nội bộ
     * @tparam InputType Kiểu đầu vào (tuple hoặc string)
     * @param input Dữ liệu đầu vào cần validate
     * @return ValidationResult chứa kết quả validation
     */
    template<typename InputType>
    static ValidationResult validateInternal(const InputType& input) {
        ValidationResult result;
        validateSpecific(input, result);
        return result;
    }

    /**
     * @brief Validate tuple chứa email, phone, address
     * @param input Tuple (email, phone, address)
     * @param result ValidationResult để ghi lỗi
     */
    static void validateSpecific(const std::tuple<std::string, std::string, std::string>& input, ValidationResult& result) {
        const auto& [email, phone, address] = input;
        validateEmail(email, result);
        validatePhone(phone, result);
        validateAddress(address, result);
    }

    /**
     * @brief Validate chuỗi định dạng sau khi parse
     * @param value Chuỗi cần parse và validate
     * @param result ValidationResult để ghi lỗi
     */
    static void validateSpecific(const std::string& value, ValidationResult& result) {
        auto parsed = ContactInfoParser::parse(value);
        if (!parsed) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_EMAIL_FORMAT);
            return;
        }
        validateSpecific(*parsed, result);
    }

    /**
     * @brief Validate địa chỉ email
     * @param email Chuỗi email cần kiểm tra
     * @param result ValidationResult để ghi lỗi
     */
    static void validateEmail(const std::string& email, ValidationResult& result) {
        if (email.empty()) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::EMPTY_EMAIL);
            return;
        }
        if (email.length() > MAX_EMAIL_LENGTH) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::EMAIL_TOO_LONG);
            return;
        }
        if (!std::regex_match(email, EMAIL_REGEX)) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_EMAIL_FORMAT);
        }
    }

    /**
     * @brief Validate số điện thoại
     * @param phone Chuỗi số điện thoại cần kiểm tra
     * @param result ValidationResult để ghi lỗi
     */
    static void validatePhone(const std::string& phone, ValidationResult& result) {
        if (phone.empty()) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::EMPTY_PHONE);
            return;
        }
        // Remove '+' prefix for length check
        std::string phoneDigits = phone;
        if (!phoneDigits.empty() && phoneDigits[0] == '+') {
            phoneDigits = phoneDigits.substr(1);
        }
        if (phoneDigits.length() > MAX_PHONE_LENGTH) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::PHONE_TOO_LONG);
            return;
        }
        if (!std::regex_match(phone, PHONE_REGEX)) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_PHONE_FORMAT);
        }
    }

    /**
     * @brief Validate địa chỉ (tùy chọn)
     * @param address Chuỗi địa chỉ cần kiểm tra
     * @param result ValidationResult để ghi lỗi
     */
    static void validateAddress(const std::string& address, ValidationResult& result) {
        if (!address.empty()) {
            if (address.length() > MAX_ADDRESS_LENGTH) {
                ContactInfoErrorHelper::addError(result, ContactInfoError::ADDRESS_TOO_LONG);
                return;
            }
            if (!std::regex_match(address, ADDRESS_REGEX)) {
                ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_ADDRESS_FORMAT);
            }
        }
    }

public:
    /**
     * @brief Validate thông tin liên lạc từ tuple
     * @param input Tuple chứa (email, phone, address)
     * @return ValidationResult chứa kết quả validation và danh sách lỗi nếu có
     */
    static ValidationResult validate(const std::tuple<std::string, std::string, std::string>& input) {
        return validateInternal(input);
    }

    /**
     * @brief Validate thông tin liên lạc từ chuỗi định dạng
     * @param value Chuỗi theo định dạng "email|phone|address"
     * @return ValidationResult chứa kết quả validation và danh sách lỗi nếu có
     */
    static ValidationResult validate(const std::string& value) {
        return validateInternal(value);
    }
};

// Khởi tạo static regex patterns
inline const std::regex ContactInfoValidator::EMAIL_REGEX(
    R"(^[a-zA-Z0-9](\.?[a-zA-Z0-9_\-+%])*@[a-zA-Z0-9]([a-zA-Z0-9\-]*[a-zA-Z0-9])?(\.[a-zA-Z]{2,})+$)"
);

inline const std::regex ContactInfoValidator::PHONE_REGEX("^\\+?[0-9]{10,15}$");
inline const std::regex ContactInfoValidator::ADDRESS_REGEX("^[a-zA-Z0-9\\s,.-]{0,100}$");

#endif