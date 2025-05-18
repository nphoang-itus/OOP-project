#ifndef SEAT_NUMBER_VALIDATOR_H
#define SEAT_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"
#include "SeatNumberError.h"
#include "../seat_class_map/SeatClassMap.h"

class SeatNumberValidator {
private:
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    static bool isValidFormat(const std::string& value) {
        // Format: E001, B01, etc.
        std::regex pattern("^[A-Z][0-9]{2,3}$");
        return std::regex_match(value, pattern);
    }

    static bool isValidSequenceNumber(const std::string& value, const SeatClassMap& seatLayout) {
        char classCode = value[0];
        int sequenceNumber = std::stoi(value.substr(1));
        
        // Find the seat class and its count
        for (const auto& [seatClass, count] : seatLayout.getSeatCounts()) {
            if (seatClass.getCode()[0] == classCode) {
                return sequenceNumber > 0 && sequenceNumber <= count;
            }
        }
        return false;
    }

public:
    static ValidationResult validate(const std::string& value, const SeatClassMap& seatLayout) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::EMPTY_SEAT_NUMBER);
            return result;
        }

        // Validate format
        if (!isValidFormat(value)) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::INVALID_FORMAT);
            return result;
        }

        // Validate class code exists in seat layout
        char classCode = value[0];
        bool classFound = false;
        for (const auto& [seatClass, _] : seatLayout.getSeatCounts()) {
            if (seatClass.getCode()[0] == classCode) {
                classFound = true;
                break;
            }
        }
        if (!classFound) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::INVALID_SEAT_CLASS);
            return result;
        }

        // Validate sequence number
        if (!isValidSequenceNumber(value, seatLayout)) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::INVALID_SEQUENCE_NUMBER);
        }
        
        return result;
    }
};

#endif 