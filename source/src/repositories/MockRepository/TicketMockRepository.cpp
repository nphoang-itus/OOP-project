#include <vector>
#include <unordered_map>
#include "TicketMockRepository.h"
#include "core/exceptions/Result.h"
#include "utils/Logger.h"

Result<Ticket> TicketMockRepository::create(const Ticket &ticket)
{
    // Tạo ticket mới với ID mặc định (ví dụ 0)
    auto newTicketResult = Ticket::create(
        ticket.getTicketNumber(),
        ticket.getPassenger(),
        ticket.getFlight(),
        ticket.getSeatNumber(),
        ticket.getPrice());

    if (!newTicketResult)
    {
        return std::unexpected(newTicketResult.error());
    }

    auto ticketPtr = std::make_shared<Ticket>(newTicketResult.value());
    ticketPtr->setId(_nextId++); // Set ID mới

    _tickets[ticketPtr->getId()] = ticketPtr;

    return *ticketPtr;
}

Result<Ticket> TicketMockRepository::findById(const int &id)
{
    auto it = _tickets.find(id);
    if (it != _tickets.end())
    {
        return *it->second;
    }
    return std::unexpected(CoreError("Ticket not found with id: " + std::to_string(id), "NOT_FOUND"));
}

Result<std::vector<Ticket>> TicketMockRepository::findAll()
{
    std::vector<Ticket> result;
    result.reserve(_tickets.size());
    for (const auto &[id, ticket] : _tickets)
    {
        result.push_back(*ticket);
    }
    return result;
}

Result<Ticket> TicketMockRepository::update(const Ticket &ticket)
{
    auto it = _tickets.find(ticket.getId());
    if (it != _tickets.end())
    {
        *it->second = ticket;
        return *it->second;
    }
    return std::unexpected(CoreError("Ticket not found with id: " + std::to_string(ticket.getId()), "NOT_FOUND"));
}

Result<bool> TicketMockRepository::deleteById(const int &id)
{
    auto it = _tickets.find(id);
    if (it != _tickets.end())
    {
        _tickets.erase(it);
        return true;
    }
    return false;
}

Result<bool> TicketMockRepository::exists(const int &id)
{
    return _tickets.find(id) != _tickets.end();
}

Result<size_t> TicketMockRepository::count()
{
    return _tickets.size();
}

Result<Ticket> TicketMockRepository::findByTicketNumber(const TicketNumber &ticketNumber)
{
    for (const auto &[id, ticket] : _tickets)
    {
        if (ticket->getTicketNumber() == ticketNumber)
        {
            return *ticket;
        }
    }
    return std::unexpected(CoreError("Ticket not found with number: " + ticketNumber.toString(), "NOT_FOUND"));
}

Result<bool> TicketMockRepository::existsTicket(const TicketNumber &ticketNumber)
{
    for (const auto &[id, ticket] : _tickets)
    {
        if (ticket->getTicketNumber() == ticketNumber)
        {
            return true;
        }
    }
    return false;
}

Result<std::vector<Ticket>> TicketMockRepository::findByPassengerId(int passengerId)
{
    std::vector<Ticket> result;
    for (const auto &[id, ticket] : _tickets)
    {
        if (ticket->getPassenger()->getId() == passengerId)
        {
            result.push_back(*ticket);
        }
    }
    return result;
}

Result<std::vector<Ticket>> TicketMockRepository::findByFlightId(int flightId)
{
    std::vector<Ticket> result;
    for (const auto &[id, ticket] : _tickets)
    {
        if (ticket->getFlight()->getId() == flightId)
        {
            result.push_back(*ticket);
        }
    }
    return result;
}

void TicketMockRepository::clear()
{
    _tickets.clear();
    _nextId = 1;
}

const std::unordered_map<int, std::shared_ptr<Ticket>> &TicketMockRepository::getTickets() const
{
    return _tickets;
}