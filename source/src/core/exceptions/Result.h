/**
 * @file Result.h
 * @brief Kiểu Result để xử lý lỗi không dùng exception trong hệ thống quản lý chuyến bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <expected>
#include <system_error>

/**
 * @brief Đại diện cho một lỗi xảy ra trong logic nghiệp vụ cốt lõi
 * 
 * CoreError đóng gói thông tin lỗi bao gồm thông báo có thể đọc được
 * và mã lỗi tùy chọn để xử lý lập trình. Struct này được sử dụng
 * trong toàn bộ hệ thống để cung cấp thông tin lỗi chi tiết mà không ném exception.
 */
struct CoreError
{
    std::string message; ///< Mô tả lỗi có thể đọc được
    std::string code;    ///< Mã lỗi tùy chọn để xử lý lập trình (mặc định: rỗng)

    /**
     * @brief Tạo CoreError với thông báo và mã tùy chọn
     * @param message Mô tả lỗi có thể đọc được
     * @param code Mã lỗi tùy chọn để phân loại (mặc định: rỗng)
     */
    CoreError(const std::string &message, const std::string &code = "")
        : message(std::move(message)), code(std::move(code)) {}
};

/**
 * @brief Kiểu Result có thể chứa giá trị kiểu T hoặc CoreError
 * 
 * Template alias này cung cấp cách tiếp cận hàm số để xử lý lỗi sử dụng
 * std::expected. Các thao tác trả về Result<T> thay vì ném exception,
 * cho phép xử lý lỗi rõ ràng và kiểm soát luồng tốt hơn.
 * 
 * @tparam T Kiểu giá trị khi thao tác thành công
 */
template <class T>
using Result = std::expected<T, CoreError>;

/**
 * @brief Kiểu Result cho các thao tác không trả về giá trị
 * 
 * VoidResult đại diện cho kết quả của các thao tác có thể thành công
 * (không có giá trị trả về) hoặc thất bại với CoreError.
 */
using VoidResult = std::expected<void, CoreError>;

/**
 * @brief Tạo Result thành công từ rvalue
 * @tparam T Kiểu của giá trị (tự động suy luận)
 * @param value Giá trị để bao bọc trong Result thành công
 * @return Result chứa giá trị được cung cấp
 */
template <typename T>
inline Result<std::remove_reference_t<T>> Success(T &&value)
{
    return std::expected<std::remove_reference_t<T>, CoreError>(std::forward<T>(value));
}

/**
 * @brief Tạo Result thành công từ lvalue
 * @tparam T Kiểu của giá trị (tự động suy luận)
 * @param value Giá trị để bao bọc trong Result thành công
 * @return Result chứa bản sao của giá trị được cung cấp
 */
template <typename T>
inline Result<std::remove_reference_t<T>> Success(const T &value)
{
    return std::expected<std::remove_reference_t<T>, CoreError>(value);
}

/**
 * @brief Tạo VoidResult thành công
 * @return VoidResult cho biết hoàn thành thành công
 */
inline VoidResult Success()
{
    return std::expected<void, CoreError>();
}

/**
 * @brief Tạo Result thất bại với lỗi được chỉ định
 * @tparam T Kiểu trả về mong đợi của thao tác thất bại
 * @param error CoreError mô tả điều gì đã xảy ra sai
 * @return Result chứa lỗi
 */
template <typename T>
inline Result<T> Failure(CoreError error)
{
    return std::unexpected(std::move(error));
}

/**
 * @brief Tạo VoidResult thất bại với lỗi được chỉ định
 * @param error CoreError mô tả điều gì đã xảy ra sai
 * @return VoidResult chứa lỗi
 */
inline VoidResult Failure(CoreError error)
{
    return std::unexpected(std::move(error));
}

/**
 * @brief Kiểm tra xem Result có chứa lỗi không (toán tử NOT logic)
 * @tparam T Kiểu giá trị của Result
 * @param result Result để kiểm tra
 * @return true nếu Result chứa lỗi, false nếu chứa giá trị
 */
template <class T>
inline bool operator!(const Result<T> &result)
{
    return !result.has_value();
}

/**
 * @brief So sánh trạng thái thành công của Result với giá trị boolean
 * @tparam T Kiểu giá trị của Result
 * @param result Result để kiểm tra
 * @param value Giá trị boolean để so sánh
 * @return true nếu Result.has_value() bằng giá trị boolean được cung cấp
 */
template <class T>
inline bool operator==(const Result<T> &result, bool value)
{
    return result.has_value() == value;
}

#endif