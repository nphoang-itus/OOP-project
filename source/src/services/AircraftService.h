/**
 * @file AircraftService.h
 * @brief Dịch vụ quản lý máy bay trong hệ thống đặt vé máy bay
 * @author OOP Project Team
 * @date 2024-2025
 */

#ifndef AIRCRAFT_SERVICE_H
#define AIRCRAFT_SERVICE_H

#include "../core/entities/Aircraft.h"
#include "../repositories/MySQLRepository/AircraftRepository.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "../repositories/MySQLRepository/TicketRepository.h"
#include "../utils/Logger.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @class AircraftService
 * @brief Lớp dịch vụ chịu trách nhiệm quản lý các thao tác liên quan đến máy bay
 *
 * Lớp này cung cấp các chức năng CRUD cơ bản và các thao tác nghiệp vụ
 * cho việc quản lý máy bay, bao gồm kiểm tra ghế trống, lớp ghế có sẵn.
 */
class AircraftService
{
private:
    std::shared_ptr<AircraftRepository> _aircraftRepository; ///< Repository để truy cập dữ liệu máy bay
    std::shared_ptr<FlightRepository> _flightRepository;     ///< Repository để truy cập dữ liệu chuyến bay
    std::shared_ptr<TicketRepository> _ticketRepository;     ///< Repository để truy cập dữ liệu vé
    std::shared_ptr<Logger> _logger;                         ///< Logger để ghi log hệ thống

    /**
     * @brief Lấy thông tin máy bay theo ID
     * @param id ID của máy bay
     * @return Result<Aircraft> Kết quả chứa thông tin máy bay hoặc lỗi
     */
    Result<Aircraft> getAircraftById(int id);

    /**
     * @brief Kiểm tra máy bay có tồn tại theo ID
     * @param id ID của máy bay cần kiểm tra
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> existsById(const int &id);

    /**
     * @brief Xóa máy bay theo ID
     * @param id ID của máy bay cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deleteById(const int &id);

public:
    /**
     * @brief Constructor khởi tạo AircraftService với các dependency
     * @param aircraftRepository Repository quản lý dữ liệu máy bay
     * @param flightRepository Repository quản lý dữ liệu chuyến bay
     * @param ticketRepository Repository quản lý dữ liệu vé
     * @param logger Logger để ghi log (tùy chọn, sẽ sử dụng singleton nếu null)
     */
    AircraftService(
        std::shared_ptr<AircraftRepository> aircraftRepository,
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<TicketRepository> ticketRepository,
        std::shared_ptr<Logger> logger = nullptr) : _aircraftRepository(std::move(aircraftRepository)), _flightRepository(std::move(flightRepository)), _ticketRepository(std::move(ticketRepository)), _logger(std::move(logger))
    {
        if (!_logger)
        {
            _logger = Logger::getInstance();
        }
    }

    // Core CRUD operations
    /**
     * @brief Lấy thông tin máy bay theo số serial
     * @param serial Số serial của máy bay
     * @return Result<Aircraft> Kết quả chứa thông tin máy bay hoặc lỗi
     */
    Result<Aircraft> getAircraft(const AircraftSerial &serial);

    /**
     * @brief Lấy danh sách tất cả máy bay
     * @return Result<std::vector<Aircraft>> Danh sách tất cả máy bay hoặc lỗi
     */
    Result<std::vector<Aircraft>> getAllAircraft();

    /**
     * @brief Kiểm tra máy bay có tồn tại theo số serial
     * @param serial Số serial của máy bay
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> aircraftExists(const AircraftSerial &serial);

    /**
     * @brief Tạo mới một máy bay
     * @param aircraft Đối tượng máy bay cần tạo
     * @return Result<Aircraft> Máy bay đã được tạo hoặc lỗi
     */
    Result<Aircraft> createAircraft(const Aircraft &aircraft);

    /**
     * @brief Cập nhật thông tin máy bay
     * @param aircraft Đối tượng máy bay với thông tin mới
     * @return Result<Aircraft> Máy bay đã được cập nhật hoặc lỗi
     */
    Result<Aircraft> updateAircraft(const Aircraft &aircraft);

    /**
     * @brief Xóa máy bay theo số serial
     * @param serial Số serial của máy bay cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deleteAircraft(const AircraftSerial &serial);

    // Business operations
    /**
     * @brief Lấy danh sách các hạng ghế có sẵn trên máy bay
     * @param serial Số serial của máy bay
     * @return Result<std::vector<std::string>> Danh sách các hạng ghế hoặc lỗi
     */
    Result<std::vector<std::string>> getAvailableSeatClasses(const AircraftSerial &serial);

    /**
     * @brief Lấy danh sách ghế trống theo hạng ghế
     * @param serial Số serial của máy bay
     * @param seatClass Hạng ghế cần kiểm tra
     * @return Result<std::vector<std::string>> Danh sách số ghế trống hoặc lỗi
     */
    Result<std::vector<std::string>> getAvailableSeats(const AircraftSerial &serial, const std::string &seatClass);

    /**
     * @brief Kiểm tra ghế có trống không
     * @param serial Số serial của máy bay
     * @param seatNumber Số ghế cần kiểm tra
     * @return Result<bool> true nếu ghế trống, false nếu đã có người ngồi
     */
    Result<bool> isSeatAvailable(const AircraftSerial &serial, const std::string &seatNumber);
};

#endif // AIRCRAFT_SERVICE_H