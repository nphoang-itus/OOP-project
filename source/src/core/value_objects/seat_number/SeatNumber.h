/**
 * @file SeatNumber.h
 * @brief Định nghĩa lớp value object SeatNumber
 * @author Nguyễn Phúc Hoàng
 */

#ifndef SEAT_NUMBER_H
#define SEAT_NUMBER_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include <functional>

#include "../../exceptions/ValidationResult.h"
#include "SeatNumberValidator.h"
#include "../seat_class_map/SeatClassMap.h"

/**
 * @class SeatNumber
 * @brief Đại diện cho số ghế trong máy bay
 * 
 * Lớp này là một value object đóng gói thông tin về số ghế
 * bao gồm mã hạng ghế và số thứ tự, được xác thực theo bố trí ghế của máy bay.
 */
class SeatNumber {
private:
    std::string _value; ///< Giá trị số ghế (ví dụ: "E001", "B02")
    std::shared_ptr<const SeatClassMap> _seatLayout; ///< Con trỏ đến bố trí ghế của máy bay
    
    /**
     * @brief Constructor riêng tư để tạo instance SeatNumber
     * @param value Giá trị số ghế
     * @param seatLayout Con trỏ đến bố trí ghế
     */
    SeatNumber(const std::string& value, std::shared_ptr<const SeatClassMap> seatLayout) 
        : _value(value), _seatLayout(std::move(seatLayout)) {}

    /**
     * @brief Phương thức nội bộ để tạo SeatNumber
     * @param value Giá trị số ghế cần tạo
     * @param seatLayout Con trỏ đến bố trí ghế
     * @return Result chứa SeatNumber hoặc lỗi xác thực
     */
    static Result<SeatNumber> createInternal(const std::string& value, 
                                           std::shared_ptr<const SeatClassMap> seatLayout) {
        auto validationResult = SeatNumberValidator::validate(value, *seatLayout);
        if (!validationResult.isValid()) {
            return getValidationFailure<SeatNumber>(validationResult);
        }
        return Success(SeatNumber(value, seatLayout));
    }

public:
    /**
     * @brief Constructor mặc định
     */
    SeatNumber() : _seatLayout(nullptr) {}
    
    /**
     * @brief Tạo SeatNumber từ giá trị và bố trí ghế
     * @param value Giá trị số ghế (ví dụ: "E001", "B02")
     * @param seatLayout Bố trí ghế của máy bay
     * @return Result chứa SeatNumber hoặc lỗi xác thực
     */
    static Result<SeatNumber> create(const std::string& value, const SeatClassMap& seatLayout) {
        return createInternal(value, std::make_shared<SeatClassMap>(seatLayout));
    }

    /**
     * @brief Lấy giá trị số ghế
     * @return Giá trị số ghế dưới dạng chuỗi
     */
    const std::string& getValue() const { return _value; }
    
    /**
     * @brief Lấy mã hạng ghế
     * @return Ký tự đầu tiên của số ghế (mã hạng)
     */
    char getClassCode() const { return _value[0]; }
    
    /**
     * @brief Lấy số thứ tự ghế
     * @return Số thứ tự ghế trong hạng
     */
    int getSequenceNumber() const { return std::stoi(_value.substr(1)); }
    
    /**
     * @brief Lấy bố trí ghế của máy bay
     * @return Reference đến SeatClassMap
     */
    const SeatClassMap& getSeatLayout() const { 
        return *_seatLayout; 
    }

    /**
     * @brief Chuyển đổi số ghế thành chuỗi
     * @return Chuỗi biểu diễn số ghế
     */
    std::string toString() const {
        return _value;
    }

    /**
     * @brief Toán tử bằng
     * @param other SeatNumber khác để so sánh
     * @return true nếu hai số ghế bằng nhau, false nếu ngược lại
     */
    bool operator==(const SeatNumber& other) const {
        return _value == other._value;
    }

    /**
     * @brief Toán tử không bằng
     * @param other SeatNumber khác để so sánh
     * @return true nếu hai số ghế không bằng nhau, false nếu ngược lại
     */
    bool operator!=(const SeatNumber& other) const {
        return !(*this == other);
    }

    /**
     * @brief Toán tử nhỏ hơn (để sử dụng trong container có thứ tự)
     * @param other SeatNumber khác để so sánh
     * @return true nếu số ghế này nhỏ hơn số ghế kia, false nếu ngược lại
     */
    bool operator<(const SeatNumber& other) const {
        return _value < other._value;
    }
};

/**
 * @namespace std
 * @brief Namespace tiêu chuẩn C++
 */
namespace std {
    /**
     * @struct hash<SeatNumber>
     * @brief Chuyên môn hóa template hash cho lớp SeatNumber
     * 
     * Cung cấp hàm hash để SeatNumber có thể được sử dụng trong
     * các container như unordered_map và unordered_set.
     */
    template<>
    struct hash<SeatNumber> {
        /**
         * @brief Tính toán giá trị hash cho SeatNumber
         * @param seatNumber Đối tượng SeatNumber cần tính hash
         * @return Giá trị hash của SeatNumber
         */
        size_t operator()(const SeatNumber& seatNumber) const {
            return hash<string>()(seatNumber.toString());
        }
    };
}

#endif