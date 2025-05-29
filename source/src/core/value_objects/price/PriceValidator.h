/**
 * @file PriceValidator.h
 * @brief Định nghĩa lớp PriceValidator để xác thực giá tiền
 * @author Nhóm dự án OOP
 */

#ifndef PRICE_VALIDATOR_H
#define PRICE_VALIDATOR_H

#include <string>
#include <regex>
#include <algorithm>
#include <sstream>
#include "../../exceptions/ValidationResult.h"
#include "PriceError.h"
#include "CurrencyRegistry.h"

/**
 * @class PriceValidator
 * @brief Cung cấp chức năng xác thực cho giá tiền
 * 
 * Lớp này chứa các phương thức tĩnh để xác thực giá tiền ở các định dạng khác nhau
 * và đảm bảo chúng đáp ứng các tiêu chí yêu cầu cho số tiền và loại tiền tệ.
 */
class PriceValidator {
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
     * @brief Kiểm tra xem loại tiền tệ có hợp lệ không
     * @param currency Loại tiền tệ cần kiểm tra
     * @return true nếu tiền tệ hợp lệ, false nếu ngược lại
     */
    static bool isValidCurrency(const std::string& currency) {
        return CurrencyRegistry::isValidCurrency(currency);
    }

    /**
     * @brief Kiểm tra xem số tiền có hợp lệ không
     * @param amount Số tiền cần kiểm tra
     * @return true nếu số tiền không âm, false nếu âm
     */
    static bool isValidAmount(double amount) {
        return amount >= 0;
    }

    /**
     * @brief Chuẩn hóa dấu phân cách thập phân
     * @param value Chuỗi cần chuẩn hóa
     * @return Chuỗi đã được chuẩn hóa với dấu chấm làm phân cách thập phân
     */
    static std::string normalizeDecimalSeparator(const std::string& value) {
        std::string result = value;
        std::replace(result.begin(), result.end(), ',', '.');
        return result;
    }

public:
    /**
     * @brief Xác thực giá tiền từ định dạng chuỗi kết hợp
     * @param value Chuỗi có định dạng "SỐ_TIỀN TIỀN_TỆ"
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            PriceErrorHelper::addError(result, PriceError::EMPTY_PRICE);
            return result;
        }

        // Validate format (AMOUNT CURRENCY)
        // Support both dot and comma as decimal separators
        std::regex pattern("^-?[0-9]+([.,][0-9]+)?\\s+[A-Za-z]+$");
        if (!std::regex_match(value, pattern)) {
            PriceErrorHelper::addError(result, PriceError::INVALID_FORMAT);
            return result;
        }

        // Split and validate components
        std::string normalizedValue = normalizeDecimalSeparator(value);
        std::istringstream iss(normalizedValue);
        double amount;
        std::string currency;
        
        if (iss >> amount >> currency) {
            if (!isValidAmount(amount)) {
                PriceErrorHelper::addError(result, PriceError::NEGATIVE_AMOUNT);
            }

            if (!isValidCurrency(currency)) {
                PriceErrorHelper::addError(result, PriceError::INVALID_CURRENCY);
            }
        } else {
            PriceErrorHelper::addError(result, PriceError::INVALID_AMOUNT);
        }
        
        return result;
    }

    /**
     * @brief Xác thực giá tiền từ các thành phần riêng biệt
     * @param input Cặp chứa số tiền và loại tiền tệ
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::pair<double, std::string>& input) {
        ValidationResult result;
        const auto& [amount, currency] = input;
        
        if (!isValidAmount(amount)) {
            PriceErrorHelper::addError(result, PriceError::NEGATIVE_AMOUNT);
        }

        if (!isValidCurrency(currency)) {
            PriceErrorHelper::addError(result, PriceError::INVALID_CURRENCY);
        }
        
        return result;
    }
};

#endif