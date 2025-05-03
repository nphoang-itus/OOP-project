/**
 * @file FlightRepository.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLIGHT_REPOSITORY_H
#define FLIGHT_REPOSITORY_H

#include "RepositoryInterface.h"
#include "../core/Flight.h"
#include "../database/DatabaseConnection.h"
#include "../utils/Logger.h"

#include <memory>
#include <vector>
#include <optional>

class FlightRepository : public IRepository<Flight> {
private:
    std::shared_ptr<IDatabaseConnection> _dbConnection;
    std::shared_ptr<Logger> _logger;
    
    Flight _mapResultToFlight(IDatabaseResult& result);

public:
    explicit FlightRepository(std::shared_ptr<IDatabaseConnection> _dbConnection);
    
    std::vector<Flight> findAll() override;
    std::optional<Flight> findById(int id) override;
    bool save(Flight& flight) override;
    bool update(const Flight& flight) override;
    bool remove(int id) override;
    
    // Additional methods
    std::vector<Flight> findAvailableFlights();
    std::optional<Flight> findByFlightNo(const std::string& flightNo);
};

#endif