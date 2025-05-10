/**
 * @file PassengerRepository.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

    #include "PassengerRepository.h"
#include "../utils/TableConstants.h"

#include "memory"
#include "stdexcept"
#include <sstream>
#include <format>

namespace PT = Tables::Passenger;

Passenger PassengerRepository::_mapResultToPassenger(IDatabaseResult& result) {
    Passenger passenger;
    
    passenger.setId(result.getInt(PT::ColumnNumber::ID));
    passenger.setName(result.getString(PT::ColumnNumber::NAME));
    passenger.setPassport(result.getString(PT::ColumnNumber::PASSPORT));

    // Map Các trường có thể NULL
    try {
        passenger.setPhone(result.getString(PT::ColumnNumber::PHONE));
    } catch (...) {
        passenger.setPhone("");
    }

    try {
        passenger.setAddress(result.getString(PT::ColumnNumber::ADDRESS));
    } catch (...) {
        passenger.setAddress("");
    }
    
    return passenger;
}

PassengerRepository::PassengerRepository(std::shared_ptr<IDatabaseConnection> dbConnection)
    : _dbConnection(std::move(dbConnection)), _logger(Logger::getInstance()) {

    _logger->debug("Create Passenger Repository Object.");

    if (!_dbConnection) {
        _logger->error("Database connection is null in PassengerRepository constructor");
        throw std::invalid_argument("Database connection cannot be null");
    }

    _logger->info("Passenger Repository initialized successfully");
}

std::vector<Passenger> PassengerRepository::findAll() {
    std::vector<Passenger> passengers;

    try {
        _logger->debug("Finding all passenger");

        auto result = _dbConnection->executeQuery(PT::getOrderedSelectClause());

        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return passengers; 
        }

        while (result->next()) {
            passengers.push_back(_mapResultToPassenger(*result));
        }

        _logger->info("Found " + std::to_string(passengers.size()) + " passengers");
    } catch (const std::exception& e) {
        _logger->error("Exception in findAll(): " + std::string(e.what()));
    }

    return passengers;
}

std::optional<Passenger> PassengerRepository::findById(const int& id) {
    try {
        _logger->debug("Finding passenger by ID: " + std::to_string(id));

        std::string query = PT::getOrderedSelectClause() +
                            " WHERE " + PT::ColumnName[PT::ID] + 
                            " = " + std::to_string(id);

        auto result = _dbConnection->executeQuery(query);

        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return std::nullopt; 
        }

        if (result->next()) {
            _logger->info("Found passenger with ID: " + std::to_string(id));
            return _mapResultToPassenger(*result);;
        }

        _logger->info("Passenger with ID " + std::to_string(id) + " not found");
    } catch (const std::exception& e) {
        _logger->error("Exception in findById(): " + std::string(e.what()));
    }
    
    return std::nullopt;
}

bool PassengerRepository::save(Passenger& passenger) {
    try {
        _logger->debug("Saving passenger: " + passenger.getPassport());

        std::ostringstream ss;
        ss << "INSERT INTO " << PT::NAME_TABLE << " ("
           << PT::ColumnName[PT::NAME] << ", "
           << PT::ColumnName[PT::PHONE] << ", "
           << PT::ColumnName[PT::PASSPORT] << ", "
           << PT::ColumnName[PT::ADDRESS]
           << ") VALUES ('"
           << passenger.getName() << "', '"
           << passenger.getPhone() << "', '"
           << passenger.getPassport() << "', '"
           << passenger.getAddress() << "')";
        
        if (_dbConnection->execute(ss.str())) {
            passenger.setId(_dbConnection->getLastInsertId());
            _logger->info("Passenger saved successfully with ID: " + std::to_string(passenger.getId()));
            return true;
        }
        
        _logger->error("Failed to save passenger: " + _dbConnection->getLastError());
    } catch (const std::exception& e) {
        _logger->error("Exception in save(): " + std::string(e.what()));
    }
    
    return false;
};

bool PassengerRepository::update(const Passenger& passenger) {
    try {
        _logger->debug("Updating passenegr with ID: " + std::to_string(passenger.getId()));
        
        std::string query = std::format(
            "UPDATE {} SET {} = '{}', {} = '{}', {} = '{}', {} = '{}' WHERE {} = {} ",
            PT::NAME_TABLE,
            PT::ColumnName[PT::NAME], passenger.getName(),
            PT::ColumnName[PT::PHONE], passenger.getPhone(),
            PT::ColumnName[PT::PASSPORT], passenger.getPassport(),
            PT::ColumnName[PT::ADDRESS], passenger.getAddress(),
            PT::ColumnName[PT::ID], passenger.getId()
        );
        
        bool success = _dbConnection->execute(query);
        
        if (success) {
            _logger->info("Passenger updated successfully with ID: " + std::to_string(passenger.getId()));
        } else {
            _logger->error("Failed to update passenger: " + _dbConnection->getLastError());
        }
        
        return success;
    } catch (const std::exception& e) {
        _logger->error("Exception in update(): " + std::string(e.what()));
    }
    
    return false;
}

bool PassengerRepository::remove(const int& id) {
    try {
        _logger->debug("Removing passenger with ID: " + std::to_string(id));
        
        std::string query = std::format (
            "DELETE FROM {} WHERE {} = {}",
            PT::NAME_TABLE,
            PT::ColumnName[PT::ID],
            id
        );
        
        bool success = _dbConnection->execute(query);
        
        if (success) {
            _logger->info("Passenger removed successfully with ID: " + std::to_string(id));
        } else {
            _logger->error("Failed to remove passenger: " + _dbConnection->getLastError());
        }
        
        return success;
    } catch (const std::exception& e) {
        _logger->error("Exception in remove(): " + std::string(e.what()));
    }
    
    return false;
}

std::optional<Passenger> PassengerRepository::findByPassport(const std::string& passport) {
    try {
        _logger->debug("Finding passenger by passport: " + passport);
        
        std::string query = std::format (
            "SELECT * FROM {} WHERE {} = '{}'",
            PT::NAME_TABLE,
            PT::ColumnName[PT::PASSPORT],
            passport
        );
        
        auto result = _dbConnection->executeQuery(query);
        
        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return std::nullopt;
        }
        
        if (result->next()) {
            _logger->info("Found passenger with passport: " + passport);
            return _mapResultToPassenger(*result);
        }
        
        _logger->info("Passenger with passport " + passport + " not found");
    } catch (const std::exception& e) {
        _logger->error("Exception in findByPassport(): " + std::string(e.what()));
    }
    
    return std::nullopt;
}

std::vector<Passenger> PassengerRepository::findByName(const std::string& name) {
    std::vector<Passenger> passengers;
    
    try {
        _logger->debug("Finding available flights");
        
        std::string query = std::format (
            "SELECT * FROM {} WHERE {} = '{}'",
            PT::NAME_TABLE,
            PT::ColumnName[PT::NAME],
            name
        );
        
        auto result = _dbConnection->executeQuery(query);
        
        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return passengers;
        }
        
        while (result->next()) {
            passengers.push_back(_mapResultToPassenger(*result));
        }
        
        _logger->info("Found " + std::to_string(passengers.size()) + " has name " + name);
    } catch (const std::exception& e) {
        _logger->error("Exception in findByName(): " + std::string(e.what()));
    }
    
    return passengers;
}