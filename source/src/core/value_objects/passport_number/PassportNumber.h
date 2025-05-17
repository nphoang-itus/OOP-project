#ifndef PASSPORT_NUMBER_H
#define PASSPORT_NUMBER_H

#include <string>
#include <functional>

// Forward declaration
class PassportNumberRegistry;

class PassportNumber {
private:
    std::string _number;
    std::string _issuingCountry;

    // Private constructor - only PassportNumberRegistry can create instances
    PassportNumber(std::string number, std::string issuingCountry) 
        : _number(std::move(number)), _issuingCountry(std::move(issuingCountry)) {}

    // Friend class that can create PassportNumber instances
    friend class PassportNumberRegistry;

public:
    // Default constructor for unordered_map
    PassportNumber() = default;

    // Getters
    const std::string& getNumber() const { return _number; }
    const std::string& getIssuingCountry() const { return _issuingCountry; }

    // Comparison operators
    bool operator==(const PassportNumber& other) const {
        return _number == other._number && _issuingCountry == other._issuingCountry;
    }

    bool operator!=(const PassportNumber& other) const {
        return !(*this == other);
    }
};

// Hash function for PassportNumber
namespace std {
    template<>
    struct hash<PassportNumber> {
        size_t operator()(const PassportNumber& passport) const {
            return hash<string>()(passport.getNumber()) ^ 
                   (hash<string>()(passport.getIssuingCountry()) << 1);
        }
    };
}

#endif