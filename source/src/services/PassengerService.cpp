#include "PassengerService.h"

Result<Passenger> PassengerService::getPassengerById(const int &id)
{
    try
    {
        _logger->info("Getting passenger by ID: " + std::to_string(id));
        auto result = _repository->findById(id);
        if (result.has_value())
        {
            _logger->info("Successfully retrieved passenger with ID: " + std::to_string(id));
        }
        else
        {
            _logger->warning("Passenger not found with ID: " + std::to_string(id));
        }
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error getting passenger by ID: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<std::vector<Passenger>> PassengerService::getAllPassengers()
{
    try
    {
        _logger->info("Getting all passengers");
        auto result = _repository->findAll();
        if (result.has_value())
        {
            _logger->info("Successfully retrieved " + std::to_string(result.value().size()) + " passengers");
        }
        else
        {
            _logger->warning("Failed to retrieve passengers");
        }
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error getting all passengers: " + std::string(e.what()));
        return Failure<std::vector<Passenger>>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<Passenger> PassengerService::createPassenger(const Passenger &passenger)
{
    try
    {
        _logger->info("Creating new passenger");
        auto result = _repository->create(passenger);
        if (result.has_value())
        {
            _logger->info("Successfully created passenger with ID: " + std::to_string(result.value().getId()));
        }
        else
        {
            _logger->warning("Failed to create passenger");
        }
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error creating passenger: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<Passenger> PassengerService::updatePassenger(const Passenger &passenger)
{
    try
    {
        _logger->info("Updating passenger with ID: " + std::to_string(passenger.getId()));
        auto result = _repository->update(passenger);
        if (result.has_value())
        {
            _logger->info("Successfully updated passenger with ID: " + std::to_string(passenger.getId()));
        }
        else
        {
            _logger->warning("Failed to update passenger with ID: " + std::to_string(passenger.getId()));
        }
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error updating passenger: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> PassengerService::deletePassenger(const int &id)
{
    try
    {
        _logger->info("Deleting passenger with ID: " + std::to_string(id));
        auto result = _repository->deleteById(id);
        if (result.has_value())
        {
            _logger->info("Successfully deleted passenger with ID: " + std::to_string(id));
        }
        else
        {
            _logger->warning("Failed to delete passenger with ID: " + std::to_string(id));
        }
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error deleting passenger: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<Passenger> PassengerService::getPassengerByPassport(const PassportNumber &passport)
{
    try
    {
        _logger->info("Getting passenger by passport");
        auto result = _repository->findByPassportNumber(passport);
        if (result.has_value())
        {
            _logger->info("Successfully retrieved passenger by passport");
        }
        else
        {
            _logger->warning("Passenger not found with given passport");
        }
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error getting passenger by passport: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> PassengerService::passengerExists(const int &id)
{
    try
    {
        _logger->info("Checking if passenger exists with ID: " + std::to_string(id));
        auto result = _repository->exists(id);
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error checking passenger existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<bool> PassengerService::passportExists(const PassportNumber &passport)
{
    try
    {
        _logger->info("Checking if passport exists");
        auto result = _repository->existsPassport(passport);
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error checking passport existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what())));
    }
}

Result<size_t> PassengerService::getPassengerCount()
{
    try
    {
        _logger->info("Getting passenger count");
        auto result = _repository->count();
        if (result.has_value())
        {
            _logger->info("Passenger count: " + std::to_string(result.value()));
        }
        return result;
    }
    catch (const std::exception &e)
    {
        _logger->error("Error getting passenger count: " + std::string(e.what()));
        return Failure<size_t>(CoreError("Database error: " + std::string(e.what())));
    }
}