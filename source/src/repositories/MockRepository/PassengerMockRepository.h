#ifndef PASSENGER_MOCK_REPOSITORY_H
#define PASSENGER_MOCK_REPOSITORY_H

#include <vector>
#include <unordered_map>
#include "core/entities/Passenger.h"
#include "repositories/InterfaceRepository.h"
#include "core/exceptions/Result.h"
#include "utils/Logger.h"
#include <memory>

class PassengerMockRepository : public IRepository<Passenger>
{
private:
    std::unordered_map<int, Passenger> _passengers;
    int _nextId = 1;
    std::shared_ptr<Logger> _logger;

public:
    PassengerMockRepository(std::shared_ptr<Logger> logger) : _logger(logger) {}

    Result<Passenger> findById(const int &id) override;
    Result<std::vector<Passenger>> findAll() override;
    Result<bool> exists(const int &id) override;
    Result<size_t> count() override;
    Result<Passenger> create(const Passenger &passenger) override;
    Result<Passenger> update(const Passenger &passenger) override;
    Result<bool> deleteById(const int &id) override;
    void clear();
    const std::unordered_map<int, Passenger> &getPassengers() const;

    Result<Passenger> findByPassportNumber(const PassportNumber &passport);
    Result<bool> existsPassport(const PassportNumber &passport);
    Result<bool> deleteByPassport(const PassportNumber &passport);
};

#endif