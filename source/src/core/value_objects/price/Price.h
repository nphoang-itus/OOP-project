/**
 * @file Price.h
 * @brief Định nghĩa lớp value object Price
 * @author Nhóm dự án OOP
 */

#ifndef PRICE_H
#define PRICE_H

#include <string>
#include <utility>
#include <optional>
#include <algorithm>
#include "../../exceptions/ValidationResult.h"
#include "PriceValidator.h"
#include "PriceParser.h"
#include "PriceFormatter.h"
#include "CurrencyRegistry.h"

/**
 * @class Price
 * @brief Đại diện cho giá tiền cùng với loại tiền tệ
 * 
 * Lớp này là một value object đóng gói số tiền cùng với
 * loại tiền tệ. Nó cung cấp các khả năng xác thực, phân tích và định dạng
 * cho giá tiền.
 */
class Price {
private:
    double _amount;        ///< Số tiền
    std::string _currency; ///< Loại tiền tệ (chữ hoa)

    /**
     * @brief Constructor riêng tư để tạo instance Price
     * @param amount Số tiền
     * @param currency Loại tiền tệ
     */
    Price(const double& amount, const std::string& currency) 
        : _amount(amount) {
        std::string upperCurrency = currency;
        
        // Convert currency to uppercase
        std::transform(upperCurrency.begin(), upperCurrency.end(), upperCurrency.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        
        _currency = upperCurrency;
    }

    /**
     * @brief Phương thức template nội bộ để tạo Price từ các loại đầu vào khác nhau
     * @tparam InputType Loại đầu vào (string hoặc pair)
     * @param input Đầu vào để tạo Price
     * @return Result chứa Price hoặc lỗi
     */
    template<typename InputType>
    static Result<Price> createInternal(const InputType& input) {
        auto validationResult = PriceValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<Price>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::pair<double, std::string>>) {
            const auto& [amount, currency] = input;
            return Success(Price(amount, currency));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = PriceParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<Price>(CoreError("Failed to parse price", "PARSE_ERROR"));
            }
            const auto& [amount, currency] = parsed.value();
            return Success(Price(amount, currency));
        }
        return Failure<Price>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    /**
     * @brief Constructor mặc định
     */
    Price() = default;

    /**
     * @brief Tạo Price từ chuỗi đã định dạng
     * @param value Chuỗi có định dạng "SỐ_TIỀN TIỀN_TỆ"
     * @return Result chứa Price hoặc lỗi xác thực
     */
    static Result<Price> create(const std::string& value) {
        return createInternal(value);    
    }

    /**
     * @brief Tạo Price từ các thành phần riêng biệt
     * @param amount Số tiền
     * @param currency Loại tiền tệ
     * @return Result chứa Price hoặc lỗi xác thực
     */
    static Result<Price> create(const double& amount, const std::string& currency) {
        return createInternal(std::make_pair(amount, currency));
    }

    /**
     * @brief Lấy số tiền
     * @return Số tiền dưới dạng double
     */
    const double& getAmount() const {
        return _amount;
    }

    /**
     * @brief Lấy loại tiền tệ
     * @return Mã tiền tệ (chữ hoa)
     */
    const std::string& getCurrency() const {
        return _currency;
    }

    /**
     * @brief Lấy tên đầy đủ của loại tiền tệ
     * @return Optional chứa tên tiền tệ nếu có, nullopt nếu không tìm thấy
     */
    std::optional<std::string> getCurrencyName() const {
        return CurrencyRegistry::getName(_currency);
    }

    /**
     * @brief Chuyển đổi giá tiền thành định dạng chuỗi
     * @return Chuỗi đã định dạng theo "SỐ_TIỀN TIỀN_TỆ"
     */
    std::string toString() const {
        return PriceFormatter::toString(_amount, _currency);
    }

    /**
     * @brief Toán tử bằng
     * @param other Price khác để so sánh
     * @return true nếu cả hai giá tiền bằng nhau, false nếu ngược lại
     */
    bool operator==(const Price& other) const {
        return _amount == other._amount && _currency == other._currency;
    }

    /**
     * @brief Toán tử không bằng
     * @param other Price khác để so sánh
     * @return true nếu giá tiền không bằng nhau, false nếu ngược lại
     */
    bool operator!=(const Price& other) const {
        return !(*this == other);
    }
};

#endif
