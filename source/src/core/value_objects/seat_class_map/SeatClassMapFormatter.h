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
        // std::stringstream ss;
        // bool first = true;
        // for (const auto& [seatClass, count] : classMap) {
        //     if (!first) ss << ",";
        //     ss << seatClass.getName() << ":" << count;
        //     first = false;
        // }
        // return ss.str();

        std::vector<std::string> parts;
        for (const auto& [seatClass, count] : classMap) {
            parts.push_back(seatClass.getName() + ":" + std::to_string(count));
        }
        
        // Sắp xếp để đảm bảo kết quả nhất quán
        std::sort(parts.begin(), parts.end());
        
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result += ",";
            result += parts[i];
        }
        return result;
    }

    // Format SeatClassMap to string with custom separator
    static std::string toString(const std::unordered_map<SeatClass, int>& classMap, const std::string& separator) {
        // std::stringstream ss;
        // bool first = true;
        // for (const auto& [seatClass, count] : classMap) {
        //     if (!first) ss << separator;
        //     ss << seatClass.getName() << ":" << count;
        //     first = false;
        // }
        // return ss.str();
        std::vector<std::string> parts;
        for (const auto& [seatClass, count] : classMap) {
            parts.push_back(seatClass.getName() + ":" + std::to_string(count));
        }
        
        // Sắp xếp để đảm bảo kết quả nhất quán
        std::sort(parts.begin(), parts.end());
        
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result += separator;
            result += parts[i];
        }
        return result;
    }

    // Format SeatClassMap to string with custom formatting
    static std::string toString(const std::unordered_map<SeatClass, int>& classMap, 
                                const std::string& separator,
                                const std::string& keyValueSeparator) {
        std::vector<std::string> parts;
        for (const auto& [seatClass, count] : classMap) {
            parts.push_back(seatClass.getName() + keyValueSeparator + std::to_string(count));
        }
        
        // Sắp xếp để đảm bảo kết quả nhất quán
        std::sort(parts.begin(), parts.end());
        
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result += separator;
            result += parts[i];
        }
        return result;
    }
};

#endif 