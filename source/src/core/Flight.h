/**
 * @file Flight.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>

class Flight {
private:
    int _id;
    std::string _no;
    std::string _name;
    std::string _from;
    std::string _destination;
    std::string _departureTime;
    std::string _leaveTime;
    std::string _arrivalTime;
    double _amount;
    char _availability;

public:
    // Constructor
    Flight();
    Flight( const std::string& no, const std::string& name, const std::string& from, const std::string& destination,
            const std::string& departureTime, const std::string& leaveTime, const std::string& arrivalTime, const double& amount, const char& availability);

    // Getter
    int getId() const;
    std::string getNo() const;
    std::string getName() const;
    std::string getFrom() const;
    std::string getDestination() const;
    std::string getDepartureTime() const;
    std::string getLeaveTime() const;
    std::string getArrivalTime() const;
    double getAmount() const;
    char getAvailability() const;

    // Setter
    void setId(const int& id);
    void setNo(const std::string& no);
    void setName(const std::string& name);
    void setFrom(const std::string& from);
    void setDestination(const std::string& destination);
    void setDepartureTime(const std::string& departureTime);
    void setLeaveTime(const std::string& leaveTime);
    void setArrivalTime(const std::string& arrivalTime);
    void setAmount(const double& amount);
    void setAvailability(const char& availability);
};

#endif