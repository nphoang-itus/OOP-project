/**
 * @file PassengerService.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef PASSENGER_SERVICE_H
#define PASSENGER_SERVICE_H

#include "ServiceInterface.h"
#include "../core/Passenger.h"
#include "../repositories/PassengerRepository.h"
#include "../utils/Logger.h"

#include <optional>
#include <vector>
#include <string>
#include <memory>

class IPassengerService : public IService<Passenger> {
public:
    virtual std::optional<Passenger> findByPassport(const std::string& passport) = 0;
    virtual std::vector<Passenger> findByName(const std::string& name) = 0;
    virtual bool update(const Passenger& passenger) = 0;
    virtual bool isPassportExists(const std::string& passport) = 0;
};

class PassengerService : public IPassengerService {
private:
    std::shared_ptr<PassengerRepository> _passengerRepository; ///< Repository để truy xuất dữ liệu hành khách
    std::shared_ptr<Logger> _logger; ///< Logger để ghi log

    // bool validatePassenger(const Passenger& passenger);
public:
    explicit PassengerService(std::shared_ptr<PassengerRepository> passengerRepository);
    std::vector<Passenger> findAll() override;
    std::optional<Passenger> findById(const int& id) override;
    bool save(Passenger& passenger) override;
    bool remove(const int& id) override;

    std::optional<Passenger> findByPassport(const std::string& passport) override;
    std::vector<Passenger> findByName(const std::string& name) override;
    bool update(const Passenger& passenger) override;
    bool isPassportExists(const std::string& passport) override;
};

#endif