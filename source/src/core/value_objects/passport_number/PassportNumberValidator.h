/**
 * @file PassportNumberValidator.h
 * @brief Định nghĩa lớp PassportNumberValidator để xác thực số hộ chiếu
 * @author Nguyễn Phúc Hoàng
 */

#ifndef PASSPORT_NUMBER_VALIDATOR_H
#define PASSPORT_NUMBER_VALIDATOR_H

#include <string>
#include <regex>
#include <algorithm>
#include "../../exceptions/ValidationResult.h"
#include "PassportNumberError.h"

/**
 * @class PassportNumberValidator
 * @brief Cung cấp chức năng xác thực cho số hộ chiếu
 * 
 * Lớp này chứa các phương thức tĩnh để xác thực số hộ chiếu ở các định dạng khác nhau
 * và đảm bảo chúng đáp ứng các tiêu chí yêu cầu cho mã quốc gia và định dạng số.
 */
class PassportNumberValidator {
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
     * @brief Xác thực xem mã quốc gia có đúng định dạng không
     * @param countryCode Mã quốc gia cần xác thực
     * @return true nếu hợp lệ (mã 2-3 chữ cái), false nếu ngược lại
     */
    static bool isValidCountryCode(const std::string& countryCode) {
        // Check if it's a 2,3-letter country code
        if (countryCode.length() != 2) return false;
        std::regex countryPattern("^[A-Za-z]{2,3}$");
        return std::regex_match(countryCode, countryPattern);
    }

    /**
     * @brief Xác thực xem số hộ chiếu có đúng định dạng không
     * @param number Số hộ chiếu cần xác thực
     * @return true nếu hợp lệ (6-9 chữ số), false nếu ngược lại
     */
    static bool isValidPassportNumber(const std::string& number) {
        // Check if it's 6-9 digits
        std::regex numberPattern("^[0-9]{6,9}$");
        return std::regex_match(number, numberPattern);
    }

public:
    /**
     * @brief Xác thực số hộ chiếu từ định dạng chuỗi kết hợp
     * @param value Chuỗi có định dạng "QUỐC_GIA:SỐ"
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::EMPTY_PASSPORT_NUMBER);
            return result;
        }

        // Validate format (COUNTRY:NUMBER)
        std::regex pattern("^[A-Za-z0-9]{1,3}:[0-9]+$");
        if (!std::regex_match(value, pattern)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_FORMAT);
            return result;
        }

        // Split and validate components
        size_t colonPos = value.find(':');
        if (colonPos != std::string::npos) {
            std::string countryCode = value.substr(0, colonPos);
            std::string number = value.substr(colonPos + 1);

            if (!isValidCountryCode(countryCode)) {
                PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_ISSUING_COUNTRY);
            }

            if (!isValidPassportNumber(number)) {
                PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_NUMBER_LENGTH);
            }
        }
        
        return result;
    }

    /**
     * @brief Xác thực số hộ chiếu từ các thành phần riêng biệt
     * @param input Cặp chứa mã quốc gia và số
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::pair<std::string, std::string>& input) {
        ValidationResult result;
        const auto& [countryCode, number] = input;
        
        // Check empty inputs
        if (isEmpty(countryCode) || isEmpty(number)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::EMPTY_PASSPORT_NUMBER);
            return result;
        }

        // Validate country code
        if (!isValidCountryCode(countryCode)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_ISSUING_COUNTRY);
        }

        // Validate passport number
        if (!isValidPassportNumber(number)) {
            PassportNumberErrorHelper::addError(result, PassportNumberError::INVALID_NUMBER_LENGTH);
        }
        
        return result;
    }
};

#endif