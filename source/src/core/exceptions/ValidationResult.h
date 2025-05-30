/**
 * @file ValidationResult.h
 * @brief Hệ thống kết quả validation để thu thập và quản lý nhiều lỗi validation
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef VALIDATION_RESULT_H
#define VALIDATION_RESULT_H

#include "Result.h"
#include <vector>
#include <string>

/**
 * @brief Đại diện cho một lỗi validation đơn lẻ cho một trường cụ thể
 *
 * ValidationError đóng gói thông tin về một lỗi validation,
 * bao gồm trường nào thất bại validation, thông báo mô tả,
 * và mã lỗi tùy chọn để xử lý lập trình.
 */
struct ValidationError
{
    std::string fieldName;  ///< Tên trường thất bại validation
    std::string message;    ///< Mô tả lỗi validation có thể đọc được
    std::string errorCode;  ///< Mã lỗi tùy chọn để xử lý lập trình

    /**
     * @brief Tạo ValidationError với tên trường, thông báo và mã lỗi tùy chọn
     * @param fieldName Tên trường thất bại validation
     * @param message Mô tả lỗi có thể đọc được
     * @param errorCode Mã lỗi tùy chọn để phân loại (mặc định: rỗng)
     */
    ValidationError(std::string fieldName, std::string message, std::string errorCode = "")
        : fieldName(std::move(fieldName)), message(std::move(message)), errorCode(std::move(errorCode)) {}
};

/**
 * @brief Thu thập và quản lý nhiều lỗi validation
 *
 * ValidationResult cho phép tích lũy nhiều lỗi validation trong
 * quá trình validation, cung cấp cái nhìn tổng thể về tất cả các
 * lỗi validation thay vì dừng lại ở lỗi đầu tiên. Điều này đặc biệt
 * hữu ích cho validation form và các tình huống validation đối tượng phức tạp.
 */
class ValidationResult
{
private:
    std::vector<ValidationError> _errors; ///< Bộ sưu tập các lỗi validation

public:
    /**
     * @brief Default constructor tạo kết quả validation rỗng
     */
    ValidationResult() = default;

    /**
     * @brief Thêm một lỗi validation vào kết quả
     * @param error ValidationError để thêm vào bộ sưu tập
     */
    void addError(ValidationError error)
    {
        _errors.push_back(std::move(error));
    }

    /**
     * @brief Thêm một lỗi validation sử dụng các thành phần riêng lẻ
     * @param fieldName Tên trường thất bại validation
     * @param message Mô tả lỗi có thể đọc được
     * @param errorCode Mã lỗi tùy chọn để phân loại (mặc định: rỗng)
     */
    void addError(const std::string &fieldName, const std::string &message, const std::string &errorCode = "")
    {
        _errors.emplace_back(fieldName, message, errorCode);
    }

    /**
     * @brief Kiểm tra xem validation có vượt qua không (không có lỗi)
     * @return true nếu không có lỗi validation nào được ghi nhận, false nếu ngược lại
     */
    bool isValid() const
    {
        return _errors.empty();
    }

    /**
     * @brief Lấy tất cả lỗi validation
     * @return Tham chiếu const đến vector các đối tượng ValidationError
     */
    const std::vector<ValidationError> &getErrors() const
    {
        return _errors;
    }

    /**
     * @brief Lấy chuỗi định dạng chứa tất cả thông báo lỗi
     * @return Chuỗi các thông báo lỗi phân cách bằng dấu chấm phẩy, rỗng nếu không có lỗi
     */
    std::string getErrorMessages() const
    {
        if (_errors.empty())
            return "";

        std::string result;
        for (size_t i = 0; i < _errors.size(); ++i)
        {
            if (i > 0)
                result += "; ";
            result += _errors[i].fieldName + ": " + _errors[i].message;
        }
        return result;
    }

    /**
     * @brief Kết hợp kết quả validation này với một kết quả khác
     * @param other ValidationResult để hợp nhất với kết quả này
     *
     * Tất cả lỗi từ ValidationResult khác sẽ được thêm vào kết quả này,
     * cho phép validation phân cấp trong đó các sub-validation có thể được
     * kết hợp vào kết quả validation cha.
     */
    void combine(const ValidationResult &other)
    {
        _errors.insert(_errors.end(), other._errors.begin(), other._errors.end());
    }
};

/**
 * @brief Chuyển đổi ValidationResult thành VoidResult
 * @param validation ValidationResult để chuyển đổi
 * @return Success() nếu validation hợp lệ, Failure với thông báo lỗi kết hợp nếu không hợp lệ
 *
 * Hàm này kết nối hệ thống validation với hệ thống Result,
 * cho phép kết quả validation được sử dụng trong chuỗi xử lý lỗi dựa trên Result.
 */
inline VoidResult toResult(const ValidationResult &validation)
{
    if (validation.isValid())
    {
        return Success();
    }
    return Failure(CoreError(validation.getErrorMessages(), "VALIDATION_ERROR"));
}

/**
 * @brief Tạo Result<T> thất bại từ ValidationResult
 * @tparam T Kiểu trả về mong đợi của thao tác thất bại
 * @param validationResult ValidationResult chứa các lỗi validation
 * @return Result<T> chứa CoreError với thông báo lỗi và mã kết hợp
 *
 * Hàm này chuyển đổi lỗi validation thành lỗi Result có kiểu,
 * kết hợp tất cả mã lỗi thành một chuỗi phân cách bằng dấu chấm phẩy cho trường code của CoreError.
 */
template <typename T>
Result<T> getValidationFailure(const ValidationResult &validationResult)
{
    std::string typeErrors;
    for (size_t i = 0; i < validationResult.getErrors().size(); ++i)
    {
        if (i > 0)
            typeErrors += ';';
        typeErrors += validationResult.getErrors()[i].errorCode;
    }
    return std::unexpected(CoreError(
        validationResult.getErrorMessages(),
        typeErrors));
}

#endif