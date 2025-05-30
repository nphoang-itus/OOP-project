/**
 * @file SeatNumberValidator.h
 * @brief Định nghĩa lớp SeatNumberValidator để xác thực số ghế
 * @author Nguyễn Phúc Hoàng
 */

#ifndef SEAT_NUMBER_VALIDATOR_H
#define SEAT_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include "../../exceptions/ValidationResult.h"
#include "SeatNumberError.h"
#include "../seat_class_map/SeatClassMap.h"

/**
 * @class SeatNumberValidator
 * @brief Cung cấp chức năng xác thực cho số ghế
 * 
 * Lớp này chứa các phương thức tĩnh để xác thực số ghế theo bố trí ghế của máy bay
 * và đảm bảo số ghế đáp ứng các tiêu chí về định dạng và tính hợp lệ.
 */
class SeatNumberValidator {
private:
    /**
     * @brief Kiểm tra xem chuỗi có rỗng không
     * @param value Chuỗi cần kiểm tra
     * @return true nếu chuỗi rỗng, false nếu ngược lại
     */
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    /**
     * @brief Kiểm tra định dạng số ghế có hợp lệ không
     * @param value Số ghế cần kiểm tra
     * @return true nếu định dạng hợp lệ (ví dụ: E001, B01), false nếu không
     */
    static bool isValidFormat(const std::string& value) {
        // Format: E001, B01, etc.
        std::regex pattern("^[A-Z][0-9]{2,3}$");
        return std::regex_match(value, pattern);
    }

    /**
     * @brief Kiểm tra số thứ tự ghế có hợp lệ trong bố trí ghế không
     * @param value Số ghế cần kiểm tra
     * @param seatLayout Bố trí ghế của máy bay
     * @return true nếu số thứ tự hợp lệ, false nếu không
     */
    static bool isValidSequenceNumber(const std::string& value, const SeatClassMap& seatLayout) {
        char classCode = value[0];
        int sequenceNumber = std::stoi(value.substr(1));
        
        // Find the seat class and its count
        for (const auto& [seatClass, count] : seatLayout.getSeatCounts()) {
            if (seatClass.getCode()[0] == classCode) {
                return sequenceNumber > 0 && sequenceNumber <= count;
            }
        }
        return false;
    }

public:
    /**
     * @brief Xác thực số ghế theo bố trí ghế của máy bay
     * @param value Số ghế cần xác thực
     * @param seatLayout Bố trí ghế của máy bay
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::string& value, const SeatClassMap& seatLayout) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::EMPTY_SEAT_NUMBER);
            return result;
        }

        // Validate format
        if (!isValidFormat(value)) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::INVALID_FORMAT);
            return result;
        }

        // Validate class code exists in seat layout
        char classCode = value[0];
        bool classFound = false;
        for (const auto& [seatClass, _] : seatLayout.getSeatCounts()) {
            if (seatClass.getCode()[0] == classCode) {
                classFound = true;
                break;
            }
        }
        if (!classFound) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::INVALID_SEAT_CLASS);
            return result;
        }

        // Validate sequence number
        if (!isValidSequenceNumber(value, seatLayout)) {
            SeatNumberErrorHelper::addError(result, SeatNumberError::INVALID_SEQUENCE_NUMBER);
        }
        
        return result;
    }
};

#endif