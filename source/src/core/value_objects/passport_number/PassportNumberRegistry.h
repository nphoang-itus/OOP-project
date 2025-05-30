/**
 * @file PassportNumberRegistry.h
 * @brief Định nghĩa lớp PassportNumberRegistry để quản lý các quốc gia cấp hộ chiếu
 * @author Nguyễn Phúc Hoàng
 */

#ifndef PASSPORT_NUMBER_REGISTRY_H
#define PASSPORT_NUMBER_REGISTRY_H

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include "PassportNumber.h"

/**
 * @class PassportNumberRegistry
 * @brief Registry để quản lý các quốc gia cấp hộ chiếu và tra cứu hộ chiếu
 * 
 * Lớp này cung cấp một registry tập trung cho các quốc gia cấp hộ chiếu
 * và cho phép tra cứu hộ chiếu theo số hoặc quốc gia cấp phát.
 */
class PassportNumberRegistry {
private:
    static std::unordered_map<std::string, PassportNumber> _registry; ///< Bộ nhớ lưu trữ registry
    static bool _initialized; ///< Cờ khởi tạo

    /**
     * @brief Khởi tạo registry với các quốc gia mặc định
     * 
     * Phương thức này được gọi tự động và thiết lập các quốc gia
     * cấp hộ chiếu mặc định trong registry.
     */
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
    /**
     * @brief Đăng ký một quốc gia cấp phát mới trong registry
     * @param code Mã quốc gia
     * @param name Tên quốc gia
     */
    static void registerIssuingCountry(const std::string& code, const std::string& name) {
        _registry[code] = PassportNumber::create(code, name).value();
    }

    /**
     * @brief Truy xuất hộ chiếu theo số của nó
     * @param number Số hộ chiếu cần tìm kiếm
     * @return Optional PassportNumber nếu tìm thấy, nullopt nếu không
     */
    static std::optional<PassportNumber> getByNumber(const std::string& number) {
        initialize();
        auto it = _registry.find(number);
        if (it != _registry.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    /**
     * @brief Lấy tất cả các quốc gia cấp hộ chiếu đã đăng ký
     * @return Vector chứa tất cả PassportNumber đã đăng ký
     */
    static std::vector<PassportNumber> getAll() {
        initialize();
        std::vector<PassportNumber> result;
        result.reserve(_registry.size());
        for (const auto& [_, passport] : _registry) {
            result.push_back(passport);
        }
        return result;
    }

    /**
     * @brief Truy xuất hộ chiếu theo mã quốc gia cấp phát
     * @param code Mã quốc gia cần tìm kiếm
     * @return Optional PassportNumber nếu tìm thấy, nullopt nếu không
     */
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