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
constexpr Result<std::remove_reference_t<T>> Success(T&& value) {
    return std::expected<std::remove_reference_t<T>, CoreError>(std::forward<T>(value));
}

template<typename T>
constexpr Result<std::remove_reference_t<T>> Success(const T& value) {
    return std::expected<std::remove_reference_t<T>, CoreError>(value);
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

// Add operator bool for Result type
template<class T>
bool operator!(const Result<T>& result) {
    return !result.has_value();
}

template<class T>
bool operator==(const Result<T>& result, bool value) {
    return result.has_value() == value;
}

#endif