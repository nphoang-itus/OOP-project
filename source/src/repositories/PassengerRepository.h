/**
 * @file PassengerRepository.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef PASSENGER_REPOSITORY_H
#define PASSENGER_REPOSITORY_H

#include "RepositoryInterface.h"
#include "../core/Passenger.h"
#include "../database/DatabaseConnection.h"
#include "../utils/Logger.h"

#include <memory>
#include <vector>
#include <optional>

class PassengerRepository : public IRepository<Passenger> {
private:
    std::shared_ptr<IDatabaseConnection> _dbConnection;
    std::shared_ptr<Logger> _logger;
    
    Passenger _mapResultToPassenger(IDatabaseResult& result);

public:
    explicit PassengerRepository(std::shared_ptr<IDatabaseConnection> dbConnection);
    
    std::vector<Passenger> findAll() override;
    std::optional<Passenger> findById(const int& id) override;
    bool save(Passenger& passenger) override;
    bool update(const Passenger& passenger) override;
    bool remove(const int& id) override;

    std::optional<Passenger> findByPassport(const std::string& passport);
    std::vector<Passenger> findByName(const std::string& name);
};

#endif