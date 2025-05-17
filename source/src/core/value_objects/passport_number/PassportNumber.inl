#ifndef PASSPORT_NUMBER_INL
#define PASSPORT_NUMBER_INL

#include "PassportNumber.h"
#include "PassportNumberFormatter.h"
#include "PassportNumberParser.h"

// Helper functions for backward compatibility
inline std::string passportNumberToString(const PassportNumber& passport) {
    return PassportNumberFormatter::toString(passport);
}

inline std::optional<PassportNumber> stringToPassportNumber(const std::string& str) {
    return PassportNumberParser::parse(str);
}

#endif 