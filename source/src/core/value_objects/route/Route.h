#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include "../../exceptions/ValidationResult.h"
#include "RouteValidator.h"
#include "RouteParser.h"
#include "RouteFormatter.h"

class Route {
private:
    std::string _origin;
    std::string _originCode;
    std::string _destination;
    std::string _destinationCode;
    
    Route(const std::string& origin, const std::string& originCode,
          const std::string& destination, const std::string& destinationCode) 
        : _origin(origin), _originCode(originCode),
          _destination(destination), _destinationCode(destinationCode) {}

    template<typename InputType>
    static Result<Route> createInternal(const InputType& input) {
        auto validationResult = RouteValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<Route>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::tuple<std::string, std::string, std::string, std::string>>) {
            const auto& [origin, originCode, destination, destinationCode] = input;
            return Success(Route(origin, originCode, destination, destinationCode));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = RouteParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<Route>(CoreError("Failed to parse route", "PARSE_ERROR"));
            }
            const auto& [origin, originCode, destination, destinationCode] = parsed.value();
            return Success(Route(origin, originCode, destination, destinationCode));
        }
        return Failure<Route>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    static Result<Route> create(const std::string& value) {
        return createInternal(value);    
    }

    static Result<Route> create(const std::string& origin, const std::string& originCode,
                              const std::string& destination, const std::string& destinationCode) {
        return createInternal(std::make_tuple(origin, originCode, destination, destinationCode));
    }

    const std::string& getOrigin() const { return _origin; }
    const std::string& getOriginCode() const { return _originCode; }
    const std::string& getDestination() const { return _destination; }
    const std::string& getDestinationCode() const { return _destinationCode; }

    std::string toString() const {
        return RouteFormatter::toString(_origin, _originCode, _destination, _destinationCode);
    }

    bool operator==(const Route& other) const {
        return _originCode == other._originCode && 
               _destinationCode == other._destinationCode;
    }

    bool operator!=(const Route& other) const {
        return !(*this == other);
    }
};

#endif