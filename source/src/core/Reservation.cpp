/**
 * @file Reservation.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief File chứa các định nghĩa lớp Đặt vé (Reservation)
 * @version 0.1
 * @date 2025-05-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Reservation.h"

Reservation::Reservation() : _id(0) {}
Reservation::Reservation(const std::string& ticketNo, const std::shared_ptr<Flight>& flight, const std::shared_ptr<Passenger>& passenger)
        :   _id(0), _ticketNo(ticketNo), _flight(flight), _passenger(passenger) {}

// Getter
int Reservation::getId() const {
    return _id;
}

std::string Reservation::getTicketNo() const {
    return _ticketNo;
}

std::shared_ptr<Flight> Reservation::getFlight() const {
    return _flight;
}

std::shared_ptr<Passenger> Reservation::getPassenger() const {
    return _passenger;
}

// Setter
void Reservation::setId(const int& id) {
    _id = id;
}

void Reservation::setTicketNo(const std::string& ticketNo) {
    _ticketNo = ticketNo;
}

void Reservation::setFlight(const std::shared_ptr<Flight>& flight) {
    _flight = flight;
}

void Reservation::setPassenger(const std::shared_ptr<Passenger>& passenger) {
    _passenger = passenger;
}

