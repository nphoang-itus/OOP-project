#ifndef FLIGHT_REPOSITORY_H
#define FLIGHT_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Flight.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include <memory>
#include <vector>

class FlightRepository : public IRepository<Flight> {
private:
    std::shared_ptr<IDatabaseConnection> _connection;
    std::shared_ptr<Logger> _logger;

    Flight mapRowToFlight(const std::map<std::string, std::string>& row) const;
    std::map<SeatNumber, bool> getSeatAvailability(const Flight& flight) const;

public:
    FlightRepository(std::shared_ptr<IDatabaseConnection> connection, 
                    std::shared_ptr<Logger> logger = nullptr)
    : _connection(std::move(connection)), _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    };

    Result<Flight> findById(const int& id) override;
    Result<std::vector<Flight>> findAll() override;
    Result<bool> exists(const int& id) override;
    Result<size_t> count() override;
    Result<Flight> create(const Flight& flight) override;
    Result<Flight> update(const Flight& flight) override;
    Result<bool> deleteById(const int& id) override;

    // Phương thức riêng của FLight
    Result<Flight> findByFlightNumber(const FlightNumber& number);
    Result<bool> existsFlight(const FlightNumber& number);
    Result<std::vector<Flight>> findFlightByAircraft(const AircraftSerial& serial);

    // Phương thức quản lý seat availability
    Result<bool> reserveSeat(const Flight& flight, const SeatNumber& seatNumber);
    Result<bool> releaseSeat(const Flight& flight, const SeatNumber& seatNumber);
    Result<std::vector<SeatNumber>> getAvailableSeats(const Flight& flight);
    Result<std::vector<SeatNumber>> getReservedSeats(const Flight& flight);
    Result<bool> isSeatAvailable(const Flight& flight, const SeatNumber& seatNumber);
};

#endif // FLIGHT_REPOSITORY_H 