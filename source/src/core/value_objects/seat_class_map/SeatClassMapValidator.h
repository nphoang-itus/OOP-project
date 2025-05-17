#ifndef SEAT_CLASS_MAP_VALIDATOR_H
#define SEAT_CLASS_MAP_VALIDATOR_H

#include <string>
#include <regex>
#include <unordered_map>
#include <algorithm>
#include <cctype>

#include "../../exceptions/ValidationResult.h"
#include "SeatClass.h"

enum class SeatClassMapError {
    EMPTY_SEAT_CLASS_MAP, // String hoặc Map rỗng
    INVALID_FORMAT, // Format String sai định dạng (CLASS_NAME:SEAT_COUNT)
    INVALID_SEAT_TYPE, // Sai tên ghế (Phải thuộc seat class)
    INVALID_SEAT_COUNT,
    INVALID_CASE_MIXING // Thêm mã lỗi mới cho case mixing
};

struct SeatClassMapErrorHelper {
    static std::string toString(SeatClassMapError error) {
        static const std::unordered_map<SeatClassMapError, std::string> errorMap = {
            {SeatClassMapError::EMPTY_SEAT_CLASS_MAP, "EMPTY_SEAT_CLASS_MAP"},
            {SeatClassMapError::INVALID_FORMAT, "INVALID_FORMAT"},
            {SeatClassMapError::INVALID_SEAT_TYPE, "INVALID_SEAT_TYPE"},
            {SeatClassMapError::INVALID_SEAT_COUNT, "INVALID_SEAT_COUNT"},
            {SeatClassMapError::INVALID_CASE_MIXING, "INVALID_CASE_MIXING"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    static std::string getMessage(SeatClassMapError error) {
        static const std::unordered_map<SeatClassMapError, std::string> messageMap = {
            {SeatClassMapError::EMPTY_SEAT_CLASS_MAP, "Seat class map cannot be empty"},
            {SeatClassMapError::INVALID_FORMAT, "Seat class map must be in format 'CLASS_NAME:SEAT_COUNT,CLASS_NAME:SEAT_COUNT,...'"},
            {SeatClassMapError::INVALID_SEAT_TYPE, "Seat type name must be one of: ECONOMY, BUSINESS, FIRST"},
            {SeatClassMapError::INVALID_SEAT_COUNT, "Seat count must be a positive number"},
            {SeatClassMapError::INVALID_CASE_MIXING, "Seat class name must be all uppercase or all lowercase"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Unknown error occurred";
    }

    static void addError(ValidationResult& result, SeatClassMapError error) {
        result.addError(
            "seatClassMap",
            getMessage(error),
            toString(error)
        );
    }
};

class SeatClassMapValidator {
private:
    // Template method for validation process
    template<typename InputType>
    static ValidationResult validateInternal(const InputType& input) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(input)) {
            SeatClassMapErrorHelper::addError(result, SeatClassMapError::EMPTY_SEAT_CLASS_MAP);
            return result;
        }

        // Validate specific input type
        validateSpecific(input, result);
        
        return result;
    }

    // Helper methods for string validation
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    static bool isEmpty(const std::unordered_map<SeatClass, int>& seatMap) {
        return seatMap.empty();
    }

    static void validateSpecific(const std::string& value, ValidationResult& result) {
        // Validate format
        std::regex pattern("^([A-Za-z0-9]+:[-]?[0-9]+)(,[A-Za-z0-9]+:[-]?[0-9]+)*$");
        if (!std::regex_match(value, pattern)) {
            SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_FORMAT);
            return;
        }

        // Validate each class entry
        std::string input = value;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        
        while ((pos = input.find(delimiter)) != std::string::npos) {
            token = input.substr(0, pos);
            validateClassEntry(token, result);
            input.erase(0, pos + delimiter.length());
        }
        validateClassEntry(input, result);
    }

    static void validateSpecific(const std::unordered_map<SeatClass, int>& seatMap, ValidationResult& result) {
        for (const auto& [seatClass, count] : seatMap) {
            if (count <= 0) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_COUNT);
            }
        }
    }

    static bool isAllUppercase(const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return !std::isalpha(c) || std::isupper(c);
        });
    }
    
    static bool isAllLowercase(const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return !std::isalpha(c) || std::islower(c);
        });
    }

    static void validateClassEntry(const std::string& entry, ValidationResult& result) {
        size_t colonPos = entry.find(':');
        if (colonPos != std::string::npos) {
            std::string className = entry.substr(0, colonPos);
            std::string seatCount = entry.substr(colonPos + 1);

            // Validate case mixing
            if (!isAllUppercase(className) && !isAllLowercase(className)) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_CASE_MIXING);
                return;
            }

            // Convert to uppercase for validation
            std::string uppercaseClassName = className;
            std::transform(uppercaseClassName.begin(), uppercaseClassName.end(), 
                          uppercaseClassName.begin(), ::toupper);

            // Validate class name
            if (!SeatClassRegistry::getByName(uppercaseClassName)) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_TYPE);
            }

            // Validate seat count
            try {
                int count = std::stoi(seatCount);
                if (count <= 0) {
                    SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_COUNT);
                }
            } catch (...) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_COUNT);
            }
        }
    }

public:
    // Public validation methods that delegate to internal template method
    static ValidationResult validate(const std::string& value) {
        return validateInternal(value);
    }

    static ValidationResult validate(const std::unordered_map<SeatClass, int>& seatMap) {
        return validateInternal(seatMap);
    }
};

#endif