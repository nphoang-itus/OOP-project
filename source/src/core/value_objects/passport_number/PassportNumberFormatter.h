#ifndef PASSPORT_NUMBER_FORMATTER_H
#define PASSPORT_NUMBER_FORMATTER_H

#include <string>
#include "PassportNumber.h"

class PassportNumberFormatter {
public:
    // Format PassportNumber to string
    static std::string toString(const PassportNumber& passport) {
        return passport.getNumber() + " (" + passport.getIssuingCountry() + ")";
    }

    // Format PassportNumber to string with custom separator
    static std::string toString(const PassportNumber& passport, const std::string& separator) {
        return passport.getNumber() + separator + passport.getIssuingCountry();
    }
};

#endif 