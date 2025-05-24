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

public:
    FlightRepository(std::shared_ptr<IDatabaseConnection> connection, 
                    std::shared_ptr<Logger> logger = nullptr)
    : _connection(std::move(connection)), _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    }
                    ;

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
};

#endif // FLIGHT_REPOSITORY_H 