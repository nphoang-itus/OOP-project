#include "Flight.h"

// Constructor
Flight::Flight() : _id(0), _amount(0.0), _availability(false) {}

Flight::Flight( const std::string& no, const std::string& name, const std::string& from, const std::string& destination,
                const std::string& departureTime, const std::string& leaveTime, const std::string& arrivalTime, const double& amount, const bool& availability)
            :   _id(0), _no(no), _name(name), _from(from), _destination(destination),
                _departureTime(departureTime), _leaveTime(leaveTime), _arrivalTime(arrivalTime), _amount(amount), _availability(availability) {}

// Getter
int Flight::getId() const {
    return _id;
}

std::string Flight::getNo() const {
    return _no;
}

std::string Flight::getName() const {
    return _name;
}

std::string Flight::getFrom() const {
    return _from;
}

std::string Flight::getDestination() const {
    return _destination;
}

std::string Flight::getDepartureTime() const {
    return _departureTime;
}

std::string Flight::getLeaveTime() const {
    return _leaveTime;
}

std::string Flight::getArrivalTime() const {
    return _arrivalTime;
}

double Flight::getAmount() const {
    return _amount;
}

bool Flight::getAvailability() const {
    return _availability;
}

// Setter
void Flight::setId(const int& id) {
    _id = id;
}

void Flight::setNo(const std::string& no) {
    _no = no;
}

void Flight::setName(const std::string& name) {
    _name = name;
}

void Flight::setFrom(const std::string& from) {
    _from = from;
}

void Flight::setDestination(const std::string& destination) {
    _destination = destination;
}

void Flight::setDepartureTime(const std::string& departureTime) {
    _departureTime = departureTime;
}

void Flight::setLeaveTime(const std::string& leaveTime) {
    _leaveTime = leaveTime;
}

void Flight::setArrivalTime(const std::string& arrivalTime) {
    _arrivalTime = arrivalTime;
}

void Flight::setAmount(const double& amount) {
    _amount = amount;
}

void Flight::setAvailability(const bool& availability) {
    _availability = availability;
}