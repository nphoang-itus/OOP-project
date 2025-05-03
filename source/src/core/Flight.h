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
#include <ctime>

class Flight {
private:
    int _id;
    std::string _no;
    std::string _name;
    std::string _from;
    std::string _destination;
    std::tm _departureTime;
    std::tm _leaveTime;
    std::tm _arrivalTime;
    double _amount;
    char _availability;

public:
    // Constructor
    Flight();
    Flight( const std::string& no, const std::string& name, const std::string& from, const std::string& destination,
            const std::tm& departureTime, const std::tm& leaveTime, const std::tm& arrivalTime,
            const double& amount, const char& availability);

    // Getter
    int getId() const;
    std::string getNo() const;
    std::string getName() const;
    std::string getFrom() const;
    std::string getDestination() const;
    std::tm getDepartureTime() const;
    std::tm getLeaveTime() const;
    std::tm getArrivalTime() const;
    double getAmount() const;
    char getAvailability() const;

    // Setter
    void setId(const int& id);
    void setNo(const std::string& no);
    void setName(const std::string& name);
    void setFrom(const std::string& from);
    void setDestination(const std::string& destination);
    void setDepartureTime(const std::tm& departureTime);
    void setLeaveTime(const std::tm& leaveTime);
    void setArrivalTime(const std::tm& arrivalTime);
    void setAmount(const double& amount);
    void setAvailability(const char& availability);
};

#endif