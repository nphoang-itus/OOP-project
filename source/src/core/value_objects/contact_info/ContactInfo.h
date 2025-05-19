#ifndef CONTACT_INFO_H
#define CONTACT_INFO_H

#include <string>
#include <tuple>
#include "../../exceptions/Result.h"
#include "ContactInfoValidator.h"
#include "ContactInfoParser.h"
#include "ContactInfoFormatter.h"

class ContactInfo {
private:
    std::string _email;
    std::string _phone;
    std::string _address;

    // Private constructors
    ContactInfo(std::string email, std::string phone, std::string address)
        : _email(std::move(email)), _phone(std::move(phone)), _address(std::move(address)) {}

    // Template method for creation process
    template<typename InputType>
    static Result<ContactInfo> createInternal(const InputType& input) {
        auto validationResult = ContactInfoValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<ContactInfo>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::tuple<std::string, std::string, std::string>>) {
            const auto& [email, phone, address] = input;
            return Success(ContactInfo(email, phone, address));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = ContactInfoParser::parse(input);
            if (!parsed) {
                return Failure<ContactInfo>(CoreError("Failed to parse contact info", "PARSE_ERROR"));
            }
            const auto& [email, phone, address] = *parsed;
            return Success(ContactInfo(email, phone, address));
        }
        return Failure<ContactInfo>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    ContactInfo() = default;

    // Factory methods
    static Result<ContactInfo> create(const std::string& email, const std::string& phone, const std::string& address = "") {
        return createInternal(std::make_tuple(email, phone, address));
    }

    static Result<ContactInfo> create(const std::string& value) {
        return createInternal(value);
    }

    // Getters
    const std::string& getEmail() const { return _email; }
    const std::string& getPhone() const { return _phone; }
    const std::string& getAddress() const { return _address; }

    // Value object operations
    bool operator==(const ContactInfo& other) const {
        return _email == other._email && 
               _phone == other._phone && 
               _address == other._address;
    }

    bool operator!=(const ContactInfo& other) const {
        return !(*this == other);
    }

    std::string toString() const {
        return ContactInfoFormatter::toString(_email, _phone, _address);
    }
};

#endif