#ifndef AIRCRAFT_REPOSITORY_H
#define AIRCRAFT_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Aircraft.h"
#include "../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

class AircraftRepository : public IRepository<Aircraft> {
private:
    std::shared_ptr<IDatabaseConnection> _connection;
    std::shared_ptr<Logger> _logger;

public:
    explicit AircraftRepository(std::shared_ptr<IDatabaseConnection> connection, 
                              std::shared_ptr<Logger> logger = nullptr)
        : _connection(std::move(connection))
        , _logger(std::move(logger)) {}

    Result<Aircraft> findById(const int& id) override;
    Result<std::vector<Aircraft>> findAll() override;
    Result<bool> exists(const int& id) override;
    Result<size_t> count() override;
    Result<Aircraft> create(const Aircraft& aircraft) override;
    Result<Aircraft> update(const Aircraft& aircraft) override;
    Result<bool> deleteById(const int& id) override;

private:
    Aircraft mapRowToAircraft(const std::map<std::string, std::string>& row) const;
};

#endif // AIRCRAFT_REPOSITORY_H
