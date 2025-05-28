#include "FlightService.h"
#include "../core/exceptions/Result.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

// Private helper methods
Result<Flight> FlightService::getFlightById(int id) {
    return _flightRepository->findById(id);
}

Result<bool> FlightService::existsById(const int& id) {
    return _flightRepository->exists(id);
}

Result<bool> FlightService::deleteById(const int& id) {
    return _flightRepository->deleteById(id);
}

// Core CRUD operations
Result<Flight> FlightService::getFlight(const FlightNumber& number) {
    if (_logger) _logger->debug("Getting flight with number: " + number.toString());
    return _flightRepository->findByFlightNumber(number);
}

Result<std::vector<Flight>> FlightService::getAllFlights() {
    if (_logger) _logger->debug("Getting all flights");
    return _flightRepository->findAll();
}

Result<bool> FlightService::flightExists(const FlightNumber& number) {
    if (_logger) _logger->debug("Checking if flight exists with number: " + number.toString());
    return _flightRepository->existsFlight(number);
}

Result<Flight> FlightService::createFlight(const Flight& flight) {
    if (_logger) _logger->debug("Creating flight with number: " + flight.getFlightNumber().toString());

    // Business rule: Check if flight with same number already exists
    auto existsResult = _flightRepository->existsFlight(flight.getFlightNumber());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check flight existence");
        return Failure<Flight>(CoreError("Failed to check flight existence", "DB_ERROR"));
    }
    if (existsResult.value()) {
        if (_logger) _logger->error("Flight with number " + flight.getFlightNumber().toString() + " already exists");
        return Failure<Flight>(CoreError("Flight with this number already exists", "DUPLICATE_FLIGHT_NUMBER"));
    }

    // Business rule: Check if aircraft exists
    auto aircraftResult = _aircraftRepository->findBySerialNumber(flight.getAircraft()->getSerial());
    if (!aircraftResult) {
        if (_logger) _logger->error("Failed to find aircraft");
        return Failure<Flight>(aircraftResult.error());
    }

    // Create flight
    return _flightRepository->create(flight);
}

Result<Flight> FlightService::updateFlight(const Flight& flight) {
    if (_logger) _logger->debug("Updating flight with number: " + flight.getFlightNumber().toString());

    // Check if flight exists
    auto existsResult = _flightRepository->existsFlight(flight.getFlightNumber());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check flight existence");
        return Failure<Flight>(CoreError("Failed to check flight existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Flight with number " + flight.getFlightNumber().toString() + " not found");
        return Failure<Flight>(CoreError("Flight not found", "NOT_FOUND"));
    }

    // Business rule: Check if aircraft exists
    auto aircraftResult = _aircraftRepository->findBySerialNumber(flight.getAircraft()->getSerial());
    if (!aircraftResult) {
        if (_logger) _logger->error("Failed to find aircraft");
        return Failure<Flight>(aircraftResult.error());
    }

    // Update flight
    return _flightRepository->update(flight);
}

Result<bool> FlightService::deleteFlight(const FlightNumber& number) {
    if (_logger) _logger->debug("Deleting flight with number: " + number.toString());

    // Check if flight exists
    auto existsResult = _flightRepository->existsFlight(number);
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check flight existence");
        return Failure<bool>(CoreError("Failed to check flight existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Flight with number " + number.toString() + " not found");
        return Failure<bool>(CoreError("Flight not found", "NOT_FOUND"));
    }

    // Get flight to check for dependencies
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight details");
        return Failure<bool>(flightResult.error());
    }

    // Business rule: Check if flight has any tickets
    auto ticketsResult = _ticketRepository->findByFlightId(flightResult.value().getId());
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to check flight tickets");
        return Failure<bool>(ticketsResult.error());
    }
    if (!ticketsResult.value().empty()) {
        if (_logger) _logger->error("Cannot delete flight that has tickets");
        return Failure<bool>(CoreError("Cannot delete flight that has tickets", "FLIGHT_HAS_TICKETS"));
    }

    // Delete flight
    return _flightRepository->deleteById(flightResult.value().getId());
}

// Business operations
Result<std::vector<Flight>> FlightService::getFlightsByAircraft(const AircraftSerial& serial) {
    if (_logger) _logger->debug("Getting flights for aircraft: " + serial.toString());

    // Check if aircraft exists
    auto aircraftResult = _aircraftRepository->findBySerialNumber(serial);
    if (!aircraftResult) {
        if (_logger) _logger->error("Failed to find aircraft");
        return Failure<std::vector<Flight>>(aircraftResult.error());
    }

    return _flightRepository->findFlightByAircraft(serial);
}

