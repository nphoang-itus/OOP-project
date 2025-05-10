/**
 * @file ResevationRepositoty.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef RESERVATION_REPOSITORY_H
#define RESERVATION_REPOSITORY_H

#include "RepositoryInterface.h"
#include "../core/Reservation.h"
#include "../database/DatabaseConnection.h"
#include "../repositories/FlightRepository.h"
#include "../repositories/PassengerRepository.h"
#include "../utils/Logger.h"

#include <string>

class ReservationRepository : public IRepository<Reservation> {
private:
    std::shared_ptr<IDatabaseConnection> _dbConnection;
    std::shared_ptr<Logger> _logger;

    Reservation _mapResultToReservation(IDatabaseResult& result, FlightRepository& flightRepo, PassengerRepository& passengerRepo);

public:
    explicit ReservationRepository(std::shared_ptr<IDatabaseConnection> dbConnection);

    std::vector<Reservation> findAll() override;
    std::optional<Reservation> findById(const int& id) override;
    bool save(Reservation& reservation) override;
    bool update(const Reservation& reservation) override;
    bool remove(const int& id) override;

    std::optional<Reservation> findByTicketNo(const std::string& ticketNo);
};

#endif