#ifndef TICKET_REPOSITORY_H
#define TICKET_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Ticket.h"
#include "../../core/value_objects/ticket_number/TicketNumber.h"
#include "../../core/value_objects/seat_number/SeatNumber.h"
#include "../../core/value_objects/price/Price.h"
#include "../../core/value_objects/ticket_status/TicketStatus.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include "AircraftRepository.h"
#include "FlightRepository.h"
#include "PassengerRepository.h"
#include <memory>
#include <string>
#include <vector>

class TicketRepository : public IRepository<Ticket> {
private:
    std::shared_ptr<IDatabaseConnection> _connection;
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<AircraftRepository> _aircraftRepository;
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<PassengerRepository> _passengerRepository;

public:
    explicit TicketRepository(
        std::shared_ptr<IDatabaseConnection> connection,
        std::shared_ptr<PassengerRepository> passengerRepository,
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<Logger> logger = nullptr
    )
        : _connection(std::move(connection))
        , _passengerRepository(std::move(passengerRepository))
        , _flightRepository(std::move(flightRepository))
        , _logger(std::move(logger))
    {}

    Result<Ticket> findById(const int& id) override;
    Result<std::vector<Ticket>> findAll() override;
    Result<bool> exists(const int& id) override;
    Result<size_t> count() override;
    Result<Ticket> create(const Ticket& ticket) override;
    Result<Ticket> update(const Ticket& ticket) override;
    Result<bool> deleteById(const int& id) override;
    
    Result<Ticket> findByTicketNumber(const TicketNumber& ticketNumber);
    Result<bool> existsTicket(const TicketNumber& ticketNumber);
    Result<std::vector<Ticket>> findByPassengerId(int passengerId);
    Result<std::vector<Ticket>> findByFlightId(int flightId);
    Result<std::vector<Ticket>> findBySerialNumber(const AircraftSerial& serial);
};

#endif