#ifndef TICKET_H
#define TICKET_H

#include "IEntity.h"
#include "Passenger.h"
#include "Flight.h"
#include "../value_objects/ticket_number/TicketNumber.h"
#include "../value_objects/seat_number/SeatNumber.h"
#include "../value_objects/ticket_status/TicketStatus.h"
#include "../value_objects/flight_status/FlightStatus.h"
#include "../exceptions/Result.h"
#include <memory>
#include <string>
#include <sstream>

class Ticket : public IEntity {
protected:
    TicketNumber _ticketNumber;
    std::shared_ptr<Passenger> _passenger;
    std::shared_ptr<Flight> _flight;
    SeatNumber _seatNumber;
    TicketStatus _status;

    Ticket(TicketNumber ticketNumber, std::shared_ptr<Passenger> passenger,
           std::shared_ptr<Flight> flight, SeatNumber seatNumber)
        : _ticketNumber(std::move(ticketNumber))
        , _passenger(std::move(passenger))
        , _flight(std::move(flight))
        , _seatNumber(std::move(seatNumber))
        , _status(TicketStatus::PENDING) {
    }

public:
    // Factory methods
    static Result<Ticket> create(const TicketNumber& ticketNumber, std::shared_ptr<Passenger> passenger,
                               std::shared_ptr<Flight> flight, const SeatNumber& seatNumber) {
        if (!passenger) {
            return Failure<Ticket>(CoreError("Passenger cannot be null", "INVALID_PASSENGER"));
        }
        if (!flight) {
            return Failure<Ticket>(CoreError("Flight cannot be null", "INVALID_FLIGHT"));
        }
        return Success(Ticket(ticketNumber, passenger, flight, seatNumber));
    }

    static Result<Ticket> create(const std::string& ticketNumber, std::shared_ptr<Passenger> passenger,
                               std::shared_ptr<Flight> flight, const std::string& seatNumber) {
        auto ticketNumberResult = TicketNumber::create(ticketNumber);
        if (!ticketNumberResult) {
            return Failure<Ticket>(CoreError("Invalid ticket number: " + ticketNumberResult.error().message, "INVALID_TICKET_NUMBER"));
        }

        if (!passenger) {
            return Failure<Ticket>(CoreError("Passenger cannot be null", "INVALID_PASSENGER"));
        }

        if (!flight) {
            return Failure<Ticket>(CoreError("Flight cannot be null", "INVALID_FLIGHT"));
        }

        auto seatNumberResult = SeatNumber::create(seatNumber, flight->getAircraft()->getSeatLayout());
        if (!seatNumberResult) {
            return Failure<Ticket>(CoreError("Invalid seat number: " + seatNumberResult.error().message, "INVALID_SEAT_NUMBER"));
        }

        return Success(Ticket(*ticketNumberResult, passenger, flight, *seatNumberResult));
    }

    // Status management methods
    void setStatus(TicketStatus status) {
        _status = status;
    }

    TicketStatus getStatus() const {
        return _status;
    }

    std::string getStatusString() const {
        return TicketStatusUtil::toString(_status);
    }

    std::string getStatusVietnamese() const {
        return TicketStatusUtil::toVietnamese(_status);
    }

    // IEntity interface implementation
    int getId() const override {
        return _id;
    }

    std::string getStringId() const override {
        return _ticketNumber.toString();
    }

    void setId(int id) override {
        _id = id;
    }

    std::string toString() const override {
        return "Ticket{id=" + std::to_string(_id) + 
               ", ticketNumber=" + _ticketNumber.toString() + 
               ", passenger=" + _passenger->toString() + 
               ", flight=" + _flight->toString() + 
               ", seatNumber=" + _seatNumber.toString() + 
               ", status=" + getStatusString() + "}";
    }

    bool equals(const IEntity& other) const override {
        if (const auto* ticket = dynamic_cast<const Ticket*>(&other)) {
            return _id == ticket->_id;
        }
        return false;
    }

    std::unique_ptr<IEntity> clone() const override {
        auto clone = std::unique_ptr<Ticket>(new Ticket(_ticketNumber, _passenger, _flight, _seatNumber));
        clone->_id = _id;
        clone->_status = _status;
        return clone;
    }

    // Getters
    const TicketNumber& getTicketNumber() const { return _ticketNumber; }
    const std::shared_ptr<Passenger>& getPassenger() const { return _passenger; }
    const std::shared_ptr<Flight>& getFlight() const { return _flight; }
    const SeatNumber& getSeatNumber() const { return _seatNumber; }

    // Business logic methods
    bool canBeCancelled() const {
        return _status == TicketStatus::PENDING || _status == TicketStatus::CONFIRMED;
    }

    bool canBeRefunded() const {
        return _status == TicketStatus::CONFIRMED && 
               _flight->getStatus() != FlightStatus::CANCELLED &&
               _flight->getStatus() != FlightStatus::DEPARTED &&
               _flight->getStatus() != FlightStatus::IN_FLIGHT &&
               _flight->getStatus() != FlightStatus::LANDED;
    }

    bool canBeCheckedIn() const {
        return _status == TicketStatus::CONFIRMED && 
               _flight->getStatus() == FlightStatus::BOARDING;
    }

    bool canBeBoarded() const {
        return _status == TicketStatus::CHECKED_IN && 
               _flight->getStatus() == FlightStatus::BOARDING;
    }

    bool canBeCompleted() const {
        return _status == TicketStatus::BOARDED && 
               _flight->getStatus() == FlightStatus::LANDED;
    }
};

#endif