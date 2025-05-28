#include "AircraftService.h"
#include "../core/exceptions/Result.h"
#include <algorithm>

// Private helper methods
Result<Aircraft> AircraftService::getAircraftById(int id) {
    return _aircraftRepository->findById(id);
}

Result<bool> AircraftService::existsById(const int& id) {
    return _aircraftRepository->exists(id);
}

Result<bool> AircraftService::deleteById(const int& id) {
    return _aircraftRepository->deleteById(id);
}

// Core CRUD operations
Result<Aircraft> AircraftService::getAircraft(const AircraftSerial& serial) {
    if (_logger) _logger->debug("Getting aircraft with serial: " + serial.toString());
    return _aircraftRepository->findBySerialNumber(serial);
}

Result<std::vector<Aircraft>> AircraftService::getAllAircraft() {
    if (_logger) _logger->debug("Getting all aircraft");
    return _aircraftRepository->findAll();
}

Result<bool> AircraftService::aircraftExists(const AircraftSerial& serial) {
    if (_logger) _logger->debug("Checking if aircraft exists with serial: " + serial.toString());
    return _aircraftRepository->existsAircraft(serial);
}

Result<Aircraft> AircraftService::createAircraft(const Aircraft& aircraft) {
    if (_logger) _logger->debug("Creating aircraft with serial: " + aircraft.getSerial().toString());

    // Business rule: Check if aircraft with same serial already exists
    auto existsResult = _aircraftRepository->existsAircraft(aircraft.getSerial());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check aircraft existence");
        return Failure<Aircraft>(CoreError("Failed to check aircraft existence", "DB_ERROR"));
    }
    if (existsResult.value()) {
        if (_logger) _logger->error("Aircraft with serial " + aircraft.getSerial().toString() + " already exists");
        return Failure<Aircraft>(CoreError("Aircraft with this serial number already exists", "DUPLICATE_SERIAL"));
    }

    // Create aircraft
    return _aircraftRepository->create(aircraft);
}

Result<Aircraft> AircraftService::updateAircraft(const Aircraft& aircraft) {
    if (_logger) _logger->debug("Updating aircraft with serial: " + aircraft.getSerial().toString());

    // Check if aircraft exists
    auto existsResult = _aircraftRepository->existsAircraft(aircraft.getSerial());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check aircraft existence");
        return Failure<Aircraft>(CoreError("Failed to check aircraft existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Aircraft with serial " + aircraft.getSerial().toString() + " not found");
        return Failure<Aircraft>(CoreError("Aircraft not found", "NOT_FOUND"));
    }

    // Update aircraft
    return _aircraftRepository->update(aircraft);
}

Result<bool> AircraftService::deleteAircraft(const AircraftSerial& serial) {
    if (_logger) _logger->debug("Deleting aircraft with serial: " + serial.toString());

    // Check if aircraft exists
    auto existsResult = _aircraftRepository->existsAircraft(serial);
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check aircraft existence");
        return Failure<bool>(CoreError("Failed to check aircraft existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Aircraft with serial " + serial.toString() + " not found");
        return Failure<bool>(CoreError("Aircraft not found", "NOT_FOUND"));
    }

    // Get aircraft to check for dependencies
    auto aircraftResult = _aircraftRepository->findBySerialNumber(serial);
    if (!aircraftResult) {
        if (_logger) _logger->error("Failed to get aircraft details");
        return Failure<bool>(aircraftResult.error());
    }

    // Business rule: Check if aircraft is used in any flights
    auto flightsResult = _flightRepository->findAircraft(serial);
    if (!flightsResult) {
        if (_logger) _logger->error("Failed to check aircraft flights");
        return Failure<bool>(flightsResult.error());
    }
    if (!flightsResult.value().empty()) {
        if (_logger) _logger->error("Cannot delete aircraft that is used in flights");
        return Failure<bool>(CoreError("Cannot delete aircraft that is used in flights", "AIRCRAFT_IN_USE"));
    }

    // Business rule: Check if aircraft has any tickets
    auto ticketsResult = _ticketRepository->findBySerialNumber(serial);
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to check aircraft tickets");
        return Failure<bool>(ticketsResult.error());
    }
    if (!ticketsResult.value().empty()) {
        if (_logger) _logger->error("Cannot delete aircraft that has tickets");
        return Failure<bool>(CoreError("Cannot delete aircraft that has tickets", "AIRCRAFT_HAS_TICKETS"));
    }

    // Delete aircraft
    return _aircraftRepository->deleteBySerialNumber(serial);
}

