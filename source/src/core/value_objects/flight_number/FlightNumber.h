#ifndef FLIGHT_NUMBER_H
#define FLIGHT_NUMBER_H

#include <string>
#include "../../exceptions/Result.h"
#include "FlightNumberValidator.h"
#include <functional>

class FlightNumber {
private:
    std::string _value;

    explicit FlightNumber(std::string value) : _value(std::move(value)) {}

public:
    FlightNumber() = default;

    static Result<FlightNumber> create(const std::string& value) {
        auto validationResult = FlightNumberValidator::validate(value);
        if (!validationResult.isValid()) {
            return getValidationFailure<FlightNumber>(validationResult);
        }
        return Success(FlightNumber(value));
    }

    const std::string& value() const { return _value; }

    bool operator==(const FlightNumber& other) const {
        return _value == other._value;
    }

    bool operator!=(const FlightNumber& other) const {
        return !(*this == other);
    }

    std::string toString() const {
        return _value;
    }
};

namespace std {
    template<>
    struct hash<FlightNumber> {
        size_t operator()(const FlightNumber& flightNumber) const {
            return hash<string>()(flightNumber.toString());
        }
    };
}

#endif