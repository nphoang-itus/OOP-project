#ifndef FLIGHT_NUMBER_VALIDATOR_H
#define FLIGHT_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"

enum class FlightNumberError {
    EMPTY_FLIGHT_NUMBER,
    INVALID_LENGTH,
    INVALID_FORMAT
};

struct FlightNumberErrorHelper {
    static std::string toString(FlightNumberError error) {
        switch (error) {
            case FlightNumberError::EMPTY_FLIGHT_NUMBER:
                return "EMPTY_FLIGHT_NUMBER";
            case FlightNumberError::INVALID_LENGTH:
                return "INVALID_LENGTH";
            case FlightNumberError::INVALID_FORMAT:
                return "INVALID_FORMAT";
            default:
                return "UNKNOWN_ERROR";
        }
    }

    static std::string getMessage(FlightNumberError error) {
        switch (error) {
            case FlightNumberError::EMPTY_FLIGHT_NUMBER:
                return "Flight number cannot be empty";
            case FlightNumberError::INVALID_LENGTH:
                return "Flight number must be between 3 and 6 characters";
            case FlightNumberError::INVALID_FORMAT:
                return "Flight number must start with 2 letters followed by 1-4 digits";
            default:
                return "Unknown error";
        }
    }
};

class FlightNumberValidator {
public:
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;

        if (value.empty()) {
            auto error = FlightNumberError::EMPTY_FLIGHT_NUMBER;
            result.addError(
                "flightNumber", 
                FlightNumberErrorHelper::getMessage(error),
                FlightNumberErrorHelper::toString(error)
            );
            return result;
        }

        if (value.length() < 3 || value.length() > 6) {
            auto error = FlightNumberError::INVALID_LENGTH;
            result.addError(
                "flightNumber",
                FlightNumberErrorHelper::getMessage(error),
                FlightNumberErrorHelper::toString(error)
            );
            return result;
        }

        // Format: 2 letters followed by 1-4 digits
        std::regex pattern("^[A-Z]{2}[1-9][0-9]{0,3}$");
        if (!std::regex_match(value, pattern)) {
            auto error = FlightNumberError::INVALID_FORMAT;
            result.addError(
                "flightNumber",
                FlightNumberErrorHelper::getMessage(error),
                FlightNumberErrorHelper::toString(error)
            );
            return result;
        }

        return result;
    }
};

#endif 