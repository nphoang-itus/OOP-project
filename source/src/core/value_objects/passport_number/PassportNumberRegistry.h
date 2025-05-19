#ifndef PASSPORT_NUMBER_REGISTRY_H
#define PASSPORT_NUMBER_REGISTRY_H

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include "PassportNumber.h"

class PassportNumberRegistry {
private:
    static std::unordered_map<std::string, PassportNumber> _registry;
    static bool _initialized;

    // Initialize default passport issuing countries
    static void initialize() {
        if (_initialized) return;

        // Register default issuing countries
        registerIssuingCountry("USA", "United States");
        registerIssuingCountry("GBR", "United Kingdom");
        registerIssuingCountry("VNM", "Vietnam");
        // Add more countries as needed

        _initialized = true;
    }

public:
    // Register a new issuing country
    static void registerIssuingCountry(const std::string& code, const std::string& name) {
        _registry[code] = PassportNumber(code, name);
    }

    // Get passport by number
    static std::optional<PassportNumber> getByNumber(const std::string& number) {
        initialize();
        auto it = _registry.find(number);
        if (it != _registry.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    // Get all registered issuing countries
    static std::vector<PassportNumber> getAll() {
        initialize();
        std::vector<PassportNumber> result;
        result.reserve(_registry.size());
        for (const auto& [_, passport] : _registry) {
            result.push_back(passport);
        }
        return result;
    }

    // Get passport by issuing country code
    static std::optional<PassportNumber> getByIssuingCountry(const std::string& code) {
        initialize();
        for (const auto& [_, passport] : _registry) {
            if (passport.getIssuingCountry() == code) {
                return passport;
            }
        }
        return std::nullopt;
    }
};

// Initialize static members
std::unordered_map<std::string, PassportNumber> PassportNumberRegistry::_registry;
bool PassportNumberRegistry::_initialized = false;

#endif 