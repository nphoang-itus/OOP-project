#ifndef PASSENGER_REPOSITORY_H
#define PASSENGER_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Passenger.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include "../../utils/Logger.h"
#include <memory>
#include <vector>

class PassengerRepository : public IRepository<Passenger> {
private:
    std::shared_ptr<IDatabaseConnection> _connection;
    std::shared_ptr<Logger> _logger;

public:
    PassengerRepository(std::shared_ptr<IDatabaseConnection> connection, std::shared_ptr<Logger> logger)
        : _connection(connection), _logger(logger) {}

    Result<Passenger> findById(const int& id) override;
    Result<std::vector<Passenger>> findAll() override;
    Result<bool> exists(const int& id) override;
    Result<size_t> count() override;
    Result<Passenger> create(const Passenger& passenger) override;
    Result<Passenger> update(const Passenger& passenger) override;
    Result<bool> deleteById(const int& id) override;
    
    Result<Passenger> findByPassportNumber(const PassportNumber& passport);
    Result<bool> existsPassport(const PassportNumber& passport);
};

#endif 