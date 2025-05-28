#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include <ctime>
#include "../../exceptions/ValidationResult.h"
#include "ScheduleValidator.h"
#include "ScheduleParser.h"
#include "ScheduleFormatter.h"
#include <functional>

class Schedule {
private:
    std::tm _departure;
    std::tm _arrival;
    
    Schedule(const std::tm& departure, const std::tm& arrival) 
        : _departure(departure), _arrival(arrival) {}

    template<typename InputType>
    static Result<Schedule> createInternal(const InputType& input) {
        auto validationResult = ScheduleValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<Schedule>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::pair<std::tm, std::tm>>) {
            const auto& [departure, arrival] = input;
            return Success(Schedule(departure, arrival));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = ScheduleParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<Schedule>(CoreError("Failed to parse schedule", "PARSE_ERROR"));
            }
            const auto& [departure, arrival] = parsed.value();
            return Success(Schedule(departure, arrival));
        }
        return Failure<Schedule>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    Schedule() = default;

    static Result<Schedule> create(const std::string& value) {
        return createInternal(value);    
    }

    static Result<Schedule> create(const std::string& departure, const std::string& arrival) {
        std::string combined = departure + "|" + arrival;
        return createInternal(combined);
    }

    static Result<Schedule> create(const std::tm& departure, const std::tm& arrival) {
        return createInternal(std::make_pair(departure, arrival));
    }

    const std::tm& getDeparture() const { return _departure; }
    const std::tm& getArrival() const { return _arrival; }

    std::string toString() const {
        return ScheduleFormatter::toString(_departure, _arrival);
    }

    bool operator==(const Schedule& other) const {
        return std::mktime(const_cast<std::tm*>(&_departure)) == std::mktime(const_cast<std::tm*>(&other._departure)) &&
               std::mktime(const_cast<std::tm*>(&_arrival)) == std::mktime(const_cast<std::tm*>(&other._arrival));
    }

    bool operator!=(const Schedule& other) const {
        return !(*this == other);
    }

    bool overlapsWith(const Schedule& other) const {
        auto thisDep = std::mktime(const_cast<std::tm*>(&_departure));
        auto thisArr = std::mktime(const_cast<std::tm*>(&_arrival));
        auto otherDep = std::mktime(const_cast<std::tm*>(&other._departure));
        auto otherArr = std::mktime(const_cast<std::tm*>(&other._arrival));

        return (thisDep <= otherArr && thisArr >= otherDep);
    }
};

namespace std {
    template<>
    struct hash<Schedule> {
        size_t operator()(const Schedule& schedule) const {
            return hash<string>()(schedule.toString());
        }
    };
}

#endif