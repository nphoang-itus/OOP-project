#ifndef TICKET_NUMBER_VALIDATOR_H
#define TICKET_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"
#include "TicketNumberError.h"

class TicketNumberValidator {
private:
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    static bool isValidFormat(const std::string& value) {
        // Format: MCB-YYYYMMDD-XXXX
        std::regex pattern("^[A-Z]{2}[0-9]{1,4}-[0-9]{8}-[0-9]{4}$");
        return std::regex_match(value, pattern);
    }

public:
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            TicketNumberErrorHelper::addError(result, TicketNumberError::EMPTY_TICKET_NUMBER);
            return result;
        }

        // Validate format
        if (!isValidFormat(value)) {
            TicketNumberErrorHelper::addError(result, TicketNumberError::INVALID_FORMAT);
        }
        
        return result;
    }
};

#endif 