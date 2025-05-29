/**
 * @file PassportNumberError.h
 * @brief Định nghĩa các lỗi xác thực số hộ chiếu và tiện ích xử lý lỗi
 * @author Nhóm dự án OOP
 */

#ifndef PASSPORT_NUMBER_ERROR_H
#define PASSPORT_NUMBER_ERROR_H

#include <string>
#include <unordered_map>
#include "../../exceptions/ValidationResult.h"

/**
 * @enum PassportNumberError
 * @brief Liệt kê các lỗi xác thực số hộ chiếu có thể xảy ra
 */
enum class PassportNumberError {
    EMPTY_PASSPORT_NUMBER,    ///< Số hộ chiếu rỗng
    INVALID_FORMAT,          ///< Định dạng số hộ chiếu không hợp lệ
    INVALID_ISSUING_COUNTRY, ///< Mã quốc gia cấp phát không hợp lệ
    INVALID_NUMBER_LENGTH    ///< Độ dài số hộ chiếu không hợp lệ
};

/**
 * @struct PassportNumberErrorHelper
 * @brief Struct hỗ trợ để quản lý thông báo lỗi và mã lỗi số hộ chiếu
 * 
 * Struct này cung cấp các phương thức tiện ích để chuyển đổi lỗi số hộ chiếu
 * thành biểu diễn chuỗi và thông báo thân thiện với người dùng.
 */
struct PassportNumberErrorHelper {
    /**
     * @brief Chuyển đổi PassportNumberError thành biểu diễn chuỗi
     * @param error Lỗi cần chuyển đổi
     * @return Biểu diễn chuỗi của mã lỗi
     */
    static std::string toString(PassportNumberError error) {
        static const std::unordered_map<PassportNumberError, std::string> errorMap = {
            {PassportNumberError::EMPTY_PASSPORT_NUMBER, "EMPTY_PASSPORT_NUMBER"},
            {PassportNumberError::INVALID_FORMAT, "INVALID_FORMAT"},
            {PassportNumberError::INVALID_ISSUING_COUNTRY, "INVALID_ISSUING_COUNTRY"},
            {PassportNumberError::INVALID_NUMBER_LENGTH, "INVALID_NUMBER_LENGTH"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    /**
     * @brief Lấy thông báo dễ đọc cho PassportNumberError
     * @param error Lỗi cần lấy thông báo
     * @return Thông báo lỗi thân thiện với người dùng
     */
    static std::string getMessage(PassportNumberError error) {
        static const std::unordered_map<PassportNumberError, std::string> messageMap = {
            {PassportNumberError::EMPTY_PASSPORT_NUMBER, "Số hộ chiếu không được để trống"},
            {PassportNumberError::INVALID_FORMAT, "Số hộ chiếu phải có định dạng 'QUOC_GIA:SOMAY'"},
            {PassportNumberError::INVALID_ISSUING_COUNTRY, "Quốc gia cấp phát phải là mã quốc gia 2 chữ cái hợp lệ"},
            {PassportNumberError::INVALID_NUMBER_LENGTH, "Số hộ chiếu phải có độ dài từ 6-9 chữ số"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Đã xảy ra lỗi không xác định";
    }

    /**
     * @brief Thêm lỗi số hộ chiếu vào kết quả xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     * @param error Lỗi cần thêm
     */
    static void addError(ValidationResult& result, PassportNumberError error) {
        result.addError(
            "passportNumber",
            getMessage(error),
            toString(error)
        );
    }
};

#endif