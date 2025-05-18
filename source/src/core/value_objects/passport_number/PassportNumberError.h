#ifndef PASSPORT_NUMBER_ERROR_H
#define PASSPORT_NUMBER_ERROR_H

#include <string>
#include <unordered_map>
#include "../../exceptions/ValidationResult.h"

enum class PassportNumberError {
    EMPTY_PASSPORT_NUMBER,
    INVALID_FORMAT,
    INVALID_ISSUING_COUNTRY,
    INVALID_NUMBER_LENGTH
};

struct PassportNumberErrorHelper {
    static std::string toString(PassportNumberError error) {
        static const std::unordered_map<PassportNumberError, std::string> errorMap = {
            {PassportNumberError::EMPTY_PASSPORT_NUMBER, "EMPTY_PASSPORT_NUMBER"},
            {PassportNumberError::INVALID_FORMAT, "INVALID_FORMAT"},
            {PassportNumberError::INVALID_ISSUING_COUNTRY, "INVALID_ISSUING_COUNTRY"},
            {PassportNumberError::INVALID_NUMBER_LENGTH, "INVALID_NUMBER_LENGTH"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    static std::string getMessage(PassportNumberError error) {
        static const std::unordered_map<PassportNumberError, std::string> messageMap = {
            {PassportNumberError::EMPTY_PASSPORT_NUMBER, "Passport number cannot be empty"},
            {PassportNumberError::INVALID_FORMAT, "Passport number must be in format 'COUNTRY:NUMBER'"},
            {PassportNumberError::INVALID_ISSUING_COUNTRY, "Issuing country must be a valid 2-letter country code"},
            {PassportNumberError::INVALID_NUMBER_LENGTH, "Passport number must be between 6-9 digits"}
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

#endif 