#ifndef TICKET_H
#define TICKET_H

#include <memory>
#include <string>
#include "../value_objects/seat_number/SeatNumber.h"
#include "../value_objects/ticket_number/TicketNumber.h"
#include "../value_objects/price/Price.h"
#include "IEntity.h"
#include "Flight.h"
#include "Passenger.h"

class Ticket : public IEntity {
private:
    std::shared_ptr<Flight> _flight;
    std::shared_ptr<Passenger> _passenger;
    SeatNumber _seatNumber;
    TicketNumber _ticketNumber;
    Price _price;
    bool _isCheckedIn = false;

    // Private constructor to enforce creation through factory method
    Ticket(std::shared_ptr<Flight> flight,
           std::shared_ptr<Passenger> passenger,
           const SeatNumber& seatNumber,
           const TicketNumber& ticketNumber,
           const Price& price)
        : _flight(std::move(flight))
        , _passenger(std::move(passenger))
        , _seatNumber(seatNumber)
        , _ticketNumber(ticketNumber)
        , _price(price) {}

public:
    // Factory method to create a ticket
    static Result<Ticket> create(
        std::shared_ptr<Flight> flight,
        std::shared_ptr<Passenger> passenger,
        const SeatNumber& seatNumber,
        const TicketNumber& ticketNumber,
        const Price& price) {
        
        // Validate inputs
        if (!flight) {
            return Failure<Ticket>(CoreError("Flight cannot be null", "INVALID_FLIGHT"));
        }
        if (!passenger) {
            return Failure<Ticket>(CoreError("Passenger cannot be null", "INVALID_PASSENGER"));
        }

        return Success(Ticket(flight, passenger, seatNumber, ticketNumber, price));
    }

    // Getters
    const std::shared_ptr<Flight>& getFlight() const { return _flight; }
    const std::shared_ptr<Passenger>& getPassenger() const { return _passenger; }
    const SeatNumber& getSeatNumber() const { return _seatNumber; }
    const TicketNumber& getTicketNumber() const { return _ticketNumber; }
    const Price& getPrice() const { return _price; }
    bool isCheckedIn() const { return _isCheckedIn; }

    // Business methods
    void checkIn() {
        if (_isCheckedIn) {
            throw CoreError("Ticket is already checked in", "ALREADY_CHECKED_IN");
        }
        _isCheckedIn = true;
    }

    void cancelCheckIn() {
        if (!_isCheckedIn) {
            throw CoreError("Ticket is not checked in", "NOT_CHECKED_IN");
        }
        _isCheckedIn = false;
    }

    // IEntity interface implementation
    std::string getId() const override {
        return _ticketNumber.toString();
    }

    std::string toString() const override {
        return "Ticket[" + _ticketNumber.toString() + "]";
    }

    bool equals(const IEntity& other) const override {
        const Ticket* ticket = dynamic_cast<const Ticket*>(&other);
        return ticket && *this == *ticket;
    }

    std::unique_ptr<IEntity> clone() const override {
        return std::make_unique<Ticket>(*this);
    }

    bool operator==(const Ticket& other) const {
        return _ticketNumber == other._ticketNumber;
    }

    bool operator!=(const Ticket& other) const {
        return !(*this == other);
    }
};

#endif