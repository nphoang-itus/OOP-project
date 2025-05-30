/**
 * @file AircraftSerialValidator.h
 * @brief Validator cho số serial máy bay với các quy tắc nghiệp vụ cụ thể
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef AIRCRAFT_SERIAL_VALIDATOR_H
#define AIRCRAFT_SERIAL_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"

/**
 * @brief Enum định nghĩa các loại lỗi validation có thể xảy ra với số serial máy bay
 */
enum class AircraftSerialError {
    EMPTY_AIRCRAFT_SERIAL,  ///< Số serial rỗng
    INVALID_LENGTH,         ///< Độ dài không hợp lệ
    INVALID_FORMAT          ///< Định dạng không đúng quy chuẩn
};

/**
 * @brief Helper struct cung cấp thông tin chi tiết về các lỗi AircraftSerial
 * 
 * Struct này chuyển đổi enum error thành chuỗi mã lỗi và thông báo
 * có thể đọc được để hỗ trợ debugging và hiển thị lỗi cho người dùng.
 */
struct AircraftSerialErrorHelper {
    /**
     * @brief Chuyển đổi mã lỗi thành chuỗi để lưu trữ
     * @param error Mã lỗi AircraftSerialError
     * @return Chuỗi đại diện cho mã lỗi
     */
    static std::string toString(AircraftSerialError error) {
        switch (error) {
            case AircraftSerialError::EMPTY_AIRCRAFT_SERIAL:
                return "EMPTY_AIRCRAFT_SERIAL";
            case AircraftSerialError::INVALID_LENGTH:
                return "INVALID_LENGTH";
            case AircraftSerialError::INVALID_FORMAT:
                return "INVALID_FORMAT";
            default:
                return "UNKNOWN_ERROR";
        }
    }

    /**
     * @brief Lấy thông báo lỗi có thể đọc được
     * @param error Mã lỗi AircraftSerialError
     * @return Thông báo lỗi bằng tiếng Anh cho người dùng
     */
    static std::string getMessage(AircraftSerialError error) {
        switch (error) {
            case AircraftSerialError::EMPTY_AIRCRAFT_SERIAL:
                return "Aircraft serial cannot be empty";
            case AircraftSerialError::INVALID_LENGTH:
                return "Aircraft serial must be between 3 and 10 characters";
            case AircraftSerialError::INVALID_FORMAT:
                return "Aircraft serial must start with 2-3 letters followed by 1-7 digits";
            default:
                return "Unknown error";
        }
    }
};

/**
 * @brief Lớp validator để kiểm tra tính hợp lệ của số serial máy bay
 * 
 * AircraftSerialValidator thực hiện validation theo các quy tắc nghiệp vụ:
 * - Số serial không được rỗng
 * - Độ dài từ 3-10 ký tự
 * - Định dạng: 2-3 chữ cái hoa theo sau bởi 1-7 chữ số
 * 
 * Ví dụ số serial hợp lệ: "VN123", "AA1234567", "ABC999"
 */
class AircraftSerialValidator {
public:
    /**
     * @brief Validate số serial máy bay theo các quy tắc nghiệp vụ
     * @param value Chuỗi số serial cần kiểm tra
     * @return ValidationResult chứa kết quả validation và danh sách lỗi nếu có
     * 
     * Quy trình validation:
     * 1. Kiểm tra chuỗi không rỗng
     * 2. Kiểm tra độ dài trong khoảng 3-10 ký tự
     * 3. Kiểm tra định dạng regex: ^[A-Z]{2,3}[0-9]{1,7}$
     */
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;

        if (value.empty()) {
            auto error = AircraftSerialError::EMPTY_AIRCRAFT_SERIAL;
            result.addError(
                "aircraftSerial", 
                AircraftSerialErrorHelper::getMessage(error),
                AircraftSerialErrorHelper::toString(error)
            );
            return result;
        }

        if (value.length() < 3 || value.length() > 10) {
            auto error = AircraftSerialError::INVALID_LENGTH;
            result.addError(
                "aircraftSerial",
                AircraftSerialErrorHelper::getMessage(error),
                AircraftSerialErrorHelper::toString(error)
            );
            return result;
        }

        // Format: 2-3 letters followed by 1-7 digits
        std::regex pattern("^[A-Z]{2,3}[0-9]{1,7}$");
        if (!std::regex_match(value, pattern)) {
            auto error = AircraftSerialError::INVALID_FORMAT;
            result.addError(
                "aircraftSerial",
                AircraftSerialErrorHelper::getMessage(error),
                AircraftSerialErrorHelper::toString(error)
            );
            return result;
        }

        return result;
    }
};

#endif