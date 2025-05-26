#include "FlightMockRepository.h"

Result<Flight> FlightMockRepository::findById(const int &id)
{
    auto it = _flights.find(id);
    if (it != _flights.end())
    {
        return it->second;
    }
    return std::unexpected(CoreError("Flight not found with id: " + std::to_string(id), "NOT_FOUND"));
}

Result<std::vector<Flight>> FlightMockRepository::findAll()
{
    std::vector<Flight> result;
    result.reserve(_flights.size());
    for (const auto &[id, flight] : _flights)
    {
        result.push_back(flight);
    }
    return result;
}

Result<bool> FlightMockRepository::exists(const int &id)
{
    return _flights.find(id) != _flights.end();
}

Result<size_t> FlightMockRepository::count()
{
    return _flights.size();
}

Result<Flight> FlightMockRepository::create(const Flight &flight)
{
    Flight newFlight = flight;
    newFlight.setId(_nextId++);
    _flights.emplace(newFlight.getId(), newFlight);
    return newFlight;
}

Result<Flight> FlightMockRepository::update(const Flight &flight)
{
    auto it = _flights.find(flight.getId());
    if (it != _flights.end())
    {
        it->second = flight;
        return flight;
    }
    return std::unexpected(CoreError("Flight not found with id: " + std::to_string(flight.getId()), "NOT_FOUND"));
}

Result<bool> FlightMockRepository::deleteById(const int &id)
{
    return _flights.erase(id) > 0;
}

void FlightMockRepository::clear()
{
    _flights.clear();
    _nextId = 1;
}

const std::unordered_map<int, Flight> &FlightMockRepository::getFlights() const
{
    return _flights;
}

Result<Flight> FlightMockRepository::findByFlightNumber(const FlightNumber &number)
{
    for (const auto &[id, flight] : _flights)
    {
        if (flight.getFlightNumber() == number)
        {
            return flight;
        }
    }
    return std::unexpected(CoreError("Flight not found with number: " + number.toString(), "NOT_FOUND"));
}

Result<bool> FlightMockRepository::existsFlight(const FlightNumber &number)
{
    for (const auto &[id, flight] : _flights)
    {
        if (flight.getFlightNumber() == number)
        {
            return true;
        }
    }
    return false;
}
