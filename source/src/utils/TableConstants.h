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
    }

    namespace Flight {
        constexpr const char* NAME_TABLE = "flight_tb";

        enum ColumnNumber {
            ID = 0,
            FLIGHT_NUMBER,
            ROUTE,
            SCHEDULE,
            AIRCRAFT_ID,
            STATUS
        };

        constexpr const char* ColumnName[] {
            "f_id",
            "f_number",
            "f_route",
            "f_schedule",
            "f_aircraftId",
            "f_status"
        };

        inline std::string getOrderedSelectClause() {
            return std::string("SELECT f.*, a.* FROM ") + NAME_TABLE + " f " +
                   "JOIN " + Aircraft::NAME_TABLE + " a ON f." + ColumnName[AIRCRAFT_ID] + 
                   " = a." + Aircraft::ColumnName[Aircraft::ID];
        }

        // SQL Queries
        const std::string FIND_BY_ID_QUERY = getOrderedSelectClause() + " WHERE f." + ColumnName[ID] + " = ?";
        const std::string FIND_ALL_QUERY = getOrderedSelectClause();
        const std::string EXISTS_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string COUNT_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE);
        const std::string INSERT_QUERY = "INSERT INTO " + std::string(NAME_TABLE) + " (" + 
            ColumnName[FLIGHT_NUMBER] + ", " + 
            ColumnName[ROUTE] + ", " + 
            ColumnName[SCHEDULE] + ", " + 
            ColumnName[AIRCRAFT_ID] + ", " + 
            ColumnName[STATUS] + 
            ") VALUES (?, ?, ?, ?, ?)";
        const std::string UPDATE_QUERY = "UPDATE " + std::string(NAME_TABLE) + " SET " + 
            ColumnName[FLIGHT_NUMBER] + " = ?, " + 
            ColumnName[ROUTE] + " = ?, " + 
            ColumnName[SCHEDULE] + " = ?, " + 
            ColumnName[AIRCRAFT_ID] + " = ?, " + 
            ColumnName[STATUS] + " = ? " + 
            "WHERE " + ColumnName[ID] + " = ?";
        const std::string DELETE_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
    }

    namespace Passenger {
        constexpr const char* NAME_TABLE = "passenger_tb";

        enum ColumnNumber {
            ID = 0,
            NAME,
            CONTACT_INFO,
            PASSPORT
        };

        constexpr const char* ColumnName[] {
            "p_id",
            "p_name",
            "p_contactInfo",
            "p_passport"
        };

        inline std::string getOrderedSelectClause() {
            return std::string("SELECT ") + 
                   ColumnName[ID] + ", " +
                   ColumnName[NAME] + ", " +
                   ColumnName[CONTACT_INFO] + ", " +
                   ColumnName[PASSPORT] + 
                   " FROM " + NAME_TABLE;
        }

        // SQL Queries
        const std::string FIND_BY_ID_QUERY = getOrderedSelectClause() + " WHERE " + ColumnName[ID] + " = ?";
        const std::string FIND_ALL_QUERY = getOrderedSelectClause();
        const std::string EXISTS_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
        const std::string COUNT_QUERY = "SELECT COUNT(*) FROM " + std::string(NAME_TABLE);
        const std::string INSERT_QUERY = "INSERT INTO " + std::string(NAME_TABLE) + " (" + 
            ColumnName[NAME] + ", " + 
            ColumnName[CONTACT_INFO] + ", " + 
            ColumnName[PASSPORT] + 
            ") VALUES (?, ?, ?)";
        const std::string UPDATE_QUERY = "UPDATE " + std::string(NAME_TABLE) + " SET " + 
            ColumnName[NAME] + " = ?, " + 
            ColumnName[CONTACT_INFO] + " = ?, " + 
            ColumnName[PASSPORT] + " = ? " + 
            "WHERE " + ColumnName[ID] + " = ?";
        const std::string DELETE_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
    }

    namespace Ticket {
        constexpr const char* NAME_TABLE = "ticket_tb";

        enum ColumnNumber {
            ID = 0,
            TICKET_NUMBER,
            PASSENGER_ID,
            FLIGHT_ID,
            SEAT_NUMBER,
            PRICE,
            STATUS
        };

        constexpr const char* ColumnName[] {
            "t_id",
            "t_number",
            "t_passengerId",
            "t_flightId",
            "t_seatNumber",
            "t_price",
            "t_status"
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
            ColumnName[STATUS] + 
            ") VALUES (?, ?, ?, ?, ?, ?)";
        const std::string UPDATE_QUERY = "UPDATE " + std::string(NAME_TABLE) + " SET " + 
            ColumnName[TICKET_NUMBER] + " = ?, " + 
            ColumnName[PASSENGER_ID] + " = ?, " + 
            ColumnName[FLIGHT_ID] + " = ?, " + 
            ColumnName[SEAT_NUMBER] + " = ?, " + 
            ColumnName[PRICE] + " = ?, " + 
            ColumnName[STATUS] + " = ? " + 
            "WHERE " + ColumnName[ID] + " = ?";
        const std::string DELETE_QUERY = "DELETE FROM " + std::string(NAME_TABLE) + " WHERE " + ColumnName[ID] + " = ?";
    }
}

#endif