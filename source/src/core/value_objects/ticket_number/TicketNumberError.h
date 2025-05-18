#ifndef TICKET_NUMBER_ERROR_H
#define TICKET_NUMBER_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

enum class TicketNumberError {
    EMPTY_TICKET_NUMBER,
    INVALID_FORMAT
};

struct TicketNumberErrorHelper {
    static std::string toString(TicketNumberError error) {
        switch (error) {
            case TicketNumberError::EMPTY_TICKET_NUMBER: return "EMPTY_TICKET_NUMBER";
            case TicketNumberError::INVALID_FORMAT: return "INVALID_FORMAT";
            default: return "UNKNOWN_ERROR";
        }
    }

    static std::string getMessage(TicketNumberError error) {
        switch (error) {
            case TicketNumberError::EMPTY_TICKET_NUMBER: return "Ticket number cannot be empty";
            case TicketNumberError::INVALID_FORMAT: return "Ticket number must be in format 'MCB-YYYYMMDD-XXXX'";
            default: return "Unknown error";
        }
    }

    static void addError(ValidationResult& result, TicketNumberError error) {
        result.addError(
            "ticketNumber",
            getMessage(error),
            toString(error)
        );
    }
};

#endif 