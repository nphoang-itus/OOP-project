#include "ReservationRepository.h"
#include "FlightRepository.h"
#include "PassengerRepository.h"
#include "../core/Flight.h"
#include "../core/Passenger.h"
#include "../core/Reservation.h"

#include "../utils/TableConstants.h"

#include <stdexcept>
#include <string>
#include <format>
#include <vector>
#include <memory>

namespace RT = Tables::Reservation;
namespace FT = Tables::Flight;
namespace PT = Tables::Passenger;

Reservation ReservationRepository::_mapResultToReservation(IDatabaseResult& result, FlightRepository& flightRepo, PassengerRepository& passengerRepo) {
    int reservationId = result.getInt(RT::ID);
    std::string ticketNo = result.getString(RT::TICKET_NO);
    std::string passportNo = result.getString(RT::PASSENGER);
    std::string flightNo = result.getString(RT::FLIGHT);

    auto passenger = passengerRepo.findByPassport(passportNo);
    auto flight = flightRepo.findByFlightNo(flightNo);

    if (!(passenger && flight)) {
        throw std::invalid_argument("Passenger or Flight not found.");
    }

    auto passengerPtr = std::make_shared<Passenger> (*passenger);
    auto flightPtr = std::make_shared<Flight> (*flight);

    Reservation reservation(ticketNo, flightPtr, passengerPtr);
    reservation.setId(reservationId);

    return reservation;
}

explicit ReservationRepository::ReservationRepository(std::shared_ptr<IDatabaseConnection> dbConnection) : _dbConnection(std::move(dbConnection)){
    _logger->debug("Create Reservation Repository Object.");

    if (!_dbConnection) {
        _logger->error("Database connection is null in ReservationRepository constructor");
        throw std::invalid_argument("Database connection cannot be null");
    }
    
    _logger->info("FlightRepository initialized successfully");
}

std::vector<Reservation> ReservationRepository::findAll() {
    std::vector<Reservation> reservations;

    try {
        _logger->debug("Finding all reservation");
        auto result = _dbConnection->executeQuery(RT::getOrderedSelectClause());

        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return reservations;
        }

        FlightRepository flightRepo(_dbConnection);
        PassengerRepository passengerRepo(_dbConnection);

        while (result->next()) {
            reservations.push_back(_mapResultToReservation(*result, flightRepo, passengerRepo));
        }

        _logger->info("Found " + std::to_string(reservations.size()) + " reservations");
    } catch (const std::exception& e) {
        _logger->error("Exception in findAll(): " + std::string(e.what()));
    }

    return reservations;
}


std::optional<Reservation> ReservationRepository::findById(const int& id) {
    try {
        _logger->debug("Finding reservation by ID: " + std::to_string(id));

        std::string query = RT::getOrderedSelectClause() + std::format (
                                "WHERE {} = {}",
                                FT::ColumnName[FT::ID], id
                            );

        auto result = _dbConnection->executeQuery(query);

        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return std::nullopt;
        }

        if (result->next()) {
            FlightRepository flightRepo(_dbConnection);
            PassengerRepository passengerRepo(_dbConnection);

            _logger->info("Found reservation with ID: " + std::to_string(id));
            return _mapResultToReservation(*result, flightRepo, passengerRepo);
        }

        _logger->info("Reservation with ID " + std::to_string(id) + " not found");
    } catch (const std::exception& e) {
        _logger->error("Exception in findById(): " + std::string(e.what()));
    }

    return std::nullopt;
}

