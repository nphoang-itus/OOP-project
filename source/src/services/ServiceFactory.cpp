/**
 * @file ServiceFactory.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Triển khai lớp ServiceFactory để tạo và quản lý các services.
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ServiceFactory.h"
#include <stdexcept>

ServiceFactory::ServiceFactory(
    std::shared_ptr<FlightRepository> flightRepository,
    std::shared_ptr<PassengerRepository> passengerRepository,
    std::shared_ptr<ReservationRepository> reservationRepository
) : _flightRepository(std::move(flightRepository)),
    _passengerRepository(std::move(passengerRepository)),
    _reservationRepository(std::move(reservationRepository)),
    _logger(Logger::getInstance()) {
    
    _logger->debug("Initializing ServiceFactory with injected repositories");
    
    // Kiểm tra các repositories đã được cung cấp
    if (!_flightRepository) {
        _logger->error("FlightRepository is null in ServiceFactory constructor");
        throw std::invalid_argument("FlightRepository cannot be null");
    }
    
    if (!_passengerRepository) {
        _logger->error("PassengerRepository is null in ServiceFactory constructor");
        throw std::invalid_argument("PassengerRepository cannot be null");
    }
    
    if (!_reservationRepository) {
        _logger->error("ReservationRepository is null in ServiceFactory constructor");
        throw std::invalid_argument("ReservationRepository cannot be null");
    }
}

ServiceFactory::~ServiceFactory() {
    try {
        _logger->debug("Destroying ServiceFactory");
        
        // Hủy các tham chiếu đến services
        _reservationService.reset();
        _flightService.reset();
        _passengerService.reset();
    } catch (const std::exception& e) {
        _logger->error("Error in ServiceFactory destructor: " + std::string(e.what()));
    }
}

std::shared_ptr<FlightService> ServiceFactory::getFlightService() {
    try {
        _logger->debug("Getting FlightService instance");
        
        // Tạo FlightService nếu chưa có
        if (!_flightService) {
            _flightService = std::make_shared<FlightService>(_flightRepository);
            _logger->debug("Created new FlightService instance");
        }
        
        return _flightService;
    } catch (const std::exception& e) {
        _logger->error("Exception in getFlightService: " + std::string(e.what()));
        throw;
    }
}

std::shared_ptr<PassengerService> ServiceFactory::getPassengerService() {
    try {
        _logger->debug("Getting PassengerService instance");
        
        // Tạo PassengerService nếu chưa có
        if (!_passengerService) {
            _passengerService = std::make_shared<PassengerService>(_passengerRepository);
            _logger->debug("Created new PassengerService instance");
        }
        
        return _passengerService;
    } catch (const std::exception& e) {
        _logger->error("Exception in getPassengerService: " + std::string(e.what()));
        throw;
    }
}

std::shared_ptr<ReservationService> ServiceFactory::getReservationService() {
    try {
        _logger->debug("Getting ReservationService instance");
        
        // Tạo FlightService và PassengerService nếu cần thiết
        if (!_flightService) {
            _flightService = getFlightService();
        }
        
        if (!_passengerService) {
            _passengerService = getPassengerService();
        }
        
        // Tạo ReservationService nếu chưa có
        if (!_reservationService) {
            _reservationService = std::make_shared<ReservationService>(
                _reservationRepository, _flightService, _passengerService);
            _logger->debug("Created new ReservationService instance");
        }
        
        return _reservationService;
    } catch (const std::exception& e) {
        _logger->error("Exception in getReservationService: " + std::string(e.what()));
        throw;
    }
}