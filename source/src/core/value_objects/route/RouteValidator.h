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

        // Validate format
        std::regex pattern("^[A-Za-z ]+\\([A-Z]{3}\\)-[A-Za-z ]+\\([A-Z]{3}\\)$");
        if (!std::regex_match(value, pattern)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_FORMAT);
            return result;
        }

        // Extract and validate components
        size_t dashPos = value.find('-');
        if (dashPos != std::string::npos) {
            std::string originPart = value.substr(0, dashPos);
            std::string destPart = value.substr(dashPos + 1);

            // Extract origin code
            size_t originCodeStart = originPart.find('(');
            size_t originCodeEnd = originPart.find(')');
            if (originCodeStart != std::string::npos && originCodeEnd != std::string::npos) {
                std::string originCode = originPart.substr(originCodeStart + 1, 3);
                if (!isValidAirportCode(originCode)) {
                    RouteErrorHelper::addError(result, RouteError::INVALID_ORIGIN_CODE);
                }
            }

            // Extract destination code
            size_t destCodeStart = destPart.find('(');
            size_t destCodeEnd = destPart.find(')');
            if (destCodeStart != std::string::npos && destCodeEnd != std::string::npos) {
                std::string destCode = destPart.substr(destCodeStart + 1, 3);
                if (!isValidAirportCode(destCode)) {
                    RouteErrorHelper::addError(result, RouteError::INVALID_DESTINATION_CODE);
                }
            }
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