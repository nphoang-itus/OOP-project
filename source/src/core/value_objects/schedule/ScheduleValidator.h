/**
 * @file ScheduleValidator.h
 * @brief Định nghĩa lớp ScheduleValidator để xác thực lịch trình bay
 * @author Nhóm dự án OOP
 */

#ifndef SCHEDULE_VALIDATOR_H
#define SCHEDULE_VALIDATOR_H

#include <string>
#include <regex>
#include <ctime>
#include "../../exceptions/ValidationResult.h"
#include "ScheduleError.h"
#include "ScheduleParser.h"

/**
 * @class ScheduleValidator
 * @brief Cung cấp chức năng xác thực cho lịch trình bay
 * 
 * Lớp này chứa các phương thức tĩnh để xác thực lịch trình bay ở các định dạng khác nhau
 * và đảm bảo chúng đáp ứng các tiêu chí yêu cầu cho thời gian và tính hợp lệ của lịch trình.
 */
class ScheduleValidator {
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
     * @brief Kiểm tra xem định dạng ngày giờ có hợp lệ không
     * @param dateTime Chuỗi ngày giờ cần kiểm tra
     * @return true nếu định dạng hợp lệ (YYYY-MM-DD HH:mm), false nếu không
     */
    static bool isValidDateTime(const std::string& dateTime) {
        std::regex pattern("^\\d{4}-(?:0[1-9]|1[0-2])-(?:0[1-9]|[12]\\d|3[01]) (?:[01]\\d|2[0-3]):[0-5]\\d$");
        return std::regex_match(dateTime, pattern);
    }

    /**
     * @brief Kiểm tra xem thời gian đến có sau thời gian khởi hành không
     * @param departure Thời gian khởi hành
     * @param arrival Thời gian đến
     * @return true nếu thời gian đến sau thời gian khởi hành, false nếu không
     */
    static bool isArrivalAfterDeparture(const std::tm& departure, const std::tm& arrival) {
        std::time_t depTime = std::mktime(const_cast<std::tm*>(&departure));
        std::time_t arrTime = std::mktime(const_cast<std::tm*>(&arrival));
        return arrTime > depTime;
    }

public:
    /**
     * @brief Xác thực lịch trình từ định dạng chuỗi kết hợp
     * @param value Chuỗi có định dạng "YYYY-MM-DD HH:mm|YYYY-MM-DD HH:mm"
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::string& value) {
        ValidationResult result;
        
        // Check empty input
        if (isEmpty(value)) {
            ScheduleErrorHelper::addError(result, ScheduleError::EMPTY_SCHEDULE);
            return result;
        }

        // Split into departure and arrival
        size_t dashPos = value.find('|');
        if (dashPos == std::string::npos) {
            ScheduleErrorHelper::addError(result, ScheduleError::INVALID_FORMAT);
            return result;
        }

        std::string departureStr = value.substr(0, dashPos);
        std::string arrivalStr = value.substr(dashPos + 1);

        // Validate format
        if (!isValidDateTime(departureStr)) {
            ScheduleErrorHelper::addError(result, ScheduleError::INVALID_DEPARTURE_TIME);
        }

        if (!isValidDateTime(arrivalStr)) {
            ScheduleErrorHelper::addError(result, ScheduleError::INVALID_ARRIVAL_TIME);
        }

        // Parse and validate times
        if (result.isValid()) {
            auto parsed = ScheduleParser::parse(value);
            if (parsed.has_value()) {
                const auto& [departure, arrival] = parsed.value();
                if (!isArrivalAfterDeparture(departure, arrival)) {
                    ScheduleErrorHelper::addError(result, ScheduleError::ARRIVAL_BEFORE_DEPARTURE);
                }
            }
        }
        
        return result;
    }

    /**
     * @brief Xác thực lịch trình từ các thời gian riêng biệt
     * @param input Cặp chứa thời gian khởi hành và thời gian đến
     * @return ValidationResult chứa các lỗi xác thực nếu có
     */
    static ValidationResult validate(const std::pair<std::tm, std::tm>& input) {
        ValidationResult result;
        const auto& [departure, arrival] = input;
        
        if (!isArrivalAfterDeparture(departure, arrival)) {
            ScheduleErrorHelper::addError(result, ScheduleError::ARRIVAL_BEFORE_DEPARTURE);
        }
        
        return result;
    }
};

#endif