/**
 * @file TableConstants.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef TABLE_CONSTANTS_H
#define TABLE_CONSTANTS_H

#include <string>
#include <sstream>
#include <format>

namespace Tables {
    namespace Aircraft {
        constexpr const char* NAME_TABLE = "aircraft";

        enum ColumnNumber {
            ID = 0,
            SERIAL,
            MODEL,
            ECONOMY_SEATS,
            BUSINESS_SEATS,
            FIRST_SEATS
        };

        constexpr const char* ColumnName[] {
            "id",
            "serial_number",
            "model",
            "economy_seats",
            "business_seats",
            "first_seats"
        };

        static std::string getOrderedSelectClause() {
            return "SELECT id, serial_number, model, economy_seats, business_seats, first_seats "
                   "FROM aircraft";
        }

        // SQL Queries
        const std::string FIND_BY_ID_QUERY = "SELECT id, serial_number, model, economy_seats, business_seats, first_seats "
                                            "FROM aircraft "
                                            "WHERE id = ?";
        const std::string FIND_ALL_QUERY = getOrderedSelectClause();
        const std::string EXISTS_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE) + " WHERE id = ?";
        const std::string COUNT_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE);
        const std::string INSERT_QUERY = "INSERT INTO " + std::string(NAME_TABLE) + " (" + 
            ColumnName[SERIAL] + ", " + 
            ColumnName[MODEL] + ", " +
            ColumnName[ECONOMY_SEATS] + ", " +
            ColumnName[BUSINESS_SEATS] + ", " +
            ColumnName[FIRST_SEATS] + 
            ") VALUES (?, ?, ?, ?, ?)";
        const std::string UPDATE_QUERY = "UPDATE " + std::string(NAME_TABLE) + " SET " + 
            ColumnName[SERIAL] + " = ?, " + 
            ColumnName[MODEL] + " = ?, " +
            ColumnName[ECONOMY_SEATS] + " = ?, " +
            ColumnName[BUSINESS_SEATS] + " = ?, " +
            ColumnName[FIRST_SEATS] + " = ? " +
            "WHERE " + ColumnName[ID] + " = ?";
        const std::string DELETE_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";

        // Additional queries for seat layout
        const std::string INSERT_SEAT_LAYOUT_QUERY = "INSERT INTO aircraft_seat_layout (aircraft_id, seat_class_code, seat_count) VALUES (?, ?, ?)";
        const std::string DELETE_SEAT_LAYOUT_QUERY = "DELETE FROM aircraft_seat_layout WHERE aircraft_id = ?";
        const std::string FIND_BY_SERIAL_NUMBER = std::format (
            "SELECT * FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[SERIAL]
        );
        const std::string EXISTS_SERIAL_QUERY = std::format (
            "SELECT COUNT(*) FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[SERIAL]
        );

        const std::string DELETE_BY_SERIAL_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[SERIAL] + " = ?";
    }

    namespace Flight {
        constexpr const char* NAME_TABLE = "flight";

        enum ColumnNumber {
            ID = 0,
            FLIGHT_NUMBER,
            DEPARTURE_CODE,
            DEPARTURE_NAME,
            ARRIVAL_CODE,
            ARRIVAL_NAME,
            AIRCRAFT_ID,
            DEPARTURE_TIME,
            ARRIVAL_TIME,
            STATUS
        };

        constexpr const char* ColumnName[] {
            "id",
            "flight_number",
            "departure_code",
            "departure_name",
            "arrival_code",
            "arrival_name",
            "aircraft_id",
            "departure_time",
            "arrival_time",
            "status"
        };

        inline std::string getOrderedSelectClause() {
            return std::string("SELECT f.*, a.serial_number, a.model, a.economy_seats, a.business_seats, a.first_seats "
                   "FROM ") + NAME_TABLE + " f " +
                   "JOIN aircraft a ON f." + ColumnName[AIRCRAFT_ID] + " = a.id";
        }

        // SQL Queries
        const std::string FIND_BY_ID_QUERY = getOrderedSelectClause() + " WHERE f." + ColumnName[ID] + " = ?";
        const std::string FIND_ALL_QUERY = getOrderedSelectClause();
        const std::string EXISTS_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string COUNT_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE);
        const std::string INSERT_QUERY = "INSERT INTO " + std::string(NAME_TABLE) + " (" + 
            ColumnName[FLIGHT_NUMBER] + ", " + 
            ColumnName[DEPARTURE_CODE] + ", " + 
            ColumnName[DEPARTURE_NAME] + ", " + 
            ColumnName[ARRIVAL_CODE] + ", " + 
            ColumnName[ARRIVAL_NAME] + ", " + 
            ColumnName[AIRCRAFT_ID] + ", " + 
            ColumnName[DEPARTURE_TIME] + ", " + 
            ColumnName[ARRIVAL_TIME] + ", " + 
            ColumnName[STATUS] + 
            ") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        const std::string UPDATE_QUERY = "UPDATE " + std::string(NAME_TABLE) + " SET " + 
            ColumnName[FLIGHT_NUMBER] + " = ?, " + 
            ColumnName[DEPARTURE_CODE] + " = ?, " + 
            ColumnName[DEPARTURE_NAME] + " = ?, " + 
            ColumnName[ARRIVAL_CODE] + " = ?, " + 
            ColumnName[ARRIVAL_NAME] + " = ?, " + 
            ColumnName[AIRCRAFT_ID] + " = ?, " + 
            ColumnName[DEPARTURE_TIME] + " = ?, " + 
            ColumnName[ARRIVAL_TIME] + " = ?, " + 
            ColumnName[STATUS] + " = ? " + 
            "WHERE " + ColumnName[ID] + " = ?";
        const std::string DELETE_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string FIND_BY_NUMBER_QUERY = getOrderedSelectClause() + " WHERE f." + ColumnName[FLIGHT_NUMBER] + " = ?";
        const std::string EXISTS_FLIGHT_QUERY = std::format (
            "SELECT COUNT(*) FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[FLIGHT_NUMBER]
        );
        const std::string FIND_AIRCRAFT = getOrderedSelectClause() + " WHERE a." + Aircraft::ColumnName[Aircraft::SERIAL] + " = ?";
    }

    namespace Passenger {
        constexpr const char* NAME_TABLE = "passenger";

        enum ColumnNumber {
            ID = 0,
            PASSPORT_NUMBER,
            NAME,
            EMAIL,
            PHONE,
            ADDRESS
        };

        constexpr const char* ColumnName[] {
            "id",
            "passport_number",
            "name",
            "email",
            "phone",
            "address"
        };

        inline std::string getOrderedSelectClause() {
            return std::string("SELECT ") + 
                   ColumnName[ID] + ", " +
                   ColumnName[PASSPORT_NUMBER] + ", " +
                   ColumnName[NAME] + ", " +
                   ColumnName[EMAIL] + ", " +
                   ColumnName[PHONE] + ", " +
                   ColumnName[ADDRESS] + 
                   " FROM " + NAME_TABLE;
        }

        // SQL Queries
        const std::string FIND_BY_ID_QUERY = getOrderedSelectClause() + " WHERE " + ColumnName[ID] + " = ?";
        const std::string FIND_ALL_QUERY = getOrderedSelectClause();
        const std::string EXISTS_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string COUNT_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE);
        const std::string INSERT_QUERY = "INSERT INTO " + std::string(NAME_TABLE) + " (" + 
            ColumnName[PASSPORT_NUMBER] + ", " + 
            ColumnName[NAME] + ", " + 
            ColumnName[EMAIL] + ", " + 
            ColumnName[PHONE] + ", " + 
            ColumnName[ADDRESS] + 
            ") VALUES (?, ?, ?, ?, ?)";
        const std::string UPDATE_QUERY = "UPDATE " + std::string(NAME_TABLE) + " SET " + 
            ColumnName[PASSPORT_NUMBER] + " = ?, " + 
            ColumnName[NAME] + " = ?, " + 
            ColumnName[EMAIL] + " = ?, " + 
            ColumnName[PHONE] + " = ?, " + 
            ColumnName[ADDRESS] + " = ? " + 
            "WHERE " + ColumnName[ID] + " = ?";
        const std::string DELETE_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string FIND_BY_PASSPORT_QUERY = std::format (
            "SELECT * FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[PASSPORT_NUMBER]
        );
        const std::string EXISTS_PASSPORT_QUERY = std::format (
            "SELECT COUNT(*) FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[PASSPORT_NUMBER]
        );
    }

    namespace Ticket {
        constexpr const char* NAME_TABLE = "ticket";

        enum ColumnNumber {
            ID = 0,
            TICKET_NUMBER,
            PASSENGER_ID,
            FLIGHT_ID,
            SEAT_NUMBER,
            PRICE,
            CURRENCY,
            STATUS
        };

        constexpr const char* ColumnName[] {
            "id",
            "ticket_number",
            "passenger_id",
            "flight_id",
            "seat_number",
            "price",
            "currency",
            "status"
        };

        inline std::string getOrderedSelectClause() {
            return std::string("SELECT t.*, p.*, f.*, a.* FROM ") + NAME_TABLE + " t " +
                   "JOIN " + Passenger::NAME_TABLE + " p ON t." + ColumnName[PASSENGER_ID] + 
                   " = p." + Passenger::ColumnName[Passenger::ID] + " " +
                   "JOIN " + Flight::NAME_TABLE + " f ON t." + ColumnName[FLIGHT_ID] + 
                   " = f." + Flight::ColumnName[Flight::ID] + " " +
                   "JOIN " + Aircraft::NAME_TABLE + " a ON f." + Flight::ColumnName[Flight::AIRCRAFT_ID] + 
                   " = a." + Aircraft::ColumnName[Aircraft::ID];
        }

        // SQL Queries
        const std::string FIND_BY_ID_QUERY = getOrderedSelectClause() + " WHERE t." + ColumnName[ID] + " = ?";
        const std::string FIND_ALL_QUERY = getOrderedSelectClause();
        const std::string EXISTS_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string COUNT_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE);
        const std::string INSERT_QUERY = "INSERT INTO " + std::string(NAME_TABLE) + " (" + 
            ColumnName[TICKET_NUMBER] + ", " + 
            ColumnName[PASSENGER_ID] + ", " + 
            ColumnName[FLIGHT_ID] + ", " + 
            ColumnName[SEAT_NUMBER] + ", " + 
            ColumnName[PRICE] + ", " + 
            ColumnName[CURRENCY] + ", " + 
            ColumnName[STATUS] + 
            ") VALUES (?, ?, ?, ?, ?, ?, ?)";
        const std::string UPDATE_QUERY = "UPDATE " + std::string(NAME_TABLE) + " SET " + 
            ColumnName[TICKET_NUMBER] + " = ?, " + 
            ColumnName[PASSENGER_ID] + " = ?, " + 
            ColumnName[FLIGHT_ID] + " = ?, " + 
            ColumnName[SEAT_NUMBER] + " = ?, " + 
            ColumnName[PRICE] + " = ?, " + 
            ColumnName[CURRENCY] + " = ?, " + 
            ColumnName[STATUS] + " = ? " + 
            "WHERE " + ColumnName[ID] + " = ?";
        const std::string DELETE_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string FIND_BY_TICKET_NUMBER_QUERY = std::format (
            "SELECT * FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[TICKET_NUMBER]
        );
        const std::string EXISTS_TICKET_QUERY = std::format (
            "SELECT COUNT(*) FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[TICKET_NUMBER]
        );
        const std::string FIND_BY_PASSENGER_ID_QUERY = std::format (
            "SELECT * FROM {} WHERE {} = ?",
            NAME_TABLE, ColumnName[PASSENGER_ID]
        );
        const std::string FIND_BY_SERIAL_NUMBER_QUERY = std::format (
            "SELECT t.* FROM {} t "
            "JOIN {} f ON t.{} = f.{} "
            "JOIN {} a ON f.{} = a.{} "
            "WHERE a.{} = ?",
            NAME_TABLE, Flight::NAME_TABLE, ColumnName[FLIGHT_ID], Flight::ColumnName[Flight::ID],
            Aircraft::NAME_TABLE, Flight::ColumnName[Flight::AIRCRAFT_ID], Aircraft::ColumnName[Aircraft::ID],
            Aircraft::ColumnName[Aircraft::SERIAL]
        );
    }
}

#endif