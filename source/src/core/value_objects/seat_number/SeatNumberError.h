#ifndef SEAT_NUMBER_ERROR_H
#define SEAT_NUMBER_ERROR_H

#include <string>
#include "../../exceptions/ValidationResult.h"

enum class SeatNumberError {
    EMPTY_SEAT_NUMBER,
    INVALID_FORMAT,
    INVALID_CLASS_CODE,
    INVALID_SEQUENCE_NUMBER,
    INVALID_SEAT_CLASS
};

struct SeatNumberErrorHelper {
    static std::string toString(SeatNumberError error) {
        switch (error) {
            case SeatNumberError::EMPTY_SEAT_NUMBER: return "EMPTY_SEAT_NUMBER";
            case SeatNumberError::INVALID_FORMAT: return "INVALID_FORMAT";
            case SeatNumberError::INVALID_CLASS_CODE: return "INVALID_CLASS_CODE";
            case SeatNumberError::INVALID_SEQUENCE_NUMBER: return "INVALID_SEQUENCE_NUMBER";
            case SeatNumberError::INVALID_SEAT_CLASS: return "INVALID_SEAT_CLASS";
            default: return "UNKNOWN_ERROR";
        }
    }

    static std::string getMessage(SeatNumberError error) {
        switch (error) {
            case SeatNumberError::EMPTY_SEAT_NUMBER: return "seatNumber: Seat number cannot be empty";
            case SeatNumberError::INVALID_FORMAT: return "seatNumber: Seat number must be in format 'CLASS_CODE[SEQUENCE_NUMBER]'";
            case SeatNumberError::INVALID_CLASS_CODE: return "seatNumber: Invalid seat class code";
            case SeatNumberError::INVALID_SEQUENCE_NUMBER: return "seatNumber: Invalid sequence number";
            case SeatNumberError::INVALID_SEAT_CLASS: return "seatNumber: Seat class not found in aircraft layout";
            default: return "seatNumber: Unknown error";
        }
    }

    static void addError(ValidationResult& result, SeatNumberError error) {
        result.addError(toString(error), getMessage(error));
    }
};

#endif 