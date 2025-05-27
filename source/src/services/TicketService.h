#ifndef TICKET_SERVICE_H
#define TICKET_SERVICE_H

#include "repositories/MockRepository/TicketMockRepository.h"
#include "core/entities/Ticket.h"
#include "core/value_objects/ticket_number/TicketNumber.h"
#include "core/value_objects/seat_number/SeatNumber.h"
#include "core/value_objects/price/Price.h"
#include "core/value_objects/ticket_status/TicketStatus.h"
#include "utils/Logger.h"
#include "InterfaceService.h"
#include <memory>
#include <string>
#include <vector>

class TicketService : public IService<Ticket>
{
private:
    std::shared_ptr<TicketMockRepository> _ticketRepository;
    std::shared_ptr<Logger> _logger;

public:
    explicit TicketService(std::shared_ptr<TicketMockRepository> ticketRepository,
                           std::shared_ptr<Logger> logger = nullptr)
        : _ticketRepository(std::move(ticketRepository)), _logger(std::move(logger)) {}

    // IService interface implementation
    Result<Ticket> getById(const int &id) override;
    Result<std::vector<Ticket>> getAll() override;
    Result<bool> exists(const int &id) override;
    Result<size_t> count() override;
    Result<Ticket> create(const Ticket &entity) override;
    Result<Ticket> update(const Ticket &entity) override;
    Result<bool> deleteById(const int &id) override;

    // Ticket-specific methods
    Result<Ticket> findByTicketNumber(const TicketNumber &ticketNumber);
    Result<bool> existsTicket(const TicketNumber &ticketNumber);
    Result<std::vector<Ticket>> findByPassengerId(int passengerId);
    Result<std::vector<Ticket>> findBySerialNumber(const AircraftSerial &serial);
};

#endif