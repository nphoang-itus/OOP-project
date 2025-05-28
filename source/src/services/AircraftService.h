#ifndef AIRCRAFT_SERVICE_H
#define AIRCRAFT_SERVICE_H

#include "../core/entities/Aircraft.h"
#include "../repositories/MySQLRepository/AircraftRepository.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "../repositories/MySQLRepository/TicketRepository.h"
#include "../utils/Logger.h"
#include <memory>
#include <vector>
#include <string>

class AircraftService {
private:
    std::shared_ptr<AircraftRepository> _aircraftRepository;
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<TicketRepository> _ticketRepository;
    std::shared_ptr<Logger> _logger;

    // Private helper methods
    Result<Aircraft> getAircraftById(int id);
    Result<bool> existsById(const int& id);
    Result<bool> deleteById(const int& id);

public:
    AircraftService(
        std::shared_ptr<AircraftRepository> aircraftRepository,
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<TicketRepository> ticketRepository,
        std::shared_ptr<Logger> logger = nullptr
    ) : _aircraftRepository(std::move(aircraftRepository))
      , _flightRepository(std::move(flightRepository))
      , _ticketRepository(std::move(ticketRepository))
      , _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    }

    // Core CRUD operations
    Result<Aircraft> getAircraft(const AircraftSerial& serial);
    Result<std::vector<Aircraft>> getAllAircraft();
    Result<bool> aircraftExists(const AircraftSerial& serial);
    Result<Aircraft> createAircraft(const Aircraft& aircraft);
    Result<Aircraft> updateAircraft(const Aircraft& aircraft);
    Result<bool> deleteAircraft(const AircraftSerial& serial);
    
    // Business operations
    Result<std::vector<std::string>> getAvailableSeatClasses(const AircraftSerial& serial);
    Result<std::vector<std::string>> getAvailableSeats(const AircraftSerial& serial, const std::string& seatClass);
    Result<bool> isSeatAvailable(const AircraftSerial& serial, const std::string& seatNumber);
};

#endif // AIRCRAFT_SERVICE_H