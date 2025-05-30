/**
 * @file ScheduleError.h
 * @brief Định nghĩa các lỗi xác thực lịch trình bay và tiện ích xử lý lỗi
 * @author Nguyễn Phúc Hoàng
 */

#ifndef SCHEDULE_ERROR_H
#define SCHEDULE_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

/**
 * @enum ScheduleError
 * @brief Liệt kê các lỗi xác thực lịch trình bay có thể xảy ra
 */
enum class ScheduleError {
    EMPTY_SCHEDULE,            ///< Lịch trình rỗng
    INVALID_FORMAT,           ///< Định dạng lịch trình không hợp lệ
    INVALID_DEPARTURE_TIME,   ///< Thời gian khởi hành không hợp lệ
    INVALID_ARRIVAL_TIME,     ///< Thời gian đến không hợp lệ
    ARRIVAL_BEFORE_DEPARTURE  ///< Thời gian đến trước thời gian khởi hành
};

/**
 * @struct ScheduleErrorHelper
 * @brief Struct hỗ trợ để quản lý thông báo lỗi và mã lỗi lịch trình bay
 * 
 * Struct này cung cấp các phương thức tiện ích để chuyển đổi lỗi lịch trình bay
 * thành biểu diễn chuỗi và thông báo thân thiện với người dùng.
 */
struct ScheduleErrorHelper {
    /**
     * @brief Chuyển đổi ScheduleError thành biểu diễn chuỗi
     * @param error Lỗi cần chuyển đổi
     * @return Biểu diễn chuỗi của mã lỗi
     */
    static std::string toString(ScheduleError error) {
        switch (error) {
            case ScheduleError::EMPTY_SCHEDULE: return "EMPTY_SCHEDULE";
            case ScheduleError::INVALID_FORMAT: return "INVALID_FORMAT";
            case ScheduleError::INVALID_DEPARTURE_TIME: return "INVALID_DEPARTURE_TIME";
            case ScheduleError::INVALID_ARRIVAL_TIME: return "INVALID_ARRIVAL_TIME";
            case ScheduleError::ARRIVAL_BEFORE_DEPARTURE: return "ARRIVAL_BEFORE_DEPARTURE";
            default: return "UNKNOWN_ERROR";
        }
    }

    /**
     * @brief Lấy thông báo dễ đọc cho ScheduleError
     * @param error Lỗi cần lấy thông báo
     * @return Thông báo lỗi thân thiện với người dùng
     */
    static std::string getMessage(ScheduleError error) {
        switch (error) {
            case ScheduleError::EMPTY_SCHEDULE: return "Schedule cannot be empty";
            case ScheduleError::INVALID_FORMAT: return "Schedule must be in format 'YYYY-MM-DD HH:mm'";
            case ScheduleError::INVALID_DEPARTURE_TIME: return "Invalid departure time";
            case ScheduleError::INVALID_ARRIVAL_TIME: return "Invalid arrival time";
            case ScheduleError::ARRIVAL_BEFORE_DEPARTURE: return "Arrival time must be after departure time";
            default: return "Unknown error";
        }
    }

    /**
     * @brief Thêm lỗi lịch trình bay vào kết quả xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     * @param error Lỗi cần thêm
     */
    static void addError(ValidationResult& result, ScheduleError error) {
        result.addError(
            "schedule",
            getMessage(error),
            toString(error)
        );
    }
};

#endif