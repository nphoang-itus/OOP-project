#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

inline std::string convertTimeToString(const std::tm& time) {
    std::ostringstream oss;
    oss << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline std::tm convertStringToTime(const std::string& string) {
    std::tm time = {};
    std::istringstream iss(string);
    iss >> std::get_time(&time, "%Y-%m-%d %H:%M:%S");
    if (iss.fail()) {
        throw std::runtime_error("Failed to parse time string: " + string);
    }
    return time;
}

#endif