Result<std::vector<Flight>> FlightService::getFlightsByRoute(const std::string& originCode, const std::string& destinationCode) {
    if (_logger) _logger->debug("Getting flights for route: " + originCode + "-" + destinationCode);

    // TODO: Implement route-based flight search
    return Failure<std::vector<Flight>>(CoreError("Not implemented", "NOT_IMPLEMENTED"));
}

Result<std::vector<Flight>> FlightService::getFlightsByDate(const std::string& date) {
    if (_logger) _logger->debug("Getting flights for date: " + date);

    // TODO: Implement date-based flight search
    return Failure<std::vector<Flight>>(CoreError("Not implemented", "NOT_IMPLEMENTED"));
}

Result<bool> FlightService::isSeatAvailable(const FlightNumber& number, const std::string& seatNumber) {
    if (_logger) _logger->debug("Checking if seat is available for flight: " + number.toString() + ", seat: " + seatNumber);

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if seat is valid for the aircraft
    if (!flightResult.value().getAircraft()->isValidSeatNumber(seatNumber)) {
        if (_logger) _logger->error("Invalid seat number: " + seatNumber);
        return Failure<bool>(CoreError("Invalid seat number", "INVALID_SEAT_NUMBER"));
    }

    // Check if seat is available
    return Success(flightResult.value().isSeatAvailable(seatNumber));
}

Result<std::vector<std::string>> FlightService::getAvailableSeats(const FlightNumber& number, const std::string& seatClass) {
    if (_logger) _logger->debug("Getting available seats for flight: " + number.toString() + ", class: " + seatClass);

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<std::vector<std::string>>(flightResult.error());
    }

    // Validate seat class
    std::string classCode;
    if (seatClass == "ECONOMY") classCode = "E";
    else if (seatClass == "BUSINESS") classCode = "B";
    else if (seatClass == "FIRST") classCode = "F";
    else {
        if (_logger) _logger->error("Invalid seat class: " + seatClass);
        return Failure<std::vector<std::string>>(CoreError("Invalid seat class", "INVALID_SEAT_CLASS"));
    }

    // Check if aircraft has this seat class
    if (!flightResult.value().getAircraft()->hasSeatClass(classCode)) {
        if (_logger) _logger->error("Aircraft does not have seat class: " + seatClass);
        return Failure<std::vector<std::string>>(CoreError("Aircraft does not have this seat class", "INVALID_SEAT_CLASS"));
    }

    // Get available seats from database
    auto availableSeatsResult = _flightRepository->getAvailableSeats(flightResult.value());
    if (!availableSeatsResult) {
        if (_logger) _logger->error("Failed to get available seats from database");
        return Failure<std::vector<std::string>>(availableSeatsResult.error());
    }

    // Filter seats by class
    std::vector<std::string> filteredSeats;
    for (const auto& seatNumber : availableSeatsResult.value()) {
        if (seatNumber.getValue().substr(0, 1) == classCode) {
            filteredSeats.push_back(seatNumber.getValue());
        }
    }

    return Success(filteredSeats);
}

Result<bool> FlightService::updateFlightStatus(const FlightNumber& number, FlightStatus status) {
    if (_logger) _logger->debug("Updating flight status for flight: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Update status
    auto flight = flightResult.value();
    flight.setStatus(status);

    // Save changes
    auto updateResult = _flightRepository->update(flight);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update flight status");
        return Failure<bool>(updateResult.error());
    }

    return Success(true);
}

Result<bool> FlightService::isFlightFull(const FlightNumber& number) {
    if (_logger) _logger->debug("Checking if flight is full: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Get tickets for this flight
    auto ticketsResult = _ticketRepository->findByFlightId(flightResult.value().getId());
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get flight tickets");
        return Failure<bool>(ticketsResult.error());
    }

    // Check if number of tickets equals aircraft capacity
    auto totalSeatCount = flightResult.value().getAircraft()->getSeatLayout().getTotalSeatCount();

    return Success(ticketsResult.value().size() >= totalSeatCount);
}

Result<bool> FlightService::cancelFlight(const FlightNumber& number, const std::string& reason) {
    if (_logger) _logger->debug("Cancelling flight: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if flight can be cancelled
    if (flightResult.value().getStatus() == FlightStatus::CANCELLED) {
        if (_logger) _logger->error("Flight is already cancelled");
        return Failure<bool>(CoreError("Flight is already cancelled", "INVALID_STATUS"));
    }

    // Update flight status
    auto flight = flightResult.value();
    flight.setStatus(FlightStatus::CANCELLED);
    
    auto updateResult = _flightRepository->update(flight);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update flight status");
        return Failure<bool>(updateResult.error());
    }

    return Success(true);
}

