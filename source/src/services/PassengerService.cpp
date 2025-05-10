/**
 * @file PassengerService.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Triển khai lớp PassengerService quản lý các chức năng nghiệp vụ liên quan đến hành khách.
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "PassengerService.h"
#include <stdexcept>
#include <regex>

PassengerService::PassengerService(std::shared_ptr<PassengerRepository> passengerRepository)
    : _passengerRepository(std::move(passengerRepository)), _logger(Logger::getInstance()) {
    
    _logger->debug("Creating PassengerService instance");
    
    if (!_passengerRepository) {
        _logger->error("PassengerRepository is null in PassengerService constructor");
        throw std::invalid_argument("PassengerRepository cannot be null");
    }
}

// bool PassengerService::validatePassenger(const Passenger& passenger) {
//     // Kiểm tra tên hành khách
//     if (passenger.getName().empty()) {
//         _logger->error("Invalid passenger data: Name is empty");
//         return false;
//     }
    
//     // Kiểm tra số hộ chiếu
//     if (passenger.getPassport().empty()) {
//         _logger->error("Invalid passenger data: Passport is empty");
//         return false;
//     }
    
//     // Kiểm tra định dạng số điện thoại (cơ bản)
//     if (!passenger.getPhone().empty()) {
//         std::regex phoneRegex("^[0-9+\\-\\(\\) ]+$");
//         if (!std::regex_match(passenger.getPhone(), phoneRegex)) {
//             _logger->error("Invalid passenger data: Phone number format is invalid");
//             return false;
//         }
//     }
    
//     return true;
// }

std::vector<Passenger> PassengerService::findAll() {
    _logger->debug("PassengerService::findAll - Retrieving all passengers");
    return _passengerRepository->findAll();
}

std::optional<Passenger> PassengerService::findById(const int& id) {
    _logger->debug("PassengerService::findById - Finding passenger with ID: " + std::to_string(id));
    return _passengerRepository->findById(id);
}

bool PassengerService::save(Passenger& passenger) {
    _logger->debug("PassengerService::save - Saving passenger with passport: " + passenger.getPassport());
    
    // Validate passenger data
    // if (!validatePassenger(passenger)) {
    //     return false;
    // }
    
    // Check if passport already exists
    if (isPassportExists(passenger.getPassport())) {
        _logger->warning("Passport " + passenger.getPassport() + " already exists in the system");
        return false;
    }
    
    return _passengerRepository->save(passenger);
}

bool PassengerService::remove(const int& id) {
    _logger->debug("PassengerService::remove - Removing passenger with ID: " + std::to_string(id));
    return _passengerRepository->remove(id);
}

std::optional<Passenger> PassengerService::findByPassport(const std::string& passport) {
    _logger->debug("PassengerService::findByPassport - Finding passenger with passport: " + passport);
    return _passengerRepository->findByPassport(passport);
}

std::vector<Passenger> PassengerService::findByName(const std::string& name) {
    _logger->debug("PassengerService::findByName - Finding passengers with name: " + name);
    return _passengerRepository->findByName(name);
}

bool PassengerService::update(const Passenger& passenger) {
    _logger->debug("PassengerService::update - Updating passenger with ID: " + std::to_string(passenger.getId()));
    
    // // Validate passenger data
    // if (!validatePassenger(passenger)) {
    //     return false;
    // }
    
    // Check if the updated passport belongs to another passenger
    auto existingPassenger = _passengerRepository->findByPassport(passenger.getPassport());
    if (existingPassenger && existingPassenger->getId() != passenger.getId()) {
        _logger->warning("Cannot update: Passport " + passenger.getPassport() + " already assigned to another passenger");
        return false;
    }
    
    return _passengerRepository->update(passenger);
}

bool PassengerService::isPassportExists(const std::string& passport) {
    _logger->debug("PassengerService::isPassportExists - Checking if passport exists: " + passport);
    
    auto passenger = _passengerRepository->findByPassport(passport);
    return passenger.has_value();
}