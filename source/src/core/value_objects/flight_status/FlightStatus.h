// core/enums/FlightStatus.h
#ifndef FLIGHT_STATUS_H
#define FLIGHT_STATUS_H

#include <string>

/**
 * @brief Enum cho trạng thái chuyến bay
 */
enum class FlightStatus {
    SCHEDULED,      // Đã lên lịch
    BOARDING,       // Đang lên máy bay
    DEPARTED,       // Đã cất cánh
    IN_FLIGHT,      // Đang bay
    LANDED,         // Đã hạ cánh
    CANCELLED,      // Đã hủy
    DELAYED         // Bị trễ
};

class FlightStatusUtil {
public:
    static std::string toString(FlightStatus status);
    static FlightStatus fromString(const std::string& str);
    static std::string toVietnamese(FlightStatus status);
};

#endif