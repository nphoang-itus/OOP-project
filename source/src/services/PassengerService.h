/**
 * @file PassengerService.h
 * @brief Dịch vụ quản lý hành khách trong hệ thống đặt vé máy bay
 * @author OOP Project Team
 * @date 2024-2025
 */

#ifndef PASSENGER_SERVICE_H
#define PASSENGER_SERVICE_H

#include "../core/entities/Passenger.h"
#include "../repositories/MySQLRepository/PassengerRepository.h"
#include "../repositories/MySQLRepository/TicketRepository.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "../utils/Logger.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @class PassengerService
 * @brief Lớp dịch vụ chịu trách nhiệm quản lý các thao tác liên quan đến hành khách
 * 
 * Lớp này cung cấp các chức năng CRUD cơ bản và các thao tác nghiệp vụ
 * cho việc quản lý hành khách, bao gồm kiểm tra đặt chỗ đang hoạt động,
 * xác thực quy tắc nghiệp vụ.
 */
class PassengerService {
private:
    std::shared_ptr<PassengerRepository> _passengerRepository;  ///< Repository để truy cập dữ liệu hành khách
    std::shared_ptr<TicketRepository> _ticketRepository;        ///< Repository để truy cập dữ liệu vé
    std::shared_ptr<FlightRepository> _flightRepository;        ///< Repository để truy cập dữ liệu chuyến bay
    std::shared_ptr<Logger> _logger;                            ///< Logger để ghi log hệ thống

    /**
     * @brief Lấy thông tin hành khách theo ID
     * @param id ID của hành khách
     * @return Result<Passenger> Kết quả chứa thông tin hành khách hoặc lỗi
     */
    Result<Passenger> getPassengerById(int id);
    
    /**
     * @brief Kiểm tra hành khách có tồn tại theo ID
     * @param id ID của hành khách cần kiểm tra
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> existsById(const int& id);
    
    /**
     * @brief Xóa hành khách theo ID
     * @param id ID của hành khách cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deleteById(const int& id);

public:
    /**
     * @brief Constructor khởi tạo PassengerService với các dependency
     * @param passengerRepository Repository quản lý dữ liệu hành khách
     * @param ticketRepository Repository quản lý dữ liệu vé
     * @param flightRepository Repository quản lý dữ liệu chuyến bay
     * @param logger Logger để ghi log (tùy chọn, sẽ sử dụng singleton nếu null)
     */
    PassengerService(
        std::shared_ptr<PassengerRepository> passengerRepository,
        std::shared_ptr<TicketRepository> ticketRepository,
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<Logger> logger = nullptr
    ) : _passengerRepository(std::move(passengerRepository))
      , _ticketRepository(std::move(ticketRepository))
      , _flightRepository(std::move(flightRepository))
      , _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    }

    // Core CRUD operations
    /**
     * @brief Lấy thông tin hành khách theo số hộ chiếu
     * @param passport Số hộ chiếu của hành khách
     * @return Result<Passenger> Kết quả chứa thông tin hành khách hoặc lỗi
     */
    Result<Passenger> getPassenger(const PassportNumber& passport);
    
    /**
     * @brief Lấy danh sách tất cả hành khách
     * @return Result<std::vector<Passenger>> Danh sách tất cả hành khách hoặc lỗi
     */
    Result<std::vector<Passenger>> getAllPassengers();
    
    /**
     * @brief Kiểm tra hành khách có tồn tại theo số hộ chiếu
     * @param passport Số hộ chiếu của hành khách
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> passengerExists(const PassportNumber& passport);
    
    /**
     * @brief Tạo mới một hành khách
     * @param passenger Đối tượng hành khách cần tạo
     * @return Result<Passenger> Hành khách đã được tạo hoặc lỗi
     */
    Result<Passenger> createPassenger(const Passenger& passenger);
    
    /**
     * @brief Cập nhật thông tin hành khách
     * @param passenger Đối tượng hành khách với thông tin mới
     * @return Result<Passenger> Hành khách đã được cập nhật hoặc lỗi
     */
    Result<Passenger> updatePassenger(const Passenger& passenger);
    
    /**
     * @brief Xóa hành khách theo số hộ chiếu
     * @param passport Số hộ chiếu của hành khách cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deletePassenger(const PassportNumber& passport);
    
    // Passenger validation and business rules
    /**
     * @brief Kiểm tra có thể xóa hành khách hay không
     * @param passport Số hộ chiếu của hành khách
     * @return Result<bool> true nếu có thể xóa, false nếu không thể
     */
    Result<bool> canDeletePassenger(const PassportNumber& passport);
    
    /**
     * @brief Kiểm tra hành khách có đặt chỗ đang hoạt động
     * @param passport Số hộ chiếu của hành khách
     * @return Result<bool> true nếu có đặt chỗ đang hoạt động, false nếu không
     */
    Result<bool> hasActiveBookings(const PassportNumber& passport);
    
    /**
     * @brief Lấy tổng số chuyến bay mà hành khách đã đặt
     * @param passport Số hộ chiếu của hành khách
     * @return Result<int> Tổng số chuyến bay hoặc lỗi
     */
    Result<int> getTotalFlightCount(const PassportNumber& passport);
};

#endif // PASSENGER_SERVICE_H