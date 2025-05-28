#include "PassengerRepository.h"
#include "../../core/exceptions/Result.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include <sstream>
#include <map>

using namespace Tables::Passenger;

Result<Passenger> PassengerRepository::findById(const int& id) {
    try {
        if (_logger) _logger->debug("Finding passenger by id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(FIND_BY_ID_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding passenger by id: " + std::to_string(id));
            return Failure<Passenger>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding passenger by id: " + std::to_string(id));
            return Failure<Passenger>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding passenger by id: " + std::to_string(id));
            return Failure<Passenger>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("Passenger not found with id: " + std::to_string(id));
            return Failure<Passenger>(CoreError("Passenger not found with id: " + std::to_string(id), "NOT_FOUND"));
        }

        auto idResult = dbResult->getInt(ColumnName[ID]);
        auto passportResult = dbResult->getString(ColumnName[PASSPORT_NUMBER]);
        auto nameResult = dbResult->getString(ColumnName[NAME]);
        auto emailResult = dbResult->getString(ColumnName[EMAIL]);
        auto phoneResult = dbResult->getString(ColumnName[PHONE]);
        auto addressResult = dbResult->getString(ColumnName[ADDRESS]);

        if (!idResult || !passportResult || !nameResult || !emailResult || !phoneResult || !addressResult) {
            if (_logger) _logger->error("Failed to get passenger data for id: " + std::to_string(id));
            return Failure<Passenger>(CoreError("Failed to get passenger data", "DATA_ERROR"));
        }

        // Create contact info string
        std::stringstream contactInfoStr;
        contactInfoStr << emailResult.value() << "|" << phoneResult.value();
        if (!addressResult.value().empty()) {
            contactInfoStr << "|" << addressResult.value();
        }

        auto passport = PassportNumber::create(passportResult.value()).value();
        auto contactInfo = ContactInfo::create(contactInfoStr.str()).value();
        auto passenger = Passenger::create(nameResult.value(), contactInfo, passport).value();
        passenger.setId(idResult.value());

        if (_logger) _logger->debug("Successfully found passenger with id: " + std::to_string(id));
        return Success(passenger);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding passenger by id: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<std::vector<Passenger>> PassengerRepository::findAll() {
    try {
        if (_logger) _logger->debug("Finding all passengers");

        auto result = _connection->executeQuery(FIND_ALL_QUERY);
        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding all passengers");
            return Failure<std::vector<Passenger>>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        std::vector<Passenger> passengers;
        auto dbResult = std::move(result.value());
        
        while (dbResult->next().value()) {
            auto idResult = dbResult->getInt(ColumnName[ID]);
            if (!idResult) break;  // Break if we can't get the ID, indicating end of results
            
            auto passportResult = dbResult->getString(ColumnName[PASSPORT_NUMBER]);
            auto nameResult = dbResult->getString(ColumnName[NAME]);
            auto emailResult = dbResult->getString(ColumnName[EMAIL]);
            auto phoneResult = dbResult->getString(ColumnName[PHONE]);
            auto addressResult = dbResult->getString(ColumnName[ADDRESS]);

            if (!passportResult || !nameResult || !emailResult || !phoneResult || !addressResult) {
                if (_logger) _logger->warning("Skipping invalid passenger data");
                continue;
            }

            // Create contact info string
            std::stringstream contactInfoStr;
            contactInfoStr << emailResult.value() << "|" << phoneResult.value();
            if (!addressResult.value().empty()) {
                contactInfoStr << "|" << addressResult.value();
            }

            auto passport = PassportNumber::create(passportResult.value());
            if (!passport) {
                if (_logger) _logger->warning("Invalid passport number: " + passportResult.value());
                continue;
            }

            auto contactInfo = ContactInfo::create(contactInfoStr.str());
            if (!contactInfo) {
                if (_logger) _logger->warning("Invalid contact info: " + contactInfoStr.str());
                continue;
            }

            auto passenger = Passenger::create(nameResult.value(), *contactInfo, *passport);
            if (!passenger) {
                if (_logger) _logger->warning("Failed to create passenger");
                continue;
            }

            passenger->setId(idResult.value());
            passengers.push_back(*passenger);
        }

        if (_logger) _logger->debug("Successfully found " + std::to_string(passengers.size()) + " passengers");
        return Success(passengers);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding all passengers: " + std::string(e.what()));
        return Failure<std::vector<Passenger>>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> PassengerRepository::exists(const int& id) {
    try {
        if (_logger) _logger->debug("Checking existence of passenger with id: " + std::to_string(id));

        auto prepareResult = _connection->prepareStatement(EXISTS_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking passenger existence");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for checking passenger existence");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for checking passenger existence");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("Passenger not found with id: " + std::to_string(id));
            return Success(false);
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count for passenger existence check");
            return Failure<bool>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        bool exists = countResult.value() > 0;
        if (_logger) _logger->debug("Passenger " + std::to_string(id) + " " + (exists ? "exists" : "does not exist"));
        return Success(exists);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error checking passenger existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<size_t> PassengerRepository::count() {
    try {
        if (_logger) _logger->debug("Counting total passengers");

        auto result = _connection->executeQuery(COUNT_QUERY);
        if (!result) {
            if (_logger) _logger->error("Failed to execute query for counting passengers");
            return Failure<size_t>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("No passengers found");
            return Success(0);
        }

        auto countResult = dbResult->getInt(0);
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count");
            return Failure<size_t>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        size_t count = static_cast<size_t>(countResult.value());
        if (_logger) _logger->debug("Total passenger count: " + std::to_string(count));
        return Success(count);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error counting passengers: " + std::string(e.what()));
        return Failure<size_t>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Passenger> PassengerRepository::create(const Passenger& passenger) {
    try {
        if (_logger) _logger->debug("Creating new passenger");

        // Start transaction
        _connection->beginTransaction();

        // Insert passenger
        auto prepareResult = _connection->prepareStatement(INSERT_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for creating passenger");
            return Failure<Passenger>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        // Split contact info into email, phone, and address
        std::string email = passenger.getContactInfo().getEmail();
        std::string phone = passenger.getContactInfo().getPhone();
        std::string address = passenger.getContactInfo().getAddress();

        auto setPassportResult = _connection->setString(stmtId, 1, passenger.getPassport().toString());
        auto setNameResult = _connection->setString(stmtId, 2, passenger.getName());
        auto setEmailResult = _connection->setString(stmtId, 3, email);
        auto setPhoneResult = _connection->setString(stmtId, 4, phone);
        auto setAddressResult = _connection->setString(stmtId, 5, address);

        if (!setPassportResult || !setNameResult || !setEmailResult || !setPhoneResult || !setAddressResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameters for creating passenger");
            return Failure<Passenger>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for creating passenger");
            return Failure<Passenger>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        auto idResult = _connection->getLastInsertId();
        if (!idResult) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to get last insert id for passenger");
            return Failure<Passenger>(CoreError("Failed to get last insert id", "GET_ID_FAILED"));
        }

        _connection->commitTransaction();

        auto newPassenger = passenger;
        newPassenger.setId(idResult.value());
        if (_logger) _logger->debug("Successfully created passenger with id: " + std::to_string(idResult.value()));
        return Success(newPassenger);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error creating passenger: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Passenger> PassengerRepository::update(const Passenger& passenger) {
    try {
        if (_logger) _logger->debug("Updating passenger with id: " + std::to_string(passenger.getId()));

        // First check if passenger exists
        auto existsResult = exists(passenger.getId());
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check passenger existence");
            return Failure<Passenger>(CoreError("Failed to check passenger existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Passenger not found with id: " + std::to_string(passenger.getId()));
            return Failure<Passenger>(CoreError("Passenger not found with id: " + std::to_string(passenger.getId()), "DB_ERROR"));
        }

        // Start transaction
        _connection->beginTransaction();

        // Update passenger
        auto prepareResult = _connection->prepareStatement(UPDATE_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for updating passenger");
            return Failure<Passenger>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        // Split contact info into email, phone, and address
        std::string email = passenger.getContactInfo().getEmail();
        std::string phone = passenger.getContactInfo().getPhone();
        std::string address = passenger.getContactInfo().getAddress();

        auto setPassportResult = _connection->setString(stmtId, 1, passenger.getPassport().toString());
        auto setNameResult = _connection->setString(stmtId, 2, passenger.getName());
        auto setEmailResult = _connection->setString(stmtId, 3, email);
        auto setPhoneResult = _connection->setString(stmtId, 4, phone);
        auto setAddressResult = _connection->setString(stmtId, 5, address);
        auto setIdResult = _connection->setInt(stmtId, 6, passenger.getId());

        if (!setPassportResult || !setNameResult || !setEmailResult || !setPhoneResult || !setAddressResult || !setIdResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameters for updating passenger");
            return Failure<Passenger>(CoreError("Failed to set parameters", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for updating passenger");
            return Failure<Passenger>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        _connection->commitTransaction();

        if (_logger) _logger->debug("Successfully updated passenger with id: " + std::to_string(passenger.getId()));
        return Success(passenger);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error updating passenger: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> PassengerRepository::deleteById(const int& id) {
    try {
        if (_logger) _logger->debug("Deleting passenger with id: " + std::to_string(id));

        // First check if passenger exists
        auto existsResult = exists(id);
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check passenger existence");
            return Failure<bool>(CoreError("Failed to check passenger existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Passenger not found with id: " + std::to_string(id));
            return Failure<bool>(CoreError("Passenger not found with id: " + std::to_string(id), "DB_ERROR"));
        }

        // Start transaction
        _connection->beginTransaction();

        // Delete passenger
        auto prepareResult = _connection->prepareStatement(DELETE_QUERY);
        if (!prepareResult) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to prepare statement for deleting passenger");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setInt(stmtId, 1, id);
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameter for deleting passenger");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for deleting passenger");
            return Failure<bool>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        _connection->commitTransaction();

        if (_logger) _logger->debug("Successfully deleted passenger with id: " + std::to_string(id));
        return Success(true);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error deleting passenger: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<Passenger> PassengerRepository::findByPassportNumber(const PassportNumber& passport) {
    try {
        if (_logger) _logger->debug("Finding passenger by passport number: " + passport.toString());

        auto prepareResult = _connection->prepareStatement(FIND_BY_PASSPORT_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for finding passenger by passport number: " + passport.toString());
            return Failure<Passenger>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, passport.toString());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for finding passenger by passport number: " + passport.toString());
            return Failure<Passenger>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for finding passenger by passport number: " + passport.toString());
            return Failure<Passenger>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next().value()) {
            if (_logger) _logger->warning("Passenger not found with passport number: " + passport.toString());
            return Failure<Passenger>(CoreError("Passenger not found with passport number: " + passport.toString(), "NOT_FOUND"));
        }

        auto idResult = dbResult->getInt(ColumnName[ID]);
        auto passportResult = dbResult->getString(ColumnName[PASSPORT_NUMBER]);
        auto nameResult = dbResult->getString(ColumnName[NAME]);
        auto emailResult = dbResult->getString(ColumnName[EMAIL]);
        auto phoneResult = dbResult->getString(ColumnName[PHONE]);
        auto addressResult = dbResult->getString(ColumnName[ADDRESS]);

        if (!idResult || !passportResult || !nameResult || !emailResult || !phoneResult || !addressResult) {
            if (_logger) _logger->error("Failed to get passenger data for passport number: " + passport.toString());
            return Failure<Passenger>(CoreError("Failed to get passenger data", "DATA_ERROR"));
        }

        // Create contact info string
        std::stringstream contactInfoStr;
        contactInfoStr << emailResult.value() << "|" << phoneResult.value();
        if (!addressResult.value().empty()) {
            contactInfoStr << "|" << addressResult.value();
        }

        auto passport = PassportNumber::create(passportResult.value()).value();
        auto contactInfo = ContactInfo::create(contactInfoStr.str()).value();
        auto passenger = Passenger::create(nameResult.value(), contactInfo, passport).value();
        passenger.setId(idResult.value());

        if (_logger) _logger->debug("Successfully found passenger with passport number: " + passport.toString());
        return Success(passenger);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error finding passenger by passport number: " + std::string(e.what()));
        return Failure<Passenger>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> PassengerRepository::existsPassport(const PassportNumber& passport) {
    try {
        if (_logger) _logger->debug("Checking existence of passenger with passport number: " + passport.toString());

        auto prepareResult = _connection->prepareStatement(EXISTS_PASSPORT_QUERY);
        if (!prepareResult) {
            if (_logger) _logger->error("Failed to prepare statement for checking passenger existence");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, passport.toString());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            if (_logger) _logger->error("Failed to set parameter for checking passenger existence");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeQueryStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            if (_logger) _logger->error("Failed to execute query for checking passenger existence");
            return Failure<bool>(CoreError("Failed to execute query", "QUERY_FAILED"));
        }

        auto dbResult = std::move(result.value());
        if (!dbResult->next()) {
            if (_logger) _logger->debug("Passenger not found with passport number: " + passport.toString());
            return Success(false);
        }

        auto countResult = dbResult->getInt(0);  // COUNT(*) is always the first column (index 0)
        if (!countResult) {
            if (_logger) _logger->error("Failed to get count for passenger existence check");
            return Failure<bool>(CoreError("Failed to get count", "COUNT_FAILED"));
        }

        bool exists = countResult.value() > 0;
        if (_logger) _logger->debug("Passenger " + passport.toString() + " " + (exists ? "exists" : "does not exist"));
        return Success(exists);
    } catch (const std::exception& e) {
        if (_logger) _logger->error("Error checking passenger existence: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}

Result<bool> PassengerRepository::deleteByPassportNumber(const PassportNumber& passport) {
    try {
        if (_logger) _logger->debug("Deleting passenger with passport: " + passport.toString());

        // First check if passenger exists
        auto existsResult = existsPassport(passport);
        if (!existsResult) {
            if (_logger) _logger->error("Failed to check passenger existence");
            return Failure<bool>(CoreError("Failed to check passenger existence", "DB_ERROR"));
        }
        if (!existsResult.value()) {
            if (_logger) _logger->error("Passenger not found with passport: " + passport.toString());
            return Failure<bool>(CoreError("Passenger not found with passport: " + passport.toString(), "DB_ERROR"));
        }

        // Start transaction
        _connection->beginTransaction();

        // Delete passenger
        auto prepareResult = _connection->prepareStatement(DELETE_BY_PASSPORT_QUERY);
        if (!prepareResult) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to prepare statement for deleting passenger");
            return Failure<bool>(CoreError("Failed to prepare statement", "PREPARE_FAILED"));
        }
        int stmtId = prepareResult.value();

        auto setParamResult = _connection->setString(stmtId, 1, passport.toString());
        if (!setParamResult) {
            _connection->freeStatement(stmtId);
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to set parameter for deleting passenger");
            return Failure<bool>(CoreError("Failed to set parameter", "PARAM_FAILED"));
        }

        auto result = _connection->executeStatement(stmtId);
        _connection->freeStatement(stmtId);

        if (!result) {
            _connection->rollbackTransaction();
            if (_logger) _logger->error("Failed to execute statement for deleting passenger");
            return Failure<bool>(CoreError("Failed to execute statement", "EXECUTE_FAILED"));
        }

        _connection->commitTransaction();

        if (_logger) _logger->debug("Successfully deleted passenger with passport: " + passport.toString());
        return Success(true);
    } catch (const std::exception& e) {
        _connection->rollbackTransaction();
        if (_logger) _logger->error("Error deleting passenger: " + std::string(e.what()));
        return Failure<bool>(CoreError("Database error: " + std::string(e.what()), "DB_ERROR"));
    }
}