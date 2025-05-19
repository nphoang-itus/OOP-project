#ifndef CONTACT_INFO_PARSER_H
#define CONTACT_INFO_PARSER_H

#include <string>
#include <tuple>
#include <optional>
#include <regex>

class ContactInfoParser {
private:
    static const std::regex CONTACT_INFO_REGEX;

public:
    static std::optional<std::tuple<std::string, std::string, std::string>> parse(const std::string& value) {
        std::smatch matches;
        if (std::regex_match(value, matches, CONTACT_INFO_REGEX)) {
            return std::make_tuple(matches[1].str(), matches[2].str(), matches[3].str());
        }
        return std::nullopt;
    }
};

// Initialize static regex pattern
// Format: email|phone|address
const std::regex ContactInfoParser::CONTACT_INFO_REGEX(
    "^([^|]+)\\|([^|]+)\\|(.*)$"
);

#endif 