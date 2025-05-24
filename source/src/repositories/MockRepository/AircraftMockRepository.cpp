#include <vector>
#include <unordered_map>
#include "AircraftMockRepository.h"

Result<Aircraft> AircraftMockRepository::findById(const int &id)
{
    auto it = _aircrafts.find(id);
    if (it != _aircrafts.end())
    {
        return it->second;
    }
    return std::unexpected(CoreError("Aircraft not found with id: " + std::to_string(id), "NOT_FOUND"));
}

Result<std::vector<Aircraft>> AircraftMockRepository::findAll()
{
    std::vector<Aircraft> result;
    result.reserve(_aircrafts.size());
    for (const auto &[id, aircraft] : _aircrafts)
    {
        result.push_back(aircraft);
    }
    return result;
}

Result<bool> AircraftMockRepository::exists(const int &id)
{
    return _aircrafts.find(id) != _aircrafts.end();
}

Result<size_t> AircraftMockRepository::count()
{
    return _aircrafts.size();
}

Result<Aircraft> AircraftMockRepository::create(const Aircraft &aircraft)
{
    Aircraft newAircraft = aircraft;
    newAircraft.setId(_nextId++);
    _aircrafts.emplace(newAircraft.getId(), newAircraft);
    return newAircraft;
}

Result<Aircraft> AircraftMockRepository::update(const Aircraft &aircraft)
{
    auto it = _aircrafts.find(aircraft.getId());
    if (it != _aircrafts.end())
    {
        it->second = aircraft;
        return aircraft;
    }
    return std::unexpected(CoreError("Aircraft not found with id: " + std::to_string(aircraft.getId()), "NOT_FOUND"));
}

Result<bool> AircraftMockRepository::deleteById(const int &id)
{
    auto it = _aircrafts.find(id);
    if (it != _aircrafts.end())
    {
        _aircrafts.erase(it);
        return true;
    }
    return false;
}

void AircraftMockRepository::clear()
{
    _aircrafts.clear();
    _nextId = 1;
}

const std::unordered_map<int, Aircraft> &AircraftMockRepository::getAircrafts() const
{
    return _aircrafts;
}

Result<Aircraft> AircraftMockRepository::findBySerialNumber(const AircraftSerial &serial)
{
    for (const auto &[id, aircraft] : _aircrafts)
    {
        if (aircraft.getSerial() == serial)
        {
            return aircraft;
        }
    }
    return std::unexpected(CoreError("Aircraft not found with serial number: " + serial.toString(), "NOT_FOUND"));
}