Result<bool> FlightService::delayFlight(const FlightNumber& number, const std::tm& newDepartureTime) {
    if (_logger) _logger->debug("Delaying flight: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if flight can be delayed
    if (flightResult.value().getStatus() == FlightStatus::CANCELLED) {
        if (_logger) _logger->error("Cannot delay cancelled flight");
        return Failure<bool>(CoreError("Cannot delay cancelled flight", "INVALID_STATUS"));
    }

    std::tm currentDeparture = flightResult.value().getSchedule().getDeparture();
    
    auto currentTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&currentDeparture));
    auto newTimePoint = std::chrono::system_clock::from_time_t(std::mktime(const_cast<std::tm*>(&newDepartureTime)));
    
    // Compare using chrono
    if (newTimePoint <= currentTimePoint) {
        if (_logger) _logger->error("New departure time must be later than current time");
        return Failure<bool>(CoreError("Invalid departure time", "INVALID_TIME"));
    }
    
    // Calculate delay duration
    auto delayDuration = newTimePoint - currentTimePoint;
    
    // Calculate new arrival time
    std::tm currentArrival = flightResult.value().getSchedule().getArrival();
    auto arrivalTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&currentArrival));
    auto newArrivalTimePoint = arrivalTimePoint + delayDuration;
    
    std::time_t newArrivalTime_t = std::chrono::system_clock::to_time_t(newArrivalTimePoint);
    std::tm newArrival = *std::localtime(&newArrivalTime_t);
    
    // Create new schedule
    auto newScheduleResult = Schedule::create(newDepartureTime, newArrival);
    if (!newScheduleResult) {
        return Failure<bool>(newScheduleResult.error());
    }
    
    // Update flight
    auto flight = flightResult.value();
    flight.setSchedule(newScheduleResult.value());
    flight.setStatus(FlightStatus::DELAYED);
    
    auto updateResult = _flightRepository->update(flight);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update flight schedule");
        return Failure<bool>(updateResult.error());
    }
    
    return Success(true);
}

