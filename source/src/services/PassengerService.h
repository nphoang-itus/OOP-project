#ifndef PASSENGER_SERVICE_H
#define PASSENGER_SERVICE_H

#include "repositories/MockRepository/PassengerMockRepository.h"
#include "core/entities/Passenger.h"
#include "core/exceptions/Result.h"
#include "utils/Logger.h"
#include <memory>
#include <vector>

class PassengerService
{
private:
    std::shared_ptr<PassengerMockRepository> _repository;
    std::shared_ptr<Logger> _logger;

public:
    PassengerService(std::shared_ptr<PassengerMockRepository> repository, std::shared_ptr<Logger> logger)
        : _repository(repository), _logger(logger) {}

    Result<Passenger> getPassengerById(const int &id);
    Result<std::vector<Passenger>> getAllPassengers();
    Result<Passenger> createPassenger(const Passenger &passenger);
    Result<Passenger> updatePassenger(const Passenger &passenger);
    Result<bool> deletePassenger(const int &id);

    Result<Passenger> getPassengerByPassport(const PassportNumber &passport);
    Result<bool> passengerExists(const int &id);
    Result<bool> passportExists(const PassportNumber &passport);
    Result<size_t> getPassengerCount();
};

#endif