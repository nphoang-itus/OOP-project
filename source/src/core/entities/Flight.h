#ifndef FLIGHT_H
#define FLIGHT_H

#include "IEntity.h"
#include "Aircraft.h"
#include "../value_objects/flight_number/FlightNumber.h"
#include "../value_objects/route/Route.h"
#include "../value_objects/schedule/Schedule.h"
#include "../value_objects/seat_class_map/SeatClassMap.h"
#include "../value_objects/seat_number/SeatNumber.h"
#include "../value_objects/flight_status/FlightStatus.h"
#include "../exceptions/Result.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

class Flight : public IEntity {
protected:
    FlightNumber _flightNumber;
    Route _route;
    Schedule _schedule;
    std::shared_ptr<Aircraft> _aircraft;
    std::unordered_map<SeatNumber, bool> _seatAvailability;
    FlightStatus _status;

    // Helper function to generate seat numbers based on class and count
    static std::vector<SeatNumber> generateSeatNumbers(const std::string& classCode, int count, const SeatClassMap& seatLayout) {
        std::vector<SeatNumber> seats;
        seats.reserve(count);
        
        // Determine padding based on count
        int padding = count > 99 ? 3 : 2;
        
        for (int i = 1; i <= count; i++) {
            std::stringstream ss;
            ss << classCode << std::setfill('0') << std::setw(padding) << i;
            auto seatNumberResult = SeatNumber::create(ss.str(), seatLayout);
            if (seatNumberResult) {
                seats.push_back(*seatNumberResult);
            }
        }
        return seats;
    }

    // Initialize seat availability based on aircraft's seat layout
    void initializeSeats() {
        const auto& seatLayout = _aircraft->getSeatLayout();
        for (const auto& [seatClass, count] : seatLayout.getSeatCounts()) {
            auto seatNumbers = generateSeatNumbers(seatClass.getCode(), count, seatLayout);
            for (const auto& seatNumber : seatNumbers) {
                _seatAvailability[seatNumber] = true; // true means available
            }
        }
    }

    Flight(FlightNumber flightNumber, Route route, Schedule schedule, std::shared_ptr<Aircraft> aircraft)
        : _flightNumber(std::move(flightNumber))
        , _route(std::move(route))
        , _schedule(std::move(schedule))
        , _aircraft(std::move(aircraft))
        , _status(FlightStatus::SCHEDULED) {
        initializeSeats();
    }

public:
    // Factory methods
    static Result<Flight> create(const FlightNumber& flightNumber, const Route& route, 
                               const Schedule& schedule, std::shared_ptr<Aircraft> aircraft) {
        if (!aircraft) {
            return Failure<Flight>(CoreError("Aircraft cannot be null", "INVALID_AIRCRAFT"));
        }
        return Success(Flight(flightNumber, route, schedule, aircraft));
    }

    static Result<Flight> create(const std::string& flightNumber, const std::string& route,
                               const std::string& schedule, std::shared_ptr<Aircraft> aircraft) {
        auto flightNumberResult = FlightNumber::create(flightNumber);
        if (!flightNumberResult) {
            return Failure<Flight>(CoreError("Invalid flight number: " + flightNumberResult.error().message, "INVALID_FLIGHT_NUMBER"));
        }

        auto routeResult = Route::create(route);
        if (!routeResult) {
            return Failure<Flight>(CoreError("Invalid route: " + routeResult.error().message, "INVALID_ROUTE"));
        }

        auto scheduleResult = Schedule::create(schedule);
        if (!scheduleResult) {
            return Failure<Flight>(CoreError("Invalid schedule: " + scheduleResult.error().message, "INVALID_SCHEDULE"));
        }

        if (!aircraft) {
            return Failure<Flight>(CoreError("Aircraft cannot be null", "INVALID_AIRCRAFT"));
        }

        return Success(Flight(*flightNumberResult, *routeResult, *scheduleResult, aircraft));
    }

    // Status management methods
    void setStatus(FlightStatus status) {
        _status = status;
    }

    FlightStatus getStatus() const {
        return _status;
    }

    std::string getStatusString() const {
        return FlightStatusUtil::toString(_status);
    }

    std::string getStatusVietnamese() const {
        return FlightStatusUtil::toVietnamese(_status);
    }

    // IEntity interface implementation
    int getId() const override {
        return _id;
    }

    std::string getStringId() const override {
        return _flightNumber.toString();
    }

    void setId(int id) override {
        _id = id;
    }

    void setSchedule(const Schedule& schedule) {
        _schedule = schedule;
    }

    std::string toString() const override {
        return "Flight{id=" + std::to_string(_id) + 
               ", flightNumber=" + _flightNumber.toString() + 
               ", route=" + _route.toString() + 
               ", schedule=" + _schedule.toString() + 
               ", aircraft=" + _aircraft->toString() + 
               ", status=" + getStatusString() + "}";
    }

    bool equals(const IEntity& other) const override {
        if (const auto* flight = dynamic_cast<const Flight*>(&other)) {
            return _id == flight->_id;
        }
        return false;
    }

    std::unique_ptr<IEntity> clone() const override {
        auto clone = std::unique_ptr<Flight>(new Flight(_flightNumber, _route, _schedule, _aircraft));
        clone->_id = _id;
        clone->_status = _status;
        clone->_seatAvailability = _seatAvailability;
        return clone;
    }

    // Getters
    const FlightNumber& getFlightNumber() const { return _flightNumber; }
    const Route& getRoute() const { return _route; }
    const Schedule& getSchedule() const { return _schedule; }
    const std::shared_ptr<Aircraft>& getAircraft() const { return _aircraft; }
    const std::unordered_map<SeatNumber, bool>& getSeatAvailability() const { return _seatAvailability; }

    // Seat management methods
    bool isSeatAvailable(const std::string& seatNumberStr) const {
        if (_status == FlightStatus::CANCELLED) {
            return false;
        }
        auto seatNumberResult = SeatNumber::create(seatNumberStr, _aircraft->getSeatLayout());
        if (!seatNumberResult) {
            return false;
        }
        const SeatNumber& seatNumber = *seatNumberResult;
        auto it = _seatAvailability.find(seatNumber);
        return it != _seatAvailability.end() && it->second;
    }

    bool reserveSeat(const std::string& seatNumberStr) {
        if (_status == FlightStatus::CANCELLED || _status == FlightStatus::DEPARTED || 
            _status == FlightStatus::IN_FLIGHT || _status == FlightStatus::LANDED) {
            return false;
        }
        auto seatNumberResult = SeatNumber::create(seatNumberStr, _aircraft->getSeatLayout());
        if (!seatNumberResult) {
            return false;
        }
        const SeatNumber& seatNumber = *seatNumberResult;
        auto it = _seatAvailability.find(seatNumber);
        if (it != _seatAvailability.end() && it->second) {
            it->second = false;
            return true;
        }
        return false;
    }

    bool releaseSeat(const std::string& seatNumberStr) {
        if (_status == FlightStatus::CANCELLED || _status == FlightStatus::DEPARTED || 
            _status == FlightStatus::IN_FLIGHT || _status == FlightStatus::LANDED) {
            return false;
        }
        auto seatNumberResult = SeatNumber::create(seatNumberStr, _aircraft->getSeatLayout());
        if (!seatNumberResult) {
            return false;
        }
        const SeatNumber& seatNumber = *seatNumberResult;
        auto it = _seatAvailability.find(seatNumber);
        if (it != _seatAvailability.end() && !it->second) {
            it->second = true;
            return true;
        }
        return false;
    }
};

#endif