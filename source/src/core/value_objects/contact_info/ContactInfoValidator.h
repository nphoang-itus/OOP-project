#ifndef CONTACT_INFO_VALIDATOR_H
#define CONTACT_INFO_VALIDATOR_H

#include <string>
#include <regex>
#include <tuple>
#include <unordered_map>
#include "ContactInfoParser.h"
#include "../../exceptions/ValidationResult.h"

enum class ContactInfoError {
    EMPTY_EMAIL,
    INVALID_EMAIL_FORMAT,
    EMAIL_TOO_LONG,
    EMPTY_PHONE,
    INVALID_PHONE_FORMAT,
    PHONE_TOO_LONG,
    INVALID_ADDRESS_FORMAT,
    ADDRESS_TOO_LONG
};

struct ContactInfoErrorHelper {
    static std::string toString(ContactInfoError error) {
        static const std::unordered_map<ContactInfoError, std::string> errorMap = {
            {ContactInfoError::EMPTY_EMAIL, "EMPTY_EMAIL"},
            {ContactInfoError::INVALID_EMAIL_FORMAT, "INVALID_EMAIL_FORMAT"},
            {ContactInfoError::EMAIL_TOO_LONG, "EMAIL_TOO_LONG"},
            {ContactInfoError::EMPTY_PHONE, "EMPTY_PHONE"},
            {ContactInfoError::INVALID_PHONE_FORMAT, "INVALID_PHONE_FORMAT"},
            {ContactInfoError::PHONE_TOO_LONG, "PHONE_TOO_LONG"},
            {ContactInfoError::INVALID_ADDRESS_FORMAT, "INVALID_ADDRESS_FORMAT"},
            {ContactInfoError::ADDRESS_TOO_LONG, "ADDRESS_TOO_LONG"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    static std::string getMessage(ContactInfoError error) {
        static const std::unordered_map<ContactInfoError, std::string> messageMap = {
            {ContactInfoError::EMPTY_EMAIL, "Email cannot be empty"},
            {ContactInfoError::INVALID_EMAIL_FORMAT, "Invalid email format"},
            {ContactInfoError::EMAIL_TOO_LONG, "Email must not exceed 254 characters"},
            {ContactInfoError::EMPTY_PHONE, "Phone number cannot be empty"},
            {ContactInfoError::INVALID_PHONE_FORMAT, "Invalid phone number format"},
            {ContactInfoError::PHONE_TOO_LONG, "Phone number must not exceed 15 digits"},
            {ContactInfoError::INVALID_ADDRESS_FORMAT, "Invalid address format"},
            {ContactInfoError::ADDRESS_TOO_LONG, "Address must not exceed 100 characters"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Unknown error occurred";
    }

    static void addError(ValidationResult& result, ContactInfoError error) {
        result.addError(
            "contactInfo",
            getMessage(error),
            toString(error)
        );
    }
};

class ContactInfoValidator {
private:
    static const std::regex EMAIL_REGEX;
    static const std::regex PHONE_REGEX;
    static const std::regex ADDRESS_REGEX;

    // Length constants
    static constexpr size_t MAX_EMAIL_LENGTH = 254;  // RFC 5321
    static constexpr size_t MAX_PHONE_LENGTH = 15;   // E.164 standard
    static constexpr size_t MAX_ADDRESS_LENGTH = 100;

    template<typename InputType>
    static ValidationResult validateInternal(const InputType& input) {
        ValidationResult result;
        validateSpecific(input, result);
        return result;
    }

    static void validateSpecific(const std::tuple<std::string, std::string, std::string>& input, ValidationResult& result) {
        const auto& [email, phone, address] = input;
        validateEmail(email, result);
        validatePhone(phone, result);
        validateAddress(address, result);
    }

    static void validateSpecific(const std::string& value, ValidationResult& result) {
        auto parsed = ContactInfoParser::parse(value);
        if (!parsed) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_EMAIL_FORMAT);
            return;
        }
        validateSpecific(*parsed, result);
    }

    static void validateEmail(const std::string& email, ValidationResult& result) {
        if (email.empty()) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::EMPTY_EMAIL);
            return;
        }
        if (email.length() > MAX_EMAIL_LENGTH) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::EMAIL_TOO_LONG);
            return;
        }
        if (!std::regex_match(email, EMAIL_REGEX)) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_EMAIL_FORMAT);
        }
    }

    static void validatePhone(const std::string& phone, ValidationResult& result) {
        if (phone.empty()) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::EMPTY_PHONE);
            return;
        }
        // Remove '+' prefix for length check
        std::string phoneDigits = phone;
        if (!phoneDigits.empty() && phoneDigits[0] == '+') {
            phoneDigits = phoneDigits.substr(1);
        }
        if (phoneDigits.length() > MAX_PHONE_LENGTH) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::PHONE_TOO_LONG);
            return;
        }
        if (!std::regex_match(phone, PHONE_REGEX)) {
            ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_PHONE_FORMAT);
        }
    }

    static void validateAddress(const std::string& address, ValidationResult& result) {
        if (!address.empty()) {
            if (address.length() > MAX_ADDRESS_LENGTH) {
                ContactInfoErrorHelper::addError(result, ContactInfoError::ADDRESS_TOO_LONG);
                return;
            }
            if (!std::regex_match(address, ADDRESS_REGEX)) {
                ContactInfoErrorHelper::addError(result, ContactInfoError::INVALID_ADDRESS_FORMAT);
            }
        }
    }

public:
    static ValidationResult validate(const std::tuple<std::string, std::string, std::string>& input) {
        return validateInternal(input);
    }

    static ValidationResult validate(const std::string& value) {
        return validateInternal(value);
    }
};

// Initialize static regex patterns
const std::regex ContactInfoValidator::EMAIL_REGEX(
    R"(^[a-zA-Z0-9](\.?[a-zA-Z0-9_\-+%])*@[a-zA-Z0-9]([a-zA-Z0-9\-]*[a-zA-Z0-9])?(\.[a-zA-Z]{2,})+$)"
);

const std::regex ContactInfoValidator::PHONE_REGEX("^\\+?[0-9]{10,15}$");
const std::regex ContactInfoValidator::ADDRESS_REGEX("^[a-zA-Z0-9\\s,.-]{0,100}$");

#endif 