// Business operations
Result<std::vector<std::string>> AircraftService::getAvailableSeatClasses(const AircraftSerial& serial) {
    if (_logger) _logger->debug("Getting available seat classes for aircraft: " + serial.toString());

    // Get aircraft
    auto aircraftResult = _aircraftRepository->findBySerialNumber(serial);
    if (!aircraftResult) {
        if (_logger) _logger->error("Failed to get aircraft");
        return Failure<std::vector<std::string>>(aircraftResult.error());
    }

    // Get seat classes from aircraft
    std::vector<std::string> seatClasses;
    const auto& seatLayout = aircraftResult.value().getSeatLayout();
    
    if (seatLayout.hasSeatClass("E")) seatClasses.push_back("ECONOMY");
    if (seatLayout.hasSeatClass("B")) seatClasses.push_back("BUSINESS");
    if (seatLayout.hasSeatClass("F")) seatClasses.push_back("FIRST");

    return Success(seatClasses);
}

Result<std::vector<std::string>> AircraftService::getAvailableSeats(const AircraftSerial& serial, const std::string& seatClass) {
    if (_logger) _logger->debug("Getting available seats for aircraft: " + serial.toString() + ", class: " + seatClass);

    // Get aircraft
    auto aircraftResult = _aircraftRepository->findBySerialNumber(serial);
    if (!aircraftResult) {
        if (_logger) _logger->error("Failed to get aircraft");
        return Failure<std::vector<std::string>>(aircraftResult.error());
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
    if (!aircraftResult.value().hasSeatClass(classCode)) {
        if (_logger) _logger->error("Aircraft does not have seat class: " + seatClass);
        return Failure<std::vector<std::string>>(CoreError("Aircraft does not have this seat class", "INVALID_SEAT_CLASS"));
    }

    int seatCount = aircraftResult.value().getSeatCount(classCode);
    int padding = std::to_string(seatCount).length();
    
    // Get all seats for this class
    std::vector<std::string> availableSeats;
    for (int i = 1; i <= seatCount; i++) {
        std::stringstream ss;
        ss << classCode << std::setfill('0') << std::setw(padding) << i;
        std::string seatNumber = ss.str();
        availableSeats.push_back(seatNumber);
    }

    // Get tickets for this aircraft
    auto ticketsResult = _ticketRepository->findBySerialNumber(serial);
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get tickets");
        return Failure<std::vector<std::string>>(ticketsResult.error());
    }

    // Remove occupied seats
    for (const auto& ticket : ticketsResult.value()) {
        if (ticket.getSeatNumber().getValue().substr(0, 1) == classCode) {
            auto it = std::find(availableSeats.begin(), availableSeats.end(), ticket.getSeatNumber().getValue());
            if (it != availableSeats.end()) {
                availableSeats.erase(it);
            }
        }
    }

    return Success(availableSeats);
}

Result<bool> AircraftService::isSeatAvailable(const AircraftSerial& serial, const std::string& seatNumber) {
    if (_logger) _logger->debug("Checking if seat is available for aircraft: " + serial.toString() + ", seat: " + seatNumber);

    // Get aircraft
    auto aircraftResult = _aircraftRepository->findBySerialNumber(serial);
    if (!aircraftResult) {
        if (_logger) _logger->error("Failed to get aircraft");
        return Failure<bool>(aircraftResult.error());
    }

    // Validate seat number
    if (!aircraftResult.value().isValidSeatNumber(seatNumber)) {
        if (_logger) _logger->error("Invalid seat number: " + seatNumber);
        return Failure<bool>(CoreError("Invalid seat number", "INVALID_SEAT_NUMBER"));
    }

    // Get tickets for this aircraft
    auto ticketsResult = _ticketRepository->findBySerialNumber(serial);
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get tickets");
        return Failure<bool>(ticketsResult.error());
    }

    // Check if seat is occupied
    for (const auto& ticket : ticketsResult.value()) {
        if (ticket.getSeatNumber().getValue() == seatNumber) {
            return Success(false);
        }
    }

    return Success(true);
}
