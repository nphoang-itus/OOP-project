/**
 * @file ServiceFactory.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef SERVICE_FACTORY_H
#define SERVICE_FACTORY_H

#include "../utils/Logger.h"
#include "../repositories/FlightRepository.h"
#include "../repositories/PassengerRepository.h"
#include "../repositories/ReservationRepository.h"
#include "FlightService.h"
#include "PassengerService.h"
#include "ReservationService.h"


#include <memory>

class ServiceFactory {
private:
    std::shared_ptr<Logger> _logger; ///< Logger để ghi log
    
    // Các repository
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<PassengerRepository> _passengerRepository;
    std::shared_ptr<ReservationRepository> _reservationRepository;
    
    // Các service
    std::shared_ptr<FlightService> _flightService;
    std::shared_ptr<PassengerService> _passengerService;
    std::shared_ptr<ReservationService> _reservationService;

public:
    ServiceFactory(
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<PassengerRepository> passengerRepository,
        std::shared_ptr<ReservationRepository> reservationRepository
    );

    ~ServiceFactory();

    std::shared_ptr<FlightService> getFlightService();
    std::shared_ptr<PassengerService> getPassengerService();
    std::shared_ptr<ReservationService> getReservationService();
};

#endif