#ifndef CONTACT_INFO_FORMATTER_H
#define CONTACT_INFO_FORMATTER_H

#include <string>

class ContactInfoFormatter {
public:
    static std::string toString(
        std::string email,
        std::string phone,
        std::string address
    ) {
        return (
                email   + '|'
            +   phone   + '|'
            +   address
        );
    }

    static std::string toString(
        std::string email,
        std::string phone,
        std::string address,
        std::string separator
    ) {
        return (
                email   + separator
            +   phone   + separator
            +   address
        );
    }
};

#endif 