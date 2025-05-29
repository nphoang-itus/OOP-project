/**
 * @file RouteValidator.h
 * @brief Định nghĩa lớp RouteValidator để xác thực tuyến đường bay
 * @author Nhóm dự án OOP
 */

#ifndef ROUTE_VALIDATOR_H
#define ROUTE_VALIDATOR_H

#include <string>
#include <regex>
#include <tuple>
#include "../../exceptions/ValidationResult.h"
#include "RouteError.h"

/**
 * @class RouteValidator
 * @brief Cung cấp chức năng xác thực cho tuyến đường bay
 * 
 * Lớp này chứa các phương thức tĩnh để xác thực tuyến đường bay ở các định dạng khác nhau
 * và đảm bảo chúng đáp ứng các tiêu chí yêu cầu cho mã sân bay và thông tin tuyến đường.
 */
class RouteValidator {
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
     * @brief Kiểm tra xem mã sân bay có hợp lệ không
     * @param code Mã sân bay cần kiểm tra
     * @return true nếu là mã IATA 3 chữ cái hợp lệ, false nếu không
     */
    static bool isValidAirportCode(const std::string& code) {
        // Check if it's a valid 3-letter IATA code
        if (code.length() != 3) return false;
        std::regex codePattern("^[A-Z]{3}$");
        return std::regex_match(code, codePattern);
    }

public:
    /**
     * @brief Xác thực tuyến đường từ định dạng chuỗi kết hợp
     * @param value Chuỗi có định dạng "ĐIỂM_XUẤT_PHÁT(MÃ_XUẤT_PHÁT)-ĐIỂM_ĐẾN(MÃ_ĐIỂM_ĐẾN)"
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            RouteErrorHelper::addError(result, RouteError::EMPTY_ROUTE);
            return result;
        }

        // Validate format and extract components using regex
        std::regex pattern("^([A-Za-z0-9 ]+)\\(([A-Za-z0-9 ]+)\\)-([A-Za-z0-9 ]+)\\(([A-Za-z0-9 ]+)\\)$");
        std::smatch matches;
        
        if (!std::regex_match(value, matches, pattern)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_FORMAT);
            return result;
        }

        // matches[0] contains the full match
        // matches[1] contains the origin city name
        // matches[2] contains the origin code
        // matches[3] contains the destination city name
        // matches[4] contains the destination code

        std::string originCode = matches[2].str();
        std::string destinationCode = matches[4].str();

        // Validate airport codes
        if (!isValidAirportCode(originCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_ORIGIN_CODE);
        }

        if (!isValidAirportCode(destinationCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_DESTINATION_CODE);
        }

        // Check if origin and destination are the same
        if (originCode == destinationCode) {
            RouteErrorHelper::addError(result, RouteError::SAME_ORIGIN_DESTINATION);
        }
        
        return result;
    }

    /**
     * @brief Xác thực tuyến đường từ các thành phần riêng biệt
     * @param input Tuple chứa tên và mã của điểm xuất phát và điểm đến
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::tuple<std::string, std::string, std::string, std::string>& input) {
        ValidationResult result;
        const auto& [origin, originCode, destination, destinationCode] = input;
        
        // Check empty inputs
        if (isEmpty(origin) || isEmpty(originCode) || isEmpty(destination) || isEmpty(destinationCode)) {
            RouteErrorHelper::addError(result, RouteError::EMPTY_ROUTE);
            return result;
        }

        // Validate origin code
        if (!isValidAirportCode(originCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_ORIGIN_CODE);
        }

        // Validate destination code
        if (!isValidAirportCode(destinationCode)) {
            RouteErrorHelper::addError(result, RouteError::INVALID_DESTINATION_CODE);
        }

        // Check if origin and destination are the same
        if (originCode == destinationCode) {
            RouteErrorHelper::addError(result, RouteError::SAME_ORIGIN_DESTINATION);
        }
        
        return result;
    }
};

#endif