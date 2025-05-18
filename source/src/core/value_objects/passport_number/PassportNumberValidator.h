#ifndef PASSPORT_NUMBER_VALIDATOR_H
#define PASSPORT_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include <algorithm>
#include "../../exceptions/ValidationResult.h"
#include "PassportNumberError.h"

class PassportNumberValidator {
private:
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    static bool isValidCountryCode(const std::string& countryCode) {
        // Check if it's a 2,3-letter country code
        if (countryCode.length() != 2) return false;
        std::regex countryPattern("^[A-Za-z]{2,3}$");
        return std::regex_match(countryCode, countryPattern);
    }

    static bool isValidPassportNumber(const std::string& number) {
        // Check if it's 6-9 digits
        std::regex numberPattern("^[0-9]{6,9}$");
        return std::regex_match(number, numberPattern);
    }

public:
    // Validate combined string format (COUNTRY:NUMBER)
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::EMPTY_PASSPORT_NUMBER);
            return result;
        }

        // Validate format (COUNTRY:NUMBER)
        std::regex pattern("^[A-Za-z0-9]{2,3}:[0-9]+$");
        if (!std::regex_match(value, pattern)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_FORMAT);
            return result;
        }

        // Split and validate components
        size_t colonPos = value.find(':');
        if (colonPos != std::string::npos) {
            std::string countryCode = value.substr(0, colonPos);
            std::string number = value.substr(colonPos + 1);

            if (!isValidCountryCode(countryCode)) {
                PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_ISSUING_COUNTRY);
            }

            if (!isValidPassportNumber(number)) {
                PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_NUMBER_LENGTH);
            }
        }
        
        return result;
    }

    // Validate separate country code and number
    static ValidationResult validate(const std::pair<std::string, std::string>& input) {
        ValidationResult result;
        const auto& [countryCode, number] = input;
        
        // Check empty inputs
        if (isEmpty(countryCode) || isEmpty(number)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::EMPTY_PASSPORT_NUMBER);
            return result;
        }

        // Validate country code
        if (!isValidCountryCode(countryCode)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_ISSUING_COUNTRY);
        }

        // Validate passport number
        if (!isValidPassportNumber(number)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_NUMBER_LENGTH);
        }
        
        return result;
    }
};

#endif