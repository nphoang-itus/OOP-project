/**
 * @file AircraftSerial.h
 * @brief Value object đại diện cho số serial máy bay trong hệ thống quản lý chuyến bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef AIRCRAFT_SERIAL_H
#define AIRCRAFT_SERIAL_H

#include <string>
#include "../../exceptions/Result.h"
#include "AircraftSerialValidator.h"

/**
 * @brief Value object đại diện cho số serial duy nhất của máy bay
 * 
 * AircraftSerial đóng gói số serial máy bay và đảm bảo tính hợp lệ của nó.
 * Số serial phải tuân theo định dạng cụ thể: 2-3 chữ cái theo sau bởi 1-7 chữ số.
 * Value object này là bất biến và cung cấp các phương thức factory để tạo instance an toàn.
 */
class AircraftSerial {
private:
    std::string _value; ///< Giá trị số serial máy bay được lưu trữ

    /**
     * @brief Constructor riêng tư để đảm bảo tạo instance chỉ thông qua factory method
     * @param value Giá trị số serial đã được validate
     */
    explicit AircraftSerial(std::string value) : _value(std::move(value)) {}

public:
    /**
     * @brief Constructor mặc định tạo AircraftSerial rỗng
     */
    AircraftSerial() = default;

    /**
     * @brief Tạo AircraftSerial từ chuỗi với validation
     * @param value Chuỗi số serial cần validate và tạo object
     * @return Result chứa AircraftSerial hợp lệ hoặc lỗi validation
     * 
     * Phương thức này validate định dạng số serial:
     * - Không được rỗng
     * - Độ dài từ 3-10 ký tự
     * - Định dạng: 2-3 chữ cái hoa + 1-7 chữ số
     */
    static Result<AircraftSerial> create(const std::string& value) {
        auto validationResult = AircraftSerialValidator::validate(value);
        if (!validationResult.isValid()) {
            return getValidationFailure<AircraftSerial>(validationResult);
        }
        return Success(AircraftSerial(value));
    }

    /**
     * @brief Lấy giá trị số serial
     * @return Tham chiếu const đến chuỗi số serial
     */
    const std::string& value() const { return _value; }

    /**
     * @brief So sánh bằng với AircraftSerial khác
     * @param other AircraftSerial để so sánh
     * @return true nếu hai số serial giống nhau
     */
    bool operator==(const AircraftSerial& other) const {
        return _value == other._value;
    }

    /**
     * @brief So sánh khác với AircraftSerial khác
     * @param other AircraftSerial để so sánh
     * @return true nếu hai số serial khác nhau
     */
    bool operator!=(const AircraftSerial& other) const {
        return !(*this == other);
    }

    /**
     * @brief Chuyển đổi thành chuỗi để hiển thị
     * @return Chuỗi biểu diễn số serial máy bay
     */
    std::string toString() const {
        return _value;
    }
};

#endif