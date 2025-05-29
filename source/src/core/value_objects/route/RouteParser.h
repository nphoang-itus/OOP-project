/**
 * @file RouteParser.h
 * @brief Định nghĩa lớp RouteParser để phân tích chuỗi tuyến đường bay
 * @author Nhóm dự án OOP
 */

#ifndef ROUTE_PARSER_H
#define ROUTE_PARSER_H

#include <string>
#include <tuple>
#include <optional>

/**
 * @class RouteParser
 * @brief Lớp tiện ích để phân tích chuỗi tuyến đường bay
 * 
 * Lớp này cung cấp chức năng phân tích chuỗi tuyến đường bay
 * có định dạng "ĐIỂM_XUẤT_PHÁT(MÃ)-ĐIỂM_ĐẾN(MÃ)" thành các thành phần riêng biệt.
 */
class RouteParser {
public:
    /**
     * @brief Phân tích chuỗi tuyến đường thành các thành phần
     * @param value Chuỗi có định dạng "ĐIỂM_XUẤT_PHÁT(MÃ_XUẤT_PHÁT)-ĐIỂM_ĐẾN(MÃ_ĐIỂM_ĐẾN)"
     * @return Optional tuple của (tên_xuất_phát, mã_xuất_phát, tên_đích, mã_đích) nếu thành công, nullopt nếu không
     */
    static std::optional<std::tuple<std::string, std::string, std::string, std::string>> parse(const std::string& value) {
        size_t dashPos = value.find('-');
        if (dashPos == std::string::npos) return std::nullopt;

        std::string originPart = value.substr(0, dashPos);
        std::string destPart = value.substr(dashPos + 1);

        // Parse origin
        size_t originCodeStart = originPart.find('(');
        size_t originCodeEnd = originPart.find(')');
        if (originCodeStart == std::string::npos || originCodeEnd == std::string::npos) return std::nullopt;
        std::string origin = originPart.substr(0, originCodeStart);
        std::string originCode = originPart.substr(originCodeStart + 1, 3);

        // Parse destination
        size_t destCodeStart = destPart.find('(');
        size_t destCodeEnd = destPart.find(')');
        if (destCodeStart == std::string::npos || destCodeEnd == std::string::npos) return std::nullopt;
        std::string destination = destPart.substr(0, destCodeStart);
        std::string destinationCode = destPart.substr(destCodeStart + 1, 3);

        return std::make_tuple(origin, originCode, destination, destinationCode);
    }
};

#endif