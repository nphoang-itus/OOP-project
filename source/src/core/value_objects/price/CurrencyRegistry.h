/**
 * @file CurrencyRegistry.h
 * @brief Định nghĩa lớp CurrencyRegistry để quản lý các loại tiền tệ
 * @author Nhóm dự án OOP
 */

#ifndef CURRENCY_REGISTRY_H
#define CURRENCY_REGISTRY_H

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <algorithm>

/**
 * @class CurrencyRegistry
 * @brief Registry để quản lý các loại tiền tệ và tra cứu thông tin tiền tệ
 * 
 * Lớp này cung cấp một registry tập trung cho các loại tiền tệ
 * và cho phép tra cứu tên tiền tệ, kiểm tra tính hợp lệ của mã tiền tệ.
 */
class CurrencyRegistry {
private:
    inline static std::unordered_map<std::string, std::string> _registry; ///< Bộ nhớ lưu trữ registry
    inline static bool _initialized = false; ///< Cờ khởi tạo

    /**
     * @brief Khởi tạo registry với các tiền tệ mặc định
     * 
     * Phương thức này được gọi tự động và thiết lập các loại
     * tiền tệ mặc định trong registry.
     */
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

    /**
     * @brief Chuyển đổi chuỗi thành chữ hoa
     * @param str Chuỗi cần chuyển đổi
     * @return Chuỗi đã được chuyển thành chữ hoa
     */
    static std::string toUpperCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

public:
    /**
     * @brief Đăng ký một loại tiền tệ mới
     * @param code Mã tiền tệ (sẽ được chuyển thành chữ hoa)
     * @param name Tên tiền tệ
     */
    static void registerCurrency(const std::string& code, const std::string& name) {
        _registry[toUpperCase(code)] = name;
    }

    /**
     * @brief Lấy tên tiền tệ theo mã
     * @param code Mã tiền tệ cần tra cứu
     * @return Optional chứa tên tiền tệ nếu tìm thấy, nullopt nếu không
     */
    static std::optional<std::string> getName(const std::string& code) {
        initialize();
        auto it = _registry.find(toUpperCase(code));
        if (it != _registry.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    /**
     * @brief Lấy tất cả mã tiền tệ đã đăng ký
     * @return Vector chứa tất cả mã tiền tệ
     */
    static std::vector<std::string> getAllCodes() {
        initialize();
        std::vector<std::string> result;
        result.reserve(_registry.size());
        for (const auto& [code, _] : _registry) {
            result.push_back(code);
        }
        return result;
    }

    /**
     * @brief Kiểm tra xem mã tiền tệ có hợp lệ không
     * @param code Mã tiền tệ cần kiểm tra
     * @return true nếu mã tiền tệ hợp lệ, false nếu không
     */
    static bool isValidCurrency(const std::string& code) {
        initialize();
        return _registry.find(toUpperCase(code)) != _registry.end();
    }
};

#endif