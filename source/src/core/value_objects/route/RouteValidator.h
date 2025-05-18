#ifndef ROUTE_VALIDATOR_H
#define ROUTE_VALIDATOR_H

#include <string>
#include <regex>
#include <tuple>
#include "../../exceptions/ValidationResult.h"
#include "RouteError.h"

class RouteValidator {
private:
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    static bool isValidAirportCode(const std::string& code) {
        // Check if it's a valid 3-letter IATA code
        if (code.length() != 3) return false;
        std::regex codePattern("^[A-Z]{3}$");
        return std::regex_match(code, codePattern);
    }

public:
    // Validate combined string format (ORIGIN(ORIGIN_CODE)-DESTINATION(DESTINATION_CODE))
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            RouteErrorHelper::addError(result, RouteError::EMPTY_ROUTE);
            return result;
        }

        // Validate format and extract components using regex
        std::regex pattern("^([A-Za-z0-9 ]+)\\(([A-Za-z0-9 ]+)\\)-([A-Za-z0-9 ]+)\\(([A-Za-z0-9 ]+)\\)$");
        std::smatch matches;
        
        if (!std::regex_match(value, matches, pattern)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_FORMAT);
            return result;
        }

        // matches[0] contains the full match
        // matches[1] contains the origin city name
        // matches[2] contains the origin code
        // matches[3] contains the destination city name
        // matches[4] contains the destination code

        std::string originCode = matches[2].str();
        std::string destinationCode = matches[4].str();

        // Validate airport codes
        if (!isValidAirportCode(originCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_ORIGIN_CODE);
        }

        if (!isValidAirportCode(destinationCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_DESTINATION_CODE);
        }

        // Check if origin and destination are the same
        if (originCode == destinationCode) {
            RouteErrorHelper::addError(result, RouteError::SAME_ORIGIN_DESTINATION);
        }
        
        return result;
    }

    // Validate separate components
    static ValidationResult validate(const std::tuple<std::string, std::string, std::string, std::string>& input) {
        ValidationResult result;
        const auto& [origin, originCode, destination, destinationCode] = input;
        
        // Check empty inputs
        if (isEmpty(origin) || isEmpty(originCode) || isEmpty(destination) || isEmpty(destinationCode)) {
            RouteErrorHelper::addError(result, RouteError::EMPTY_ROUTE);
            return result;
        }

        // Validate origin code
        if (!isValidAirportCode(originCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_ORIGIN_CODE);
        }

        // Validate destination code
        if (!isValidAirportCode(destinationCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_DESTINATION_CODE);
        }

        // Check if origin and destination are the same
        if (originCode == destinationCode) {
            RouteErrorHelper::addError(result, RouteError::SAME_ORIGIN_DESTINATION);
        }
        
        return result;
    }
};

#endif 