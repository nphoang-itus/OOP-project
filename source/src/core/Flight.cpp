/**
 * @file Flight.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @version 0.1
 * @date 2025-04-30
 * 
 * @brief Triển khai các phương thức của lớp Flight mô tả thông tin chuyến bay.
 */

#include "Flight.h"

// Constructor
Flight::Flight() : _id(0), _amount(0.0), _availability('A') {}

Flight::Flight( const std::string& no, const std::string& name, const std::string& from, const std::string& destination,
                const std::tm& departureTime, const std::tm& leaveTime, const std::tm& arrivalTime, const double& amount,
                const char& availability)
            :   _id(0), _no(no), _name(name), _from(from), _destination(destination),
                _departureTime(departureTime), _leaveTime(leaveTime), _arrivalTime(arrivalTime), _amount(amount), _availability(availability) {}

// Getter
int Flight::getId() const { return _id; }
std::string Flight::getNo() const { return _no; }
std::string Flight::getName() const { return _name; }
std::string Flight::getFrom() const { return _from; }
std::string Flight::getDestination() const { return _destination; }
std::tm Flight::getDepartureTime() const { return _departureTime; }
std::tm Flight::getLeaveTime() const { return _leaveTime; }
std::tm Flight::getArrivalTime() const { return _arrivalTime; }
double Flight::getAmount() const { return _amount; }
char Flight::getAvailability() const { return _availability; }

// Setter
void Flight::setId(const int& id) { _id = id; }
void Flight::setNo(const std::string& no) { _no = no; }
void Flight::setName(const std::string& name) { _name = name; }
void Flight::setFrom(const std::string& from) { _from = from; }
void Flight::setDestination(const std::string& destination) { _destination = destination; }
void Flight::setDepartureTime(const std::tm& departureTime) { _departureTime = departureTime; }
void Flight::setLeaveTime(const std::tm& leaveTime) { _leaveTime = leaveTime; }
void Flight::setArrivalTime(const std::tm& arrivalTime) { _arrivalTime = arrivalTime; }
void Flight::setAmount(const double& amount) { _amount = amount; }
void Flight::setAvailability(const char& availability) { _availability = availability; }