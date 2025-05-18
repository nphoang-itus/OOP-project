#ifndef SCHEDULE_VALIDATOR_H
#define SCHEDULE_VALIDATOR_H

#include <string>
#include <regex>
#include <ctime>
#include "../../exceptions/ValidationResult.h"
#include "ScheduleError.h"
#include "ScheduleParser.h"

class ScheduleValidator {
private:
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    static bool isValidDateTime(const std::string& dateTime) {
        std::regex pattern("^\\d{4}-(?:0[1-9]|1[0-2])-(?:0[1-9]|[12]\\d|3[01]) (?:[01]\\d|2[0-3]):[0-5]\\d$");
        return std::regex_match(dateTime, pattern);
    }

    static bool isArrivalAfterDeparture(const std::tm& departure, const std::tm& arrival) {
        std::time_t depTime = std::mktime(const_cast<std::tm*>(&departure));
        std::time_t arrTime = std::mktime(const_cast<std::tm*>(&arrival));
        return arrTime > depTime;
    }

public:
    // Validate combined string format (YYYY-MM-DD HH:mm-YYYY-MM-DD HH:mm)
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            ScheduleErrorHelper::addError(result, ScheduleError::EMPTY_SCHEDULE);
            return result;
        }

        // Split into departure and arrival
        size_t dashPos = value.find('-');
        if (dashPos == std::string::npos) {
            ScheduleErrorHelper::addError(result, ScheduleError::INVALID_FORMAT);
            return result;
        }

        std::string departureStr = value.substr(0, dashPos);
        std::string arrivalStr = value.substr(dashPos + 1);

        // Validate format
        if (!isValidDateTime(departureStr)) {
            ScheduleErrorHelper::addError(result, ScheduleError::INVALID_DEPARTURE_TIME);
        }

        if (!isValidDateTime(arrivalStr)) {
            ScheduleErrorHelper::addError(result, ScheduleError::INVALID_ARRIVAL_TIME);
        }

        // Parse and validate times
        if (result.isValid()) {
            auto parsed = ScheduleParser::parse(value);
            if (parsed.has_value()) {
                const auto& [departure, arrival] = parsed.value();
                if (!isArrivalAfterDeparture(departure, arrival)) {
                    ScheduleErrorHelper::addError(result, ScheduleError::ARRIVAL_BEFORE_DEPARTURE);
                }
            }
        }
        
        return result;
    }

    // Validate separate departure and arrival times
    static ValidationResult validate(const std::pair<std::tm, std::tm>& input) {
        ValidationResult result;
        const auto& [departure, arrival] = input;
        
        if (!isArrivalAfterDeparture(departure, arrival)) {
            ScheduleErrorHelper::addError(result, ScheduleError::ARRIVAL_BEFORE_DEPARTURE);
        }
        
        return result;
    }
};

#endif 