/**
 * @file Passenger.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Khai báo lớp Passenger đại diện cho thông tin hành khách trong hệ thống đặt vé máy bay.
 * @version 0.1
 * @date 2025-04-30
 * 
 * Lớp Passenger quản lý các thông tin cá nhân cần thiết cho việc đặt vé như:
 * tên, số điện thoại, số hộ chiếu (hoặc CCCD), địa chỉ và mã định danh hành khách (ID).
 */

#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

/**
 * @class Passenger
 * @brief Đại diện cho một hành khách trong hệ thống.
 * 
 * Lưu trữ và quản lý các thông tin cá nhân cơ bản của hành khách.
 */
class Passenger {
private:
    int _id;                  ///< Mã định danh duy nhất của hành khách.
    std::string _name;        ///< Tên đầy đủ của hành khách.
    std::string _phone;       ///< Số điện thoại liên hệ.
    std::string _passport;    ///< Số hộ chiếu hoặc căn cước công dân.
    std::string _address;     ///< Địa chỉ cư trú hoặc liên hệ của hành khách.

public:
    // ====================== Constructor ======================

    /// @name Constructor
    /// Các hàm constructor
    /// @{

    Passenger();
    Passenger(const std::string& name, const std::string& phone, const std::string& passport, const std::string& address);
    
    /// @}

    // ====================== Getter ======================

    /// @name Getter
    /// Các phương thức truy cập thông tin
    /// @{

    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getPassport() const;
    std::string getAddress() const;

    /// @}

    // ====================== Setter ======================

    /// @name Setter
    /// Các phương thức thiết lập thông tin
    /// @{
    
    void setId(const int& id);
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setPassport(const std::string& passport);
    void setAddress(const std::string& address);

    ///@}
};

#endif // PASSENGER_H
