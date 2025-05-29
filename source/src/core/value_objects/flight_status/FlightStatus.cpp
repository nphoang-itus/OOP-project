/**
 * @file FlightStatus.cpp
 * @brief Implementation của FlightStatusUtil cho chuyển đổi trạng thái chuyến bay
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#include "FlightStatus.h"
#include <stdexcept>

std::string FlightStatusUtil::toString(FlightStatus status) {
    switch (status) {
        case FlightStatus::SCHEDULED: return "SCHEDULED";
        case FlightStatus::BOARDING: return "BOARDING";
        case FlightStatus::DEPARTED: return "DEPARTED";
        case FlightStatus::IN_FLIGHT: return "IN_FLIGHT";
        case FlightStatus::LANDED: return "LANDED";
        case FlightStatus::CANCELLED: return "CANCELLED";
        case FlightStatus::DELAYED: return "DELAYED";
        default: return "UNKNOWN";
    }
}

FlightStatus FlightStatusUtil::fromString(const std::string& str) {
    if (str == "SCHEDULED") return FlightStatus::SCHEDULED;
    if (str == "BOARDING") return FlightStatus::BOARDING;
    if (str == "DEPARTED") return FlightStatus::DEPARTED;
    if (str == "IN_FLIGHT") return FlightStatus::IN_FLIGHT;
    if (str == "LANDED") return FlightStatus::LANDED;
    if (str == "CANCELLED") return FlightStatus::CANCELLED;
    if (str == "DELAYED") return FlightStatus::DELAYED;
    throw std::invalid_argument("Unknown flight status: " + str);
}

std::string FlightStatusUtil::toVietnamese(FlightStatus status) {
    switch (status) {
        case FlightStatus::SCHEDULED: return "Đã lên lịch";
        case FlightStatus::BOARDING: return "Đang lên máy bay";
        case FlightStatus::DEPARTED: return "Đã cất cánh";
        case FlightStatus::IN_FLIGHT: return "Đang bay";
        case FlightStatus::LANDED: return "Đã hạ cánh";
        case FlightStatus::CANCELLED: return "Đã hủy";
        case FlightStatus::DELAYED: return "Bị trễ";
        default: return "Không xác định";
    }
}