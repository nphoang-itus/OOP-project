/**
 * @file FlightStatus.h
 * @brief Enum và utility cho trạng thái chuyến bay trong hệ thống quản lý chuyến bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef FLIGHT_STATUS_H
#define FLIGHT_STATUS_H

#include <string>

/**
 * @brief Enum định nghĩa các trạng thái có thể có của chuyến bay
 * 
 * FlightStatus đại diện cho vòng đời của một chuyến bay từ lúc được lên lịch
 * cho đến khi hoàn thành hoặc bị hủy. Các trạng thái này được sử dụng để
 * kiểm soát logic nghiệp vụ và xác định các thao tác được phép thực hiện.
 */
enum class FlightStatus {
    SCHEDULED,      ///< Đã lên lịch - Chuyến bay đã được tạo và lên lịch
    BOARDING,       ///< Đang lên máy bay - Hành khách có thể check-in và lên máy bay
    DEPARTED,       ///< Đã cất cánh - Chuyến bay đã rời khỏi sân bay xuất phát
    IN_FLIGHT,      ///< Đang bay - Chuyến bay đang trên không
    LANDED,         ///< Đã hạ cánh - Chuyến bay đã đến sân bay đích
    CANCELLED,      ///< Đã hủy - Chuyến bay bị hủy bỏ
    DELAYED         ///< Bị trễ - Chuyến bay bị hoãn so với lịch trình ban đầu
};

/**
 * @brief Lớp utility cung cấp các phương thức chuyển đổi cho FlightStatus
 * 
 * FlightStatusUtil cung cấp các phương thức để chuyển đổi giữa enum FlightStatus
 * và các biểu diễn chuỗi khác nhau (tiếng Anh và tiếng Việt) để hiển thị
 * và lưu trữ dữ liệu.
 */
class FlightStatusUtil {
public:
    /**
     * @brief Chuyển đổi FlightStatus thành chuỗi tiếng Anh
     * @param status Trạng thái chuyến bay cần chuyển đổi
     * @return Chuỗi tiếng Anh đại diện cho trạng thái (ví dụ: "SCHEDULED", "BOARDING")
     */
    static std::string toString(FlightStatus status);

    /**
     * @brief Chuyển đổi chuỗi tiếng Anh thành FlightStatus
     * @param str Chuỗi tiếng Anh đại diện cho trạng thái
     * @return FlightStatus tương ứng
     * @throw std::invalid_argument nếu chuỗi không hợp lệ
     */
    static FlightStatus fromString(const std::string& str);

    /**
     * @brief Chuyển đổi FlightStatus thành chuỗi tiếng Việt
     * @param status Trạng thái chuyến bay cần chuyển đổi
     * @return Chuỗi tiếng Việt đại diện cho trạng thái (ví dụ: "Đã lên lịch", "Đang lên máy bay")
     */
    static std::string toVietnamese(FlightStatus status);
};

#endif