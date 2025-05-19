#ifndef SCHEDULE_PARSER_H
#define SCHEDULE_PARSER_H

#include <string>
#include <tuple>
#include <optional>
#include <ctime>

class ScheduleParser {
public:
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