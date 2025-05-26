#include "AircraftService.h"

Result<Aircraft> AircraftService::getById(const int &id)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Getting aircraft by ID: " + std::to_string(id));
        return _repository->findById(id);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting aircraft by ID: " + std::string(e.what()));
        return Failure<Aircraft>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<std::vector<Aircraft>> AircraftService::getAll()
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Getting all aircraft");
        return _repository->findAll();
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting all aircraft: " + std::string(e.what()));
        return Failure<std::vector<Aircraft>>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> AircraftService::exists(const int &id)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->debug("Checking if aircraft exists with ID: " + std::to_string(id));
        return _repository->exists(id);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error checking aircraft existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<size_t> AircraftService::count()
{
    try
    {
        auto logger = Logger::getInstance();
        logger->debug("Getting total aircraft count");
        return _repository->count();
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting aircraft count: " + std::string(e.what()));
        return Failure<size_t>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<Aircraft> AircraftService::create(const Aircraft &entity)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Creating new aircraft with serial: " + entity.getSerial().value());
        auto existsResult = aircraftExistsBySerialNumber(entity.getSerial());
        if (!existsResult.has_value())
        {
            return Failure<Aircraft>(existsResult.error());
        }
        if (existsResult.value())
        {
            return Failure<Aircraft>(CoreError("Aircraft with serial number already exists"));
        }
        return _repository->create(entity);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error creating aircraft: " + std::string(e.what()));
        return Failure<Aircraft>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<Aircraft> AircraftService::update(const Aircraft &entity)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Updating aircraft with ID: " + std::to_string(entity.getId()));
        auto existsResult = exists(entity.getId());
        if (!existsResult.has_value())
        {
            return Failure<Aircraft>(existsResult.error());
        }
        if (!existsResult.value())
        {
            return Failure<Aircraft>(CoreError("Aircraft not found"));
        }
        return _repository->update(entity);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error updating aircraft: " + std::string(e.what()));
        return Failure<Aircraft>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> AircraftService::deleteById(const int &id)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Deleting aircraft with ID: " + std::to_string(id));
        auto existsResult = exists(id);
        if (!existsResult.has_value())
        {
            return Failure<bool>(existsResult.error());
        }
        if (!existsResult.value())
        {
            return Failure<bool>(CoreError("Aircraft not found"));
        }
        return _repository->deleteById(id);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error deleting aircraft: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

// Aircraft-specific operations
Result<Aircraft> AircraftService::getAircraftBySerialNumber(const AircraftSerial &serial)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Getting aircraft by serial number: " + serial.value());
        return _repository->findBySerialNumber(serial);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error getting aircraft by serial number: " + std::string(e.what()));
        return Failure<Aircraft>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> AircraftService::aircraftExistsBySerialNumber(const AircraftSerial &serial)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->debug("Checking if aircraft exists with serial: " + serial.value());
        return _repository->existsAircraft(serial);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error checking aircraft existence by serial: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> AircraftService::deleteAircraftBySerialNumber(const AircraftSerial &serial)
{
    try
    {
        auto logger = Logger::getInstance();
        logger->info("Deleting aircraft with serial number: " + serial.value());
        auto existsResult = aircraftExistsBySerialNumber(serial);
        if (!existsResult.has_value())
        {
            return Failure<bool>(existsResult.error());
        }
        if (!existsResult.value())
        {
            return Failure<bool>(CoreError("Aircraft not found"));
        }
        return _repository->deleteBySerialNumber(serial);
    }
    catch (const std::exception &e)
    {
        auto logger = Logger::getInstance();
        logger->error("Error deleting aircraft by serial number: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}