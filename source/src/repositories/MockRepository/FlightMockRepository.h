#ifndef FLIGHT_MOCK_REPOSITORY_H
#define FLIGHT_MOCK_REPOSITORY_H

#include <vector>
#include <unordered_map>
#include "core/entities/Flight.h"
#include "repositories/InterfaceRepository.h"
#include "core/exceptions/Result.h"

class FlightMockRepository : public IRepository<Flight>
{
private:
    std::unordered_map<int, Flight> _flights;
    int _nextId = 1;

public:
    Result<Flight> findById(const int &id) override;
    Result<std::vector<Flight>> findAll() override;
    Result<bool> exists(const int &id) override;
    Result<size_t> count() override;
    Result<Flight> create(const Flight &flight) override;
    Result<Flight> update(const Flight &flight) override;
    Result<bool> deleteById(const int &id) override;
    void clear();
    const std::unordered_map<int, Flight> &getFlights() const;

    // Phương thức riêng của Flight
    Result<Flight> findByFlightNumber(const FlightNumber &number);
    Result<bool> existsFlight(const FlightNumber &number);
};

#endif
