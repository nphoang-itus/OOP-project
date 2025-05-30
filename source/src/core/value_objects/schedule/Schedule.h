/**
 * @file Schedule.h
 * @brief Định nghĩa lớp value object Schedule
 * @author Nguyễn Phúc Hoàng
 */

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <memory>
#include <utility>
#include <optional>
#include <ctime>
#include "../../exceptions/ValidationResult.h"
#include "ScheduleValidator.h"
#include "ScheduleParser.h"
#include "ScheduleFormatter.h"
#include <functional>

/**
 * @class Schedule
 * @brief Đại diện cho lịch trình bay từ thời gian khởi hành đến thời gian đến
 * 
 * Lớp này là một value object đóng gói thông tin về lịch trình bay
 * bao gồm thời gian khởi hành và thời gian đến.
 */
class Schedule {
private:
    std::tm _departure; ///< Thời gian khởi hành
    std::tm _arrival;   ///< Thời gian đến
    
    /**
     * @brief Constructor riêng tư để tạo instance Schedule
     * @param departure Thời gian khởi hành
     * @param arrival Thời gian đến
     */
    Schedule(const std::tm& departure, const std::tm& arrival) 
        : _departure(departure), _arrival(arrival) {}

    /**
     * @brief Phương thức template nội bộ để tạo Schedule từ các loại đầu vào khác nhau
     * @tparam InputType Loại đầu vào (string hoặc pair)
     * @param input Đầu vào để tạo Schedule
     * @return Result chứa Schedule hoặc lỗi
     */
    template<typename InputType>
    static Result<Schedule> createInternal(const InputType& input) {
        auto validationResult = ScheduleValidator::validate(input);
        if (!validationResult.isValid()) {
            return getValidationFailure<Schedule>(validationResult);
        }

        if constexpr (std::is_same_v<InputType, std::pair<std::tm, std::tm>>) {
            const auto& [departure, arrival] = input;
            return Success(Schedule(departure, arrival));
        } else if constexpr (std::is_same_v<InputType, std::string>) {
            auto parsed = ScheduleParser::parse(input);
            if (!parsed.has_value()) {
                return Failure<Schedule>(CoreError("Failed to parse schedule", "PARSE_ERROR"));
            }
            const auto& [departure, arrival] = parsed.value();
            return Success(Schedule(departure, arrival));
        }
        return Failure<Schedule>(CoreError("Unsupported input type", "UNSUPPORTED_TYPE"));
    }

public:
    /**
     * @brief Constructor mặc định
     */
    Schedule() = default;

    /**
     * @brief Tạo Schedule từ chuỗi đã định dạng
     * @param value Chuỗi có định dạng "YYYY-MM-DD HH:mm|YYYY-MM-DD HH:mm"
     * @return Result chứa Schedule hoặc lỗi xác thực
     */
    static Result<Schedule> create(const std::string& value) {
        return createInternal(value);    
    }

    /**
     * @brief Tạo Schedule từ các chuỗi thời gian riêng biệt
     * @param departure Chuỗi thời gian khởi hành
     * @param arrival Chuỗi thời gian đến
     * @return Result chứa Schedule hoặc lỗi xác thực
     */
    static Result<Schedule> create(const std::string& departure, const std::string& arrival) {
        std::string combined = departure + "|" + arrival;
        return createInternal(combined);
    }

    /**
     * @brief Tạo Schedule từ các đối tượng tm
     * @param departure Thời gian khởi hành
     * @param arrival Thời gian đến
     * @return Result chứa Schedule hoặc lỗi xác thực
     */
    static Result<Schedule> create(const std::tm& departure, const std::tm& arrival) {
        return createInternal(std::make_pair(departure, arrival));
    }

    /**
     * @brief Lấy thời gian khởi hành
     * @return Thời gian khởi hành dưới dạng tm
     */
    const std::tm& getDeparture() const { return _departure; }
    
    /**
     * @brief Lấy thời gian đến
     * @return Thời gian đến dưới dạng tm
     */
    const std::tm& getArrival() const { return _arrival; }

    /**
     * @brief Chuyển đổi lịch trình thành định dạng chuỗi
     * @return Chuỗi đã định dạng theo "YYYY-MM-DD HH:mm|YYYY-MM-DD HH:mm"
     */
    std::string toString() const {
        return ScheduleFormatter::toString(_departure, _arrival);
    }

    /**
     * @brief Toán tử bằng
     * @param other Schedule khác để so sánh
     * @return true nếu cả hai lịch trình bằng nhau, false nếu ngược lại
     */
    bool operator==(const Schedule& other) const {
        return std::mktime(const_cast<std::tm*>(&_departure)) == std::mktime(const_cast<std::tm*>(&other._departure)) &&
               std::mktime(const_cast<std::tm*>(&_arrival)) == std::mktime(const_cast<std::tm*>(&other._arrival));
    }

    /**
     * @brief Toán tử không bằng
     * @param other Schedule khác để so sánh
     * @return true nếu lịch trình không bằng nhau, false nếu ngược lại
     */
    bool operator!=(const Schedule& other) const {
        return !(*this == other);
    }

    /**
     * @brief Kiểm tra xem lịch trình này có trùng lặp với lịch trình khác không
     * @param other Lịch trình khác để kiểm tra
     * @return true nếu có trùng lặp thời gian, false nếu không
     */
    bool overlapsWith(const Schedule& other) const {
        auto thisDep = std::mktime(const_cast<std::tm*>(&_departure));
        auto thisArr = std::mktime(const_cast<std::tm*>(&_arrival));
        auto otherDep = std::mktime(const_cast<std::tm*>(&other._departure));
        auto otherArr = std::mktime(const_cast<std::tm*>(&other._arrival));

        return (thisDep <= otherArr && thisArr >= otherDep);
    }
};

/**
 * @namespace std
 * @brief Namespace tiêu chuẩn C++
 */
namespace std {
    /**
     * @struct hash<Schedule>
     * @brief Chuyên môn hóa template hash cho lớp Schedule
     * 
     * Cung cấp hàm hash để Schedule có thể được sử dụng trong
     * các container như unordered_map và unordered_set.
     */
    template<>
    struct hash<Schedule> {
        /**
         * @brief Tính toán giá trị hash cho Schedule
         * @param schedule Đối tượng Schedule cần tính hash
         * @return Giá trị hash của Schedule
         */
        size_t operator()(const Schedule& schedule) const {
            return hash<string>()(schedule.toString());
        }
    };
}

#endif