Result<bool> FlightService::reserveSeat(const FlightNumber& number, const std::string& seatNumber) {
    if (_logger) _logger->debug("Reserving seat for flight: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if seat is valid
    if (!flightResult.value().getAircraft()->isValidSeatNumber(seatNumber)) {
        if (_logger) _logger->error("Invalid seat number: " + seatNumber);
        return Failure<bool>(CoreError("Invalid seat number", "INVALID_SEAT_NUMBER"));
    }

    // Create seat number object
    auto seatNumberResult = SeatNumber::create(seatNumber, flightResult.value().getAircraft()->getSeatLayout());
    if (!seatNumberResult) {
        if (_logger) _logger->error("Invalid seat number: " + seatNumber);
        return Failure<bool>(CoreError("Invalid seat number", "INVALID_SEAT_NUMBER"));
    }

    // Check if seat is available
    auto isAvailableResult = _flightRepository->isSeatAvailable(flightResult.value(), *seatNumberResult);
    if (!isAvailableResult) {
        if (_logger) _logger->error("Failed to check seat availability");
        return Failure<bool>(isAvailableResult.error());
    }

    if (!isAvailableResult.value()) {
        if (_logger) _logger->error("Seat is not available: " + seatNumber);
        return Failure<bool>(CoreError("Seat is not available", "SEAT_NOT_AVAILABLE"));
    }

    // Reserve seat in database
    auto reserveResult = _flightRepository->reserveSeat(flightResult.value(), *seatNumberResult);
    if (!reserveResult) {
        if (_logger) _logger->error("Failed to reserve seat in database");
        return Failure<bool>(reserveResult.error());
    }

    return Success(true);
}

Result<bool> FlightService::releaseSeat(const FlightNumber& number, const std::string& seatNumber) {
    if (_logger) _logger->debug("Releasing seat for flight: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if seat is valid
    if (!flightResult.value().getAircraft()->isValidSeatNumber(seatNumber)) {
        if (_logger) _logger->error("Invalid seat number: " + seatNumber);
        return Failure<bool>(CoreError("Invalid seat number", "INVALID_SEAT_NUMBER"));
    }

    // Create seat number object
    auto seatNumberResult = SeatNumber::create(seatNumber, flightResult.value().getAircraft()->getSeatLayout());
    if (!seatNumberResult) {
        if (_logger) _logger->error("Invalid seat number: " + seatNumber);
        return Failure<bool>(CoreError("Invalid seat number", "INVALID_SEAT_NUMBER"));
    }

    // Check if seat is reserved
    auto isAvailableResult = _flightRepository->isSeatAvailable(flightResult.value(), *seatNumberResult);
    if (!isAvailableResult) {
        if (_logger) _logger->error("Failed to check seat availability");
        return Failure<bool>(isAvailableResult.error());
    }

    if (isAvailableResult.value()) {
        if (_logger) _logger->error("Seat is not reserved: " + seatNumber);
        return Failure<bool>(CoreError("Seat is not reserved", "SEAT_NOT_RESERVED"));
    }

    // Release seat in database
    auto releaseResult = _flightRepository->releaseSeat(flightResult.value(), *seatNumberResult);
    if (!releaseResult) {
        if (_logger) _logger->error("Failed to release seat in database");
        return Failure<bool>(releaseResult.error());
    }

    return Success(true);
}

Result<int> FlightService::getRemainingCapacity(const FlightNumber& number) {
    if (_logger) _logger->debug("Getting remaining capacity for flight: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<int>(flightResult.error());
    }

    // Get tickets for this flight
    auto ticketsResult = _ticketRepository->findByFlightId(flightResult.value().getId());
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get flight tickets");
        return Failure<int>(ticketsResult.error());
    }

    // Get reserved seats
    auto reservedSeatsResult = _flightRepository->getReservedSeats(flightResult.value());
    if (!reservedSeatsResult) {
        if (_logger) _logger->error("Failed to get reserved seats");
        return Failure<int>(reservedSeatsResult.error());
    }

    // Calculate remaining capacity
    auto totalSeatCount = flightResult.value().getAircraft()->getSeatLayout().getTotalSeatCount();
    int remainingCapacity = totalSeatCount - ticketsResult.value().size() - reservedSeatsResult.value().size();

    return Success(remainingCapacity);
}

Result<bool> FlightService::canBoardPassenger(const FlightNumber& number, const PassportNumber& passport) {
    if (_logger) _logger->debug("Checking if passenger can board flight: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check flight status
    if (flightResult.value().getStatus() != FlightStatus::SCHEDULED) {
        if (_logger) _logger->error("Flight is not in scheduled status");
        return Failure<bool>(CoreError("Flight is not in scheduled status", "INVALID_STATUS"));
    }

    // Check if passenger has a ticket
    auto ticketsResult = _ticketRepository->findByFlightId(flightResult.value().getId());
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get flight tickets");
        return Failure<bool>(ticketsResult.error());
    }

    // Check if any ticket belongs to this passenger
    for (const auto& ticket : ticketsResult.value()) {
        if (ticket.getPassenger()->getPassport().toString() == passport.toString()) {
            if (ticket.getStatus() == TicketStatus::CANCELLED || 
                ticket.getStatus() == TicketStatus::REFUNDED) {
                return Failure<bool>(CoreError("Ticket is cancelled/refunded", "INVALID_TICKET"));
            }
            return Success(true);
        }
    }

    return Success(false);
}

Result<bool> FlightService::isFlightDepartureImminent(const FlightNumber& number) {
    if (_logger) _logger->debug("Checking if flight departure is imminent: " + number.toString());

    // Get flight
    auto flightResult = _flightRepository->findByFlightNumber(number);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto departureTime = flightResult.value().getSchedule().getDeparture();
    auto departureTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&departureTime));

    // Check if departure is within next 30 minutes
    auto timeUntilDeparture = std::chrono::duration_cast<std::chrono::minutes>(
        departureTimePoint - now
    );

    return Success(timeUntilDeparture.count() <= 30 && timeUntilDeparture.count() >= 0);
}

Result<std::vector<Flight>> FlightService::getConflictingFlights(const AircraftSerial& serial, const Schedule& schedule) {
    if (_logger) _logger->debug("Getting conflicting flights for aircraft: " + serial.toString());

    // Get all flights for this aircraft
    auto flightsResult = _flightRepository->findFlightByAircraft(serial);
    if (!flightsResult) {
        if (_logger) _logger->error("Failed to get aircraft flights");
        return Failure<std::vector<Flight>>(flightsResult.error());
    }

    std::vector<Flight> conflictingFlights;
    for (const auto& flight : flightsResult.value()) {
        // Check if schedules overlap
        if (schedule.overlapsWith(flight.getSchedule())) {
            conflictingFlights.push_back(flight);
        }
    }

    return Success(conflictingFlights);
}

Result<bool> FlightService::validateScheduleForAircraft(const AircraftSerial& serial, const Schedule& schedule) {
    if (_logger) _logger->debug("Validating schedule for aircraft: " + serial.toString());

    // Get conflicting flights
    auto conflictsResult = getConflictingFlights(serial, schedule);
    if (!conflictsResult) {
        if (_logger) _logger->error("Failed to get conflicting flights");
        return Failure<bool>(conflictsResult.error());
    }

    // Schedule is valid if there are no conflicts
    return Success(conflictsResult.value().empty());
}