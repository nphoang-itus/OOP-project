/**
 * @file RouteError.h
 * @brief Định nghĩa các lỗi xác thực tuyến đường bay và tiện ích xử lý lỗi
 * @author Nguyễn Phúc Hoàng
 */

#ifndef ROUTE_ERROR_H
#define ROUTE_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

/**
 * @enum RouteError
 * @brief Liệt kê các lỗi xác thực tuyến đường bay có thể xảy ra
 */
enum class RouteError {
    EMPTY_ROUTE,                ///< Tuyến đường rỗng
    INVALID_FORMAT,            ///< Định dạng tuyến đường không hợp lệ
    INVALID_ORIGIN_CODE,       ///< Mã sân bay xuất phát không hợp lệ
    INVALID_DESTINATION_CODE,  ///< Mã sân bay đích không hợp lệ
    SAME_ORIGIN_DESTINATION    ///< Điểm xuất phát và điểm đến giống nhau
};

/**
 * @struct RouteErrorHelper
 * @brief Struct hỗ trợ để quản lý thông báo lỗi và mã lỗi tuyến đường bay
 * 
 * Struct này cung cấp các phương thức tiện ích để chuyển đổi lỗi tuyến đường bay
 * thành biểu diễn chuỗi và thông báo thân thiện với người dùng.
 */
struct RouteErrorHelper {
    /**
     * @brief Chuyển đổi RouteError thành biểu diễn chuỗi
     * @param error Lỗi cần chuyển đổi
     * @return Biểu diễn chuỗi của mã lỗi
     */
    static std::string toString(RouteError error) {
        switch (error) {
            case RouteError::EMPTY_ROUTE: return "EMPTY_ROUTE";
            case RouteError::INVALID_FORMAT: return "INVALID_FORMAT";
            case RouteError::INVALID_ORIGIN_CODE: return "INVALID_ORIGIN_CODE";
            case RouteError::INVALID_DESTINATION_CODE: return "INVALID_DESTINATION_CODE";
            case RouteError::SAME_ORIGIN_DESTINATION: return "SAME_ORIGIN_DESTINATION";
            default: return "UNKNOWN_ERROR";
        }
    }

    /**
     * @brief Lấy thông báo dễ đọc cho RouteError
     * @param error Lỗi cần lấy thông báo
     * @return Thông báo lỗi thân thiện với người dùng
     */
    static std::string getMessage(RouteError error) {
        switch (error) {
            case RouteError::EMPTY_ROUTE: return "Route cannot be empty";
            case RouteError::INVALID_FORMAT: return "Route must be in format 'ORIGIN(ORIGIN_CODE)-DESTINATION(DESTINATION_CODE)'";
            case RouteError::INVALID_ORIGIN_CODE: return "Origin code must be a valid 3-letter IATA code";
            case RouteError::INVALID_DESTINATION_CODE: return "Destination code must be a valid 3-letter IATA code";
            case RouteError::SAME_ORIGIN_DESTINATION: return "Origin and destination cannot be the same";
            default: return "Unknown error";
        }
    }

    /**
     * @brief Thêm lỗi tuyến đường bay vào kết quả xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     * @param error Lỗi cần thêm
     */
    static void addError(ValidationResult& result, RouteError error) {
        result.addError("route", getMessage(error), toString(error));
    }
};

#endif