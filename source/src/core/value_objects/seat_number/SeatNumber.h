#ifndef SEAT_NUMBER_H
#define SEAT_NUMBER_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include <functional>

#include "../../exceptions/ValidationResult.h"
#include "SeatNumberValidator.h"
#include "../seat_class_map/SeatClassMap.h"

class SeatNumber {
private:
    std::string _value;
    std::shared_ptr<const SeatClassMap> _seatLayout;
    
    SeatNumber(const std::string& value, std::shared_ptr<const SeatClassMap> seatLayout) 
        : _value(value), _seatLayout(std::move(seatLayout)) {}

    static Result<SeatNumber> createInternal(const std::string& value, 
                                           std::shared_ptr<const SeatClassMap> seatLayout) {
        auto validationResult = SeatNumberValidator::validate(value, *seatLayout);
        if (!validationResult.isValid()) {
            return getValidationFailure<SeatNumber>(validationResult);
        }
        return Success(SeatNumber(value, seatLayout));
    }

public:
    // Constructor mặc định (có thể thêm nếu cần)
    SeatNumber() : _seatLayout(nullptr) {}
    
    // Phương thức factory
    static Result<SeatNumber> create(const std::string& value, const SeatClassMap& seatLayout) {
        return createInternal(value, std::make_shared<SeatClassMap>(seatLayout));
    }

    // Copy constructor và toán tử gán được tạo tự động, không cần định nghĩa

    const std::string& getValue() const { return _value; }
    char getClassCode() const { return _value[0]; }
    int getSequenceNumber() const { return std::stoi(_value.substr(1)); }
    
    const SeatClassMap& getSeatLayout() const { 
        return *_seatLayout; 
    }

    std::string toString() const {
        return _value;
    }

    bool operator==(const SeatNumber& other) const {
        return _value == other._value;
    }

    bool operator!=(const SeatNumber& other) const {
        return !(*this == other);
    }

    bool operator<(const SeatNumber& other) const {
        return _value < other._value;
    }
};

namespace std {
    template<>
    struct hash<SeatNumber> {
        size_t operator()(const SeatNumber& seatNumber) const {
            return hash<string>()(seatNumber.toString());
        }
    };
}

#endif