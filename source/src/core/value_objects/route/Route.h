/**
 * @file Route.h
 * @brief Định nghĩa lớp value object Route
 * @author Nguyễn Phúc Hoàng
 */

#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include "../../exceptions/ValidationResult.h"
#include "RouteValidator.h"
#include "RouteParser.h"
#include "RouteFormatter.h"

/**
 * @class Route
 * @brief Đại diện cho tuyến đường bay từ điểm xuất phát đến điểm đến
 * 
 * Lớp này là một value object đóng gói thông tin về tuyến đường bay
 * bao gồm tên và mã sân bay của điểm xuất phát và điểm đến.
 */
class Route {
private:
    std::string _origin;           ///< Tên điểm xuất phát
    std::string _originCode;       ///< Mã sân bay điểm xuất phát
    std::string _destination;      ///< Tên điểm đến
    std::string _destinationCode;  ///< Mã sân bay điểm đến
    
    /**
     * @brief Constructor riêng tư để tạo instance Route
     * @param origin Tên điểm xuất phát
     * @param originCode Mã sân bay điểm xuất phát
     * @param destination Tên điểm đến
     * @param destinationCode Mã sân bay điểm đến
     */
    Route(const std::string& origin, const std::string& originCode,
          const std::string& destination, const std::string& destinationCode) 
        : _origin(origin), _originCode(originCode),
          _destination(destination), _destinationCode(destinationCode) {}

    /**
     * @brief Phương thức template nội bộ để tạo Route từ các loại đầu vào khác nhau
     * @tparam InputType Loại đầu vào (string hoặc tuple)
     * @param input Đầu vào để tạo Route
     * @return Result chứa Route hoặc lỗi
     */
    template<typename InputType>
    static Result<Route> createInternal(const InputType& input) {
        auto validationResult = RouteValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<Route>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::tuple<std::string, std::string, std::string, std::string>>) {
            const auto& [origin, originCode, destination, destinationCode] = input;
            return Success(Route(origin, originCode, destination, destinationCode));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = RouteParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<Route>(CoreError("Failed to parse route", "PARSE_ERROR"));
            }
            const auto& [origin, originCode, destination, destinationCode] = parsed.value();
            return Success(Route(origin, originCode, destination, destinationCode));
        }
        return Failure<Route>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    /**
     * @brief Constructor mặc định
     */
    Route() = default;
    
    /**
     * @brief Tạo Route từ chuỗi đã định dạng
     * @param value Chuỗi có định dạng "ĐIỂM_XUẤT_PHÁT(MÃ)-ĐIỂM_ĐẾN(MÃ)"
     * @return Result chứa Route hoặc lỗi xác thực
     */
    static Result<Route> create(const std::string& value) {
        return createInternal(value);    
    }

    /**
     * @brief Tạo Route từ các thành phần riêng biệt
     * @param origin Tên điểm xuất phát
     * @param originCode Mã sân bay điểm xuất phát
     * @param destination Tên điểm đến
     * @param destinationCode Mã sân bay điểm đến
     * @return Result chứa Route hoặc lỗi xác thực
     */
    static Result<Route> create(const std::string& origin, const std::string& originCode,
                              const std::string& destination, const std::string& destinationCode) {
        return createInternal(std::make_tuple(origin, originCode, destination, destinationCode));
    }

    /**
     * @brief Lấy tên điểm xuất phát
     * @return Tên điểm xuất phát
     */
    const std::string& getOrigin() const { return _origin; }
    
    /**
     * @brief Lấy mã sân bay điểm xuất phát
     * @return Mã sân bay điểm xuất phát
     */
    const std::string& getOriginCode() const { return _originCode; }
    
    /**
     * @brief Lấy tên điểm đến
     * @return Tên điểm đến
     */
    const std::string& getDestination() const { return _destination; }
    
    /**
     * @brief Lấy mã sân bay điểm đến
     * @return Mã sân bay điểm đến
     */
    const std::string& getDestinationCode() const { return _destinationCode; }

    /**
     * @brief Chuyển đổi tuyến đường thành định dạng chuỗi
     * @return Chuỗi đã định dạng theo "ĐIỂM_XUẤT_PHÁT(MÃ)-ĐIỂM_ĐẾN(MÃ)"
     */
    std::string toString() const {
        return RouteFormatter::toString(_origin, _originCode, _destination, _destinationCode);
    }

    /**
     * @brief Toán tử bằng
     * @param other Route khác để so sánh
     * @return true nếu cả hai tuyến đường bằng nhau, false nếu ngược lại
     */
    bool operator==(const Route& other) const {
        return _originCode == other._originCode && 
               _destinationCode == other._destinationCode;
    }

    /**
     * @brief Toán tử không bằng
     * @param other Route khác để so sánh
     * @return true nếu tuyến đường không bằng nhau, false nếu ngược lại
     */
    bool operator!=(const Route& other) const {
        return !(*this == other);
    }
};

/**
 * @namespace std
 * @brief Namespace tiêu chuẩn C++
 */
namespace std {
    /**
     * @struct hash<Route>
     * @brief Chuyên môn hóa template hash cho lớp Route
     * 
     * Cung cấp hàm hash để Route có thể được sử dụng trong
     * các container như unordered_map và unordered_set.
     */
    template<>
    struct hash<Route> {
        /**
         * @brief Tính toán giá trị hash cho Route
         * @param route Đối tượng Route cần tính hash
         * @return Giá trị hash của Route
         */
        size_t operator()(const Route& route) const {
            return hash<string>()(route.toString());
        }
    };
}

#endif