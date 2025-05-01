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
}

#endif