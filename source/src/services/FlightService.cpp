#include "FlightService.h"

Result<Flight> FlightService::getById(const int &id)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Getting flight by ID: " + std::to_string(id));
        return _repository->findById(id);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting flight by ID: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<std::vector<Flight>> FlightService::getAll()
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Getting all flights");
        return _repository->findAll();
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting all flights: " + std::string(e.what()));
        return Failure<std::vector<Flight>>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> FlightService::exists(const int &id)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->debug("Checking if flight exists with ID: " + std::to_string(id));
        return _repository->exists(id);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error checking flight existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<size_t> FlightService::count()
{
    try
    {
        auto logger = Logger::getInstance();
        logger->debug("Getting total flight count");
        return _repository->count();
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting flight count: " + std::string(e.what()));
        return Failure<size_t>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<Flight> FlightService::create(const Flight &entity)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Creating new flight with number: " + entity.getFlightNumber().value());
        auto existsResult = flightExistsByNumber(entity.getFlightNumber());
        if (!existsResult.has_value())
        {
            return Failure<Flight>(existsResult.error());
        }
        if (existsResult.value())
        {
            return Failure<Flight>(CoreError("Flight with flight number already exists"));
        }
        return _repository->create(entity);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error creating flight: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<Flight> FlightService::update(const Flight &entity)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Updating flight with ID: " + std::to_string(entity.getId()));
        auto existsResult = exists(entity.getId());
        if (!existsResult.has_value())
        {
            return Failure<Flight>(existsResult.error());
        }
        if (!existsResult.value())
        {
            return Failure<Flight>(CoreError("Flight not found"));
        }
        return _repository->update(entity);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error updating flight: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> FlightService::deleteById(const int &id)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Deleting flight with ID: " + std::to_string(id));
        auto existsResult = exists(id);
        if (!existsResult.has_value())
        {
            return Failure<bool>(existsResult.error());
        }
        if (!existsResult.value())
        {
            return Failure<bool>(CoreError("Flight not found"));
        }
        return _repository->deleteById(id);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error deleting flight: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

// Flight-specific operations
Result<Flight> FlightService::getFlightByNumber(const FlightNumber &number)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Getting flight by flight number: " + number.value());
        return _repository->findByFlightNumber(number);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting flight by number: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> FlightService::flightExistsByNumber(const FlightNumber &number)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->debug("Checking if flight exists with number: " + number.value());
        return _repository->existsFlight(number);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error checking flight existence by number: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}