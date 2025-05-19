#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <expected>
#include <system_error>


struct CoreError {
    std::string message;
    std::string code;

    CoreError(const std::string& message, const std::string& code = "")
        : message(std::move(message)), code(std::move(code)) {}
};

template<class T>
using Result = std::expected<T, CoreError>;

using VoidResult = std::expected<void, CoreError>;

template<typename T>
constexpr Result<T> Success(T&& value) {
    return std::expected<T, CoreError>(std::forward<T>(value));
}

template<typename T>
constexpr Result<T> Success(const T& value) {
    return std::expected<T, CoreError>(value);
}

constexpr VoidResult Success() {
    return std::expected<void, CoreError>();
}

template<typename T>
constexpr Result<T> Failure(CoreError error) {
    return std::unexpected(std::move(error));
}

constexpr VoidResult Failure(CoreError error) {
    return std::unexpected(std::move(error));
}

#endif