/**
 * @file ReservationService.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef RESERVATION_SERVICE_H
#define RESERVATION_SERVICE_H

#include "ServiceInterface.h"
#include "../core/Reservation.h"
#include "../repositories/ReservationRepository.h"
#include "FlightService.h"
#include "PassengerService.h"

#include <optional>
#include <string>
#include <vector>

class IReservationService : public IService<Reservation> {
public:
    virtual std::optional<Reservation> findByTicketNo(const std::string& ticketNo) = 0;
    virtual std::vector<Reservation> findByPassenger(const std::string& passportNo) = 0;
    virtual std::vector<Reservation> findByFlight(const std::string& flightNo) = 0;
    virtual bool update(const Reservation& reservation) = 0;
    virtual std::optional<Reservation> createReservation(const std::string& flightNo, const Passenger& passenger) = 0;
    virtual bool cancelReservation(const std::string& ticketNo) = 0;
};

class ReservationService : public IReservationService {
private:
    std::shared_ptr<ReservationRepository> _reservationRepository; ///< Repository để truy xuất dữ liệu đặt vé
    std::shared_ptr<FlightService> _flightService; ///< Service để quản lý chuyến bay
    std::shared_ptr<PassengerService> _passengerService; ///< Service để quản lý hành khách
    std::shared_ptr<Logger> _logger; ///< Logger để ghi log

    std::string generateTicketNumber(const std::string& flightNo, const std::string& passportNo);
    // bool validateReservation(const Reservation& reservation);

public:
    ReservationService(
        std::shared_ptr<ReservationRepository> reservationRepository,
        std::shared_ptr<FlightService> flightService,
        std::shared_ptr<PassengerService> passengerService
    );

    std::vector<Reservation> findAll() override;
    std::optional<Reservation> findById(const int& id) override;
    bool save(Reservation& reservation) override;
    bool remove(const int& id) override;

    std::optional<Reservation> findByTicketNo(const std::string& ticketNo) override;
    std::vector<Reservation> findByPassenger(const std::string& passportNo) override;
    std::vector<Reservation> findByFlight(const std::string& flightNo) override;
    bool update(const Reservation& reservation) override;
    std::optional<Reservation> createReservation(const std::string& flightNo, const Passenger& passenger) override;
    bool cancelReservation(const std::string& ticketNo) override;
};

#endif