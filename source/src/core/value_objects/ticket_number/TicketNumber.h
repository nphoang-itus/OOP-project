/**
 * @file TicketNumber.h
 * @brief Value object đại diện cho số vé máy bay trong hệ thống quản lý chuyến bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef TICKET_NUMBER_H
#define TICKET_NUMBER_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include "../../exceptions/ValidationResult.h"
#include "TicketNumberValidator.h"

/**
 * @brief Value object đại diện cho số vé máy bay duy nhất
 * 
 * TicketNumber đóng gói số vé máy bay và đảm bảo tính hợp lệ theo định dạng chuẩn.
 * Số vé phải tuân theo định dạng: MCB-YYYYMMDD-XXXX (Mã hãng + Ngày + Số thứ tự).
 * Value object này là bất biến và cung cấp các phương thức factory để tạo instance an toàn.
 */
class TicketNumber {
private:
    std::string _value; ///< Giá trị số vé được lưu trữ
    
    /**
     * @brief Constructor riêng tư để đảm bảo tạo instance chỉ thông qua factory method
     * @param value Giá trị số vé đã được validate
     */
    TicketNumber(const std::string& value) : _value(value) {}

    /**
     * @brief Template method cho quy trình tạo TicketNumber
     * @param value Chuỗi số vé cần validate và tạo object
     * @return Result chứa TicketNumber hợp lệ hoặc lỗi validation
     */
    static Result<TicketNumber> createInternal(const std::string& value) {
        auto validationResult = TicketNumberValidator::validate(value);
        if (!validationResult.isValid()) {
            return getValidationFailure<TicketNumber>(validationResult);
        }
        return Success(TicketNumber(value));
    }

public:
    /**
     * @brief Constructor mặc định tạo TicketNumber rỗng
     */
    TicketNumber() = default;

    /**
     * @brief Tạo TicketNumber từ chuỗi với validation
     * @param value Chuỗi số vé cần validate và tạo object
     * @return Result chứa TicketNumber hợp lệ hoặc lỗi validation
     * 
     * Phương thức này validate định dạng số vé:
     * - Không được rỗng
     * - Định dạng: MCB-YYYYMMDD-XXXX
     *   - MCB: 2 chữ cái + 1-4 chữ số (mã hãng hàng không)
     *   - YYYYMMDD: 8 chữ số (ngày tạo vé)
     *   - XXXX: 4 chữ số (số thứ tự)
     * 
     * Ví dụ hợp lệ: "VN123-20240315-0001", "AA1-20240315-9999"
     */
    static Result<TicketNumber> create(const std::string& value) {
        return createInternal(value);
    }

    /**
     * @brief Lấy giá trị số vé
     * @return Tham chiếu const đến chuỗi số vé
     */
    const std::string& getValue() const { return _value; }

    /**
     * @brief Chuyển đổi thành chuỗi để hiển thị
     * @return Chuỗi biểu diễn số vé
     */
    std::string toString() const {
        return _value;
    }

    /**
     * @brief So sánh bằng với TicketNumber khác
     * @param other TicketNumber để so sánh
     * @return true nếu hai số vé giống nhau
     */
    bool operator==(const TicketNumber& other) const {
        return _value == other._value;
    }

    /**
     * @brief So sánh khác với TicketNumber khác
     * @param other TicketNumber để so sánh
     * @return true nếu hai số vé khác nhau
     */
    bool operator!=(const TicketNumber& other) const {
        return !(*this == other);
    }
};

#endif
