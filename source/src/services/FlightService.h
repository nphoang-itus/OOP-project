/**
 * @file FlightService.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLIGHT_SERVICE_H
#define FLIGHT_SERVICE_H

#include "ServiceInterface.h"
#include "../core/Flight.h"
#include <../repositories/FlightRepository.h>
#include <../utils/Logger.h>

#include <vector>
#include <optional>
#include <string>
#include <time.h>
#include <memory>

class IFlightService : public IService<Flight> {
public:
    virtual std::vector<Flight> findAvailableFlights() = 0;
    virtual std::optional<Flight> findByFlightNo(const std::string& flightNo) = 0;
    virtual bool update(const Flight& flight) = 0;
    virtual std::vector<Flight> findByRoute(const std::string& from, const std::string& destination) = 0;
    virtual std::vector<Flight> findByDateRange(const std::tm& startDate, const std::tm& endDate) = 0;
};

class FlightService : public IFlightService {
private:
    std::shared_ptr<FlightRepository> _flightRepository; ///< Repository để truy xuất dữ liệu chuyến bay
    std::shared_ptr<Logger> _logger; ///< Logger để ghi log

public:
    explicit FlightService(std::shared_ptr<FlightRepository> flightRepository);
    
    // Triển khai các phương thức từ IService
    std::vector<Flight> findAll() override;
    std::optional<Flight> findById(const int& id) override;
    bool save(Flight& flight) override;
    bool remove(const int& id) override;
    
    // Triển khai các phương thức từ IFlightService
    std::vector<Flight> findAvailableFlights() override;
    std::optional<Flight> findByFlightNo(const std::string& flightNo) override;
    bool update(const Flight& flight) override;
    std::vector<Flight> findByRoute(const std::string& from, const std::string& destination) override;
    std::vector<Flight> findByDateRange(const std::tm& startDate, const std::tm& endDate) override;
    
    /**
     * @brief Kiểm tra xem một chuyến bay có đang có sẵn để đặt vé không.
     * @param flightNo Số hiệu chuyến bay cần kiểm tra.
     * @return true nếu chuyến bay có sẵn, false nếu không.
     */
    bool isFlightAvailable(const std::string& flightNo);
    
    /**
     * @brief Cập nhật trạng thái chuyến bay.
     * @param flightNo Số hiệu chuyến bay cần cập nhật.
     * @param availability Trạng thái mới ('A' cho Available, 'N' cho Not Available)
     * @return true nếu cập nhật thành công, false nếu thất bại.
     */
    bool updateAvailability(const std::string& flightNo, char availability);
};

#endif
