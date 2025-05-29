/**
 * @file TicketNumberValidator.h
 * @brief Validator cho số vé máy bay theo định dạng chuẩn hàng không
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef TICKET_NUMBER_VALIDATOR_H
#define TICKET_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"
#include "TicketNumberError.h"

/**
 * @brief Lớp validator để kiểm tra tính hợp lệ của số vé máy bay
 * 
 * TicketNumberValidator thực hiện validation theo định dạng chuẩn:
 * - Số vé không được rỗng
 * - Định dạng: MCB-YYYYMMDD-XXXX
 *   - MCB: Mã hãng hàng không (2 chữ cái + 1-4 chữ số)
 *   - YYYYMMDD: Ngày tạo vé (8 chữ số)
 *   - XXXX: Số thứ tự (4 chữ số)
 * 
 * Ví dụ số vé hợp lệ: "VN123-20240315-0001", "AA1-20240315-9999"
 */
class TicketNumberValidator {
private:
    /**
     * @brief Kiểm tra xem chuỗi có rỗng không
     * @param value Chuỗi cần kiểm tra
     * @return true nếu chuỗi rỗng
     */
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    /**
     * @brief Kiểm tra định dạng số vé có hợp lệ không
     * @param value Chuỗi số vé cần kiểm tra
     * @return true nếu định dạng hợp lệ
     * 
     * Sử dụng regex pattern: ^[A-Z]{2}[0-9]{1,4}-[0-9]{8}-[0-9]{4}$
     */
    static bool isValidFormat(const std::string& value) {
        // Format: MCB-YYYYMMDD-XXXX
        std::regex pattern("^[A-Z]{2}[0-9]{1,4}-[0-9]{8}-[0-9]{4}$");
        return std::regex_match(value, pattern);
    }

public:
    /**
     * @brief Validate số vé máy bay theo định dạng chuẩn
     * @param value Chuỗi số vé cần kiểm tra
     * @return ValidationResult chứa kết quả validation và danh sách lỗi nếu có
     * 
     * Quy trình validation:
     * 1. Kiểm tra chuỗi không rỗng
     * 2. Kiểm tra định dạng regex: ^[A-Z]{2}[0-9]{1,4}-[0-9]{8}-[0-9]{4}$
     *    - [A-Z]{2}: 2 chữ cái hoa (mã hãng)
     *    - [0-9]{1,4}: 1-4 chữ số (số hiệu hãng)
     *    - [0-9]{8}: 8 chữ số (ngày tháng)
     *    - [0-9]{4}: 4 chữ số (số thứ tự)
     */
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            TicketNumberErrorHelper::addError(result, TicketNumberError::EMPTY_TICKET_NUMBER);
            return result;
        }

        // Validate format
        if (!isValidFormat(value)) {
            TicketNumberErrorHelper::addError(result, TicketNumberError::INVALID_FORMAT);
        }
        
        return result;
    }
};

#endif