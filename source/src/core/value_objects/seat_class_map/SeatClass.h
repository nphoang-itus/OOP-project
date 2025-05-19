#ifndef SEAT_CLASS_H
#define SEAT_CLASS_H

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include <functional>

// Forward declaration
class SeatClassRegistry;

class SeatClass {
private:
    std::string _name;
    std::string _code;

    // Private constructor - only SeatClassRegistry can create instances
    SeatClass(std::string name, std::string code) 
        : _name(std::move(name)), _code(std::move(code)) {}

    // Friend class that can create SeatClass instances
    friend class SeatClassRegistry;

public:
    // Default constructor for unordered_map
    SeatClass() = default;

    // Getters
    const std::string& getName() const { return _name; }
    const std::string& getCode() const { return _code; }

    // Comparison operators
    bool operator==(const SeatClass& other) const {
        return _name == other._name;
    }

    bool operator!=(const SeatClass& other) const {
        return !(*this == other);
    }
};

// Hash function for SeatClass
namespace std {
    template<>
    struct hash<SeatClass> {
        size_t operator()(const SeatClass& seatClass) const {
            return hash<string>()(seatClass.getName());
        }
    };
}

// Registry class to manage seat classes
class SeatClassRegistry {
private:
    inline static std::unordered_map<std::string, SeatClass> _registry;
    inline static bool _initialized = false;

    // Initialize default seat classes
    static void initialize() {
        if (_initialized) return;

        // Register default seat classes
        registerSeatClass("ECONOMY", "E");
        registerSeatClass("BUSINESS", "B");
        registerSeatClass("FIRST", "F");

        _initialized = true;
    }

public:
    // Register a new seat class
    static void registerSeatClass(const std::string& name, const std::string& code) {
        _registry[name] = SeatClass(name, code);
    }

    // Get seat class by name
    static std::optional<SeatClass> getByName(const std::string& name) {
        initialize();
        auto it = _registry.find(name);
        if (it != _registry.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    // Get all registered seat classes
    static std::vector<SeatClass> getAll() {
        initialize();
        std::vector<SeatClass> result;
        result.reserve(_registry.size());
        for (const auto& [_, seatClass] : _registry) {
            result.push_back(seatClass);
        }
        return result;
    }

    // Get seat class by code
    static std::optional<SeatClass> getByCode(const std::string& code) {
        initialize();
        for (const auto& [_, seatClass] : _registry) {
            if (seatClass.getCode() == code) {
                return seatClass;
            }
        }
        return std::nullopt;
    }
};

// // Initialize static members
// std::unordered_map<std::string, SeatClass> SeatClassRegistry::_registry;
// bool SeatClassRegistry::_initialized = false;

// Helper functions for backward compatibility
inline std::string seatClassToString(const SeatClass& seatClass) {
    return seatClass.getName();
}

inline std::optional<SeatClass> stringToSeatClass(const std::string& str) {
    return SeatClassRegistry::getByName(str);
}

#endif 