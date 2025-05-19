#ifndef PASSPORT_NUMBER_PARSER_H
#define PASSPORT_NUMBER_PARSER_H

#include <string>
#include <utility>
#include <optional>

class PassportNumberParser {
public:
    static std::optional<std::pair<std::string, std::string>> parse(const std::string& value) {
        size_t colonPos = value.find(':');
        if (colonPos != std::string::npos) {
            std::string countryCode = value.substr(0, colonPos);
            std::string number = value.substr(colonPos + 1);
            return std::make_pair(countryCode, number);
        }
        return std::nullopt;
    }
};

#endif 