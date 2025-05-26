#include <vector>
#include <unordered_map>
#include "PassengerMockRepository.h"

Result<Passenger> PassengerMockRepository::findById(const int &id)
{
    auto it = _passengers.find(id);
    if (it != _passengers.end())
    {
        return it->second;
    }
    return std::unexpected(CoreError("Passenger not found with id: " + std::to_string(id), "NOT_FOUND"));
}

Result<std::vector<Passenger>> PassengerMockRepository::findAll()
{
    std::vector<Passenger> result;
    result.reserve(_passengers.size());
    for (const auto &[id, passenger] : _passengers)
    {
        result.push_back(passenger);
    }
    return result;
}

Result<bool> PassengerMockRepository::exists(const int &id)
{
    return _passengers.find(id) != _passengers.end();
}

Result<size_t> PassengerMockRepository::count()
{
    return _passengers.size();
}

Result<Passenger> PassengerMockRepository::create(const Passenger &passenger)
{
    Passenger newPassenger = passenger;
    newPassenger.setId(_nextId++);
    _passengers.emplace(newPassenger.getId(), newPassenger);
    return newPassenger;
}

Result<Passenger> PassengerMockRepository::update(const Passenger &passenger)
{
    auto it = _passengers.find(passenger.getId());
    if (it != _passengers.end())
    {
        it->second = passenger;
        return passenger;
    }
    return std::unexpected(CoreError("Passenger not found with id: " + std::to_string(passenger.getId()), "NOT_FOUND"));
}

Result<bool> PassengerMockRepository::deleteById(const int &id)
{
    auto it = _passengers.find(id);
    if (it != _passengers.end())
    {
        _passengers.erase(it);
        return true;
    }
    return false;
}

void PassengerMockRepository::clear()
{
    _passengers.clear();
    _nextId = 1;
}

const std::unordered_map<int, Passenger> &PassengerMockRepository::getPassengers() const
{
    return _passengers;
}

Result<Passenger> PassengerMockRepository::findByPassportNumber(const PassportNumber &passport)
{
    for (const auto &[id, passenger] : _passengers)
    {
        if (passenger.getPassport() == passport)
        {
            return passenger;
        }
    }
    return std::unexpected(CoreError("Passenger not found with passport: " + passport.toString(), "NOT_FOUND"));
}

Result<bool> PassengerMockRepository::existsPassport(const PassportNumber &passport)
{
    for (const auto &[id, passenger] : _passengers)
    {
        if (passenger.getPassport() == passport)
        {
            return true;
        }
    }
    return false;
}

Result<bool> PassengerMockRepository::deleteByPassport(const PassportNumber &passport)
{
    for (auto it = _passengers.begin(); it != _passengers.end(); ++it)
    {
        if (it->second.getPassport() == passport)
        {
            _passengers.erase(it);
            return true;
        }
    }
    return false;
}