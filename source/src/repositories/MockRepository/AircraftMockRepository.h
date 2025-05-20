#ifndef AIRCRAFT_MOCK_REPOSITORY_H
#define AIRCRAFT_MOCK_REPOSITORY_H

#include <vector>
#include <unordered_map>
#include "../../src/core/entities/Aircraft.h"
#include "../../src/repositories/InterfaceRepository.h"
#include "../../src/core/exceptions/Result.h"

class AircraftMockRepository : public IRepository<Aircraft> {
private:
    std::unordered_map<int, Aircraft> _aircrafts;
    int _nextId = 1;

public:
    Result<Aircraft> findById(const int& id) override {
        auto it = _aircrafts.find(id);
        if (it != _aircrafts.end()) {
            return Success(it->second);
        }
        return Failure<Aircraft>(CoreError("Aircraft not found with id: " + std::to_string(id), "NOT_FOUND"));
    }

    Result<std::vector<Aircraft>> findAll() override {
        std::vector<Aircraft> result;
        result.reserve(_aircrafts.size());
        for (const auto& [id, aircraft] : _aircrafts) {
            result.push_back(aircraft);
        }
        return Success(result);
    }

    Result<bool> exists(const int& id) override {
        return Success(_aircrafts.find(id) != _aircrafts.end());
    }

    Result<size_t> count() override {
        return Success(_aircrafts.size());
    }

    Result<Aircraft> create(const Aircraft& aircraft) override {
        Aircraft newAircraft = aircraft;
        newAircraft.setId(_nextId++);
        _aircrafts[newAircraft.getId()] = newAircraft;
        return Success(newAircraft);
    }

    Result<Aircraft> update(const Aircraft& aircraft) override {
        auto it = _aircrafts.find(aircraft.getId());
        if (it != _aircrafts.end()) {
            _aircrafts[aircraft.getId()] = aircraft;
            return Success(aircraft);
        }
        return Failure<Aircraft>(CoreError("Aircraft not found with id: " + std::to_string(aircraft.getId()), "NOT_FOUND"));
    }

    Result<bool> deleteById(const int& id) override {
        auto it = _aircrafts.find(id);
        if (it != _aircrafts.end()) {
            _aircrafts.erase(it);
            return Success(true);
        }
        return Success(false);
    }

    // Helper methods for testing
    void clear() {
        _aircrafts.clear();
        _nextId = 1;
    }

    const std::unordered_map<int, Aircraft>& getAircrafts() const {
        return _aircrafts;
    }
};

#endif