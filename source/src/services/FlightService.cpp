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

    // Get all seats for this class
    std::vector<std::string> availableSeats;
    const auto& seatAvailability = flightResult.value().getSeatAvailability();
    for (const auto& [seatNumber, isAvailable] : seatAvailability) {
        if (isAvailable && seatNumber.getValue().substr(0, 1) == classCode) {
            availableSeats.push_back(seatNumber.getValue());
        }
    }

    return Success(availableSeats);
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