/**
 * @file SeatClass.h
 * @brief Định nghĩa lớp SeatClass và SeatClassRegistry để quản lý hạng ghế
 * @author Nhóm dự án OOP
 */

#ifndef SEAT_CLASS_H
#define SEAT_CLASS_H

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include <functional>

// Forward declaration
class SeatClassRegistry;

/**
 * @class SeatClass
 * @brief Đại diện cho hạng ghế trong máy bay
 * 
 * Lớp này đóng gói thông tin về hạng ghế bao gồm tên và mã hạng ghế.
 * Chỉ có SeatClassRegistry mới có thể tạo instance của lớp này.
 */
class SeatClass {
private:
    std::string _name; ///< Tên hạng ghế
    std::string _code; ///< Mã hạng ghế

    /**
     * @brief Constructor riêng tư - chỉ SeatClassRegistry có thể tạo instance
     * @param name Tên hạng ghế
     * @param code Mã hạng ghế
     */
    SeatClass(std::string name, std::string code) 
        : _name(std::move(name)), _code(std::move(code)) {}

    /**
     * @brief Lớp bạn có thể tạo instance SeatClass
     */
    friend class SeatClassRegistry;

public:
    /**
     * @brief Constructor mặc định cho unordered_map
     */
    SeatClass() = default;

    /**
     * @brief Lấy tên hạng ghế
     * @return Tên hạng ghế
     */
    const std::string& getName() const { return _name; }
    
    /**
     * @brief Lấy mã hạng ghế
     * @return Mã hạng ghế
     */
    const std::string& getCode() const { return _code; }

    /**
     * @brief Toán tử bằng
     * @param other SeatClass khác để so sánh
     * @return true nếu tên hạng ghế bằng nhau, false nếu ngược lại
     */
    bool operator==(const SeatClass& other) const {
        return _name == other._name;
    }

    /**
     * @brief Toán tử không bằng
     * @param other SeatClass khác để so sánh
     * @return true nếu hạng ghế không bằng nhau, false nếu ngược lại
     */
    bool operator!=(const SeatClass& other) const {
        return !(*this == other);
    }
};

/**
 * @namespace std
 * @brief Namespace tiêu chuẩn C++
 */
namespace std {
    /**
     * @struct hash<SeatClass>
     * @brief Chuyên môn hóa template hash cho lớp SeatClass
     * 
     * Cung cấp hàm hash để SeatClass có thể được sử dụng trong
     * các container như unordered_map và unordered_set.
     */
    template<>
    struct hash<SeatClass> {
        /**
         * @brief Tính toán giá trị hash cho SeatClass
         * @param seatClass Đối tượng SeatClass cần tính hash
         * @return Giá trị hash của SeatClass
         */
        size_t operator()(const SeatClass& seatClass) const {
            return hash<string>()(seatClass.getName());
        }
    };
}

/**
 * @class SeatClassRegistry
 * @brief Registry để quản lý các hạng ghế
 * 
 * Lớp này cung cấp một registry tập trung cho các hạng ghế
 * và cho phép tra cứu hạng ghế theo tên hoặc mã.
 */
class SeatClassRegistry {
private:
    inline static std::unordered_map<std::string, SeatClass> _registry; ///< Bộ nhớ lưu trữ registry
    inline static bool _initialized = false; ///< Cờ khởi tạo

    /**
     * @brief Khởi tạo registry với các hạng ghế mặc định
     * 
     * Phương thức này được gọi tự động và thiết lập các hạng ghế
     * mặc định trong registry.
     */
    static void initialize() {
        if (_initialized) return;

        // Register default seat classes
        registerSeatClass("ECONOMY", "E");
        registerSeatClass("BUSINESS", "B");
        registerSeatClass("FIRST", "F");

        _initialized = true;
    }

public:
    /**
     * @brief Đăng ký một hạng ghế mới
     * @param name Tên hạng ghế
     * @param code Mã hạng ghế
     */
    static void registerSeatClass(const std::string& name, const std::string& code) {
        _registry[name] = SeatClass(name, code);
    }

    /**
     * @brief Lấy hạng ghế theo tên
     * @param name Tên hạng ghế cần tìm
     * @return Optional SeatClass nếu tìm thấy, nullopt nếu không
     */
    static std::optional<SeatClass> getByName(const std::string& name) {
        initialize();
        auto it = _registry.find(name);
        if (it != _registry.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    /**
     * @brief Lấy tất cả hạng ghế đã đăng ký
     * @return Vector chứa tất cả SeatClass đã đăng ký
     */
    static std::vector<SeatClass> getAll() {
        initialize();
        std::vector<SeatClass> result;
        result.reserve(_registry.size());
        for (const auto& [_, seatClass] : _registry) {
            result.push_back(seatClass);
        }
        return result;
    }

    /**
     * @brief Lấy hạng ghế theo mã
     * @param code Mã hạng ghế cần tìm
     * @return Optional SeatClass nếu tìm thấy, nullopt nếu không
     */
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

/**
 * @brief Chuyển đổi SeatClass thành chuỗi (để tương thích ngược)
 * @param seatClass Đối tượng SeatClass
 * @return Tên hạng ghế dưới dạng chuỗi
 */
inline std::string seatClassToString(const SeatClass& seatClass) {
    return seatClass.getName();
}

/**
 * @brief Chuyển đổi chuỗi thành SeatClass (để tương thích ngược)
 * @param str Chuỗi tên hạng ghế
 * @return Optional SeatClass nếu tìm thấy, nullopt nếu không
 */
inline std::optional<SeatClass> stringToSeatClass(const std::string& str) {
    return SeatClassRegistry::getByName(str);
}

#endif