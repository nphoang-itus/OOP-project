#ifndef FLIGHT_SERVICE_H
#define FLIGHT_SERVICE_H

#include "../core/entities/Flight.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "../repositories/MySQLRepository/AircraftRepository.h"
#include "../repositories/MySQLRepository/TicketRepository.h"
#include "../utils/Logger.h"
#include <memory>
#include <vector>
#include <string>

class FlightService {
private:
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<AircraftRepository> _aircraftRepository;
    std::shared_ptr<TicketRepository> _ticketRepository;
    std::shared_ptr<Logger> _logger;

    // Private helper methods
    Result<Flight> getFlightById(int id);
    Result<bool> existsById(const int& id);
    Result<bool> deleteById(const int& id);

public:
    FlightService(
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<AircraftRepository> aircraftRepository,
        std::shared_ptr<TicketRepository> ticketRepository,
        std::shared_ptr<Logger> logger = nullptr
    ) : _flightRepository(std::move(flightRepository))
      , _aircraftRepository(std::move(aircraftRepository))
      , _ticketRepository(std::move(ticketRepository))
      , _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    }

    // Core CRUD operations
    Result<Flight> getFlight(const FlightNumber& number);
    Result<std::vector<Flight>> getAllFlights();
    Result<bool> flightExists(const FlightNumber& number);
    Result<Flight> createFlight(const Flight& flight);
    Result<Flight> updateFlight(const Flight& flight);
    Result<bool> deleteFlight(const FlightNumber& number);
    
    // Business operations
    Result<std::vector<Flight>> getFlightsByAircraft(const AircraftSerial& serial);
    Result<std::vector<Flight>> getFlightsByRoute(const std::string& originCode, const std::string& destinationCode);
    Result<std::vector<Flight>> getFlightsByDate(const std::string& date);
    Result<bool> isSeatAvailable(const FlightNumber& number, const std::string& seatNumber);
    Result<std::vector<std::string>> getAvailableSeats(const FlightNumber& number, const std::string& seatClass);
    Result<bool> updateFlightStatus(const FlightNumber& number, FlightStatus status);
};

#endif // FLIGHT_SERVICE_H 