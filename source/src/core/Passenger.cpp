#include "Passenger.h"

Passenger::Passenger() : _id(0) {}
Passenger::Passenger(const std::string& name, const std::string& phone, const std::string& passport, const std::string& address)
                :    _id(0), _name(name), _phone(phone), _passport(passport), _address(address) {}

// Getter
int Passenger::getId() const {
    return _id;
}

std::string Passenger::getName() const {
    return _name;
}

std::string Passenger::getPhone() const {
    return _phone;
}

std::string Passenger::getPassport() const {
    return _passport;
}

std::string Passenger::getAddress() const {
    return _address;
}


// Setter
void Passenger::setId(const int& id) {
    _id = id;
}

void Passenger::setName(const std::string& name) {
    _name = name;
}

void Passenger::setPhone(const std::string& phone) {
    _phone = phone;
}

void Passenger::setPassport(const std::string& passport) {
    _passport = passport;
}

void Passenger::setAddress(const std::string& address) {
    _address = address;
}