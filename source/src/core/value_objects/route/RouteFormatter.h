/**
 * @file RouteFormatter.h
 * @brief Định nghĩa lớp RouteFormatter để định dạng tuyến đường bay
 * @author Nguyễn Phúc Hoàng
 */

#ifndef ROUTE_FORMATTER_H
#define ROUTE_FORMATTER_H

#include <string>

/**
 * @class RouteFormatter
 * @brief Lớp tiện ích để định dạng tuyến đường bay
 * 
 * Lớp này cung cấp chức năng định dạng thông tin tuyến đường bay
 * thành biểu diễn chuỗi tiêu chuẩn.
 */
class RouteFormatter {
public:
    /**
     * @brief Định dạng thông tin tuyến đường thành chuỗi tiêu chuẩn
     * @param origin Tên điểm xuất phát
     * @param originCode Mã sân bay điểm xuất phát
     * @param destination Tên điểm đến
     * @param destinationCode Mã sân bay điểm đến
     * @return Chuỗi đã định dạng theo "ĐIỂM_XUẤT_PHÁT(MÃ_XUẤT_PHÁT)-ĐIỂM_ĐẾN(MÃ_ĐIỂM_ĐẾN)"
     */
    static std::string toString(const std::string& origin, const std::string& originCode,
                              const std::string& destination, const std::string& destinationCode) {
        return origin + "(" + originCode + ")-" + destination + "(" + destinationCode + ")";
    }
};

#endif