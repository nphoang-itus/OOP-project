/**
 * @file ScheduleFormatter.h
 * @brief Định nghĩa lớp ScheduleFormatter để định dạng lịch trình bay
 * @author Nhóm dự án OOP
 */

#ifndef SCHEDULE_FORMATTER_H
#define SCHEDULE_FORMATTER_H

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

/**
 * @class ScheduleFormatter
 * @brief Lớp tiện ích để định dạng lịch trình bay
 * 
 * Lớp này cung cấp chức năng định dạng thông tin lịch trình bay
 * thành biểu diễn chuỗi tiêu chuẩn.
 */
class ScheduleFormatter {
public:
    /**
     * @brief Định dạng thời gian khởi hành và thời gian đến thành chuỗi lịch trình tiêu chuẩn
     * @param departure Thời gian khởi hành
     * @param arrival Thời gian đến
     * @return Chuỗi đã định dạng theo "YYYY-MM-DD HH:mm|YYYY-MM-DD HH:mm"
     */
    static std::string toString(const std::tm& departure, const std::tm& arrival) {
        std::stringstream ss;
        
        // Format departure time
        ss << std::setfill('0') << std::setw(4) << (departure.tm_year + 1900) << "-"
           << std::setw(2) << (departure.tm_mon + 1) << "-"
           << std::setw(2) << departure.tm_mday << " "
           << std::setw(2) << departure.tm_hour << ":"
           << std::setw(2) << departure.tm_min;
        
        ss << "|";
        
        // Format arrival time
        ss << std::setfill('0') << std::setw(4) << (arrival.tm_year + 1900) << "-"
           << std::setw(2) << (arrival.tm_mon + 1) << "-"
           << std::setw(2) << arrival.tm_mday << " "
           << std::setw(2) << arrival.tm_hour << ":"
           << std::setw(2) << arrival.tm_min;
        
        return ss.str();
    }
};

#endif