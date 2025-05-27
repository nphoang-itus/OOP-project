#ifndef FLIGHT_SERVICE_H
#define FLIGHT_SERVICE_H

#include "InterfaceService.h"
#include "core/entities/Flight.h"
#include "core/value_objects/flight_number/FlightNumber.h"
#include "core/exceptions/Result.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "utils/Logger.h"
#include <memory>
#include <vector>

class FlightService : public IService<Flight>
{
private:
    std::shared_ptr<FlightRepository> _repository;

public:
    explicit FlightService(std::shared_ptr<FlightRepository> repository)
        : _repository(std::move(repository)) {}

    // Implementation of IService interface
    Result<Flight> getById(const int &id) override;
    Result<std::vector<Flight>> getAll() override;
    Result<bool> exists(const int &id) override;
    Result<size_t> count() override;
    Result<Flight> create(const Flight &entity) override;
    Result<Flight> update(const Flight &entity) override;
    Result<bool> deleteById(const int &id) override;

    // Flight-specific operations (matching repository methods)
    Result<Flight> getFlightByNumber(const FlightNumber &number);
    Result<bool> flightExistsByNumber(const FlightNumber &number);
};

#endif // FLIGHT_SERVICE_H