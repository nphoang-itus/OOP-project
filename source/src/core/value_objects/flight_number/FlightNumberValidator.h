/**
 * @file FlightNumberValidator.h
 * @brief Validator cho số hiệu chuyến bay theo chuẩn IATA
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef FLIGHT_NUMBER_VALIDATOR_H
#define FLIGHT_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"

/**
 * @brief Enum định nghĩa các loại lỗi validation có thể xảy ra với số hiệu chuyến bay
 */
enum class FlightNumberError {
    EMPTY_FLIGHT_NUMBER,    ///< Số hiệu chuyến bay rỗng
    INVALID_LENGTH,         ///< Độ dài không hợp lệ
    INVALID_FORMAT          ///< Định dạng không đúng chuẩn IATA
};

/**
 * @brief Helper struct cung cấp thông tin chi tiết về các lỗi FlightNumber
 * 
 * Struct này chuyển đổi enum error thành chuỗi mã lỗi và thông báo
 * có thể đọc được để hỗ trợ debugging và hiển thị lỗi cho người dùng.
 */
struct FlightNumberErrorHelper {
    /**
     * @brief Chuyển đổi mã lỗi thành chuỗi để lưu trữ
     * @param error Mã lỗi FlightNumberError
     * @return Chuỗi đại diện cho mã lỗi
     */
    static std::string toString(FlightNumberError error) {
        switch (error) {
            case FlightNumberError::EMPTY_FLIGHT_NUMBER:
                return "EMPTY_FLIGHT_NUMBER";
            case FlightNumberError::INVALID_LENGTH:
                return "INVALID_LENGTH";
            case FlightNumberError::INVALID_FORMAT:
                return "INVALID_FORMAT";
            default:
                return "UNKNOWN_ERROR";
        }
    }

    /**
     * @brief Lấy thông báo lỗi có thể đọc được
     * @param error Mã lỗi FlightNumberError
     * @return Thông báo lỗi bằng tiếng Anh cho người dùng
     */
    static std::string getMessage(FlightNumberError error) {
        switch (error) {
            case FlightNumberError::EMPTY_FLIGHT_NUMBER:
                return "Flight number cannot be empty";
            case FlightNumberError::INVALID_LENGTH:
                return "Flight number must be between 3 and 6 characters";
            case FlightNumberError::INVALID_FORMAT:
                return "Flight number must start with 2 letters followed by 1-4 digits";
            default:
                return "Unknown error";
        }
    }
};

/**
 * @brief Lớp validator để kiểm tra tính hợp lệ của số hiệu chuyến bay
 * 
 * FlightNumberValidator thực hiện validation theo chuẩn IATA:
 * - Số hiệu không được rỗng
 * - Độ dài từ 3-6 ký tự
 * - Định dạng: 2 chữ cái hoa (mã hãng) + số từ 1-9999 (không bắt đầu bằng 0)
 * 
 * Ví dụ số hiệu hợp lệ: "VN123" (Vietnam Airlines), "AA1" (American Airlines)
 */
class FlightNumberValidator {
public:
    /**
     * @brief Validate số hiệu chuyến bay theo chuẩn IATA
     * @param value Chuỗi số hiệu chuyến bay cần kiểm tra
     * @return ValidationResult chứa kết quả validation và danh sách lỗi nếu có
     * 
     * Quy trình validation:
     * 1. Kiểm tra chuỗi không rỗng
     * 2. Kiểm tra độ dài trong khoảng 3-6 ký tự
     * 3. Kiểm tra định dạng regex: ^[A-Z]{2}[1-9][0-9]{0,3}$
     *    - 2 chữ cái hoa đầu (mã hãng hàng không)
     *    - Chữ số đầu tiên từ 1-9 (không được là 0)
     *    - Tối đa 3 chữ số tiếp theo (0-9)
     */
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;

        if (value.empty()) {
            auto error = FlightNumberError::EMPTY_FLIGHT_NUMBER;
            result.addError(
                "flightNumber", 
                FlightNumberErrorHelper::getMessage(error),
                FlightNumberErrorHelper::toString(error)
            );
            return result;
        }

        if (value.length() < 3 || value.length() > 6) {
            auto error = FlightNumberError::INVALID_LENGTH;
            result.addError(
                "flightNumber",
                FlightNumberErrorHelper::getMessage(error),
                FlightNumberErrorHelper::toString(error)
            );
            return result;
        }

        // Format: 2 letters followed by 1-4 digits
        std::regex pattern("^[A-Z]{2}[1-9][0-9]{0,3}$");
        if (!std::regex_match(value, pattern)) {
            auto error = FlightNumberError::INVALID_FORMAT;
            result.addError(
                "flightNumber",
                FlightNumberErrorHelper::getMessage(error),
                FlightNumberErrorHelper::toString(error)
            );
            return result;
        }

        return result;
    }
};

#endif