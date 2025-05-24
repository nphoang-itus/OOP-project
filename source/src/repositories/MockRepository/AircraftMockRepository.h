#ifndef AIRCRAFT_MOCK_REPOSITORY_H
#define AIRCRAFT_MOCK_REPOSITORY_H

#include <vector>
#include <unordered_map>
#include "core/entities/Aircraft.h"
#include "repositories/InterfaceRepository.h"
#include "core/exceptions/Result.h"

class AircraftMockRepository : public IRepository<Aircraft>
{
private:
    std::unordered_map<int, Aircraft> _aircrafts;
    int _nextId = 1;

public:
    Result<Aircraft> findById(const int &id) override;
    Result<std::vector<Aircraft>> findAll() override;
    Result<bool> exists(const int &id) override;
    Result<size_t> count() override;
    Result<Aircraft> create(const Aircraft &aircraft) override;
    Result<Aircraft> update(const Aircraft &aircraft) override;
    Result<bool> deleteById(const int &id) override;
    void clear();
    const std::unordered_map<int, Aircraft> &getAircrafts() const;
};

#endif