/**
 * @file Reservation.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Định nghĩa lớp Reservation quản lý thông tin đặt vé máy bay.
 * @version 0.1
 * @date 2025-04-30
 * 
 * Lớp này đại diện cho một đặt chỗ (reservation) trong hệ thống đặt vé máy bay,
 * liên kết giữa hành khách và chuyến bay thông qua số vé (ticket number).
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef RESERVATION_H
#define RESERVATION_H

#include "Flight.h"
#include "Passenger.h"
#include <string>
#include <memory>

/**
 * @class Reservation
 * @brief Lớp quản lý thông tin một đặt vé, bao gồm số vé, chuyến bay và hành khách liên quan.
 */
class Reservation {
private:
    int _id;                                   ///< Mã định danh đặt vé (tự động sinh từ CSDL).
    std::string _ticketNo;                     ///< Mã số vé máy bay.
    std::shared_ptr<Flight> _flight;           ///< Con trỏ thông minh tới đối tượng Flight.
    std::shared_ptr<Passenger> _passenger;     ///< Con trỏ thông minh tới đối tượng Passenger.

public:

    // ====================== Constructor ======================

    /// @name Constructor
    /// Các hàm constructor
    /// @{

    Reservation();
    Reservation(const std::string& ticketNo, const std::shared_ptr<Flight>& flight, const std::shared_ptr<Passenger>& passenger);

    /// @}

    // ====================== Getter ======================

    /// @name Getter
    /// Các phương thức truy cập thông tin
    /// @{

    int getId() const;
    std::string getTicketNo() const;
    std::shared_ptr<Flight> getFlight() const;
    std::shared_ptr<Passenger> getPassenger() const;

    /// @}

    // ====================== Setter ======================

    /// @name Setter
    /// Các phương thức thiết lập thông tin
    /// @{

    void setId(const int& id);
    void setTicketNo(const std::string& ticketNo);
    void setFlight(const std::shared_ptr<Flight>& flight);
    void setPassenger(const std::shared_ptr<Passenger>& passenger);

    /// @}
};

#endif
 