/**
 * @file Flight.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @version 0.1
 * @date 2025-04-30
 * 
 * @brief Khai báo lớp Flight dùng để biểu diễn thông tin chuyến bay trong hệ thống đặt vé máy bay.
 * 
 * File này định nghĩa lớp Flight bao gồm các thuộc tính như mã chuyến bay, điểm đi, điểm đến,
 * thời gian khởi hành, thời gian cất cánh, thời gian đến, giá vé, và trạng thái vé. Đồng thời cung cấp
 * các phương thức getter và setter để truy cập và thay đổi dữ liệu của đối tượng.
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <ctime>

/**
 * @class Flight
 * @brief Lớp đại diện cho một chuyến bay.
 * 
 * Lớp này chứa đầy đủ thông tin liên quan đến chuyến bay và cung cấp các phương thức
 * để lấy và thiết lập các thông tin này.
 */
class Flight {
private:
    int _id;                         ///< Mã định danh duy nhất cho chuyến bay
    std::string _no;                 ///< Mã hiệu chuyến bay
    std::string _name;               ///< Tên hãng hàng không hoặc tên chuyến bay
    std::string _from;               ///< Sân bay đi
    std::string _destination;        ///< Sân bay đến
    std::tm _departureTime;          ///< Thời gian khởi hành (dự kiến mở cổng)
    std::tm _leaveTime;              ///< Thời gian máy bay cất cánh
    std::tm _arrivalTime;            ///< Thời gian hạ cánh
    double _amount;                  ///< Giá vé
    char _availability;              ///< Tình trạng vé ('A' là Available, 'N' là Not available)

public:
    // ====================== Constructor ======================

    /// @name Constructor
    /// Các hàm constructor
    /// @{

    Flight();
    Flight(const std::string& no, const std::string& name, const std::string& from, const std::string& destination,
        const std::tm& departureTime, const std::tm& leaveTime, const std::tm& arrivalTime,
        const double& amount, const char& availability);
    
    /// @}

    // ====================== Getter ======================

    /// @name Getter
    /// Các phương thức truy cập thông tin
    /// @{

    int getId() const;
    std::string getNo() const;
    std::string getName() const;
    std::string getFrom() const;
    std::string getDestination() const;
    std::tm getDepartureTime() const;
    std::tm getLeaveTime() const;
    std::tm getArrivalTime() const;
    double getAmount() const;
    char getAvailability() const;

    /// @}

    // ====================== Setter ======================

    /// @name Setter
    /// Các phương thức thiết lập thông tin
    /// @{

    void setId(const int& id);
    void setNo(const std::string& no);
    void setName(const std::string& name);
    void setFrom(const std::string& from);
    void setDestination(const std::string& destination);
    void setDepartureTime(const std::tm& departureTime);
    void setLeaveTime(const std::tm& leaveTime);
    void setArrivalTime(const std::tm& arrivalTime);
    void setAmount(const double& amount);
    void setAvailability(const char& availability);

    /// @}
};

#endif
 