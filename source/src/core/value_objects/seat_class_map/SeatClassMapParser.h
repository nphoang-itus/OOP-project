#ifndef SEAT_CLASS_MAP_PARSER_H
#define SEAT_CLASS_MAP_PARSER_H

#include <string>
#include <unordered_map>
#include "SeatClass.h"

class SeatClassMapParser {
public:
    static std::unordered_map<SeatClass, int> parse(const std::string& value) {
        std::unordered_map<SeatClass, int> classMap;
        std::string input = value;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        
        while ((pos = input.find(delimiter)) != std::string::npos) {
            token = input.substr(0, pos);
            parseClassEntry(token, classMap);
            input.erase(0, pos + delimiter.length());
        }
        parseClassEntry(input, classMap);
        
        return classMap;
    }

private:
    static void parseClassEntry(const std::string& entry, std::unordered_map<SeatClass, int>& classMap) {
        size_t colonPos = entry.find(':');
        if (colonPos != std::string::npos) {
            std::string className = entry.substr(0, colonPos);
            std::string seatCount = entry.substr(colonPos + 1);
            if (auto seatClass = stringToSeatClass(className)) {
                classMap[*seatClass] = std::stoi(seatCount);
            }
        }
    }
};

#endif 