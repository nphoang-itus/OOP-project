#ifndef PASSPORT_NUMBER_H
#define PASSPORT_NUMBER_H

#include <string>
#include <memory>
#include <utility>
#include <optional>

#include "../../exceptions/ValidationResult.h"
#include "PassportNumberValidator.h"
#include "PassportNumberParser.h"
#include "PassportNumberFormatter.h"

class PassportNumber {
private:
    std::string _issuingCountry;
    std::string _number;
    
    PassportNumber(const std::string& issuingCountry, const std::string& number) 
        : _number(std::move(number)) {
        std::string upperCountryCode = issuingCountry;
        
        // Chuyển đổi countryCode thành chữ hoa
        std::transform(upperCountryCode.begin(), upperCountryCode.end(), upperCountryCode.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        
        _issuingCountry = upperCountryCode;
    }

    template<typename InputType>
    static Result<PassportNumber> createInternal(const InputType& input) {
        auto validationResult = PassportNumberValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<PassportNumber>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::pair<std::string, std::string>>) {
            const auto& [number, issuingCountry] = input;
            return Success(PassportNumber(number, issuingCountry));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = PassportNumberParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<PassportNumber>(CoreError("Failed to parse passport number", "PARSE_ERROR"));
            }
            const auto& [number, issuingCountry] = parsed.value();
            return Success(PassportNumber(number, issuingCountry));
        }
        return Failure<PassportNumber>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    PassportNumber() = default;

    static Result<PassportNumber> create(const std::string& value) {
        return createInternal(value);    
    }

    static Result<PassportNumber> create(const std::string& number, const std::string& issuingCountry) {
        return createInternal(std::make_pair(number, issuingCountry));
    }

    const std::string& getNumber() const {
        return _number;
    }

    const std::string& getIssuingCountry() const {
        return _issuingCountry;
    }

    std::string toString() const {
        return PassportNumberFormatter::toString(_issuingCountry, _number);
    }

    bool operator==(const PassportNumber& other) const {
        return _number == other._number && _issuingCountry == other._issuingCountry;
    }

    bool operator!=(const PassportNumber& other) const {
        return !(*this == other);
    }
};

#endif