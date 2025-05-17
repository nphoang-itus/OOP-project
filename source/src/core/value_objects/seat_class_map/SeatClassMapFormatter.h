#ifndef SEAT_CLASS_MAP_FORMATTER_H
#define SEAT_CLASS_MAP_FORMATTER_H

#include <string>
#include <unordered_map>
#include <sstream>
#include "SeatClass.h"

class SeatClassMapFormatter {
public:
    // Format SeatClassMap to string
    static std::string toString(const std::unordered_map<SeatClass, int>& classMap) {
        std::stringstream ss;
        bool first = true;
        for (const auto& [seatClass, count] : classMap) {
            if (!first) ss << ",";
            ss << seatClassToString(seatClass) << ":" << count;
            first = false;
        }
        return ss.str();
    }

    // Format SeatClassMap to string with custom separator
    static std::string toString(const std::unordered_map<SeatClass, int>& classMap, const std::string& separator) {
        std::stringstream ss;
        bool first = true;
        for (const auto& [seatClass, count] : classMap) {
            if (!first) ss << separator;
            ss << seatClassToString(seatClass) << ":" << count;
            first = false;
        }
        return ss.str();
    }

    // Format SeatClassMap to string with custom formatting
    static std::string toString(const std::unordered_map<SeatClass, int>& classMap, 
                              const std::string& separator,
                              const std::string& keyValueSeparator) {
        std::stringstream ss;
        bool first = true;
        for (const auto& [seatClass, count] : classMap) {
            if (!first) ss << separator;
            ss << seatClassToString(seatClass) << keyValueSeparator << count;
            first = false;
        }
        return ss.str();
    }
};

#endif 