/**
 * @file Passenger.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

class Passenger {
private:
    int _id;
    std::string _name;
    std::string _phone;
    std::string _passport;
    std::string _address;

public:
    // Constructor
    Passenger();
    Passenger(const std::string& name, const std::string& phone, const std::string& passport, const std::string& address);

    // Getter
    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getPassport() const;
    std::string getAddress() const;

    // Setter
    void setId(const int& id);
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setPassport(const std::string& passport);
    void setAddress(const std::string& address);
};

#endif