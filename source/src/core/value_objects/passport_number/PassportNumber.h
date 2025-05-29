/**
 * @file PassportNumber.h
 * @brief Định nghĩa lớp value object PassportNumber
 * @author Nhóm dự án OOP
 */

#ifndef PASSPORT_NUMBER_H
#define PASSPORT_NUMBER_H

#include <string>
#include <memory>
#include <utility>
#include <optional>

#include "../../exceptions/ValidationResult.h"
#include "PassportNumberValidator.h"
#include "PassportNumberParser.h"
#include "PassportNumberFormatter.h"

/**
 * @class PassportNumber
 * @brief Đại diện cho số hộ chiếu cùng với quốc gia cấp phát
 * 
 * Lớp này là một value object đóng gói số hộ chiếu cùng với
 * mã quốc gia cấp phát. Nó cung cấp các khả năng xác thực, phân tích và định dạng
 * cho số hộ chiếu.
 */
class PassportNumber {
private:
    std::string _issuingCountry; ///< Mã quốc gia cấp phát (chữ hoa)
    std::string _number;         ///< Số hộ chiếu
    
    /**
     * @brief Constructor riêng tư để tạo instance PassportNumber
     * @param issuingCountry Mã quốc gia cấp phát
     * @param number Số hộ chiếu
     */
    PassportNumber(const std::string& issuingCountry, const std::string& number) 
        : _number(std::move(number)) {
        std::string upperCountryCode = issuingCountry;
        
        // Chuyển đổi countryCode thành chữ hoa
        std::transform(upperCountryCode.begin(), upperCountryCode.end(), upperCountryCode.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        
        _issuingCountry = upperCountryCode;
    }

    /**
     * @brief Phương thức template nội bộ để tạo PassportNumber từ các loại đầu vào khác nhau
     * @tparam InputType Loại đầu vào (string hoặc pair)
     * @param input Đầu vào để tạo PassportNumber
     * @return Result chứa PassportNumber hoặc lỗi
     */
    template<typename InputType>
    static Result<PassportNumber> createInternal(const InputType& input) {
        auto validationResult = PassportNumberValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<PassportNumber>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::pair<std::string, std::string>>) {
            const auto& [number, issuingCountry] = input;
            return Success(PassportNumber(number, issuingCountry));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = PassportNumberParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<PassportNumber>(CoreError("Failed to parse passport number", "PARSE_ERROR"));
            }
            const auto& [number, issuingCountry] = parsed.value();
            return Success(PassportNumber(number, issuingCountry));
        }
        return Failure<PassportNumber>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    /**
     * @brief Constructor mặc định
     */
    PassportNumber() = default;

    /**
     * @brief Tạo PassportNumber từ chuỗi đã định dạng
     * @param value Chuỗi có định dạng "QUỐC_GIA:SỐ"
     * @return Result chứa PassportNumber hoặc lỗi xác thực
     */
    static Result<PassportNumber> create(const std::string& value) {
        return createInternal(value);    
    }

    /**
     * @brief Tạo PassportNumber từ các thành phần riêng biệt
     * @param number Số hộ chiếu
     * @param issuingCountry Mã quốc gia cấp phát
     * @return Result chứa PassportNumber hoặc lỗi xác thực
     */
    static Result<PassportNumber> create(const std::string& number, const std::string& issuingCountry) {
        return createInternal(std::make_pair(number, issuingCountry));
    }

    /**
     * @brief Lấy số hộ chiếu
     * @return Số hộ chiếu dưới dạng chuỗi
     */
    const std::string& getNumber() const {
        return _number;
    }

    /**
     * @brief Lấy mã quốc gia cấp phát
     * @return Mã quốc gia cấp phát (chữ hoa)
     */
    const std::string& getIssuingCountry() const {
        return _issuingCountry;
    }

    /**
     * @brief Chuyển đổi số hộ chiếu thành định dạng chuỗi
     * @return Chuỗi đã định dạng theo "QUỐC_GIA:SỐ"
     */
    std::string toString() const {
        return PassportNumberFormatter::toString(_issuingCountry, _number);
    }

    /**
     * @brief Toán tử bằng
     * @param other PassportNumber khác để so sánh
     * @return true nếu cả hai số hộ chiếu bằng nhau, false nếu ngược lại
     */
    bool operator==(const PassportNumber& other) const {
        return _number == other._number && _issuingCountry == other._issuingCountry;
    }

    /**
     * @brief Toán tử không bằng
     * @param other PassportNumber khác để so sánh
     * @return true nếu số hộ chiếu không bằng nhau, false nếu ngược lại
     */
    bool operator!=(const PassportNumber& other) const {
        return !(*this == other);
    }
};

#endif