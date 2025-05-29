/**
 * @file SeatClassMapFormatter.h
 * @brief Định nghĩa lớp SeatClassMapFormatter để định dạng bản đồ hạng ghế
 * @author Nhóm dự án OOP
 */

#ifndef SEAT_CLASS_MAP_FORMATTER_H
#define SEAT_CLASS_MAP_FORMATTER_H

#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include "SeatClass.h"

/**
 * @class SeatClassMapFormatter
 * @brief Lớp tiện ích để định dạng bản đồ hạng ghế
 * 
 * Lớp này cung cấp các phương thức để định dạng bản đồ hạng ghế
 * thành biểu diễn chuỗi với các tùy chọn định dạng khác nhau.
 */
class SeatClassMapFormatter {
public:
    /**
     * @brief Định dạng bản đồ hạng ghế thành chuỗi mặc định
     * @param classMap Map chứa hạng ghế và số lượng
     * @return Chuỗi đã định dạng theo "TÊN_HẠNG:SỐ_LƯỢNG,TÊN_HẠNG:SỐ_LƯỢNG,..."
     */
    static std::string toString(const std::unordered_map<SeatClass, int>& classMap) {
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

    /**
     * @brief Định dạng bản đồ hạng ghế thành chuỗi với dấu phân cách tùy chỉnh
     * @param classMap Map chứa hạng ghế và số lượng
     * @param separator Dấu phân cách giữa các mục
     * @return Chuỗi đã định dạng với dấu phân cách tùy chỉnh
     */
    static std::string toString(const std::unordered_map<SeatClass, int>& classMap, const std::string& separator) {
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

    /**
     * @brief Định dạng bản đồ hạng ghế thành chuỗi với định dạng tùy chỉnh
     * @param classMap Map chứa hạng ghế và số lượng
     * @param separator Dấu phân cách giữa các mục
     * @param keyValueSeparator Dấu phân cách giữa tên hạng và số lượng
     * @return Chuỗi đã định dạng với các dấu phân cách tùy chỉnh
     */
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