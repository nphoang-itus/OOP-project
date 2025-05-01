/**
 * @file FlightRepository.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "FlightRepository.h"
#include "../utils/TableConstants.h"
#include <sstream>

namespace FT = Tables::Flight;

FlightRepository::FlightRepository(std::shared_ptr<IDatabaseConnection> dbConnection) : _dbConnection(std::move(dbConnection)) {}

Flight FlightRepository::_mapResultToFlight(IDatabaseResult& result) {
    Flight flight;
    
    flight.setId(result.getInt(FT::ColumnNumber::ID));
    flight.setNo(result.getString(FT::ColumnNumber::NO));
    flight.setName(result.getString(FT::ColumnNumber::NAME));
    flight.setFrom(result.getString(FT::ColumnNumber::FROM));
    flight.setDestination(result.getString(FT::ColumnNumber::DESTINATION));
    flight.setDepartureTime(result.getString(FT::ColumnNumber::DEPARTURE_TIME));
    flight.setLeaveTime(result.getString(FT::ColumnNumber::LEAVE_TIME));
    flight.setArrivalTime(result.getString(FT::ColumnNumber::ARRIVAL_TIME));
    flight.setAmount(result.getDouble(FT::ColumnNumber::AMOUNT));
    flight.setAvailability(result.getInt(FT::ColumnNumber::AVAILABILITY) != 0);
    
    return flight;
}

std::vector<Flight> FlightRepository::findAll() {
    std::vector<Flight> flights;
    
    try {
        auto result = _dbConnection->executeQuery(FT::getOrderedSelectClause());
        
        while (result->next()) {
            flights.push_back(_mapResultToFlight(*result));
        }
    } catch (const std::exception& e) {
        // TO-DO: Làm sau khi xây dựng lớp logger
    }
    
    return flights;
}

std::optional<Flight> FlightRepository::findById(int id) {
    try {
        std::string query = FT::getOrderedSelectClause() + 
                           " WHERE " + FT::ColumnName[FT::ID] + 
                           " = " + std::to_string(id);
        
        auto result = _dbConnection->executeQuery(query);
        
        if (result->next()) {
            return _mapResultToFlight(*result);
        }
    } catch (const std::exception& e) {
        // Xử lý lỗi
    }
    
    return std::nullopt;
}

bool FlightRepository::save(Flight& flight) {
    try {
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
           << flight.getDepartureTime() << "', '"
           << flight.getLeaveTime() << "', '"
           << flight.getArrivalTime() << "', "
           << flight.getAmount() << ", '"
           << flight.getAvailability() << "')";
        
        if (_dbConnection->execute(ss.str())) {
            flight.setId(_dbConnection->getLastInsertId());
            return true;
        }
    } catch (const std::exception& e) {
        // Xử lý lỗi
    }
    
    return false;
}

bool FlightRepository::update(const Flight& flight) {
    try {
        std::ostringstream ss;
        ss << "UPDATE " << FT::NAME_TABLE << " SET "
           << FT::ColumnName[FT::NO] << " = '" << flight.getNo() << "', "
           << FT::ColumnName[FT::NAME] << " = '" << flight.getName() << "', "
           << FT::ColumnName[FT::FROM] << " = '" << flight.getFrom() << "', "
           << FT::ColumnName[FT::DESTINATION] << " = '" << flight.getDestination() << "', "
           << FT::ColumnName[FT::DEPARTURE_TIME] << " = '" << flight.getDepartureTime() << "', "
           << FT::ColumnName[FT::LEAVE_TIME] << " = '" << flight.getLeaveTime() << "', "
           << FT::ColumnName[FT::ARRIVAL_TIME] << " = '" << flight.getArrivalTime() << "', "
           << FT::ColumnName[FT::AMOUNT] << " = " << flight.getAmount() << ", "
           << FT::ColumnName[FT::AVAILABILITY] << " = '" << flight.getAvailability() << "'"
           << " WHERE " << FT::ColumnName[FT::ID] << " = " << flight.getId();
        
        return _dbConnection->execute(ss.str());
    } catch (const std::exception& e) {
        // Xử lý lỗi
    }
    
    return false;
}

bool FlightRepository::remove(int id) {
    try {
        std::string query = "DELETE FROM " + std::string(FT::NAME_TABLE) + 
                           " WHERE " + FT::ColumnName[FT::ID] + " = " + std::to_string(id);
        
        return _dbConnection->execute(query);
    } catch (const std::exception& e) {
        // Xử lý lỗi
    }
    
    return false;
}

std::vector<Flight> FlightRepository::findAvailableFlights() {
    std::vector<Flight> flights;
    
    try {
        std::string query = FT::getOrderedSelectClause() + 
                           " WHERE " + FT::ColumnName[FT::AVAILABILITY] + " = 'A'";
        
        auto result = _dbConnection->executeQuery(query);
        
        while (result->next()) {
            flights.push_back(_mapResultToFlight(*result));
        }
    } catch (const std::exception& e) {
        // Xử lý lỗi
    }
    
    return flights;
}

std::optional<Flight> FlightRepository::findByFlightNo(const std::string& flightNo) {
    try {std::string query = FT::getOrderedSelectClause() + 
                           " WHERE " + FT::ColumnName[FT::NO] + " = '" + flightNo + "'";
        
        auto result = _dbConnection->executeQuery(query);
        
        if (result->next()) {
            return _mapResultToFlight(*result);
        }
    } catch (const std::exception& e) {
        // Xử lý lỗi
    }
    
    return std::nullopt;
}