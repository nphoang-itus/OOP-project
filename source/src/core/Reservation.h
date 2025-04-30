/**
 * @file Reservation.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef RESERVATION_H
#define RESERVATION_H

#include "Flight.h"
#include "Passenger.h"
#include <string>
#include <memory> // Chứa std::shared_ptr<>

class Reservation {
private:
    int _id;
    std::string _ticketNo;
    std::shared_ptr<Flight> _flight;
    std::shared_ptr<Passenger> _passenger;

public:
    // Constructor
    Reservation();
    Reservation(const std::string& ticketNo, const std::shared_ptr<Flight>& flight, const std::shared_ptr<Passenger>& passenger);

    // Getter
    int getId() const;
    std::string getTicketNo() const;
    std::shared_ptr<Flight> getFlight() const;
    std::shared_ptr<Passenger> getPassenger() const;

    // Setter
    void setId(const int& id);
    void setTicketNo(const std::string& ticketNo);
    void setFlight(const std::shared_ptr<Flight>& flight);
    void setPassenger(const std::shared_ptr<Passenger>& passenger);
};

#endif