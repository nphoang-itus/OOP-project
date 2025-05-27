#ifndef AIRCRAFT_SERVICE_H
#define AIRCRAFT_SERVICE_H

// #include "src/repositories/MySQLRepository/AircraftRepository.h"
#include "../repositories/MockRepository/AircraftMockRepository.h"
#include "../core/entities/Aircraft.h"
#include "../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../core/exceptions/Result.h"
#include "../utils/Logger.h"
#include "InterfaceService.h"
#include <memory>
#include <vector>

class AircraftService : public IService<Aircraft>
{
private:
    std::shared_ptr<AircraftMockRepository> _repository;

public:
    explicit AircraftService(std::shared_ptr<AircraftMockRepository> repository)
        : _repository(std::move(repository)) {}

    // Implementation of IService interface
    Result<Aircraft> getById(const int &id) override;
    Result<std::vector<Aircraft>> getAll() override;
    Result<bool> exists(const int &id) override;
    Result<size_t> count() override;
    Result<Aircraft> create(const Aircraft &entity) override;
    Result<Aircraft> update(const Aircraft &entity) override;
    Result<bool> deleteById(const int &id) override;

    // Aircraft-specific operations
    Result<Aircraft> getAircraftBySerialNumber(const AircraftSerial &serial);
    Result<bool> aircraftExistsBySerialNumber(const AircraftSerial &serial);
    Result<bool> deleteAircraftBySerialNumber(const AircraftSerial &serial);
};

#endif // AIRCRAFT_SERVICE_H