#ifndef SEAT_CLASS_H
#define SEAT_CLASS_H

#include <string>
#include <optional>

enum class SeatClass {
    ECONOMY,
    BUSINESS,
    FIRST
};

// Convert SeatClass to string
inline std::string seatClassToString(SeatClass seatClass) {
    switch (seatClass) {
        case SeatClass::ECONOMY:
            return "ECONOMY";
        case SeatClass::BUSINESS:
            return "BUSINESS";
        case SeatClass::FIRST:
            return "FIRST";
        default:
            return "UNKNOWN";
    }
}

// Convert string to SeatClass
inline std::optional<SeatClass> stringToSeatClass(const std::string& str) {
    if (str == "ECONOMY" or str == "economy") return SeatClass::ECONOMY;
    if (str == "BUSINESS" or str == "business") return SeatClass::BUSINESS;
    if (str == "FIRST" or str == "first") return SeatClass::FIRST;
    return std::nullopt;
}

#endif 