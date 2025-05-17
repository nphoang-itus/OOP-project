#ifndef SEAT_CLASS_MAP_PARSER_H
#define SEAT_CLASS_MAP_PARSER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>
#include "SeatClass.h"

// Interface for parsing strategies
class ISeatClassMapParserStrategy {
public:
    virtual ~ISeatClassMapParserStrategy() = default;
    virtual std::unordered_map<SeatClass, int> parse(const std::string& input) = 0;
};

// Default parser strategy for standard format (CLASS_NAME:SEAT_COUNT,CLASS_NAME:SEAT_COUNT,...)
class StandardSeatClassMapParser : public ISeatClassMapParserStrategy {
public:
    std::unordered_map<SeatClass, int> parse(const std::string& input) override {
        std::unordered_map<SeatClass, int> classMap;
        std::string value = input;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        
        while ((pos = value.find(delimiter)) != std::string::npos) {
            token = value.substr(0, pos);
            parseClassEntry(token, classMap);
            value.erase(0, pos + delimiter.length());
        }
        parseClassEntry(value, classMap);
        
        return classMap;
    }

private:
    void parseClassEntry(const std::string& entry, std::unordered_map<SeatClass, int>& classMap) {
        size_t colonPos = entry.find(':');
        if (colonPos != std::string::npos) {
            std::string className = entry.substr(0, colonPos);
            std::string seatCount = entry.substr(colonPos + 1);

            // Convert class name to uppercase
            std::string uppercaseClassName = className;
            std::transform(uppercaseClassName.begin(), uppercaseClassName.end(), 
                          uppercaseClassName.begin(), ::toupper);

            if (auto seatClass = SeatClassRegistry::getByName(uppercaseClassName)) {
                classMap[*seatClass] = std::stoi(seatCount);
            }
        }
    }
};

// Parser factory to manage different parsing strategies
class SeatClassMapParserFactory {
private:
    static std::unordered_map<std::string, std::shared_ptr<ISeatClassMapParserStrategy>> _strategies;
    static bool _initialized;

    static void initialize() {
        if (_initialized) return;
        
        // Register default parser strategy
        registerStrategy("standard", std::make_shared<StandardSeatClassMapParser>());
        
        _initialized = true;
    }

public:
    static void registerStrategy(const std::string& name, std::shared_ptr<ISeatClassMapParserStrategy> strategy) {
        _strategies[name] = std::move(strategy);
    }

    static std::shared_ptr<ISeatClassMapParserStrategy> getStrategy(const std::string& name) {
        initialize();
        auto it = _strategies.find(name);
        if (it != _strategies.end()) {
            return it->second;
        }
        return _strategies["standard"]; // Return default strategy if not found
    }
};

// Initialize static members
std::unordered_map<std::string, std::shared_ptr<ISeatClassMapParserStrategy>> SeatClassMapParserFactory::_strategies;
bool SeatClassMapParserFactory::_initialized = false;

// Main parser class that uses strategies
class SeatClassMapParser {
public:
    static std::unordered_map<SeatClass, int> parse(const std::string& value, const std::string& strategyName = "standard") {
        auto strategy = SeatClassMapParserFactory::getStrategy(strategyName);
        return strategy->parse(value);
    }
};

#endif 