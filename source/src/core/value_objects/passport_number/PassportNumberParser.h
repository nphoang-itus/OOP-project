#ifndef PASSPORT_NUMBER_PARSER_H
#define PASSPORT_NUMBER_PARSER_H

#include <string>
#include <optional>
#include <regex>
#include "PassportNumber.h"
#include "PassportNumberRegistry.h"

class PassportNumberParser {
public:
    static std::optional<PassportNumber> parse(const std::string& value) {
        // Expected format: "NUMBER (COUNTRY)" or "NUMBER,COUNTRY"
        std::regex pattern("^([A-Z0-9]+)\\s*[,(]\\s*([A-Z]+)[)]?$");
        std::smatch matches;
        
        if (std::regex_match(value, matches, pattern)) {
            std::string number = matches[1];
            std::string country = matches[2];
            
            if (auto passport = PassportNumberRegistry::getByNumber(number)) {
                return passport;
            }
        }
        return std::nullopt;
    }
};

#endif 