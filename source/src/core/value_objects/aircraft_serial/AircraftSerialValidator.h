#ifndef AIRCRAFT_SERIAL_VALIDATOR_H
#define AIRCRAFT_SERIAL_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"

enum class AircraftSerialError {
    EMPTY_AIRCRAFT_SERIAL,
    INVALID_LENGTH,
    INVALID_FORMAT
};

struct AircraftSerialErrorHelper {
    static std::string toString(AircraftSerialError error) {
        switch (error) {
            case AircraftSerialError::EMPTY_AIRCRAFT_SERIAL:
                return "EMPTY_AIRCRAFT_SERIAL";
            case AircraftSerialError::INVALID_LENGTH:
                return "INVALID_LENGTH";
            case AircraftSerialError::INVALID_FORMAT:
                return "INVALID_FORMAT";
            default:
                return "UNKNOWN_ERROR";
        }
    }

    static std::string getMessage(AircraftSerialError error) {
        switch (error) {
            case AircraftSerialError::EMPTY_AIRCRAFT_SERIAL:
                return "Aircraft serial cannot be empty";
            case AircraftSerialError::INVALID_LENGTH:
                return "Aircraft serial must be between 3 and 10 characters";
            case AircraftSerialError::INVALID_FORMAT:
                return "Aircraft serial must start with 2-3 letters followed by 1-7 digits";
            default:
                return "Unknown error";
        }
    }
};

class AircraftSerialValidator {
public:
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;

        if (value.empty()) {
            auto error = AircraftSerialError::EMPTY_AIRCRAFT_SERIAL;
            result.addError(
                "aircraftSerial", 
                AircraftSerialErrorHelper::getMessage(error),
                AircraftSerialErrorHelper::toString(error)
            );
            return result;
        }

        if (value.length() < 3 || value.length() > 10) {
            auto error = AircraftSerialError::INVALID_LENGTH;
            result.addError(
                "aircraftSerial",
                AircraftSerialErrorHelper::getMessage(error),
                AircraftSerialErrorHelper::toString(error)
            );
            return result;
        }

        // Format: 2-3 letters followed by 1-7 digits
        std::regex pattern("^[A-Z]{2,3}[0-9]{1,7}$");
        if (!std::regex_match(value, pattern)) {
            auto error = AircraftSerialError::INVALID_FORMAT;
            result.addError(
                "aircraftSerial",
                AircraftSerialErrorHelper::getMessage(error),
                AircraftSerialErrorHelper::toString(error)
            );
            return result;
        }

        return result;
    }
};

#endif 