/**
 * @file SeatClassMapValidator.h
 * @brief Định nghĩa lớp SeatClassMapValidator để xác thực bản đồ hạng ghế
 * @author Nhóm dự án OOP
 */

#ifndef SEAT_CLASS_MAP_VALIDATOR_H
#define SEAT_CLASS_MAP_VALIDATOR_H

#include <string>
#include <regex>
#include <unordered_map>
#include <algorithm>
#include <cctype>

#include "../../exceptions/ValidationResult.h"
#include "SeatClass.h"

/**
 * @enum SeatClassMapError
 * @brief Liệt kê các lỗi xác thực bản đồ hạng ghế có thể xảy ra
 */
enum class SeatClassMapError {
    EMPTY_SEAT_CLASS_MAP,  ///< Bản đồ hạng ghế rỗng
    INVALID_FORMAT,       ///< Định dạng không hợp lệ
    INVALID_SEAT_TYPE,    ///< Loại ghế không hợp lệ
    INVALID_SEAT_COUNT,   ///< Số lượng ghế không hợp lệ
    INVALID_CASE_MIXING   ///< Lỗi trộn lẫn chữ hoa và chữ thường
};

/**
 * @struct SeatClassMapErrorHelper
 * @brief Struct hỗ trợ để quản lý thông báo lỗi và mã lỗi bản đồ hạng ghế
 * 
 * Struct này cung cấp các phương thức tiện ích để chuyển đổi lỗi bản đồ hạng ghế
 * thành biểu diễn chuỗi và thông báo thân thiện với người dùng.
 */
struct SeatClassMapErrorHelper {
    /**
     * @brief Chuyển đổi SeatClassMapError thành biểu diễn chuỗi
     * @param error Lỗi cần chuyển đổi
     * @return Biểu diễn chuỗi của mã lỗi
     */
    static std::string toString(SeatClassMapError error) {
        static const std::unordered_map<SeatClassMapError, std::string> errorMap = {
            {SeatClassMapError::EMPTY_SEAT_CLASS_MAP, "EMPTY_SEAT_CLASS_MAP"},
            {SeatClassMapError::INVALID_FORMAT, "INVALID_FORMAT"},
            {SeatClassMapError::INVALID_SEAT_TYPE, "INVALID_SEAT_TYPE"},
            {SeatClassMapError::INVALID_SEAT_COUNT, "INVALID_SEAT_COUNT"},
            {SeatClassMapError::INVALID_CASE_MIXING, "INVALID_CASE_MIXING"}
        };
        
        auto it = errorMap.find(error);
        return it != errorMap.end() ? it->second : "UNKNOWN_ERROR";
    }
    
    /**
     * @brief Lấy thông báo dễ đọc cho SeatClassMapError
     * @param error Lỗi cần lấy thông báo
     * @return Thông báo lỗi thân thiện với người dùng
     */
    static std::string getMessage(SeatClassMapError error) {
        static const std::unordered_map<SeatClassMapError, std::string> messageMap = {
            {SeatClassMapError::EMPTY_SEAT_CLASS_MAP, "Seat class map cannot be empty"},
            {SeatClassMapError::INVALID_FORMAT, "Seat class map must be in format 'CLASS_NAME:SEAT_COUNT,CLASS_NAME:SEAT_COUNT,...'"},
            {SeatClassMapError::INVALID_SEAT_TYPE, "Seat type name must be one of: ECONOMY, BUSINESS, FIRST"},
            {SeatClassMapError::INVALID_SEAT_COUNT, "Seat count must be a positive number"},
            {SeatClassMapError::INVALID_CASE_MIXING, "Seat class name must be all uppercase or all lowercase"}
        };
        
        auto it = messageMap.find(error);
        return it != messageMap.end() ? it->second : "Unknown error occurred";
    }

    /**
     * @brief Thêm lỗi bản đồ hạng ghế vào kết quả xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     * @param error Lỗi cần thêm
     */
    static void addError(ValidationResult& result, SeatClassMapError error) {
        result.addError(
            "seatClassMap",
            getMessage(error),
            toString(error)
        );
    }
};

/**
 * @class SeatClassMapValidator
 * @brief Cung cấp chức năng xác thực cho bản đồ hạng ghế
 * 
 * Lớp này chứa các phương thức tĩnh để xác thực bản đồ hạng ghế ở các định dạng khác nhau
 * và đảm bảo chúng đáp ứng các tiêu chí yêu cầu cho tên hạng ghế và số lượng ghế.
 */
