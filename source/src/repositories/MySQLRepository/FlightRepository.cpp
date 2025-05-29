#include "FlightRepository.h"
#include "../../core/exceptions/Result.h"
#include "../../utils/Logger.h"
#include <sstream>
#include <map>
#include <format>
#include <iomanip>

using namespace Tables::Flight;

/**
 * @brief Tìm kiếm chuyến bay theo ID
 * 
 * Phương thức này thực hiện truy vấn cơ sở dữ liệu để tìm chuyến bay theo ID.
 * Bao gồm cả thông tin máy bay và tình trạng ghế ngồi.
 * 
 * @param id ID của chuyến bay cần tìm
 * @return Result<Flight> Kết quả chứa đối tượng Flight hoặc lỗi
 */
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

        // Get seat availability
        auto seatAvailability = getSeatAvailability(flight);
        flight.initializeSeats(seatAvailability);

        if (_logger) _logger->debug("Successfully found flight with id: " + std::to_string(id));
        return Success(flight);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding flight by id: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

/**
 * @brief Lấy tất cả chuyến bay từ cơ sở dữ liệu
 * 
 * Phương thức này truy vấn tất cả chuyến bay có trong cơ sở dữ liệu,
 * bao gồm thông tin chi tiết về máy bay và tuyến đường.
 * 
 * @return Result<std::vector<Flight>> Vector chứa tất cả chuyến bay hoặc lỗi
 */
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

/**
 * @brief Kiểm tra sự tồn tại của chuyến bay theo ID
 * 
 * Phương thức này kiểm tra xem có chuyến bay nào với ID được cung cấp
 * tồn tại trong cơ sở dữ liệu hay không.
 * 
 * @param id ID của chuyến bay cần kiểm tra
 * @return Result<bool> True nếu tồn tại, false nếu không tồn tại, hoặc lỗi
 */
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

/**
 * @brief Đếm tổng số chuyến bay trong cơ sở dữ liệu
 * 
 * @return Result<size_t> Số lượng chuyến bay hoặc lỗi
 */
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

/**
 * @brief Tạo mới một chuyến bay trong cơ sở dữ liệu
 * 
 * Phương thức này tạo một chuyến bay mới và tự động tạo các bản ghi
 * tình trạng ghế ngồi tương ứng với cấu hình ghế của máy bay.
 * 
 * @param flight Đối tượng Flight cần tạo
 * @return Result<Flight> Chuyến bay đã được tạo với ID hoặc lỗi
 */
