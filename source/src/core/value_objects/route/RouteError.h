#ifndef ROUTE_ERROR_H
#define ROUTE_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

enum class RouteError {
    EMPTY_ROUTE,
    INVALID_FORMAT,
    INVALID_ORIGIN_CODE,
    INVALID_DESTINATION_CODE,
    SAME_ORIGIN_DESTINATION
};

struct RouteErrorHelper {
    static std::string toString(RouteError error) {
        switch (error) {
            case RouteError::EMPTY_ROUTE: return "EMPTY_ROUTE";
            case RouteError::INVALID_FORMAT: return "INVALID_FORMAT";
            case RouteError::INVALID_ORIGIN_CODE: return "INVALID_ORIGIN_CODE";
            case RouteError::INVALID_DESTINATION_CODE: return "INVALID_DESTINATION_CODE";
            case RouteError::SAME_ORIGIN_DESTINATION: return "SAME_ORIGIN_DESTINATION";
            default: return "UNKNOWN_ERROR";
        }
    }

    static std::string getMessage(RouteError error) {
        switch (error) {
            case RouteError::EMPTY_ROUTE: return "Route cannot be empty";
            case RouteError::INVALID_FORMAT: return "Route must be in format 'ORIGIN(ORIGIN_CODE)-DESTINATION(DESTINATION_CODE)'";
            case RouteError::INVALID_ORIGIN_CODE: return "Origin code must be a valid 3-letter IATA code";
            case RouteError::INVALID_DESTINATION_CODE: return "Destination code must be a valid 3-letter IATA code";
            case RouteError::SAME_ORIGIN_DESTINATION: return "Origin and destination cannot be the same";
            default: return "Unknown error";
        }
    }

    static void addError(ValidationResult& result, RouteError error) {
        result.addError("route", getMessage(error), toString(error));
    }
};

#endif 