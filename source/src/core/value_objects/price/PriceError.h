/**
 * @file PriceError.h
 * @brief Định nghĩa các lỗi xác thực giá tiền và tiện ích xử lý lỗi
 * @author Nhóm dự án OOP
 */

#ifndef PRICE_ERROR_H
#define PRICE_ERROR_H

#include <string>
#include <unordered_map>
#include "../../exceptions/ValidationResult.h"
#include "CurrencyRegistry.h"

/**
 * @enum PriceError
 * @brief Liệt kê các lỗi xác thực giá tiền có thể xảy ra
 */
enum class PriceError {
    EMPTY_PRICE,      ///< Giá tiền rỗng
    INVALID_FORMAT,   ///< Định dạng giá tiền không hợp lệ
    INVALID_CURRENCY, ///< Loại tiền tệ không hợp lệ
    INVALID_AMOUNT,   ///< Số tiền không hợp lệ
    NEGATIVE_AMOUNT   ///< Số tiền âm
};

/**
 * @struct PriceErrorHelper
 * @brief Struct hỗ trợ để quản lý thông báo lỗi và mã lỗi giá tiền
 * 
 * Struct này cung cấp các phương thức tiện ích để chuyển đổi lỗi giá tiền
 * thành biểu diễn chuỗi và thông báo thân thiện với người dùng.
 */
struct PriceErrorHelper {
    /**
     * @brief Chuyển đổi PriceError thành biểu diễn chuỗi
     * @param error Lỗi cần chuyển đổi
     * @return Biểu diễn chuỗi của mã lỗi
     */
    static std::string toString(PriceError error) {
        static const std::unordered_map<PriceError, std::string> errorMap = {
            {PriceError::EMPTY_PRICE, "EMPTY_PRICE"},
            {PriceError::INVALID_FORMAT, "INVALID_FORMAT"},
            {PriceError::INVALID_CURRENCY, "INVALID_CURRENCY"},
            {PriceError::INVALID_AMOUNT, "INVALID_AMOUNT"},
            {PriceError::NEGATIVE_AMOUNT, "NEGATIVE_AMOUNT"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    /**
     * @brief Lấy thông báo dễ đọc cho PriceError
     * @param error Lỗi cần lấy thông báo
     * @return Thông báo lỗi thân thiện với người dùng
     */
    static std::string getMessage(PriceError error) {
        static const std::unordered_map<PriceError, std::string> messageMap = {
            {PriceError::EMPTY_PRICE, "Price cannot be empty"},
            {PriceError::INVALID_FORMAT, "Price must be in format 'AMOUNT CURRENCY' (amount can use . or , as decimal separator)"},
            {PriceError::INVALID_CURRENCY, "Currency must be one of: " + getSupportedCurrencies()},
            {PriceError::INVALID_AMOUNT, "Amount must be a valid number"},
            {PriceError::NEGATIVE_AMOUNT, "Amount cannot be negative"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Unknown error occurred";
    }

    /**
     * @brief Thêm lỗi giá tiền vào kết quả xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     * @param error Lỗi cần thêm
     */
    static void addError(ValidationResult& result, PriceError error) {
        result.addError(
            "price",
            getMessage(error),
            toString(error)
        );
    }

private:
    /**
     * @brief Lấy danh sách các loại tiền tệ được hỗ trợ
     * @return Chuỗi chứa tất cả mã tiền tệ được hỗ trợ, phân cách bằng dấu phẩy
     */
    static std::string getSupportedCurrencies() {
        auto codes = CurrencyRegistry::getAllCodes();
        std::string result;
        for (size_t i = 0; i < codes.size(); ++i) {
            if (i > 0) result += ", ";
            result += codes[i];
        }
        return result;
    }
};

#endif