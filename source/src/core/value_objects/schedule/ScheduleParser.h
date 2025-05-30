/**
 * @file ScheduleParser.h
 * @brief Định nghĩa lớp ScheduleParser để phân tích chuỗi lịch trình bay
 * @author Nguyễn Phúc Hoàng
 */

#ifndef SCHEDULE_PARSER_H
#define SCHEDULE_PARSER_H

#include <string>
#include <tuple>
#include <optional>
#include <ctime>

/**
 * @class ScheduleParser
 * @brief Lớp tiện ích để phân tích chuỗi lịch trình bay
 * 
 * Lớp này cung cấp chức năng phân tích chuỗi lịch trình bay
 * có định dạng "YYYY-MM-DD HH:mm|YYYY-MM-DD HH:mm" thành các thành phần thời gian riêng biệt.
 */
class ScheduleParser {
public:
    /**
     * @brief Phân tích chuỗi lịch trình thành thời gian khởi hành và thời gian đến
     * @param value Chuỗi có định dạng "YYYY-MM-DD HH:mm|YYYY-MM-DD HH:mm"
     * @return Optional tuple của (thời_gian_khởi_hành, thời_gian_đến) nếu thành công, nullopt nếu không
     */
    static std::optional<std::tuple<std::tm, std::tm>> parse(const std::string& value) {
        size_t dashPos = value.find('|');
        if (dashPos == std::string::npos) return std::nullopt;

        std::string departureStr = value.substr(0, dashPos);
        std::string arrivalStr = value.substr(dashPos + 1);

        std::tm departure = {};
        std::tm arrival = {};

        // Parse departure time
        if (sscanf(departureStr.c_str(), "%d-%d-%d %d:%d",
                   &departure.tm_year, &departure.tm_mon, &departure.tm_mday,
                   &departure.tm_hour, &departure.tm_min) != 5) {
            return std::nullopt;
        }
        departure.tm_year -= 1900;  // Adjust year
        departure.tm_mon -= 1;      // Adjust month

        // Parse arrival time
        if (sscanf(arrivalStr.c_str(), "%d-%d-%d %d:%d",
                   &arrival.tm_year, &arrival.tm_mon, &arrival.tm_mday,
                   &arrival.tm_hour, &arrival.tm_min) != 5) {
            return std::nullopt;
        }
        arrival.tm_year -= 1900;    // Adjust year
        arrival.tm_mon -= 1;        // Adjust month

        return std::make_tuple(departure, arrival);
    }
};

#endif