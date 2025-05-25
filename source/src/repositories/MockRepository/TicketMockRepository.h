#ifndef TICKET_MOCK_REPOSITORY_H
#define TICKET_MOCK_REPOSITORY_H

#include <vector>
#include <unordered_map>
#include "core/entities/Ticket.h"
#include "repositories/InterfaceRepository.h"
#include "core/exceptions/Result.h"
#include "utils/Logger.h"
#include <memory>
#include <iostream>

class TicketMockRepository : public IRepository<Ticket>
{
private:
    std::unordered_map<int, std::shared_ptr<Ticket>> _tickets;
    int _nextId = 1;
    std::shared_ptr<Logger> _logger;

public:
    TicketMockRepository(std::shared_ptr<Logger> logger) : _logger(logger) {}

    Result<Ticket> findById(const int &id) override;
    Result<std::vector<Ticket>> findAll() override;
    Result<bool> exists(const int &id) override;
    Result<size_t> count() override;
    Result<Ticket> create(const Ticket &ticket) override;
    Result<Ticket> update(const Ticket &ticket) override;
    Result<bool> deleteById(const int &id) override;
    void clear();
    const std::unordered_map<int, std::shared_ptr<Ticket>> &getTickets() const;

    Result<Ticket> findByTicketNumber(const TicketNumber &ticketNumber);
    Result<bool> existsTicket(const TicketNumber &ticketNumber);
    Result<std::vector<Ticket>> findByPassengerId(int passengerId);
    Result<std::vector<Ticket>> findByFlightId(int flightId);
};

#endif // TICKET_MOCK_REPOSITORY_H