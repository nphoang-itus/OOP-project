#include "AircraftRepository.h"
#include "../../core/exceptions/Result.h"
#include "../../utils/Logger.h"
#include <sstream>
#include <map>

using namespace Tables::Aircraft;

Result<Aircraft> AircraftRepository::findById(const int& id) {
    try {
        if (_logger) _logger->debug("Finding aircraft by id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(FIND_BY_ID_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding aircraft by id: " + std::to_string(id));
            return Failure<Aircraft>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding aircraft by id: " + std::to_string(id));
            return Failure<Aircraft>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding aircraft by id: " + std::to_string(id));
            return Failure<Aircraft>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("Aircraft not found with id: " + std::to_string(id));
            return Failure<Aircraft>(CoreError("Aircraft not found with id: " + std::to_string(id), "NOT_FOUND"));
        }

        auto idResult = dbResult->getInt(ColumnName[ID]);
        auto serialResult = dbResult->getString(ColumnName[SERIAL]);
        auto modelResult = dbResult->getString(ColumnName[MODEL]);
        auto economySeatsResult = dbResult->getInt(ColumnName[ECONOMY_SEATS]);
        auto businessSeatsResult = dbResult->getInt(ColumnName[BUSINESS_SEATS]);
        auto firstSeatsResult = dbResult->getInt(ColumnName[FIRST_SEATS]);

        if (!idResult || !serialResult || !modelResult || !economySeatsResult || !businessSeatsResult || !firstSeatsResult) {
            if (_logger) _logger->error("Failed to get aircraft data for id: " + std::to_string(id));
            return Failure<Aircraft>(CoreError("Failed to get aircraft data", "DATA_ERROR"));
        }

        // Create seat layout string
        std::stringstream seatLayoutStr;
        if (economySeatsResult.value() > 0) seatLayoutStr << "E:" << economySeatsResult.value();
        if (businessSeatsResult.value() > 0) {
            if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
            seatLayoutStr << "B:" << businessSeatsResult.value();
        }
        if (firstSeatsResult.value() > 0) {
            if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
            seatLayoutStr << "F:" << firstSeatsResult.value();
        }

        auto serial = AircraftSerial::create(serialResult.value()).value();
        auto seatLayout = SeatClassMap::create(seatLayoutStr.str()).value();
        auto aircraft = Aircraft::create(serial, modelResult.value(), seatLayout).value();
        aircraft.setId(idResult.value());

        if (_logger) _logger->debug("Successfully found aircraft with id: " + std::to_string(id));
        return Success(aircraft);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding aircraft by id: " + std::string(e.what()));
        return Failure<Aircraft>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<std::vector<Aircraft>> AircraftRepository::findAll() {
    try {
        if (_logger) _logger->debug("Finding all aircraft");

        auto result = _connection->executeQuery(FIND_ALL_QUERY);
        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding all aircraft");
            return Failure<std::vector<Aircraft>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        std::vector<Aircraft> aircrafts;
        auto dbResult = std::move(result.value());
        
        while (dbResult->next().value()) {
            auto idResult = dbResult->getInt(ColumnName[ID]);
            if (!idResult) break;  // Break if we can't get the ID, indicating end of results
            
            auto serialResult = dbResult->getString(ColumnName[SERIAL]);
            auto modelResult = dbResult->getString(ColumnName[MODEL]);
            auto economySeatsResult = dbResult->getInt(ColumnName[ECONOMY_SEATS]);
            auto businessSeatsResult = dbResult->getInt(ColumnName[BUSINESS_SEATS]);
            auto firstSeatsResult = dbResult->getInt(ColumnName[FIRST_SEATS]);

            if (!serialResult || !modelResult || !economySeatsResult || !businessSeatsResult || !firstSeatsResult) {
                if (_logger) _logger->warning("Skipping invalid aircraft data");
                continue;
            }

            // Create seat layout string
            std::stringstream seatLayoutStr;
            if (economySeatsResult.value() > 0) seatLayoutStr << "E:" << economySeatsResult.value();
            if (businessSeatsResult.value() > 0) {
                if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
                seatLayoutStr << "B:" << businessSeatsResult.value();
            }
            if (firstSeatsResult.value() > 0) {
                if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
                seatLayoutStr << "F:" << firstSeatsResult.value();
            }

            auto serial = AircraftSerial::create(serialResult.value());
            if (!serial) {
                if (_logger) _logger->warning("Invalid serial number: " + serialResult.value());
                continue;
            }

            auto seatLayout = SeatClassMap::create(seatLayoutStr.str());
            if (!seatLayout) {
                if (_logger) _logger->warning("Invalid seat layout: " + seatLayoutStr.str());
                continue;
            }

            auto aircraft = Aircraft::create(*serial, modelResult.value(), *seatLayout);
            if (!aircraft) {
                if (_logger) _logger->warning("Failed to create aircraft");
                continue;
            }

            aircraft->setId(idResult.value());
            aircrafts.push_back(*aircraft);
        }

        if (_logger) _logger->debug("Successfully found " + std::to_string(aircrafts.size()) + " aircraft");
        return Success(aircrafts);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding all aircraft: " + std::string(e.what()));
        return Failure<std::vector<Aircraft>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> AircraftRepository::exists(const int& id) {
    try {
        if (_logger) _logger->debug("Checking existence of aircraft with id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(EXISTS_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking aircraft existence");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for checking aircraft existence");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for checking aircraft existence");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("Aircraft not found with id: " + std::to_string(id));
            return Success(false);
        }

        auto countResult = dbResult->getInt(0);  // COUNT(*) is always the first column (index 0)
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count for aircraft existence check");
            return Failure<bool>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        bool exists = countResult.value() > 0;
        if (_logger) _logger->debug("Aircraft " + std::to_string(id) + " " + (exists ? "exists" : "does not exist"));
        return Success(exists);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error checking aircraft existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<size_t> AircraftRepository::count() {
    try {
        if (_logger) _logger->debug("Counting total aircraft");

        auto result = _connection->executeQuery(COUNT_QUERY);
        if (!result) {
            if (_logger) _logger->error("Failed to execute query for counting aircraft");
            return Failure<size_t>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("No aircraft found");
            return Success(0);
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count");
            return Failure<size_t>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        size_t count = static_cast<size_t>(countResult.value());
        if (_logger) _logger->debug("Total aircraft count: " + std::to_string(count));
        return Success(count);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error counting aircraft: " + std::string(e.what()));
        return Failure<size_t>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Aircraft> AircraftRepository::create(const Aircraft& aircraft) {
    try {
        if (_logger) _logger->debug("Creating new aircraft");

        // Start transaction
        _connection->beginTransaction();

        // Insert aircraft
        auto prepareResult = _connection->prepareStatement(INSERT_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for creating aircraft");
            return Failure<Aircraft>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setSerialResult = _connection->setString(stmtId, 1, aircraft.getSerial().toString());
        auto setModelResult = _connection->setString(stmtId, 2, aircraft.getModel());
        auto setEconomySeatsResult = _connection->setInt(stmtId, 3, aircraft.getSeatLayout().getSeatCount("E"));
        auto setBusinessSeatsResult = _connection->setInt(stmtId, 4, aircraft.getSeatLayout().getSeatCount("B"));
        auto setFirstSeatsResult = _connection->setInt(stmtId, 5, aircraft.getSeatLayout().getSeatCount("F"));

        if (!setSerialResult || !setModelResult || !setEconomySeatsResult || !setBusinessSeatsResult || !setFirstSeatsResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameters for creating aircraft");
            return Failure<Aircraft>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for creating aircraft");
            return Failure<Aircraft>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        auto idResult = _connection->getLastInsertId();
        if (!idResult) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to get last insert id for aircraft");
            return Failure<Aircraft>(CoreError("Failed to get last insert id", "GET_ID_FAILED"));
        }

        _connection->commitTransaction();

        auto newAircraft = aircraft;
        newAircraft.setId(idResult.value());
        if (_logger) _logger->debug("Successfully created aircraft with id: " + std::to_string(idResult.value()));
        return Success(newAircraft);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error creating aircraft: " + std::string(e.what()));
        return Failure<Aircraft>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Aircraft> AircraftRepository::update(const Aircraft& aircraft) {
    try {
        if (_logger) _logger->debug("Updating aircraft with id: " + std::to_string(aircraft.getId()));

        // First check if aircraft exists
        auto existsResult = exists(aircraft.getId());
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check aircraft existence");
            return Failure<Aircraft>(CoreError("Failed to check aircraft existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Aircraft not found with id: " + std::to_string(aircraft.getId()));
            return Failure<Aircraft>(CoreError("Aircraft not found with id: " + std::to_string(aircraft.getId()), "DB_ERROR"));
        }

        // Start transaction
        _connection->beginTransaction();

        // Update aircraft
        auto prepareResult = _connection->prepareStatement(UPDATE_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for updating aircraft");
            return Failure<Aircraft>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setSerialResult = _connection->setString(stmtId, 1, aircraft.getSerial().toString());
        auto setModelResult = _connection->setString(stmtId, 2, aircraft.getModel());
        auto setEconomySeatsResult = _connection->setInt(stmtId, 3, aircraft.getSeatLayout().getSeatCount("E"));
        auto setBusinessSeatsResult = _connection->setInt(stmtId, 4, aircraft.getSeatLayout().getSeatCount("B"));
        auto setFirstSeatsResult = _connection->setInt(stmtId, 5, aircraft.getSeatLayout().getSeatCount("F"));
        auto setIdResult = _connection->setInt(stmtId, 6, aircraft.getId());

        if (!setSerialResult || !setModelResult || !setEconomySeatsResult || !setBusinessSeatsResult || !setFirstSeatsResult || !setIdResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameters for updating aircraft");
            return Failure<Aircraft>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for updating aircraft");
            return Failure<Aircraft>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        _connection->commitTransaction();

        if (_logger) _logger->debug("Successfully updated aircraft with id: " + std::to_string(aircraft.getId()));
        return Success(aircraft);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error updating aircraft: " + std::string(e.what()));
        return Failure<Aircraft>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> AircraftRepository::deleteById(const int& id) {
    try {
        if (_logger) _logger->debug("Deleting aircraft with id: " + std::to_string(id));

        // First check if aircraft exists
        auto existsResult = exists(id);
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check aircraft existence");
            return Failure<bool>(CoreError("Failed to check aircraft existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Aircraft not found with id: " + std::to_string(id));
            return Failure<bool>(CoreError("Aircraft not found with id: " + std::to_string(id), "DB_ERROR"));
        }

        // Start transaction
        _connection->beginTransaction();

        // Delete aircraft
        auto prepareResult = _connection->prepareStatement(DELETE_QUERY);
        if (!prepareResult) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to prepare statement for deleting aircraft");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameter for deleting aircraft");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for deleting aircraft");
            return Failure<bool>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        _connection->commitTransaction();

        if (_logger) _logger->debug("Successfully deleted aircraft with id: " + std::to_string(id));
        return Success(true);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error deleting aircraft: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Aircraft AircraftRepository::mapRowToAircraft(const std::map<std::string, std::string>& row) const {
    auto id = std::stoi(row.at(ColumnName[ID]));
    auto serial = AircraftSerial::create(row.at(ColumnName[SERIAL])).value();
    auto model = row.at(ColumnName[MODEL]);
    
    // Create seat layout string from individual seat counts
    std::stringstream seatLayoutStr;
    int economySeats = std::stoi(row.at(ColumnName[ECONOMY_SEATS]));
    int businessSeats = std::stoi(row.at(ColumnName[BUSINESS_SEATS]));
    int firstSeats = std::stoi(row.at(ColumnName[FIRST_SEATS]));
    
    if (economySeats > 0) seatLayoutStr << "E:" << economySeats;
    if (businessSeats > 0) {
        if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
        seatLayoutStr << "B:" << businessSeats;
    }
    if (firstSeats > 0) {
        if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
        seatLayoutStr << "F:" << firstSeats;
    }
    
    auto seatLayout = SeatClassMap::create(seatLayoutStr.str()).value();
    auto aircraft = Aircraft::create(serial, model, seatLayout).value();
    aircraft.setId(id);
    return aircraft;
} 