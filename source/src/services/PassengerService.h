#ifndef PASSENGER_SERVICE_H
#define PASSENGER_SERVICE_H

#include "../core/entities/Passenger.h"
#include "../repositories/MySQLRepository/PassengerRepository.h"
#include "../repositories/MySQLRepository/TicketRepository.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "../utils/Logger.h"
#include <memory>
#include <vector>
#include <string>

class PassengerService {
private:
    std::shared_ptr<PassengerRepository> _passengerRepository;
    std::shared_ptr<TicketRepository> _ticketRepository;
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<Logger> _logger;

    // Private helper methods
    Result<Passenger> getPassengerById(int id);
    Result<bool> existsById(const int& id);
    Result<bool> deleteById(const int& id);

public:
    PassengerService(
        std::shared_ptr<PassengerRepository> passengerRepository,
        std::shared_ptr<TicketRepository> ticketRepository,
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<Logger> logger = nullptr
    ) : _passengerRepository(std::move(passengerRepository))
      , _ticketRepository(std::move(ticketRepository))
      , _flightRepository(std::move(flightRepository))
      , _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    }

    // Core CRUD operations
    Result<Passenger> getPassenger(const PassportNumber& passport);
    Result<std::vector<Passenger>> getAllPassengers();
    Result<bool> passengerExists(const PassportNumber& passport);
    Result<Passenger> createPassenger(const Passenger& passenger);
    Result<Passenger> updatePassenger(const Passenger& passenger);
    Result<bool> deletePassenger(const PassportNumber& passport);
    
    // Passenger validation and business rules
    Result<bool> canDeletePassenger(const PassportNumber& passport);
    Result<bool> hasActiveBookings(const PassportNumber& passport);
    Result<int> getTotalFlightCount(const PassportNumber& passport);
};

#endif // PASSENGER_SERVICE_H