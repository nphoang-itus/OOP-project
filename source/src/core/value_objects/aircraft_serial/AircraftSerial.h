#ifndef AIRCRAFT_SERIAL_H
#define AIRCRAFT_SERIAL_H

#include <string>
#include "../../exceptions/Result.h"
#include "AircraftSerialValidator.h"

class AircraftSerial {
private:
    std::string _value;

    explicit AircraftSerial(std::string value) : _value(std::move(value)) {}

public:
    static Result<AircraftSerial> create(const std::string& value) {
        auto validationResult = AircraftSerialValidator::validate(value);
        if (!validationResult.isValid()) {
            return getValidationFailure<AircraftSerial>(validationResult);
        }
        return Success(AircraftSerial(value));
    }

    const std::string& value() const { return _value; }
};

#endif 