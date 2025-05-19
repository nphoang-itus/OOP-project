#ifndef TICKET_NUMBER_H
#define TICKET_NUMBER_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include "../../exceptions/ValidationResult.h"
#include "TicketNumberValidator.h"

class TicketNumber {
private:
    std::string _value;
    
    TicketNumber(const std::string& value) : _value(value) {}

    static Result<TicketNumber> createInternal(const std::string& value) {
        auto validationResult = TicketNumberValidator::validate(value);
        if (!validationResult.isValid()) {
            return getValidationFailure<TicketNumber>(validationResult);
        }
        return Success(TicketNumber(value));
    }

public:
    static Result<TicketNumber> create(const std::string& value) {
        return createInternal(value);
    }

    const std::string& getValue() const { return _value; }

    std::string toString() const {
        return _value;
    }

    bool operator==(const TicketNumber& other) const {
        return _value == other._value;
    }

    bool operator!=(const TicketNumber& other) const {
        return !(*this == other);
    }
};

#endif
