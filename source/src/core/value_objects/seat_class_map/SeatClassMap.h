#ifndef SEAT_CLASS_MAP_H
#define SEAT_CLASS_MAP_H

#include <string>
#include <unordered_map>
#include <vector>
#include "SeatClass.h"
#include "SeatClassMapValidator.h"
#include "SeatClassMapParser.h"
#include "SeatClassMapFormatter.h"
#include "../../exceptions/Result.h"

class SeatClassMap {
private:
    std::unordered_map<SeatClass, int> _seatCounts;

    // Private constructor for string input
    explicit SeatClassMap(const std::string& value) {
        _seatCounts = SeatClassMapParser::parse(value);
    }

    // Private constructor for map input
    explicit SeatClassMap(std::unordered_map<SeatClass, int> seatCounts)
        : _seatCounts(std::move(seatCounts)) {}

    // Template method for creation process
    template<typename InputType>
    static Result<SeatClassMap> createInternal(const InputType& input) {
        auto validationResult = SeatClassMapValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<SeatClassMap>(validationResult);
        }
        return Success(SeatClassMap(input));
    }

public:
    // Public factory methods that delegate to internal template method
    static Result<SeatClassMap> create(const std::string& value) {
        return createInternal(value);
    }

    static Result<SeatClassMap> create(const std::unordered_map<SeatClass, int>& classMap) {
        return createInternal(classMap);
    }

    const std::unordered_map<SeatClass, int>& getSeatCounts() const {
        return _seatCounts;
    }

    std::string toString() const {
        return SeatClassMapFormatter::toString(_seatCounts);
    }
};

#endif
