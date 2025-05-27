#include "TicketService.h"

Result<Ticket> TicketService::getById(const int &id)
{
    try
    {
        if (_logger)
        {
            _logger->info("Getting ticket by ID: " + std::to_string(id));
        }
        return _ticketRepository->findById(id);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error getting ticket by ID: " + std::string(e.what()));
        }
        return Failure<Ticket>(CoreError("Failed to get ticket by ID: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<std::vector<Ticket>> TicketService::getAll()
{
    try
    {
        if (_logger)
        {
            _logger->info("Getting all tickets");
        }
        return _ticketRepository->findAll();
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error getting all tickets: " + std::string(e.what()));
        }
        return Failure<std::vector<Ticket>>(CoreError("Failed to get all tickets: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<bool> TicketService::exists(const int &id)
{
    try
    {
        if (_logger)
        {
            _logger->info("Checking if ticket exists with ID: " + std::to_string(id));
        }
        return _ticketRepository->exists(id);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error checking ticket existence: " + std::string(e.what()));
        }
        return Failure<bool>(CoreError("Failed to check ticket existence: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<size_t> TicketService::count()
{
    try
    {
        if (_logger)
        {
            _logger->info("Counting tickets");
        }
        return _ticketRepository->count();
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error counting tickets: " + std::string(e.what()));
        }
        return Failure<size_t>(CoreError("Failed to count tickets: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<Ticket> TicketService::create(const Ticket &entity)
{
    try
    {
        if (_logger)
        {
            _logger->info("Creating new ticket");
        }
        return _ticketRepository->create(entity);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error creating ticket: " + std::string(e.what()));
        }
        return Failure<Ticket>(CoreError("Failed to create ticket: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<Ticket> TicketService::update(const Ticket &entity)
{
    try
    {
        if (_logger)
        {
            _logger->info("Updating ticket with ID: " + std::to_string(entity.getId()));
        }
        return _ticketRepository->update(entity);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error updating ticket: " + std::string(e.what()));
        }
        return Failure<Ticket>(CoreError("Failed to update ticket: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<bool> TicketService::deleteById(const int &id)
{
    try
    {
        if (_logger)
        {
            _logger->info("Deleting ticket with ID: " + std::to_string(id));
        }
        return _ticketRepository->deleteById(id);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error deleting ticket: " + std::string(e.what()));
        }
        return Failure<bool>(CoreError("Failed to delete ticket: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<Ticket> TicketService::findByTicketNumber(const TicketNumber &ticketNumber)
{
    try
    {
        if (_logger)
        {
            _logger->info("Finding ticket by ticket number");
        }
        return _ticketRepository->findByTicketNumber(ticketNumber);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error finding ticket by ticket number: " + std::string(e.what()));
        }
        return Failure<Ticket>(CoreError("Failed to find ticket by ticket number: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<bool> TicketService::existsTicket(const TicketNumber &ticketNumber)
{
    try
    {
        if (_logger)
        {
            _logger->info("Checking if ticket exists by ticket number");
        }
        return _ticketRepository->existsTicket(ticketNumber);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error checking ticket existence by ticket number: " + std::string(e.what()));
        }
        return Failure<bool>(CoreError("Failed to check ticket existence by ticket number: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<std::vector<Ticket>> TicketService::findByPassengerId(int passengerId)
{
    try
    {
        if (_logger)
        {
            _logger->info("Finding tickets by passenger ID: " + std::to_string(passengerId));
        }
        return _ticketRepository->findByPassengerId(passengerId);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error finding tickets by passenger ID: " + std::string(e.what()));
        }
        return Failure<std::vector<Ticket>>(CoreError("Failed to find tickets by passenger ID: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}

Result<std::vector<Ticket>> TicketService::findBySerialNumber(const AircraftSerial &serial)
{
    try
    {
        if (_logger)
        {
            _logger->info("Finding tickets by aircraft serial number");
        }
        return _ticketRepository->findBySerialNumber(serial);
    }
    catch (const std::exception &e)
    {
        if (_logger)
        {
            _logger->error("Error finding tickets by aircraft serial: " + std::string(e.what()));
        }
        return Failure<std::vector<Ticket>>(CoreError("Failed to find tickets by aircraft serial: " + std::string(e.what()), "SERVICE_ERROR"));
    }
}