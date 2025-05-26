#include "TicketRepository.h"
#include "../../core/exceptions/Result.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include <sstream>
#include <map>

// using namespace Tables;

Result<Ticket> TicketRepository::findById(const int& id) {
    try {
        if (_logger) _logger->debug("Finding ticket by id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::FIND_BY_ID_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding ticket by id: " + std::to_string(id));
            return Failure<Ticket>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding ticket by id: " + std::to_string(id));
            return Failure<Ticket>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding ticket by id: " + std::to_string(id));
            return Failure<Ticket>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("Ticket not found with id: " + std::to_string(id));
            return Failure<Ticket>(CoreError("Ticket not found with id: " + std::to_string(id), "NOT_FOUND"));
        }

        // Get ticket data
        auto idResult = dbResult->getInt(Tables::Ticket::ColumnName[Tables::Ticket::ID]);
        auto ticketNumberResult = dbResult->getString(Tables::Ticket::ColumnName[Tables::Ticket::TICKET_NUMBER]);
        auto seatNumberResult = dbResult->getString(Tables::Ticket::ColumnName[Tables::Ticket::SEAT_NUMBER]);
        auto priceResult = dbResult->getDouble(Tables::Ticket::ColumnName[Tables::Ticket::PRICE]);
        auto currencyResult = dbResult->getString(Tables::Ticket::ColumnName[Tables::Ticket::CURRENCY]);
        auto statusResult = dbResult->getString(Tables::Ticket::ColumnName[Tables::Ticket::STATUS]);
        auto passengerIdResult = dbResult->getInt(Tables::Ticket::ColumnName[Tables::Ticket::PASSENGER_ID]);
        auto flightIdResult = dbResult->getInt(Tables::Ticket::ColumnName[Tables::Ticket::FLIGHT_ID]);

        if (!idResult || !ticketNumberResult || !seatNumberResult || !priceResult || 
            !currencyResult || !statusResult || !passengerIdResult || !flightIdResult) {
            if (_logger) _logger->error("Failed to get ticket data for id: " + std::to_string(id));
            return Failure<Ticket>(CoreError("Failed to get ticket data", "DATA_ERROR"));
        }

        // Get passenger
        auto passengerResult = _passengerRepository->findById(passengerIdResult.value());
        if (!passengerResult) {
            if (_logger) _logger->error("Failed to find passenger for ticket id: " + std::to_string(id));
            return Failure<Ticket>(passengerResult.error());
        }
        auto passenger = passengerResult.value();

        // Get flight
        auto flightResult = _flightRepository->findById(flightIdResult.value());
        if (!flightResult) {
            if (_logger) _logger->error("Failed to find flight for ticket id: " + std::to_string(id));
            return Failure<Ticket>(flightResult.error());
        }
        auto flight = flightResult.value();

        // Create ticket
        auto ticketNumber = TicketNumber::create(ticketNumberResult.value()).value();
        if (!ticketNumberResult) {
            if (_logger) _logger->error("Failed to create ticket number");
            return Failure<Ticket>(ticketNumberResult.error());
        }

        auto seatNumber = SeatNumber::create(seatNumberResult.value(), flight.getAircraft()->getSeatLayout()).value();
        if (!seatNumberResult) {
            if (_logger) _logger->error("Failed to create seat number");
            return Failure<Ticket>(seatNumberResult.error());
        }

        auto price = Price::create(priceResult.value(), currencyResult.value()).value();
        if (!priceResult) {
            if (_logger) _logger->error("Failed to create price");
            return Failure<Ticket>(priceResult.error());
        }

        auto ticketResult = Ticket::create(ticketNumber, 
                                         std::make_shared<Passenger>(passenger),
                                         std::make_shared<Flight>(flight),
                                         seatNumber,
                                         price);
        if (!ticketResult) {
            if (_logger) _logger->error("Failed to create ticket");
            return Failure<Ticket>(ticketResult.error());
        }
        auto ticket = ticketResult.value();
        ticket.setId(idResult.value());
        ticket.setStatus(TicketStatusUtil::fromString(statusResult.value()));

        if (_logger) _logger->debug("Successfully found ticket with id: " + std::to_string(id));
        return Success(ticket);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding ticket by id: " + std::string(e.what()));
        return Failure<Ticket>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<std::vector<Ticket>> TicketRepository::findAll() {
    try {
        if (_logger) _logger->debug("Finding all tickets");

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::FIND_ALL_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding all tickets");
            return Failure<std::vector<Ticket>>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding all tickets");
            return Failure<std::vector<Ticket>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        std::vector<Ticket> tickets;

        while (dbResult->next().value()) {
            auto idResult = dbResult->getInt(Tables::Ticket::ColumnName[Tables::Ticket::ID]);
            if (!idResult) {
                if (_logger) _logger->error("Failed to get ticket id");
                return Failure<std::vector<Ticket>>(CoreError("Failed to get ticket id", "DATA_ERROR"));
            }

            auto ticketResult = findById(idResult.value());
            if (!ticketResult) {
                return Failure<std::vector<Ticket>>(ticketResult.error());
            }

            tickets.push_back(ticketResult.value());
        }

        if (_logger) _logger->debug("Successfully found " + std::to_string(tickets.size()) + " tickets");
        return Success(tickets);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding all tickets: " + std::string(e.what()));
        return Failure<std::vector<Ticket>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> TicketRepository::exists(const int& id) {
    try {
        if (_logger) _logger->debug("Checking if ticket exists with id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::EXISTS_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking ticket existence");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for checking ticket existence");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for checking ticket existence");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("No result returned when checking ticket existence");
            return Failure<bool>(CoreError("No result returned", "QUERY_FAILED"));
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count result");
            return Failure<bool>(CoreError("Failed to get count result", "DATA_ERROR"));
        }

        bool exists = countResult.value() > 0;
        if (_logger) _logger->debug("Ticket existence check result: " + std::to_string(exists));
        return Success(exists);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error checking ticket existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<size_t> TicketRepository::count() {
    try {
        if (_logger) _logger->debug("Counting total tickets");

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::COUNT_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for counting tickets");
            return Failure<size_t>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for counting tickets");
            return Failure<size_t>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("No result returned when counting tickets");
            return Failure<size_t>(CoreError("No result returned", "QUERY_FAILED"));
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count result");
            return Failure<size_t>(CoreError("Failed to get count result", "DATA_ERROR"));
        }

        if (_logger) _logger->debug("Successfully counted tickets: " + std::to_string(countResult.value()));
        return Success(static_cast<size_t>(countResult.value()));
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error counting tickets: " + std::string(e.what()));
        return Failure<size_t>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Ticket> TicketRepository::create(const Ticket& ticket) {
    try {
        if (_logger) _logger->debug("Creating new ticket");

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::INSERT_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for creating ticket");
            return Failure<Ticket>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        // Set parameters
        auto setTicketNumberResult = _connection->setString(stmtId, 1, ticket.getTicketNumber().getValue());
        auto setPassengerIdResult = _connection->setInt(stmtId, 2, ticket.getPassenger()->getId());
        auto setFlightIdResult = _connection->setInt(stmtId, 3, ticket.getFlight()->getId());
        auto setSeatNumberResult = _connection->setString(stmtId, 4, ticket.getSeatNumber().getValue());
        auto setPriceResult = _connection->setDouble(stmtId, 5, ticket.getPrice().getAmount());
        auto setCurrencyResult = _connection->setString(stmtId, 6, ticket.getPrice().getCurrency());
        auto setStatusResult = _connection->setString(stmtId, 7, TicketStatusUtil::toString(ticket.getStatus()));

        if (!setTicketNumberResult || !setPassengerIdResult || !setFlightIdResult || !setSeatNumberResult || 
            !setPriceResult || !setCurrencyResult || !setStatusResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameters for creating ticket");
            return Failure<Ticket>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute update for creating ticket");
            return Failure<Ticket>(CoreError("Failed to execute update", "UPDATE_FAILED"));
        }

        auto updateResult = std::move(result.value());
        if (updateResult != 1) {
            if (_logger) _logger->error("Unexpected number of rows affected: " + std::to_string(updateResult));
            return Failure<Ticket>(CoreError("Unexpected number of rows affected", "UPDATE_FAILED"));
        }

        auto lastIdResult = _connection->getLastInsertId();
        if (!lastIdResult) {
            if (_logger) _logger->error("Failed to get last insert id");
            return Failure<Ticket>(CoreError("Failed to get last insert id", "ID_ERROR"));
        }

        auto createdTicket = ticket;
        createdTicket.setId(lastIdResult.value());

        if (_logger) _logger->debug("Successfully created ticket with id: " + std::to_string(lastIdResult.value()));
        return Success(createdTicket);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error creating ticket: " + std::string(e.what()));
        return Failure<Ticket>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Ticket> TicketRepository::update(const Ticket& ticket) {
    try {
        if (_logger) _logger->debug("Updating ticket with id: " + std::to_string(ticket.getId()));

        auto existsResult = exists(ticket.getId());
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check ticket existence");
            return Failure<Ticket>(CoreError("Failed to check ticket existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Ticket not found with id: " + std::to_string(ticket.getId()));
            return Failure<Ticket>(CoreError("Ticket not found with id: " + std::to_string(ticket.getId()), "DB_ERROR"));
        }

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::UPDATE_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for updating ticket");
            return Failure<Ticket>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        // Set parameters
        auto setTicketNumberResult = _connection->setString(stmtId, 1, ticket.getTicketNumber().getValue());
        auto setPassengerIdResult = _connection->setInt(stmtId, 2, ticket.getPassenger()->getId());
        auto setFlightIdResult = _connection->setInt(stmtId, 3, ticket.getFlight()->getId());
        auto setSeatNumberResult = _connection->setString(stmtId, 4, ticket.getSeatNumber().getValue());
        auto setPriceResult = _connection->setDouble(stmtId, 5, ticket.getPrice().getAmount());
        auto setCurrencyResult = _connection->setString(stmtId, 6, ticket.getPrice().getCurrency());
        auto setStatusResult = _connection->setString(stmtId, 7, TicketStatusUtil::toString(ticket.getStatus()));
        auto setIdResult = _connection->setInt(stmtId, 8, ticket.getId());

        if (!setTicketNumberResult || !setPassengerIdResult || !setFlightIdResult || !setSeatNumberResult || 
            !setPriceResult || !setCurrencyResult || !setStatusResult || !setIdResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameters for updating ticket");
            return Failure<Ticket>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute update for updating ticket");
            return Failure<Ticket>(CoreError("Failed to execute update", "UPDATE_FAILED"));
        }

        auto updateResult = std::move(result.value());
        if (updateResult != 1) {
            if (_logger) _logger->error("Unexpected number of rows affected: " + std::to_string(updateResult));
            return Failure<Ticket>(CoreError("Unexpected number of rows affected", "UPDATE_FAILED"));
        }

        if (_logger) _logger->debug("Successfully updated ticket with id: " + std::to_string(ticket.getId()));
        return Success(ticket);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error updating ticket: " + std::string(e.what()));
        return Failure<Ticket>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> TicketRepository::deleteById(const int& id) {
    try {
        if (_logger) _logger->debug("Deleting ticket with id: " + std::to_string(id));

        auto existsResult = exists(id);
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check ticket existence");
            return Failure<bool>(CoreError("Failed to check ticket existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Ticket not found with id: " + std::to_string(id));
            return Failure<bool>(CoreError("Ticket not found with id: " + std::to_string(id), "DB_ERROR"));
        }

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::DELETE_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for deleting ticket");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for deleting ticket");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute update for deleting ticket");
            return Failure<bool>(CoreError("Failed to execute update", "UPDATE_FAILED"));
        }

        auto updateResult = std::move(result.value());
        if (updateResult != 1) {
            if (_logger) _logger->error("Unexpected number of rows affected: " + std::to_string(updateResult));
            return Failure<bool>(CoreError("Unexpected number of rows affected", "UPDATE_FAILED"));
        }

        if (_logger) _logger->debug("Successfully deleted ticket with id: " + std::to_string(id));
        return Success(true);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error deleting ticket: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Ticket> TicketRepository::findByTicketNumber(const TicketNumber& ticketNumber) {
    try {
        if (_logger) _logger->debug("Finding ticket by ticket number: " + ticketNumber.getValue());

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::FIND_BY_TICKET_NUMBER_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding ticket by ticket number");
            return Failure<Ticket>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, ticketNumber.getValue());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding ticket by ticket number");
            return Failure<Ticket>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding ticket by ticket number");
            return Failure<Ticket>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("Ticket not found with ticket number: " + ticketNumber.getValue());
            return Failure<Ticket>(CoreError("Ticket not found with ticket number: " + ticketNumber.getValue(), "NOT_FOUND"));
        }

        auto idResult = dbResult->getInt(Tables::Ticket::ColumnName[Tables::Ticket::ID]);
        if (!idResult) {
            if (_logger) _logger->error("Failed to get ticket id");
            return Failure<Ticket>(CoreError("Failed to get ticket id", "DATA_ERROR"));
        }

        return findById(idResult.value());
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding ticket by ticket number: " + std::string(e.what()));
        return Failure<Ticket>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> TicketRepository::existsTicket(const TicketNumber& ticketNumber) {
    try {
        if (_logger) _logger->debug("Checking if ticket exists with ticket number: " + ticketNumber.getValue());

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::EXISTS_TICKET_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking ticket existence");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, ticketNumber.getValue());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for checking ticket existence");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for checking ticket existence");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("No result returned when checking ticket existence");
            return Failure<bool>(CoreError("No result returned", "QUERY_FAILED"));
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count result");
            return Failure<bool>(CoreError("Failed to get count result", "DATA_ERROR"));
        }

        bool exists = countResult.value() > 0;
        if (_logger) _logger->debug("Ticket existence check result: " + std::to_string(exists));
        return Success(exists);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error checking ticket existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<std::vector<Ticket>> TicketRepository::findByPassengerId(int passengerId) {
    try {
        if (_logger) _logger->debug("Finding tickets by passenger id: " + std::to_string(passengerId));

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::FIND_BY_PASSENGER_ID_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding tickets by passenger id");
            return Failure<std::vector<Ticket>>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, passengerId);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding tickets by passenger id");
            return Failure<std::vector<Ticket>>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding tickets by passenger id");
            return Failure<std::vector<Ticket>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        std::vector<Ticket> tickets;

        while (dbResult->next().value()) {
            auto idResult = dbResult->getInt(Tables::Ticket::ColumnName[Tables::Ticket::ID]);
            if (!idResult) {
                if (_logger) _logger->error("Failed to get ticket id");
                return Failure<std::vector<Ticket>>(CoreError("Failed to get ticket id", "DATA_ERROR"));
            }

            auto ticketResult = findById(idResult.value());
            if (!ticketResult) {
                return Failure<std::vector<Ticket>>(ticketResult.error());
            }

            tickets.push_back(ticketResult.value());
        }

        if (_logger) _logger->debug("Successfully found " + std::to_string(tickets.size()) + " tickets for passenger id: " + std::to_string(passengerId));
        return Success(tickets);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding tickets by passenger id: " + std::string(e.what()));
        return Failure<std::vector<Ticket>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<std::vector<Ticket>> TicketRepository::findBySerialNumber(const AircraftSerial& serial) {
    try {
        if (_logger) _logger->debug("Finding tickets by aircraft serial number: " + serial.value());

        auto prepareResult = _connection->prepareStatement(Tables::Ticket::FIND_BY_SERIAL_NUMBER_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding tickets by serial number");
            return Failure<std::vector<Ticket>>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, serial.value());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding tickets by serial number");
            return Failure<std::vector<Ticket>>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding tickets by serial number");
            return Failure<std::vector<Ticket>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        std::vector<Ticket> tickets;

        while (dbResult->next().value()) {
            auto idResult = dbResult->getInt(Tables::Ticket::ColumnName[Tables::Ticket::ID]);
            if (!idResult) {
                if (_logger) _logger->error("Failed to get ticket id");
                return Failure<std::vector<Ticket>>(CoreError("Failed to get ticket id", "DATA_ERROR"));
            }

            auto ticketResult = findById(idResult.value());
            if (!ticketResult) {
                return Failure<std::vector<Ticket>>(ticketResult.error());
            }

            tickets.push_back(ticketResult.value());
        }

        if (_logger) _logger->debug("Successfully found " + std::to_string(tickets.size()) + " tickets for aircraft serial number: " + serial.value());
        return Success(tickets);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding tickets by serial number: " + std::string(e.what()));
        return Failure<std::vector<Ticket>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}