Result<Flight> FlightRepository::create(const Flight& flight) {
    try {
        if (_logger) _logger->debug("Creating new flight");

        // First check if flight exists
        auto existsResult = existsFlight(flight.getFlightNumber());
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check flight existence");
            return Failure<Flight>(CoreError("Failed to check flight existence", "DB_ERROR"));
        }
        if (existsResult.value()) {
            if (_logger) _logger->error("Failed to create because flight : " + flight.getFlightNumber().toString() + " exists");
            return Failure<Flight>(CoreError("Failed to create because flight : " + flight.getFlightNumber().toString() + " exists", "DUPLICATE_FLIGHT_NUMBER"));
        }

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

        // Create seat availability records
        std::string insertSeatQuery = "INSERT INTO flight_seat_availability (flight_id, seat_number, is_available) VALUES (?, ?, TRUE)";
        auto prepareSeatResult = _connection->prepareStatement(insertSeatQuery);
        if (!prepareSeatResult) {
            if (_logger) _logger->error("Failed to prepare statement for creating seat availability");
            return Failure<Flight>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int seatStmtId = prepareSeatResult.value();

        // Get seat layout from aircraft
        const auto& seatLayout = flight.getAircraft()->getSeatLayout();
        
        // Create seats for each class
        for (const auto& [classCode, count] : seatLayout.getSeatCounts()) {
            int padding = count > 99 ? 3 : 2;
            for (int i = 1; i <= count; i++) {
                std::stringstream ss;
                ss << classCode.getCode() << std::setfill('0') << std::setw(padding) << i;
                std::string seatNumber = ss.str();
                
                auto setFlightIdResult = _connection->setInt(seatStmtId, 1, idResult.value());
                auto setSeatNumberResult = _connection->setString(seatStmtId, 2, seatNumber);
                
                if (!setFlightIdResult || !setSeatNumberResult) {
                    _connection->freeStatement(seatStmtId);
                    if (_logger) _logger->error("Failed to set parameters for creating seat availability");
                    return Failure<Flight>(CoreError("Failed to set parameters", "PARAM_FAILED"));
                }

                auto seatResult = _connection->executeStatement(seatStmtId);
                if (!seatResult) {
                    _connection->freeStatement(seatStmtId);
                    if (_logger) _logger->error("Failed to create seat availability record");
                    return Failure<Flight>(CoreError("Failed to create seat availability", "CREATE_FAILED"));
                }
            }
        }

        _connection->freeStatement(seatStmtId);

        if (_logger) _logger->debug("Successfully created flight with id: " + std::to_string(idResult.value()));
        return Success(newFlight);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error creating flight: " + std::string(e.what()));
        return Failure<Flight>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

/**
 * @brief Cập nhật thông tin chuyến bay
 * 
 * @param flight Đối tượng Flight chứa thông tin cần cập nhật
 * @return Result<Flight> Chuyến bay đã được cập nhật hoặc lỗi
 */
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

/**
 * @brief Xóa chuyến bay theo ID
 * 
 * @param id ID của chuyến bay cần xóa
 * @return Result<bool> True nếu xóa thành công hoặc lỗi
 */
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

/**
 * @brief Ánh xạ dữ liệu từ hàng cơ sở dữ liệu thành đối tượng Flight
 * 
 * @param row Map chứa dữ liệu hàng từ cơ sở dữ liệu
 * @return Flight Đối tượng Flight được tạo từ dữ liệu
 */
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

/**
 * @brief Tìm kiếm chuyến bay theo số hiệu chuyến bay
 * 
 * @param number Số hiệu chuyến bay cần tìm
 * @return Result<Flight> Chuyến bay tìm được hoặc lỗi
 */
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
            return Failure<Flight>(CoreError("Flight not found with flight number: " + number.toString(), "NOT_FOUND"));
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

/**
 * @brief Kiểm tra sự tồn tại của chuyến bay theo số hiệu
 * 
 * @param number Số hiệu chuyến bay cần kiểm tra
 * @return Result<bool> True nếu tồn tại, false nếu không hoặc lỗi
 */
Result<bool> FlightRepository::existsFlight(const FlightNumber& number) {
    try {
        if (_logger) _logger->debug("Checking existence of flight with flight number: " + number.toString());

        auto prepareResult = _connection->prepareStatement(EXISTS_FLIGHT_QUERY);
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

/**
 * @brief Tìm kiếm các chuyến bay theo số seri máy bay
 * 
 * @param serial Số seri của máy bay
 * @return Result<std::vector<Flight>> Danh sách chuyến bay sử dụng máy bay này hoặc lỗi
 */
Result<std::vector<Flight>> FlightRepository::findFlightByAircraft(const AircraftSerial& serial) {
    try {
        if (!_logger) _logger->debug("Finding aircraft exists");

        auto prepareResult = _connection->prepareStatement(FIND_FLIGHT_BY_SERIAL);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding aircraft by serial: " + serial.toString());
            return Failure<std::vector<Flight>>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();
        auto setParamResult = _connection->setString(stmtId, 1, serial.toString());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding flight by serial: " + serial.toString());
            return Failure<std::vector<Flight>>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding flight by serial: " + serial.toString());
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

        if (_logger) _logger->debug("Successfully found " + std::to_string(flights.size()) + " aircraft");
        return Success(flights);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding all flights: " + std::string(e.what()));
        return Failure<std::vector<Flight>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

/**
 * @brief Lấy thông tin tình trạng tất cả ghế ngồi của chuyến bay
 * 
 * @param flight Chuyến bay cần lấy thông tin ghế
 * @return std::map<SeatNumber, bool> Map với key là số ghế và value là trạng thái available
 */
std::map<SeatNumber, bool> FlightRepository::getSeatAvailability(const Flight& flight) const {
    std::map<SeatNumber, bool> seatAvailability;
    try {
        std::string query = "SELECT seat_number, is_available FROM flight_seat_availability WHERE flight_id = ?";
        auto prepareResult = _connection->prepareStatement(query);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for getting seat availability");
            return seatAvailability;
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, flight.getId());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for getting seat availability");
            return seatAvailability;
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for getting seat availability");
            return seatAvailability;
        }

        auto dbResult = std::move(result.value());
        while (dbResult->next().value()) {

            auto seatNumberResult = dbResult->getString("seat_number");
            auto isAvailableResult = dbResult->getInt("is_available");

            if (!seatNumberResult || !isAvailableResult) {
                if (_logger) _logger->warning("Invalid seat availability data");
                continue;
            }

            auto seatNumber = SeatNumber::create(seatNumberResult.value(), flight.getAircraft()->getSeatLayout());
            if (!seatNumber) {
                if (_logger) _logger->warning("Invalid seat number: " + seatNumberResult.value());
                continue;
            }

            seatAvailability[*seatNumber] = isAvailableResult.value() != 0;
        }
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error getting seat availability: " + std::string(e.what()));
    }
    return seatAvailability;
}

/**
 * @brief Đặt trước một ghế ngồi cho chuyến bay
 * 
 * Phương thức này kiểm tra tính khả dụng của ghế trước khi đặt.
 * Chỉ cho phép đặt ghế nếu ghế đang available.
 * 
 * @param flight Chuyến bay cần đặt ghế
 * @param seatNumber Số ghế cần đặt
 * @return Result<bool> True nếu đặt thành công hoặc lỗi
 */
Result<bool> FlightRepository::reserveSeat(const Flight& flight, const SeatNumber& seatNumber) {
    try {
        if (_logger) _logger->debug("Reserving seat " + seatNumber.toString() + " for flight " + flight.getFlightNumber().toString());

        // First check if seat is available
        std::string checkQuery = "SELECT COUNT(*) FROM flight_seat_availability WHERE flight_id = ? AND seat_number = ? AND is_available = TRUE";
        auto checkPrepareResult = _connection->prepareStatement(checkQuery);
        if (!checkPrepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking seat availability");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int checkStmtId = checkPrepareResult.value();

        auto setFlightIdResult = _connection->setInt(checkStmtId, 1, flight.getId());
        auto setSeatNumberResult = _connection->setString(checkStmtId, 2, seatNumber.toString());
        
        if (!setFlightIdResult || !setSeatNumberResult) {
            _connection->freeStatement(checkStmtId);
            if (_logger) _logger->error("Failed to set parameters for checking seat availability");
            return Failure<bool>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto checkResult = _connection->executeQueryStatement(checkStmtId);
        _connection->freeStatement(checkStmtId);

        if (!checkResult) {
            if (_logger) _logger->error("Failed to execute query for checking seat availability");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(checkResult.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->error("Failed to get seat availability count");
            return Failure<bool>(CoreError("Failed to get seat availability count", "COUNT_FAILED"));
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count result");
            return Failure<bool>(CoreError("Failed to get count result", "COUNT_FAILED"));
        }

        if (countResult.value() == 0) {
            if (_logger) _logger->error("Seat is not available");
            return Failure<bool>(CoreError("Seat is not available", "SEAT_NOT_AVAILABLE"));
        }

        // Now try to reserve the seat
        std::string updateQuery = "UPDATE flight_seat_availability SET is_available = FALSE WHERE flight_id = ? AND seat_number = ? AND is_available = TRUE";
        auto updatePrepareResult = _connection->prepareStatement(updateQuery);
        if (!updatePrepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for reserving seat");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int updateStmtId = updatePrepareResult.value();

        setFlightIdResult = _connection->setInt(updateStmtId, 1, flight.getId());
        setSeatNumberResult = _connection->setString(updateStmtId, 2, seatNumber.toString());
        
        if (!setFlightIdResult || !setSeatNumberResult) {
            _connection->freeStatement(updateStmtId);
            if (_logger) _logger->error("Failed to set parameters for reserving seat");
            return Failure<bool>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto updateResult = _connection->executeStatement(updateStmtId);
        _connection->freeStatement(updateStmtId);

        if (!updateResult) {
            if (_logger) _logger->error("Failed to execute update for reserving seat");
            return Failure<bool>(CoreError("Failed to execute update", "UPDATE_FAILED"));
        }

        bool success = updateResult.value() > 0;
        if (!success) {
            if (_logger) _logger->error("Failed to reserve seat - no rows affected");
            return Failure<bool>(CoreError("Failed to reserve seat", "RESERVE_FAILED"));
        }

        if (_logger) _logger->debug("Seat reservation successful");
        return Success(true);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error reserving seat: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

/**
 * @brief Hủy đặt ghế ngồi cho chuyến bay
 * 
 * @param flight Chuyến bay cần hủy đặt ghế
 * @param seatNumber Số ghế cần hủy đặt
 * @return Result<bool> True nếu hủy thành công hoặc lỗi
 */
Result<bool> FlightRepository::releaseSeat(const Flight& flight, const SeatNumber& seatNumber) {
    try {
        if (_logger) _logger->debug("Releasing seat " + seatNumber.toString() + " for flight " + flight.getFlightNumber().toString());

        std::string query = "UPDATE flight_seat_availability SET is_available = TRUE WHERE flight_id = ? AND seat_number = ? AND is_available = FALSE";
        auto prepareResult = _connection->prepareStatement(query);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for releasing seat");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, flight.getId());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set flight_id parameter");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        setParamResult = _connection->setString(stmtId, 2, seatNumber.toString());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set seat_number parameter");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute update for releasing seat");
            return Failure<bool>(CoreError("Failed to execute update", "UPDATE_FAILED"));
        }

        bool success = result.value() > 0;
        if (_logger) _logger->debug("Seat release " + std::string(success ? "successful" : "failed"));
        return Success(success);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error releasing seat: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

/**
 * @brief Lấy danh sách các ghế còn trống cho chuyến bay
 * 
 * @param flight Chuyến bay cần kiểm tra
 * @return Result<std::vector<SeatNumber>> Danh sách ghế còn trống hoặc lỗi
 */
Result<std::vector<SeatNumber>> FlightRepository::getAvailableSeats(const Flight& flight) {
    try {
        if (_logger) _logger->debug("Getting available seats for flight " + flight.getFlightNumber().toString());

        std::string query = "SELECT seat_number FROM flight_seat_availability WHERE flight_id = ? AND is_available = TRUE";
        auto prepareResult = _connection->prepareStatement(query);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for getting available seats");
            return Failure<std::vector<SeatNumber>>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, flight.getId());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for getting available seats");
            return Failure<std::vector<SeatNumber>>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for getting available seats");
            return Failure<std::vector<SeatNumber>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        std::vector<SeatNumber> availableSeats;
        auto dbResult = std::move(result.value());
        while (dbResult->next().value()) {
            auto seatNumberResult = dbResult->getString("seat_number");
            if (!seatNumberResult) {
                if (_logger) _logger->warning("Invalid seat number data");
                continue;
            }

            auto seatNumber = SeatNumber::create(seatNumberResult.value(), flight.getAircraft()->getSeatLayout());
            if (!seatNumber) {
                if (_logger) _logger->warning("Invalid seat number: " + seatNumberResult.value());
                continue;
            }

            availableSeats.push_back(*seatNumber);
        }

        if (_logger) _logger->debug("Found " + std::to_string(availableSeats.size()) + " available seats");
        return Success(availableSeats);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error getting available seats: " + std::string(e.what()));
        return Failure<std::vector<SeatNumber>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

/**
 * @brief Lấy danh sách các ghế đã được đặt cho chuyến bay
 * 
 * @param flight Chuyến bay cần kiểm tra
 * @return Result<std::vector<SeatNumber>> Danh sách ghế đã được đặt hoặc lỗi
 */
Result<std::vector<SeatNumber>> FlightRepository::getReservedSeats(const Flight& flight) {
    try {
        if (_logger) _logger->debug("Getting reserved seats for flight " + flight.getFlightNumber().toString());

        std::string query = "SELECT seat_number FROM flight_seat_availability WHERE flight_id = ? AND is_available = FALSE";
        auto prepareResult = _connection->prepareStatement(query);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for getting reserved seats");
            return Failure<std::vector<SeatNumber>>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, flight.getId());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for getting reserved seats");
            return Failure<std::vector<SeatNumber>>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for getting reserved seats");
            return Failure<std::vector<SeatNumber>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        std::vector<SeatNumber> reservedSeats;
        auto dbResult = std::move(result.value());
        while (dbResult->next().value()) {
            auto seatNumberResult = dbResult->getString("seat_number");
            if (!seatNumberResult) {
                if (_logger) _logger->warning("Invalid seat number data");
                continue;
            }

            auto seatNumber = SeatNumber::create(seatNumberResult.value(), flight.getAircraft()->getSeatLayout());
            if (!seatNumber) {
                if (_logger) _logger->warning("Invalid seat number: " + seatNumberResult.value());
                continue;
            }

            reservedSeats.push_back(*seatNumber);
        }

        if (_logger) _logger->debug("Found " + std::to_string(reservedSeats.size()) + " reserved seats");
        return Success(reservedSeats);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error getting reserved seats: " + std::string(e.what()));
        return Failure<std::vector<SeatNumber>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

/**
 * @brief Kiểm tra xem một ghế có còn trống hay không
 * 
 * @param flight Chuyến bay cần kiểm tra
 * @param seatNumber Số ghế cần kiểm tra
 * @return Result<bool> True nếu ghế còn trống hoặc lỗi
 */
Result<bool> FlightRepository::isSeatAvailable(const Flight& flight, const SeatNumber& seatNumber) {
    if (_logger) _logger->debug("Checking if seat is available for flight: " + flight.getFlightNumber().toString() + ", seat: " + seatNumber.toString());

    auto checkPrepareResult = _connection->prepareStatement(
        "SELECT COUNT(*) FROM flight_seat_availability WHERE flight_id = ? AND seat_number = ? AND is_available = TRUE"
    );

    if (!checkPrepareResult) {
        if (_logger) _logger->error("Failed to prepare statement for checking seat availability");
        return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
    }
    int checkStmtId = checkPrepareResult.value();

    auto setFlightIdResult = _connection->setInt(checkStmtId, 1, flight.getId());
    auto setSeatNumberResult = _connection->setString(checkStmtId, 2, seatNumber.toString());
    
    if (!setFlightIdResult || !setSeatNumberResult) {
        _connection->freeStatement(checkStmtId);
        if (_logger) _logger->error("Failed to set parameters for checking seat availability");
        return Failure<bool>(CoreError("Failed to set parameters", "PARAM_FAILED"));
    }

    auto checkResult = _connection->executeQueryStatement(checkStmtId);
    _connection->freeStatement(checkStmtId);

    if (!checkResult) {
        if (_logger) _logger->error("Failed to execute query for checking seat availability");
        return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
    }

    auto dbResult = std::move(checkResult.value());
    if (!dbResult->next().value()) {
        if (_logger) _logger->error("Failed to get seat availability count");
        return Failure<bool>(CoreError("Failed to get seat availability count", "COUNT_FAILED"));
    }

    auto countResult = dbResult->getInt(0);
    if (!countResult) {
        if (_logger) _logger->error("Failed to get count result");
        return Failure<bool>(CoreError("Failed to get count result", "COUNT_FAILED"));
    }

    return Success(countResult.value() > 0);
}