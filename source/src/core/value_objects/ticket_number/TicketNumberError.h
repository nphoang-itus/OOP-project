/**
 * @file TicketNumberError.h
 * @brief Định nghĩa các loại lỗi và helper cho validation số vé máy bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef TICKET_NUMBER_ERROR_H
#define TICKET_NUMBER_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

/**
 * @brief Enum định nghĩa các loại lỗi validation có thể xảy ra với số vé máy bay
 */
enum class TicketNumberError {
    EMPTY_TICKET_NUMBER,    ///< Số vé rỗng
    INVALID_FORMAT          ///< Định dạng số vé không đúng chuẩn
};

/**
 * @brief Helper struct cung cấp thông tin chi tiết về các lỗi TicketNumber
 * 
 * Struct này chuyển đổi enum error thành chuỗi mã lỗi và thông báo
 * có thể đọc được để hỗ trợ debugging và hiển thị lỗi cho người dùng.
 */
struct TicketNumberErrorHelper {
    /**
     * @brief Chuyển đổi mã lỗi thành chuỗi để lưu trữ
     * @param error Mã lỗi TicketNumberError
     * @return Chuỗi đại diện cho mã lỗi
     */
    static std::string toString(TicketNumberError error) {
        switch (error) {
            case TicketNumberError::EMPTY_TICKET_NUMBER: return "EMPTY_TICKET_NUMBER";
            case TicketNumberError::INVALID_FORMAT: return "INVALID_FORMAT";
            default: return "UNKNOWN_ERROR";
        }
    }

    /**
     * @brief Lấy thông báo lỗi có thể đọc được
     * @param error Mã lỗi TicketNumberError
     * @return Thông báo lỗi bằng tiếng Anh cho người dùng
     */
    static std::string getMessage(TicketNumberError error) {
        switch (error) {
            case TicketNumberError::EMPTY_TICKET_NUMBER: return "Ticket number cannot be empty";
            case TicketNumberError::INVALID_FORMAT: return "Ticket number must be in format 'MCB-YYYYMMDD-XXXX'";
            default: return "Unknown error";
        }
    }

    /**
     * @brief Thêm lỗi vào ValidationResult
     * @param result ValidationResult để thêm lỗi vào
     * @param error Mã lỗi cần thêm
     */
    static void addError(ValidationResult& result, TicketNumberError error) {
        result.addError(
            "ticketNumber",
            getMessage(error),
            toString(error)
        );
    }
};

#endif