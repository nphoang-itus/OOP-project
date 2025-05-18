#ifndef FLIGHT_H
#define FLIGHT_H

#include "IEntity.h"
#include "Aircraft.h"
#include "../value_objects/flight_number/FlightNumber.h"
#include "../value_objects/route/Route.h"
#include "../value_objects/schedule/Schedule.h"
#include "../value_objects/seat_class_map/SeatClassMap.h"
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
    std::unordered_map<std::string, bool> _seatAvailability;

    // Helper function to generate seat numbers based on class and count
    static std::vector<std::string> generateSeatNumbers(const std::string& classCode, int count) {
        std::vector<std::string> seats;
        seats.reserve(count);
        
        // Determine padding based on count
        int padding = count > 99 ? 3 : 2;
        
        for (int i = 1; i <= count; i++) {
            std::stringstream ss;
            ss << classCode << std::setfill('0') << std::setw(padding) << i;
            seats.push_back(ss.str());
        }
        return seats;
    }

    // Initialize seat availability based on aircraft's seat layout
    void initializeSeats() {
        const auto& seatLayout = _aircraft->getSeatLayout();
        for (const auto& [seatClass, count] : seatLayout.getSeatCounts()) {
            auto seatNumbers = generateSeatNumbers(seatClass.getCode(), count);
            for (const auto& seatNumber : seatNumbers) {
                _seatAvailability[seatNumber] = true; // true means available
            }
        }
    }

    Flight(FlightNumber flightNumber, Route route, Schedule schedule, std::shared_ptr<Aircraft> aircraft)
        : _flightNumber(std::move(flightNumber))
        , _route(std::move(route))
        , _schedule(std::move(schedule))
        , _aircraft(std::move(aircraft)) {
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

    // IEntity interface implementation
    std::string getId() const override {
        return _flightNumber.toString();
    }

    std::string toString() const override {
        return "Flight{flightNumber=" + _flightNumber.toString() + 
               ", route=" + _route.toString() + 
               ", schedule=" + _schedule.toString() + 
               ", aircraft=" + _aircraft->toString() + "}";
    }

    bool equals(const IEntity& other) const override {
        if (const auto* flight = dynamic_cast<const Flight*>(&other)) {
            return _flightNumber == flight->_flightNumber;
        }
        return false;
    }

    std::unique_ptr<IEntity> clone() const override {
        return std::make_unique<Flight>(_flightNumber, _route, _schedule, _aircraft);
    }

    // Getters
    const FlightNumber& getFlightNumber() const { return _flightNumber; }
    const Route& getRoute() const { return _route; }
    const Schedule& getSchedule() const { return _schedule; }
    const std::shared_ptr<Aircraft>& getAircraft() const { return _aircraft; }
    const std::unordered_map<std::string, bool>& getSeatAvailability() const { return _seatAvailability; }

    // Seat management methods
    bool isSeatAvailable(const std::string& seatNumber) const {
        auto it = _seatAvailability.find(seatNumber);
        return it != _seatAvailability.end() && it->second;
    }

    bool reserveSeat(const std::string& seatNumber) {
        auto it = _seatAvailability.find(seatNumber);
        if (it != _seatAvailability.end() && it->second) {
            it->second = false;
            return true;
        }
        return false;
    }

    bool releaseSeat(const std::string& seatNumber) {
        auto it = _seatAvailability.find(seatNumber);
        if (it != _seatAvailability.end() && !it->second) {
            it->second = true;
            return true;
        }
        return false;
    }
};

#endif