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

// Định nghĩa Flight Table
namespace Tables {
    namespace Flight {
        constexpr const char* NAME_TABLE = "flight_tb";

        enum ColumnNumber {
            ID = 0,
            NO,
            NAME,
            FROM,
            DESTINATION,
            DEPARTURE_TIME,
            LEAVE_TIME,
            ARRIVAL_TIME,
            AMOUNT,
            AVAILABILITY
        };

        constexpr const char* ColumnName[] {
            "f_id",
            "f_no",
            "f_name",
            "f_from",
            "f_destination",
            "f_departureTime",
            "f_leaveTime",
            "f_arrivalTime",
            "f_amount",
            "f_availability"
        };

        inline std::string getOrderedSelectClause() {
            return std::string("SELECT ") + 
                   ColumnName[ID] + ", " +
                   ColumnName[NO] + ", " +
                   ColumnName[NAME] + ", " +
                   ColumnName[FROM] + ", " +
                   ColumnName[DESTINATION] + ", " +
                   ColumnName[DEPARTURE_TIME] + ", " +
                   ColumnName[LEAVE_TIME] + ", " +
                   ColumnName[ARRIVAL_TIME] + ", " +
                   ColumnName[AMOUNT] + ", " +
                   ColumnName[AVAILABILITY] + 
                   " FROM " + NAME_TABLE;
        }
    }

    namespace Passenger {
        constexpr const char* NAME_TABLE = "passenger_tb";

        enum ColumnNumber {
            ID = 0,
            NAME,
            PHONE,
            PASSPORT,
            ADDRESS
        };

        constexpr const char* ColumnName[] {
            "p_id",
            "p_name",
            "p_phone",
            "p_passport",
            "p_address"
        };

        inline std::string getOrderedSelectClause() {
            return std::string("SELECT ") + 
                   ColumnName[ID] + ", " +
                   ColumnName[NAME] + ", " +
                   ColumnName[PHONE] + ", " +
                   ColumnName[PASSPORT] + ", " +
                   ColumnName[ADDRESS] +
                   " FROM " + NAME_TABLE;
        }
    };

    namespace Reservation {
        constexpr const char* NAME_TABLE = "reservation_tb";

        enum ColumnNumber {
            ID = 0,
            TICKET_NO,
            FLIGHT,
            PASSENGER
        };

        constexpr const char* ColumnName[] {
            "r_id",
            "r_ticketNo",
            "r_flightNo",
            "r_passportPassenger"
        };

        inline std::string getOrderedSelectClause() {
            std::string query = std::format(
                "SELECT r.*, p.*, f.* FROM {} r "
                "JOIN {} p ON r.{} = p.{} "
                "JOIN {} f ON r.{} = f.{} ",
                NAME_TABLE,
                Passenger::NAME_TABLE , ColumnName[PASSENGER], Passenger::ColumnName[Passenger::PASSPORT],
                Flight::NAME_TABLE, ColumnName[FLIGHT], Flight::ColumnName[Flight::NO]
            );
            return query;
        }
    };
}

#endif