class SeatClassMapValidator {
private:
    /**
     * @brief Phương thức template nội bộ cho quy trình xác thực
     * @tparam InputType Loại đầu vào
     * @param input Đầu vào cần xác thực
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    template<typename InputType>
    static ValidationResult validateInternal(const InputType& input) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(input)) {
            SeatClassMapErrorHelper::addError(result, SeatClassMapError::EMPTY_SEAT_CLASS_MAP);
            return result;
        }

        // Validate specific input type
        validateSpecific(input, result);
        
        return result;
    }

    /**
     * @brief Kiểm tra xem chuỗi có rỗng không
     * @param value Chuỗi cần kiểm tra
     * @return true nếu chuỗi rỗng, false nếu ngược lại
     */
    static bool isEmpty(const std::string& value) {
        return value.empty();
    }

    /**
     * @brief Kiểm tra xem map có rỗng không
     * @param seatMap Map cần kiểm tra
     * @return true nếu map rỗng, false nếu ngược lại
     */
    static bool isEmpty(const std::unordered_map<SeatClass, int>& seatMap) {
        return seatMap.empty();
    }

    /**
     * @brief Xác thực chuỗi định dạng cụ thể
     * @param value Chuỗi cần xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     */
    static void validateSpecific(const std::string& value, ValidationResult& result) {
        // Validate format
        std::regex pattern("^([A-Za-z0-9]+:[-]?[0-9]+)(,[A-Za-z0-9]+:[-]?[0-9]+)*$");
        if (!std::regex_match(value, pattern)) {
            SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_FORMAT);
            return;
        }

        // Validate each class entry
        std::string input = value;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        
        while ((pos = input.find(delimiter)) != std::string::npos) {
            token = input.substr(0, pos);
            validateClassEntry(token, result);
            input.erase(0, pos + delimiter.length());
        }
        validateClassEntry(input, result);
    }

    /**
     * @brief Xác thực map hạng ghế cụ thể
     * @param seatMap Map hạng ghế cần xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     */
    static void validateSpecific(const std::unordered_map<SeatClass, int>& seatMap, ValidationResult& result) {
        for (const auto& [seatClass, count] : seatMap) {
            if (count <= 0) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_COUNT);
            }
        }
    }

    /**
     * @brief Kiểm tra xem chuỗi có toàn chữ hoa không
     * @param str Chuỗi cần kiểm tra
     * @return true nếu toàn chữ hoa, false nếu không
     */
    static bool isAllUppercase(const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return !std::isalpha(c) || std::isupper(c);
        });
    }
    
    /**
     * @brief Kiểm tra xem chuỗi có toàn chữ thường không
     * @param str Chuỗi cần kiểm tra
     * @return true nếu toàn chữ thường, false nếu không
     */
    static bool isAllLowercase(const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return !std::isalpha(c) || std::islower(c);
        });
    }

    /**
     * @brief Xác thực một mục hạng ghế
     * @param entry Mục hạng ghế cần xác thực
     * @param result Kết quả xác thực để thêm lỗi vào
     */
    static void validateClassEntry(const std::string& entry, ValidationResult& result) {
        size_t colonPos = entry.find(':');
        if (colonPos != std::string::npos) {
            std::string className = entry.substr(0, colonPos);
            std::string seatCount = entry.substr(colonPos + 1);

            // Validate case mixing
            if (!isAllUppercase(className) && !isAllLowercase(className)) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_CASE_MIXING);
                return;
            }

            // Convert to uppercase for validation
            std::string uppercaseClassName = className;
            std::transform(uppercaseClassName.begin(), uppercaseClassName.end(), 
                          uppercaseClassName.begin(), ::toupper);

            // Validate class name
            if (!SeatClassRegistry::getByName(uppercaseClassName)) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_TYPE);
            }

            // Validate seat count
            try {
                int count = std::stoi(seatCount);
                if (count <= 0) {
                    SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_COUNT);
                }
            } catch (...) {
                SeatClassMapErrorHelper::addError(result, SeatClassMapError::INVALID_SEAT_COUNT);
            }
        }
    }

public:
    /**
     * @brief Xác thực bản đồ hạng ghế từ chuỗi
     * @param value Chuỗi có định dạng "TÊN_HẠNG:SỐ_LƯỢNG,TÊN_HẠNG:SỐ_LƯỢNG,..."
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::string& value) {
        return validateInternal(value);
    }

    /**
     * @brief Xác thực bản đồ hạng ghế từ map
     * @param seatMap Map chứa hạng ghế và số lượng
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::unordered_map<SeatClass, int>& seatMap) {
        return validateInternal(seatMap);
    }
};

#endif