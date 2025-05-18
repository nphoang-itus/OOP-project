#ifndef ROUTE_FORMATTER_H
#define ROUTE_FORMATTER_H

#include <string>

class RouteFormatter {
public:
    static std::string toString(const std::string& origin, const std::string& originCode,
                              const std::string& destination, const std::string& destinationCode) {
        return origin + "(" + originCode + ")-" + destination + "(" + destinationCode + ")";
    }
};

#endif 