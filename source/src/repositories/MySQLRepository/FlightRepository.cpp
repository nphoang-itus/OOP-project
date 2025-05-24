#include "FlightRepository.h"
#include "../../core/exceptions/Result.h"
#include "../../utils/Logger.h"
#include <sstream>
#include <map>
#include <format>

using namespace Tables::Flight;

Result<Flight> FlightRepository::findById(const int& id) {
    try {
        if (_logger) _logger->debug("Finding flight by id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(FIND_BY_ID_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding flight by id: " + std::to_string(id));
            return Failure<Flight>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding flight by id: " + std::to_string(id));
            return Failure<Flight>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding flight by id: " + std::to_string(id));
            return Failure<Flight>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("Flight not found with id: " + std::to_string(id));
            return Failure<Flight>(CoreError("Flight not found with id: " + std::to_string(id), "NOT_FOUND"));
        }

        // Get flight data
        auto idResult = dbResult->getInt(ColumnName[ID]);
        auto flightNumberResult = dbResult->getString(ColumnName[FLIGHT_NUMBER]);
        auto departureCodeResult = dbResult->getString(ColumnName[DEPARTURE_CODE]);
        auto departureNameResult = dbResult->getString(ColumnName[DEPARTURE_NAME]);
        auto arrivalCodeResult = dbResult->getString(ColumnName[ARRIVAL_CODE]);
        auto arrivalNameResult = dbResult->getString(ColumnName[ARRIVAL_NAME]);
        auto aircraftIdResult = dbResult->getInt(ColumnName[AIRCRAFT_ID]);
        auto departureTimeResult = dbResult->getDateTime(ColumnName[DEPARTURE_TIME]);
        auto arrivalTimeResult = dbResult->getDateTime(ColumnName[ARRIVAL_TIME]);
        auto statusResult = dbResult->getString(ColumnName[STATUS]);

        // Get aircraft data
        auto serialNumberResult = dbResult->getString("serial_number");
        auto modelResult = dbResult->getString("model");
        auto economySeatsResult = dbResult->getInt("economy_seats");
        auto businessSeatsResult = dbResult->getInt("business_seats");
        auto firstSeatsResult = dbResult->getInt("first_seats");

        if (!idResult || !flightNumberResult || !departureCodeResult || !departureNameResult || 
            !arrivalCodeResult || !arrivalNameResult || !aircraftIdResult || 
            !departureTimeResult || !arrivalTimeResult || !statusResult ||
            !serialNumberResult || !modelResult || !economySeatsResult || !businessSeatsResult || !firstSeatsResult) {
            if (_logger) _logger->error("Failed to get flight data for id: " + std::to_string(id));
            return Failure<Flight>(CoreError("Failed to get flight data", "DATA_ERROR"));
        }

        // Create route string Ho Chi Minh City(SGN)-Ha Noi(HAN)
        std::stringstream routeStr;
        routeStr << std::format (
            "{}({})-{}({})",
            departureNameResult.value(), departureCodeResult.value(),
            arrivalNameResult.value(), arrivalCodeResult.value()
        );

        // Create schedule string
        std::stringstream scheduleStr;
        scheduleStr << std::put_time(&departureTimeResult.value(), "%Y-%m-%d %H:%M") << "|"
                   << std::put_time(&arrivalTimeResult.value(), "%Y-%m-%d %H:%M");

        // Create aircraft
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

        auto serial = AircraftSerial::create(serialNumberResult.value()).value();
        auto seatLayout = SeatClassMap::create(seatLayoutStr.str()).value();
        auto aircraft = Aircraft::create(serial, modelResult.value(), seatLayout).value();
        aircraft.setId(aircraftIdResult.value());

        // Create flight
        auto flightNumber = FlightNumber::create(flightNumberResult.value()).value();
        auto route = Route::create(routeStr.str()).value();
        auto schedule = Schedule::create(scheduleStr.str()).value();
        auto flight = Flight::create(flightNumber, route, schedule, std::make_shared<Aircraft>(aircraft)).value();
        flight.setId(idResult.value());
        flight.setStatus(FlightStatusUtil::fromString(statusResult.value()));

        if (_logger) _logger->debug("Successfully found flight with id: " + std::to_string(id));
        return Success(flight);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding flight by id: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<std::vector<Flight>> FlightRepository::findAll() {
    try {
        if (_logger) _logger->debug("Finding all flights");

        auto result = _connection->executeQuery(FIND_ALL_QUERY);
        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding all flights");
            return Failure<std::vector<Flight>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        std::vector<Flight> flights;
        auto dbResult = std::move(result.value());
        
        while (dbResult->next().value()) {
            auto idResult = dbResult->getInt(ColumnName[ID]);
            if (!idResult) break;  // Break if we can't get the ID, indicating end of results
            
            auto flightNumberResult = dbResult->getString(ColumnName[FLIGHT_NUMBER]);
            auto departureCodeResult = dbResult->getString(ColumnName[DEPARTURE_CODE]);
            auto departureNameResult = dbResult->getString(ColumnName[DEPARTURE_NAME]);
            auto arrivalCodeResult = dbResult->getString(ColumnName[ARRIVAL_CODE]);
            auto arrivalNameResult = dbResult->getString(ColumnName[ARRIVAL_NAME]);
            auto aircraftIdResult = dbResult->getInt(ColumnName[AIRCRAFT_ID]);
            auto departureTimeResult = dbResult->getDateTime(ColumnName[DEPARTURE_TIME]);
            auto arrivalTimeResult = dbResult->getDateTime(ColumnName[ARRIVAL_TIME]);
            auto statusResult = dbResult->getString(ColumnName[STATUS]);

            // Get aircraft data
            auto serialNumberResult = dbResult->getString("serial_number");
            auto modelResult = dbResult->getString("model");
            auto economySeatsResult = dbResult->getInt("economy_seats");
            auto businessSeatsResult = dbResult->getInt("business_seats");
            auto firstSeatsResult = dbResult->getInt("first_seats");

            if (!flightNumberResult || !departureCodeResult || !departureNameResult || 
                !arrivalCodeResult || !arrivalNameResult || !aircraftIdResult || 
                !departureTimeResult || !arrivalTimeResult || !statusResult ||
                !serialNumberResult || !modelResult || !economySeatsResult || !businessSeatsResult || !firstSeatsResult) {
                if (_logger) _logger->warning("Skipping invalid flight data");
                continue;
            }

            // Create route string
            std::stringstream routeStr;
            routeStr << std::format (
                "{}({})-{}({})",
                departureNameResult.value(), departureCodeResult.value(),
                arrivalNameResult.value(), arrivalCodeResult.value()
            );

            // Create schedule string
            std::stringstream scheduleStr;
            scheduleStr << std::put_time(&departureTimeResult.value(), "%Y-%m-%d %H:%M") << "|"
                       << std::put_time(&arrivalTimeResult.value(), "%Y-%m-%d %H:%M");

            // Create aircraft
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

            auto serial = AircraftSerial::create(serialNumberResult.value());
            if (!serial) {
                if (_logger) _logger->warning("Invalid serial number: " + serialNumberResult.value());
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
            aircraft->setId(aircraftIdResult.value());

            // Create flight
            auto flightNumber = FlightNumber::create(flightNumberResult.value());
            if (!flightNumber) {
                if (_logger) _logger->warning("Invalid flight number: " + flightNumberResult.value());
                continue;
            }

            auto route = Route::create(routeStr.str());
            if (!route) {
                if (_logger) _logger->warning("Invalid route: " + routeStr.str());
                continue;
            }

            auto schedule = Schedule::create(scheduleStr.str());
            if (!schedule) {
                if (_logger) _logger->warning("Invalid schedule: " + scheduleStr.str());
                continue;
            }

            auto flight = Flight::create(*flightNumber, *route, *schedule, std::make_shared<Aircraft>(*aircraft));
            if (!flight) {
                if (_logger) _logger->warning("Failed to create flight");
                continue;
            }

            flight->setId(idResult.value());
            flight->setStatus(FlightStatusUtil::fromString(statusResult.value()));
            flights.push_back(*flight);
        }

        if (_logger) _logger->debug("Successfully found " + std::to_string(flights.size()) + " flights");
        return Success(flights);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding all flights: " + std::string(e.what()));
        return Failure<std::vector<Flight>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> FlightRepository::exists(const int& id) {
    try {
        if (_logger) _logger->debug("Checking existence of flight with id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(EXISTS_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking flight existence");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for checking flight existence");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for checking flight existence");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("Flight not found with id: " + std::to_string(id));
            return Success(false);
        }

        auto countResult = dbResult->getInt(0);  // COUNT(*) is always the first column (index 0)
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count for flight existence check");
            return Failure<bool>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        bool exists = countResult.value() > 0;
        if (_logger) _logger->debug("Flight " + std::to_string(id) + " " + (exists ? "exists" : "does not exist"));
        return Success(exists);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error checking flight existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<size_t> FlightRepository::count() {
    try {
        if (_logger) _logger->debug("Counting total flights");

        auto result = _connection->executeQuery(COUNT_QUERY);
        if (!result) {
            if (_logger) _logger->error("Failed to execute query for counting flights");
            return Failure<size_t>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("No flights found");
            return Success(0);
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count");
            return Failure<size_t>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        size_t count = static_cast<size_t>(countResult.value());
        if (_logger) _logger->debug("Total flight count: " + std::to_string(count));
        return Success(count);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error counting flights: " + std::string(e.what()));
        return Failure<size_t>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Flight> FlightRepository::create(const Flight& flight) {
    try {
        if (_logger) _logger->debug("Creating new flight");

        // Start transaction
        _connection->beginTransaction();

        // Insert flight
        auto prepareResult = _connection->prepareStatement(INSERT_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for creating flight");
            return Failure<Flight>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setFlightNumberResult = _connection->setString(stmtId, 1, flight.getFlightNumber().toString());
        auto setDepartureCodeResult = _connection->setString(stmtId, 2, flight.getRoute().getOriginCode());
        auto setDepartureNameResult = _connection->setString(stmtId, 3, flight.getRoute().getOrigin());
        auto setArrivalCodeResult = _connection->setString(stmtId, 4, flight.getRoute().getDestinationCode());
        auto setArrivalNameResult = _connection->setString(stmtId, 5, flight.getRoute().getDestination());
        auto setAircraftIdResult = _connection->setInt(stmtId, 6, flight.getAircraft()->getId());
        auto setDepartureTimeResult = _connection->setDateTime(stmtId, 7, flight.getSchedule().getDeparture());
        auto setArrivalTimeResult = _connection->setDateTime(stmtId, 8, flight.getSchedule().getArrival());
        auto setStatusResult = _connection->setString(stmtId, 9, flight.getStatusString());

        if (!setFlightNumberResult || !setDepartureCodeResult || !setDepartureNameResult || 
            !setArrivalCodeResult || !setArrivalNameResult || !setAircraftIdResult || 
            !setDepartureTimeResult || !setArrivalTimeResult || !setStatusResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameters for creating flight");
            return Failure<Flight>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for creating flight");
            return Failure<Flight>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        auto idResult = _connection->getLastInsertId();
        if (!idResult) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to get last insert id for flight");
            return Failure<Flight>(CoreError("Failed to get last insert id", "GET_ID_FAILED"));
        }

        _connection->commitTransaction();

        auto newFlight = flight;
        newFlight.setId(idResult.value());
        if (_logger) _logger->debug("Successfully created flight with id: " + std::to_string(idResult.value()));
        return Success(newFlight);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error creating flight: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Flight> FlightRepository::update(const Flight& flight) {
    try {
        if (_logger) _logger->debug("Updating flight with id: " + std::to_string(flight.getId()));

        // First check if flight exists
        auto existsResult = exists(flight.getId());
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check flight existence");
            return Failure<Flight>(CoreError("Failed to check flight existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Flight not found with id: " + std::to_string(flight.getId()));
            return Failure<Flight>(CoreError("Flight not found with id: " + std::to_string(flight.getId()), "DB_ERROR"));
        }

        // Start transaction
        _connection->beginTransaction();

        // Update flight
        auto prepareResult = _connection->prepareStatement(UPDATE_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for updating flight");
            return Failure<Flight>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setFlightNumberResult = _connection->setString(stmtId, 1, flight.getFlightNumber().toString());
        auto setDepartureCodeResult = _connection->setString(stmtId, 2, flight.getRoute().getOriginCode());
        auto setDepartureNameResult = _connection->setString(stmtId, 3, flight.getRoute().getOrigin());
        auto setArrivalCodeResult = _connection->setString(stmtId, 4, flight.getRoute().getDestinationCode());
        auto setArrivalNameResult = _connection->setString(stmtId, 5, flight.getRoute().getDestination());
        auto setAircraftIdResult = _connection->setInt(stmtId, 6, flight.getAircraft()->getId());
        auto setDepartureTimeResult = _connection->setDateTime(stmtId, 7, flight.getSchedule().getDeparture());
        auto setArrivalTimeResult = _connection->setDateTime(stmtId, 8, flight.getSchedule().getArrival());
        auto setStatusResult = _connection->setString(stmtId, 9, flight.getStatusString());
        auto setIdResult = _connection->setInt(stmtId, 10, flight.getId());

        if (!setFlightNumberResult || !setDepartureCodeResult || !setDepartureNameResult || 
            !setArrivalCodeResult || !setArrivalNameResult || !setAircraftIdResult || 
            !setDepartureTimeResult || !setArrivalTimeResult || !setStatusResult || !setIdResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameters for updating flight");
            return Failure<Flight>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for updating flight");
            return Failure<Flight>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        _connection->commitTransaction();

        if (_logger) _logger->debug("Successfully updated flight with id: " + std::to_string(flight.getId()));
        return Success(flight);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error updating flight: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> FlightRepository::deleteById(const int& id) {
    try {
        if (_logger) _logger->debug("Deleting flight with id: " + std::to_string(id));

        // First check if flight exists
        auto existsResult = exists(id);
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check flight existence");
            return Failure<bool>(CoreError("Failed to check flight existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Flight not found with id: " + std::to_string(id));
            return Failure<bool>(CoreError("Flight not found with id: " + std::to_string(id), "DB_ERROR"));
        }

        // Start transaction
        _connection->beginTransaction();

        // Delete flight
        auto prepareResult = _connection->prepareStatement(DELETE_QUERY);
        if (!prepareResult) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to prepare statement for deleting flight");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameter for deleting flight");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for deleting flight");
            return Failure<bool>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        _connection->commitTransaction();

        if (_logger) _logger->debug("Successfully deleted flight with id: " + std::to_string(id));
        return Success(true);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error deleting flight: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Flight FlightRepository::mapRowToFlight(const std::map<std::string, std::string>& row) const {
    auto id = std::stoi(row.at(ColumnName[ID]));
    auto flightNumber = FlightNumber::create(row.at(ColumnName[FLIGHT_NUMBER])).value();
    auto route = Route::create(row.at(ColumnName[DEPARTURE_CODE]) + "-" + row.at(ColumnName[ARRIVAL_CODE])).value();
    
    // Create schedule string from departure and arrival times
    std::stringstream scheduleStr;
    scheduleStr << row.at(ColumnName[DEPARTURE_TIME]) << "|" << row.at(ColumnName[ARRIVAL_TIME]);
    auto schedule = Schedule::create(scheduleStr.str()).value();
    
    // Create aircraft
    std::stringstream seatLayoutStr;
    int economySeats = std::stoi(row.at("economy_seats"));
    int businessSeats = std::stoi(row.at("business_seats"));
    int firstSeats = std::stoi(row.at("first_seats"));
    
    if (economySeats > 0) seatLayoutStr << "E:" << economySeats;
    if (businessSeats > 0) {
        if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
        seatLayoutStr << "B:" << businessSeats;
    }
    if (firstSeats > 0) {
        if (!seatLayoutStr.str().empty()) seatLayoutStr << ",";
        seatLayoutStr << "F:" << firstSeats;
    }
    
    auto serial = AircraftSerial::create(row.at("serial_number")).value();
    auto seatLayout = SeatClassMap::create(seatLayoutStr.str()).value();
    auto aircraft = Aircraft::create(serial, row.at("model"), seatLayout).value();
    aircraft.setId(std::stoi(row.at(ColumnName[AIRCRAFT_ID])));
    
    auto flight = Flight::create(flightNumber, route, schedule, std::make_shared<Aircraft>(aircraft)).value();
    flight.setId(id);
    flight.setStatus(FlightStatusUtil::fromString(row.at(ColumnName[STATUS])));
    return flight;
}

Result<Flight> FlightRepository::findByFlightNumber(const FlightNumber& number) {
    try {
        if (_logger) _logger->debug("Finding flight by flight number: " + number.toString());

        auto prepareResult = _connection->prepareStatement(FIND_BY_NUMBER_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding flight by flight number: " + number.toString());
            return Failure<Flight>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, number.value());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding flight by flight number: " + number.toString());
            return Failure<Flight>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding flight by flight number: " + number.toString());
            return Failure<Flight>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("Flight not found with flight number: " + number.toString());
            return Failure<Flight>(CoreError("Flight not found with flight number: " + number.toString()));
        }

        // Get flight data
        auto idResult = dbResult->getInt(ColumnName[ID]);
        auto flightNumberResult = dbResult->getString(ColumnName[FLIGHT_NUMBER]);
        auto departureCodeResult = dbResult->getString(ColumnName[DEPARTURE_CODE]);
        auto departureNameResult = dbResult->getString(ColumnName[DEPARTURE_NAME]);
        auto arrivalCodeResult = dbResult->getString(ColumnName[ARRIVAL_CODE]);
        auto arrivalNameResult = dbResult->getString(ColumnName[ARRIVAL_NAME]);
        auto aircraftIdResult = dbResult->getInt(ColumnName[AIRCRAFT_ID]);
        auto departureTimeResult = dbResult->getDateTime(ColumnName[DEPARTURE_TIME]);
        auto arrivalTimeResult = dbResult->getDateTime(ColumnName[ARRIVAL_TIME]);
        auto statusResult = dbResult->getString(ColumnName[STATUS]);

        // Get aircraft data
        auto serialNumberResult = dbResult->getString("serial_number");
        auto modelResult = dbResult->getString("model");
        auto economySeatsResult = dbResult->getInt("economy_seats");
        auto businessSeatsResult = dbResult->getInt("business_seats");
        auto firstSeatsResult = dbResult->getInt("first_seats");

        if (!idResult || !flightNumberResult || !departureCodeResult || !departureNameResult || 
            !arrivalCodeResult || !arrivalNameResult || !aircraftIdResult || 
            !departureTimeResult || !arrivalTimeResult || !statusResult ||
            !serialNumberResult || !modelResult || !economySeatsResult || !businessSeatsResult || !firstSeatsResult) {
            if (_logger) _logger->error("Failed to get flight data for flight number: " + number.toString());
            return Failure<Flight>(CoreError("Failed to get flight data", "DATA_ERROR"));
        }

        // Create route string Ho Chi Minh City(SGN)-Ha Noi(HAN)
        std::stringstream routeStr;
        routeStr << std::format (
            "{}({})-{}({})",
            departureNameResult.value(), departureCodeResult.value(),
            arrivalNameResult.value(), arrivalCodeResult.value()
        );

        // Create schedule string
        std::stringstream scheduleStr;
        scheduleStr << std::put_time(&departureTimeResult.value(), "%Y-%m-%d %H:%M") << "|"
                   << std::put_time(&arrivalTimeResult.value(), "%Y-%m-%d %H:%M");

        // Create aircraft
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

        auto serial = AircraftSerial::create(serialNumberResult.value()).value();
        auto seatLayout = SeatClassMap::create(seatLayoutStr.str()).value();
        auto aircraft = Aircraft::create(serial, modelResult.value(), seatLayout).value();
        aircraft.setId(aircraftIdResult.value());

        // Create flight
        auto flightNumber = FlightNumber::create(flightNumberResult.value()).value();
        auto route = Route::create(routeStr.str()).value();
        auto schedule = Schedule::create(scheduleStr.str()).value();
        auto flight = Flight::create(flightNumber, route, schedule, std::make_shared<Aircraft>(aircraft)).value();
        flight.setId(idResult.value());
        flight.setStatus(FlightStatusUtil::fromString(statusResult.value()));

        if (_logger) _logger->debug("Successfully found flight with flight number: " + number.toString());
        return Success(flight);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding flight by id: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> FlightRepository::existsFlight(const FlightNumber& number) {
    try {
        if (_logger) _logger->debug("Checking existence of flight with flight number: " + number.toString());

        auto prepareResult = _connection->prepareStatement(EXISTS_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking flight existence");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, number.value());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for checking flight existence");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for checking flight existence");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("Flight not found with flight number: " + number.toString());
            return Success(false);
        }

        auto countResult = dbResult->getInt(0);  // COUNT(*) is always the first column (index 0)
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count for flight existence check");
            return Failure<bool>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        bool exists = countResult.value() > 0;
        if (_logger) _logger->debug("Flight " + number.value() + " " + (exists ? "exists" : "does not exist"));
        return Success(exists);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error checking flight existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}