/**
 * @file FlightRepository.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Cài đặt các hàm quản lý các thao tác với dữ liệu chuyến bay (Flight) trong cơ sở dữ liệu.
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "FlightRepository.h"
#include "../utils/TableConstants.h"
#include "../utils/Logger.h"
#include "../utils/utils.h"

#include <sstream>

namespace FT = Tables::Flight;

Flight FlightRepository::_mapResultToFlight(IDatabaseResult& result) {
    Flight flight;
    
    flight.setId(result.getInt(FT::ColumnNumber::ID));
    flight.setNo(result.getString(FT::ColumnNumber::NO));
    flight.setName(result.getString(FT::ColumnNumber::NAME));
    flight.setFrom(result.getString(FT::ColumnNumber::FROM));
    flight.setDestination(result.getString(FT::ColumnNumber::DESTINATION));
    flight.setDepartureTime(result.getDateTime(FT::ColumnNumber::DEPARTURE_TIME));
    flight.setLeaveTime(result.getDateTime(FT::ColumnNumber::LEAVE_TIME));
    flight.setArrivalTime(result.getDateTime(FT::ColumnNumber::ARRIVAL_TIME));
    flight.setAmount(result.getDouble(FT::ColumnNumber::AMOUNT));
    flight.setAvailability(result.getString(FT::ColumnNumber::AVAILABILITY)[0]);
    
    return flight;
}

FlightRepository::FlightRepository(std::shared_ptr<IDatabaseConnection> dbConnection)
    : _dbConnection(std::move(dbConnection)), _logger(Logger::getInstance()) {
    _logger->debug("Creating Flight repository object.");

    if (!_dbConnection) {
        _logger->error("Database connection is null in FlightRepository constructor");
        throw std::invalid_argument("Database connection cannot be null");
    }
    
    _logger->info("FlightRepository initialized successfully");
}

std::vector<Flight> FlightRepository::findAll() {
    std::vector<Flight> flights;
    
    try {
        _logger->debug("Finding all flights");

        auto result = _dbConnection->executeQuery(FT::getOrderedSelectClause());
        
        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return flights;
        }

        while (result->next()) {
            flights.push_back(_mapResultToFlight(*result));
        }

        _logger->info("Found " + std::to_string(flights.size()) + " flights");
    } catch (const std::exception& e) {
        _logger->error("Exception in findAll(): " + std::string(e.what()));
    }
    
    return flights;
}

std::optional<Flight> FlightRepository::findById(const int& id) {
    try {
        _logger->debug("Finding flight by ID: " + std::to_string(id));

        std::string query = FT::getOrderedSelectClause() + 
                           " WHERE " + FT::ColumnName[FT::ID] + 
                           " = " + std::to_string(id);
        
        auto result = _dbConnection->executeQuery(query);

        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return std::nullopt;
        }
        
        if (result->next()) {
            _logger->info("Found flight with ID: " + std::to_string(id));
            return _mapResultToFlight(*result);
        }

        _logger->info("Flight with ID " + std::to_string(id) + " not found");
    } catch (const std::exception& e) {
        _logger->error("Exception in findById(): " + std::string(e.what()));
    }
    
    return std::nullopt;
}

bool FlightRepository::save(Flight& flight) {
    try {
        _logger->debug("Saving flight: " + flight.getNo());

        std::ostringstream ss;
        ss << "INSERT INTO " << FT::NAME_TABLE << " ("
           << FT::ColumnName[FT::NO] << ", "
           << FT::ColumnName[FT::NAME] << ", "
           << FT::ColumnName[FT::FROM] << ", "
           << FT::ColumnName[FT::DESTINATION] << ", "
           << FT::ColumnName[FT::DEPARTURE_TIME] << ", "
           << FT::ColumnName[FT::LEAVE_TIME] << ", "
           << FT::ColumnName[FT::ARRIVAL_TIME] << ", "
           << FT::ColumnName[FT::AMOUNT] << ", "
           << FT::ColumnName[FT::AVAILABILITY]
           << ") VALUES ('"
           << flight.getNo() << "', '"
           << flight.getName() << "', '"
           << flight.getFrom() << "', '"
           << flight.getDestination() << "', '"
           << convertTimeToString(flight.getDepartureTime()) << "', '"
           << convertTimeToString(flight.getLeaveTime()) << "', '"
           << convertTimeToString(flight.getArrivalTime()) << "', "
           << flight.getAmount() << ", '"
           << flight.getAvailability() << "')";
        
        if (_dbConnection->execute(ss.str())) {
            flight.setId(_dbConnection->getLastInsertId());
            _logger->info("Flight saved successfully with ID: " + std::to_string(flight.getId()));
            return true;
        }
        
        _logger->error("Failed to save flight: " + _dbConnection->getLastError());
    } catch (const std::exception& e) {
        _logger->error("Exception in save(): " + std::string(e.what()));
    }
    
    return false;
}

bool FlightRepository::update(const Flight& flight) {
    try {
        _logger->debug("Updating flight with ID: " + std::to_string(flight.getId()));
        
        std::ostringstream ss;
        ss << "UPDATE " << FT::NAME_TABLE << " SET "
           << FT::ColumnName[FT::NO] << " = '" << flight.getNo() << "', "
           << FT::ColumnName[FT::NAME] << " = '" << flight.getName() << "', "
           << FT::ColumnName[FT::FROM] << " = '" << flight.getFrom() << "', "
           << FT::ColumnName[FT::DESTINATION] << " = '" << flight.getDestination() << "', "
           << FT::ColumnName[FT::DEPARTURE_TIME] << " = '" << convertTimeToString(flight.getDepartureTime()) << "', "
           << FT::ColumnName[FT::LEAVE_TIME] << " = '" << convertTimeToString(flight.getLeaveTime()) << "', "
           << FT::ColumnName[FT::ARRIVAL_TIME] << " = '" << convertTimeToString(flight.getArrivalTime()) << "', "
           << FT::ColumnName[FT::AMOUNT] << " = " << flight.getAmount() << ", "
           << FT::ColumnName[FT::AVAILABILITY] << " = '" << flight.getAvailability() << "'"
           << " WHERE " << FT::ColumnName[FT::ID] << " = " << flight.getId();
        
        bool success = _dbConnection->execute(ss.str());
        
        if (success) {
            _logger->info("Flight updated successfully with ID: " + std::to_string(flight.getId()));
        } else {
            _logger->error("Failed to update flight: " + _dbConnection->getLastError());
        }
        
        return success;
    } catch (const std::exception& e) {
        _logger->error("Exception in update(): " + std::string(e.what()));
    }
    
    return false;
}

bool FlightRepository::remove(const int& id) {
    try {
        _logger->debug("Removing flight with ID: " + std::to_string(id));
        
        std::string query = "DELETE FROM " + std::string(FT::NAME_TABLE) + 
                           " WHERE " + FT::ColumnName[FT::ID] + " = " + std::to_string(id);
        
        bool success = _dbConnection->execute(query);
        
        if (success) {
            _logger->info("Flight removed successfully with ID: " + std::to_string(id));
        } else {
            _logger->error("Failed to remove flight: " + _dbConnection->getLastError());
        }
        
        return success;
    } catch (const std::exception& e) {
        _logger->error("Exception in remove(): " + std::string(e.what()));
    }
    
    return false;
}

std::vector<Flight> FlightRepository::findAvailableFlights() {
    std::vector<Flight> flights;
    
    try {
        _logger->debug("Finding available flights");
        
        std::string query = FT::getOrderedSelectClause() + 
                           " WHERE " + FT::ColumnName[FT::AVAILABILITY] + " = 'A'";
        
        auto result = _dbConnection->executeQuery(query);
        
        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return flights;
        }
        
        while (result->next()) {
            flights.push_back(_mapResultToFlight(*result));
        }
        
        _logger->info("Found " + std::to_string(flights.size()) + " available flights");
    } catch (const std::exception& e) {
        _logger->error("Exception in findAvailableFlights(): " + std::string(e.what()));
    }
    
    return flights;
}

std::optional<Flight> FlightRepository::findByFlightNo(const std::string& flightNo) {
    try {
        _logger->debug("Finding flight by number: " + flightNo);
        
        std::string query = FT::getOrderedSelectClause() + 
                           " WHERE " + FT::ColumnName[FT::NO] + " = '" + flightNo + "'";
        
        auto result = _dbConnection->executeQuery(query);
        
        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return std::nullopt;
        }
        
        if (result->next()) {
            _logger->info("Found flight with number: " + flightNo);
            return _mapResultToFlight(*result);
        }
        
        _logger->info("Flight with number " + flightNo + " not found");
    } catch (const std::exception& e) {
        _logger->error("Exception in findByFlightNo(): " + std::string(e.what()));
    }
    
    return std::nullopt;
}