/**
 * @file FlightService.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Triển khai lớp FlightService quản lý các chức năng nghiệp vụ liên quan đến chuyến bay.
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "FlightService.h"
#include "../utils/utils.h"
#include <algorithm>
#include <stdexcept>

FlightService::FlightService(std::shared_ptr<FlightRepository> flightRepository)
    : _flightRepository(std::move(flightRepository)), _logger(Logger::getInstance()) {
    
    _logger->debug("Creating FlightService instance");
    
    if (!_flightRepository) {
        _logger->error("FlightRepository is null in FlightService constructor");
        throw std::invalid_argument("FlightRepository cannot be null");
    }
}

std::vector<Flight> FlightService::findAll() {
    _logger->debug("FlightService::findAll - Retrieving all flights");
    return _flightRepository->findAll();
}

std::optional<Flight> FlightService::findById(const int& id) {
    _logger->debug("FlightService::findById - Finding flight with ID: " + std::to_string(id));
    return _flightRepository->findById(id);
}

bool FlightService::save(Flight& flight) {
    _logger->debug("FlightService::save - Saving flight: " + flight.getNo());
    
    // // Validate flight data before saving
    // if (flight.getNo().empty() || flight.getName().empty() || 
    //     flight.getFrom().empty() || flight.getDestination().empty() || 
    //     flight.getAmount() < 0) {
        
    //     _logger->error("Invalid flight data provided");
    //     return false;
    // }
    
    return _flightRepository->save(flight);
}

bool FlightService::remove(const int& id) {
    _logger->debug("FlightService::remove - Removing flight with ID: " + std::to_string(id));
    return _flightRepository->remove(id);
}

std::vector<Flight> FlightService::findAvailableFlights() {
    _logger->debug("FlightService::findAvailableFlights - Retrieving available flights");
    return _flightRepository->findAvailableFlights();
}

std::optional<Flight> FlightService::findByFlightNo(const std::string& flightNo) {
    _logger->debug("FlightService::findByFlightNo - Finding flight with number: " + flightNo);
    return _flightRepository->findByFlightNo(flightNo);
}

bool FlightService::update(const Flight& flight) {
    _logger->debug("FlightService::update - Updating flight with ID: " + std::to_string(flight.getId()));
    
    // // Validate flight data before updating
    // if (flight.getNo().empty() || flight.getName().empty() || 
    //     flight.getFrom().empty() || flight.getDestination().empty() || 
    //     flight.getAmount() < 0) {
        
    //     _logger->error("Invalid flight data provided for update");
    //     return false;
    // }
    
    return _flightRepository->update(flight);
}

std::vector<Flight> FlightService::findByRoute(const std::string& from, const std::string& destination) {
    _logger->debug("FlightService::findByRoute - Finding flights from " + from + " to " + destination);
    
    std::vector<Flight> allFlights = _flightRepository->findAll();
    std::vector<Flight> matchingFlights;
    
    for (const auto& flight : allFlights) {
        if (flight.getFrom() == from && flight.getDestination() == destination) {
            matchingFlights.push_back(flight);
        }
    }
    
    _logger->info("Found " + std::to_string(matchingFlights.size()) + " flights matching route " + from + " to " + destination);
    return matchingFlights;
}

std::vector<Flight> FlightService::findByDateRange(const std::tm& startDate, const std::tm& endDate) {
    _logger->debug("FlightService::findByDateRange - Finding flights in date range");
    
    std::vector<Flight> allFlights = _flightRepository->findAll();
    std::vector<Flight> matchingFlights;
    
    // Convert tm objects to time_t for easier comparison
    std::time_t startTime = std::mktime(const_cast<std::tm*>(&startDate));
    std::time_t endTime = std::mktime(const_cast<std::tm*>(&endDate));
    
    if (startTime == -1 || endTime == -1) {
        _logger->error("Invalid date range provided");
        return matchingFlights;
    }
    
    for (const auto& flight : allFlights) {
        std::tm departureTime = flight.getDepartureTime();
        std::time_t flightTime = std::mktime(&departureTime);
        
        if (flightTime != -1 && flightTime >= startTime && flightTime <= endTime) {
            matchingFlights.push_back(flight);
        }
    }
    
    _logger->info("Found " + std::to_string(matchingFlights.size()) + " flights within the specified date range");
    return matchingFlights;
}

bool FlightService::isFlightAvailable(const std::string& flightNo) {
    _logger->debug("FlightService::isFlightAvailable - Checking availability for flight: " + flightNo);
    
    auto flight = _flightRepository->findByFlightNo(flightNo);
    if (!flight) {
        _logger->warning("Flight not found: " + flightNo);
        return false;
    }
    
    bool isAvailable = (flight->getAvailability() == 'A');
    _logger->debug("Flight " + flightNo + " availability: " + (isAvailable ? "Available" : "Not Available"));
    
    return isAvailable;
}

bool FlightService::updateAvailability(const std::string& flightNo, char availability) {
    _logger->debug("FlightService::updateAvailability - Updating availability for flight: " + flightNo);
    
    if (availability != 'A' && availability != 'N') {
        _logger->error("Invalid availability value: " + std::string(1, availability));
        return false;
    }
    
    auto flight = _flightRepository->findByFlightNo(flightNo);
    if (!flight) {
        _logger->warning("Flight not found: " + flightNo);
        return false;
    }
    
    Flight updatedFlight = *flight;
    updatedFlight.setAvailability(availability);
    
    return _flightRepository->update(updatedFlight);
}