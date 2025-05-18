#ifndef SCHEDULE_ERROR_H
#define SCHEDULE_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

enum class ScheduleError {
    EMPTY_SCHEDULE,
    INVALID_FORMAT,
    INVALID_DEPARTURE_TIME,
    INVALID_ARRIVAL_TIME,
    ARRIVAL_BEFORE_DEPARTURE
};

struct ScheduleErrorHelper {
    static std::string toString(ScheduleError error) {
        switch (error) {
            case ScheduleError::EMPTY_SCHEDULE: return "EMPTY_SCHEDULE";
            case ScheduleError::INVALID_FORMAT: return "INVALID_FORMAT";
            case ScheduleError::INVALID_DEPARTURE_TIME: return "INVALID_DEPARTURE_TIME";
            case ScheduleError::INVALID_ARRIVAL_TIME: return "INVALID_ARRIVAL_TIME";
            case ScheduleError::ARRIVAL_BEFORE_DEPARTURE: return "ARRIVAL_BEFORE_DEPARTURE";
            default: return "UNKNOWN_ERROR";
        }
    }

    static std::string getMessage(ScheduleError error) {
        switch (error) {
            case ScheduleError::EMPTY_SCHEDULE: return "Schedule cannot be empty";
            case ScheduleError::INVALID_FORMAT: return "Schedule must be in format 'YYYY-MM-DD HH:mm'";
            case ScheduleError::INVALID_DEPARTURE_TIME: return "Invalid departure time";
            case ScheduleError::INVALID_ARRIVAL_TIME: return "Invalid arrival time";
            case ScheduleError::ARRIVAL_BEFORE_DEPARTURE: return "Arrival time must be after departure time";
            default: return "Unknown error";
        }
    }

    static void addError(ValidationResult& result, ScheduleError error) {
        result.addError(
            "schedule",
            getMessage(error),
            toString(error)
        );
    }
};

#endif 