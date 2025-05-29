/**
 * @file FlightNumber.h
 * @brief Value object đại diện cho số hiệu chuyến bay trong hệ thống quản lý chuyến bay
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef FLIGHT_NUMBER_H
#define FLIGHT_NUMBER_H

#include <string>
#include "../../exceptions/Result.h"
#include "FlightNumberValidator.h"
#include <functional>

/**
 * @brief Value object đại diện cho số hiệu chuyến bay
 * 
 * FlightNumber đóng gói số hiệu chuyến bay và đảm bảo tính hợp lệ theo chuẩn IATA.
 * Số hiệu phải tuân theo định dạng: 2 chữ cái (mã hãng hàng không) + 1-4 chữ số.
 * Value object này là bất biến và hỗ trợ hash để sử dụng trong containers.
 */
class FlightNumber {
private:
    std::string _value; ///< Giá trị số hiệu chuyến bay được lưu trữ

    /**
     * @brief Constructor riêng tư để đảm bảo tạo instance chỉ thông qua factory method
     * @param value Giá trị số hiệu đã được validate
     */
    explicit FlightNumber(std::string value) : _value(std::move(value)) {}

public:
    /**
     * @brief Constructor mặc định tạo FlightNumber rỗng
     */
    FlightNumber() = default;

    /**
     * @brief Tạo FlightNumber từ chuỗi với validation
     * @param value Chuỗi số hiệu chuyến bay cần validate và tạo object
     * @return Result chứa FlightNumber hợp lệ hoặc lỗi validation
     * 
     * Phương thức này validate định dạng số hiệu chuyến bay:
     * - Không được rỗng
     * - Độ dài từ 3-6 ký tự
     * - Định dạng: 2 chữ cái hoa + số từ 1-9999 (không bắt đầu bằng 0)
     * 
     * Ví dụ hợp lệ: "VN123", "AA1", "QR9999"
     */
    static Result<FlightNumber> create(const std::string& value) {
        auto validationResult = FlightNumberValidator::validate(value);
        if (!validationResult.isValid()) {
            return getValidationFailure<FlightNumber>(validationResult);
        }
        return Success(FlightNumber(value));
    }

    /**
     * @brief Lấy giá trị số hiệu chuyến bay
     * @return Tham chiếu const đến chuỗi số hiệu
     */
    const std::string& value() const { return _value; }

    /**
     * @brief So sánh bằng với FlightNumber khác
     * @param other FlightNumber để so sánh
     * @return true nếu hai số hiệu giống nhau
     */
    bool operator==(const FlightNumber& other) const {
        return _value == other._value;
    }

    /**
     * @brief So sánh khác với FlightNumber khác
     * @param other FlightNumber để so sánh
     * @return true nếu hai số hiệu khác nhau
     */
    bool operator!=(const FlightNumber& other) const {
        return !(*this == other);
    }

    /**
     * @brief Chuyển đổi thành chuỗi để hiển thị
     * @return Chuỗi biểu diễn số hiệu chuyến bay
     */
    std::string toString() const {
        return _value;
    }
};

/**
 * @brief Specialization của std::hash cho FlightNumber
 * 
 * Cho phép sử dụng FlightNumber làm key trong std::unordered_map,
 * std::unordered_set và các container hash-based khác.
 */
namespace std {
    template<>
    struct hash<FlightNumber> {
        /**
         * @brief Tính hash value cho FlightNumber
         * @param flightNumber FlightNumber cần tính hash
         * @return Hash value của số hiệu chuyến bay
         */
        size_t operator()(const FlightNumber& flightNumber) const {
            return hash<string>()(flightNumber.toString());
        }
    };
}

#endif