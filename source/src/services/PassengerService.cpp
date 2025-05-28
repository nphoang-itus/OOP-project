#include "PassengerService.h"
#include "../core/exceptions/Result.h"
#include <algorithm>

// Private helper methods
Result<Passenger> PassengerService::getPassengerById(int id) {
    return _passengerRepository->findById(id);
}

Result<bool> PassengerService::existsById(const int& id) {
    return _passengerRepository->exists(id);
}

Result<bool> PassengerService::deleteById(const int& id) {
    return _passengerRepository->deleteById(id);
}

// Core CRUD operations
Result<Passenger> PassengerService::getPassenger(const PassportNumber& passport) {
    if (_logger) _logger->debug("Getting passenger with passport: " + passport.toString());
    return _passengerRepository->findByPassportNumber(passport);
}

Result<std::vector<Passenger>> PassengerService::getAllPassengers() {
    if (_logger) _logger->debug("Getting all passengers");
    return _passengerRepository->findAll();
}

Result<bool> PassengerService::passengerExists(const PassportNumber& passport) {
    if (_logger) _logger->debug("Checking if passenger exists with passport: " + passport.toString());
    return _passengerRepository->existsPassport(passport);
}

Result<Passenger> PassengerService::createPassenger(const Passenger& passenger) {
    if (_logger) _logger->debug("Creating passenger with passport: " + passenger.getPassport().toString());

    // Business rule: Check if passenger with same passport already exists
    auto existsResult = _passengerRepository->existsPassport(passenger.getPassport());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check passenger existence");
        return Failure<Passenger>(CoreError("Failed to check passenger existence", "DB_ERROR"));
    }
    if (existsResult.value()) {
        if (_logger) _logger->error("Passenger with passport " + passenger.getPassport().toString() + " already exists");
        return Failure<Passenger>(CoreError("Passenger with this passport already exists", "DUPLICATE_PASSPORT"));
    }

    // Create passenger
    return _passengerRepository->create(passenger);
}

Result<Passenger> PassengerService::updatePassenger(const Passenger& passenger) {
    if (_logger) _logger->debug("Updating passenger with passport: " + passenger.getPassport().toString());

    // Check if passenger exists
    auto existsResult = _passengerRepository->existsPassport(passenger.getPassport());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check passenger existence");
        return Failure<Passenger>(CoreError("Failed to check passenger existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Passenger with passport " + passenger.getPassport().toString() + " not found");
        return Failure<Passenger>(CoreError("Passenger not found", "NOT_FOUND"));
    }

    // Update passenger
    return _passengerRepository->update(passenger);
}

Result<bool> PassengerService::deletePassenger(const PassportNumber& passport) {
    if (_logger) _logger->debug("Deleting passenger with passport: " + passport.toString());

    // Check if passenger exists
    auto existsResult = _passengerRepository->existsPassport(passport);
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check passenger existence");
        return Failure<bool>(CoreError("Failed to check passenger existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Passenger with passport " + passport.toString() + " not found");
        return Failure<bool>(CoreError("Passenger not found", "NOT_FOUND"));
    }

    // Get passenger to check for dependencies
    auto passengerResult = _passengerRepository->findByPassportNumber(passport);
    if (!passengerResult) {
        if (_logger) _logger->error("Failed to get passenger details");
        return Failure<bool>(passengerResult.error());
    }

    // Business rule: Check if passenger has any active bookings
    auto canDeleteResult = canDeletePassenger(passport);
    if (!canDeleteResult) {
        if (_logger) _logger->error("Failed to check if passenger can be deleted");
        return Failure<bool>(canDeleteResult.error());
    }
    if (!canDeleteResult.value()) {
        if (_logger) _logger->error("Cannot delete passenger with active bookings");
        return Failure<bool>(CoreError("Cannot delete passenger with active bookings", "PASSENGER_HAS_BOOKINGS"));
    }

    // Delete passenger
    return _passengerRepository->deleteByPassportNumber(passport);
}

// Passenger validation and business rules
Result<bool> PassengerService::canDeletePassenger(const PassportNumber& passport) {
    if (_logger) _logger->debug("Checking if passenger can be deleted: " + passport.toString());

    // Get passenger
    auto passengerResult = _passengerRepository->findByPassportNumber(passport);
    if (!passengerResult) {
        if (_logger) _logger->error("Failed to get passenger");
        return Failure<bool>(passengerResult.error());
    }

    // Check if passenger has any active bookings
    auto hasBookingsResult = hasActiveBookings(passport);
    if (!hasBookingsResult) {
        if (_logger) _logger->error("Failed to check passenger bookings");
        return Failure<bool>(hasBookingsResult.error());
    }

    // Passenger can be deleted if they have no active bookings
    return Success(!hasBookingsResult.value());
}

Result<bool> PassengerService::hasActiveBookings(const PassportNumber& passport) {
    if (_logger) _logger->debug("Checking if passenger has active bookings: " + passport.toString());

    // Get passenger
    auto passengerResult = _passengerRepository->findByPassportNumber(passport);
    if (!passengerResult) {
        if (_logger) _logger->error("Failed to get passenger");
        return Failure<bool>(passengerResult.error());
    }

    // Get all tickets for this passenger
    auto ticketsResult = _ticketRepository->findByPassengerId(passengerResult.value().getId());
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get passenger tickets");
        return Failure<bool>(ticketsResult.error());
    }

    // Check if any ticket is active (not cancelled or refunded)
    for (const auto& ticket : ticketsResult.value()) {
        if (ticket.getStatus() != TicketStatus::CANCELLED && 
            ticket.getStatus() != TicketStatus::REFUNDED) {
            return Success(true);
        }
    }

    return Success(false);
}

Result<int> PassengerService::getTotalFlightCount(const PassportNumber& passport) {
    if (_logger) _logger->debug("Getting total flight count for passenger: " + passport.toString());

    // Get passenger
    auto passengerResult = _passengerRepository->findByPassportNumber(passport);
    if (!passengerResult) {
        if (_logger) _logger->error("Failed to get passenger");
        return Failure<int>(passengerResult.error());
    }

    // Get all tickets for this passenger
    auto ticketsResult = _ticketRepository->findByPassengerId(passengerResult.value().getId());
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get passenger tickets");
        return Failure<int>(ticketsResult.error());
    }

    // Count active tickets
    int count = 0;
    for (const auto& ticket : ticketsResult.value()) {
        if (ticket.getStatus() != TicketStatus::CANCELLED && 
            ticket.getStatus() != TicketStatus::REFUNDED) {
            count++;
        }
    }

    return Success(count);
}