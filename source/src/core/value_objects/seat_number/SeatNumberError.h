/**
 * @file SeatNumberError.h
 * @brief Định nghĩa các lỗi xác thực số ghế và tiện ích xử lý lỗi
 * @author Nguyễn Phúc Hoàng
 */

#ifndef SEAT_NUMBER_ERROR_H
#define SEAT_NUMBER_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

/**
 * @enum SeatNumberError
 * @brief Liệt kê các lỗi xác thực số ghế có thể xảy ra
 */
enum class SeatNumberError {
    EMPTY_SEAT_NUMBER,       ///< Số ghế rỗng
    INVALID_FORMAT,         ///< Định dạng số ghế không hợp lệ
    INVALID_CLASS_CODE,     ///< Mã hạng ghế không hợp lệ
    INVALID_SEQUENCE_NUMBER,///< Số thứ tự ghế không hợp lệ
    INVALID_SEAT_CLASS      ///< Hạng ghế không tồn tại trong bố trí máy bay
};

/**
 * @struct SeatNumberErrorHelper
 * @brief Struct hỗ trợ để quản lý thông báo lỗi và mã lỗi số ghế
 * 
 * Struct này cung cấp các phương thức tiện ích để chuyển đổi lỗi số ghế
 * thành biểu diễn chuỗi và thông báo thân thiện với người dùng.
 */
struct SeatNumberErrorHelper {
    /**
     * @brief Chuyển đổi SeatNumberError thành biểu diễn chuỗi
     * @param error Lỗi cần chuyển đổi
     * @return Biểu diễn chuỗi của mã lỗi
     */
    static std::string toString(SeatNumberError error) {
        switch (error) {
            case SeatNumberError::EMPTY_SEAT_NUMBER: return "EMPTY_SEAT_NUMBER";
            case SeatNumberError::INVALID_FORMAT: return "INVALID_FORMAT";
            case SeatNumberError::INVALID_CLASS_CODE: return "INVALID_CLASS_CODE";
            case SeatNumberError::INVALID_SEQUENCE_NUMBER: return "INVALID_SEQUENCE_NUMBER";
            case SeatNumberError::INVALID_SEAT_CLASS: return "INVALID_SEAT_CLASS";
            default: return "UNKNOWN_ERROR";
        }
    }

    /**
     * @brief Lấy thông báo dễ đọc cho SeatNumberError
     * @param error Lỗi cần lấy thông báo
     * @return Thông báo lỗi thân thiện với người dùng
     */
    static std::string getMessage(SeatNumberError error) {
        switch (error) {
            case SeatNumberError::EMPTY_SEAT_NUMBER: return "seatNumber: Seat number cannot be empty";
            case SeatNumberError::INVALID_FORMAT: return "seatNumber: Seat number must be in format 'CLASS_CODE[SEQUENCE_NUMBER]'";
            case SeatNumberError::INVALID_CLASS_CODE: return "seatNumber: Invalid seat class code";
            case SeatNumberError::INVALID_SEQUENCE_NUMBER: return "seatNumber: Invalid sequence number";
            case SeatNumberError::INVALID_SEAT_CLASS: return "seatNumber: Seat class not found in aircraft layout";
            default: return "seatNumber: Unknown error";
        }
    }

    /**
     * @brief Thêm lỗi số ghế vào kết quả xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     * @param error Lỗi cần thêm
     */
    static void addError(ValidationResult& result, SeatNumberError error) {
        result.addError(toString(error), getMessage(error));
    }
};

#endif