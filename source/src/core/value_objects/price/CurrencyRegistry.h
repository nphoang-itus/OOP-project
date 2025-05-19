#ifndef CURRENCY_REGISTRY_H
#define CURRENCY_REGISTRY_H

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <algorithm>

class CurrencyRegistry {
private:
    inline static std::unordered_map<std::string, std::string> _registry;
    inline static bool _initialized = false;

    // Initialize default currencies
    static void initialize() {
        if (_initialized) return;

        // Register default currencies
        registerCurrency("VND", "Vietnamese Dong");
        registerCurrency("USD", "United States Dollar");
        registerCurrency("EUR", "Euro");
        registerCurrency("GBP", "British Pound");
        registerCurrency("JPY", "Japanese Yen");
        registerCurrency("CNY", "Chinese Yuan");
        registerCurrency("KRW", "South Korean Won");
        registerCurrency("SGD", "Singapore Dollar");
        registerCurrency("AUD", "Australian Dollar");
        registerCurrency("CAD", "Canadian Dollar");

        _initialized = true;
    }

    static std::string toUpperCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

public:
    // Register a new currency
    static void registerCurrency(const std::string& code, const std::string& name) {
        _registry[toUpperCase(code)] = name;
    }

    // Get currency name by code
    static std::optional<std::string> getName(const std::string& code) {
        initialize();
        auto it = _registry.find(toUpperCase(code));
        if (it != _registry.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    // Get all registered currency codes
    static std::vector<std::string> getAllCodes() {
        initialize();
        std::vector<std::string> result;
        result.reserve(_registry.size());
        for (const auto& [code, _] : _registry) {
            result.push_back(code);
        }
        return result;
    }

    // Check if currency code is valid
    static bool isValidCurrency(const std::string& code) {
        initialize();
        return _registry.find(toUpperCase(code)) != _registry.end();
    }
};

// Initialize static members
// std::unordered_map<std::string, std::string> CurrencyRegistry::_registry;
// bool CurrencyRegistry::_initialized = false;

#endif 