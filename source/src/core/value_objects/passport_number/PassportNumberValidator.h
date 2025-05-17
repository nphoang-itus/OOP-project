#ifndef PASSPORT_NUMBER_VALIDATOR_H
#define PASSPORT_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include <unordered_map>
#include "../../exceptions/ValidationResult.h"
#include "PassportNumber.h"
#include "PassportNumberRegistry.h"

enum class PassportNumberError {
    EMPTY_PASSPORT_NUMBER,
    INVALID_FORMAT,
    INVALID_ISSUING_COUNTRY,
    INVALID_NUMBER_FORMAT
};

struct PassportNumberErrorHelper {
    static std::string toString(PassportNumberError error) {
        static const std::unordered_map<PassportNumberError, std::string> errorMap = {
            {PassportNumberError::EMPTY_PASSPORT_NUMBER, "EMPTY_PASSPORT_NUMBER"},
            {PassportNumberError::INVALID_FORMAT, "INVALID_FORMAT"},
            {PassportNumberError::INVALID_ISSUING_COUNTRY, "INVALID_ISSUING_COUNTRY"},
            {PassportNumberError::INVALID_NUMBER_FORMAT, "INVALID_NUMBER_FORMAT"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    static std::string getMessage(PassportNumberError error) {
        static const std::unordered_map<PassportNumberError, std::string> messageMap = {
            {PassportNumberError::EMPTY_PASSPORT_NUMBER, "Passport number cannot be empty"},
            {PassportNumberError::INVALID_FORMAT, "Passport number must be in format 'NUMBER (COUNTRY)' or 'NUMBER,COUNTRY'"},
            {PassportNumberError::INVALID_ISSUING_COUNTRY, "Invalid issuing country code"},
            {PassportNumberError::INVALID_NUMBER_FORMAT, "Invalid passport number format"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Unknown error occurred";
    }

    static void addError(ValidationResult& result, PassportNumberError error) {
        result.addError(
            "passportNumber",
            getMessage(error),
            toString(error)
        );
    }
};

class PassportNumberValidator {
public:
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (value.empty()) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::EMPTY_PASSPORT_NUMBER);
            return result;
        }

        // Validate format
        std::regex pattern("^([A-Z0-9]+)\\s*[,(]\\s*([A-Z]+)[)]?$");
        if (!std::regex_match(value, pattern)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_FORMAT);
            return result;
        }

        // Extract number and country
        std::smatch matches;
        std::regex_match(value, matches, pattern);
        std::string number = matches[1];
        std::string country = matches[2];

        // Validate number format (example: must be alphanumeric and 6-9 characters)
        if (number.length() < 6 || number.length() > 9) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_NUMBER_FORMAT);
        }

        // Validate issuing country
        if (!PassportNumberRegistry::getByIssuingCountry(country)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_ISSUING_COUNTRY);
        }

        return result;
    }
};

#endif 