bool ReservationRepository::save(Reservation& reservation) {
    try {
        _logger->debug("Saving reservation: " + reservation.getTicketNo());
        
        // Begin transaction
        _dbConnection->beginTransaction();

        // Lưu khách hàng nếu chưa tồn tại
        auto passenger = reservation.getPassenger();
        PassengerRepository passengerRepo(_dbConnection);

        auto existingPassenger = passengerRepo.findByPassport(passenger->getPassport());
        if (!existingPassenger) {
            // Chưa có khách hàng -> Lưu
            if (!passengerRepo.save(*passenger)) {
                _dbConnection->rollbackTransaction();
                return false;
            }
        } else {
            // Nếu tồn tại khách hàng
            passenger->setId(existingPassenger->getId());
        }

        // Lưu đặt chỗ
        std::string query = std::format(
            "INSERT INTO {} ({}, {}, {})"
            "VALUES (?, ?, ?)",
            RT::NAME_TABLE,
            RT::ColumnName[RT::TICKET_NO],
            RT::ColumnName[RT::FLIGHT],
            RT::ColumnName[RT::PASSENGER]
        );

        int stmtId = _dbConnection->prepareStatement(query);
        
        _dbConnection->setString(stmtId, 1, reservation.getTicketNo());
        _dbConnection->setString(stmtId, 2, reservation.getFlight()->getNo());
        _dbConnection->setString(stmtId, 3, reservation.getPassenger()->getPassport());
        
        bool result = _dbConnection->executeStatement(stmtId);
        
        if (result) {
            reservation.setId(_dbConnection->getLastInsertId());
            _dbConnection->commitTransaction();
        } else {
            _dbConnection->rollbackTransaction();
        }
        
        _dbConnection->freeStatement(stmtId);
        return result;
    } catch (const std::exception& e) {

    }
}

bool ReservationRepository::update(const Reservation& reservation) {
    try {
        // Bắt đầu transaction
        _dbConnection->beginTransaction();
        
        // Cập nhật hành khách
        auto passenger = reservation.getPassenger();
        PassengerRepository passengerRepo(_dbConnection);
        
        if (!passengerRepo.update(*passenger)) {
            _dbConnection->rollbackTransaction();
            return false;
        }
        
        std::string query = std::format (
            "UPDATE {}"
            "SET {} = ?, {} = ?, {} = ?"
            "WHERE {} = ?",
            RT::NAME_TABLE,
            RT::ColumnName[RT::TICKET_NO], RT::ColumnName[RT::FLIGHT], RT::ColumnName[RT::PASSENGER],
            RT::ColumnName[RT::ID]
        );

        // Cập nhật đặt chỗ
        int stmtId = _dbConnection->prepareStatement(query);
        
        _dbConnection->setString(stmtId, 1, reservation.getTicketNo());
        _dbConnection->setString(stmtId, 2, reservation.getFlight()->getNo());
        _dbConnection->setString(stmtId, 3, reservation.getPassenger()->getPassport());
        _dbConnection->setInt(stmtId, 4, reservation.getId());

        bool result = _dbConnection->executeStatement(stmtId);
        
        if (result) {
            _dbConnection->commitTransaction();
        } else {
            _dbConnection->rollbackTransaction();
        }
        
        _dbConnection->freeStatement(stmtId);
        return result;
    } catch (const std::exception& e) {

    }
    
    return false;
}

bool ReservationRepository::remove(const int& id) {
    try {
        std::string query = std::format(
            "DELETE FROM {} WHERE {} = {}",
            RT::NAME_TABLE, RT::ColumnName[RT::ID], id
        );

        bool success = _dbConnection->execute(query);
        
        return success;
    } catch (const std::exception& e) {
        _logger->error("Exception in remove(): " + std::string(e.what()));
    }
    
    return false;
}

std::optional<Reservation> ReservationRepository::findByTicketNo(const std::string& ticketNo) {
    try {
        _logger->debug("Finding reservation by ticket number: " + ticketNo);
        
        std::string query = std::format(
            "SELECT * FROM {} WHERE {} = '{}'",
            RT::NAME_TABLE, RT::ColumnName[RT::TICKET_NO], ticketNo
        );
        
        auto result = _dbConnection->executeQuery(query);
        
        if (!result) {
            _logger->error("Failed to execute query: " + _dbConnection->getLastError());
            return std::nullopt;
        }
        
        if (result->next()) {
            _logger->info("Found reservation with ticket number: " + ticketNo);
            FlightRepository flightRepo(_dbConnection);
            PassengerRepository passengerRepo(_dbConnection);

            return _mapResultToReservation(*result, flightRepo, passengerRepo);
        }
        
        _logger->info("Reservation with number " + ticketNo + " not found");
    } catch (const std::exception& e) {
        _logger->error("Exception in findByTicketNo(): " + std::string(e.what()));
    }
    
    return std::nullopt;
}