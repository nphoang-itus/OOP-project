/**
 * @file FlightService.h
 * @brief Dịch vụ quản lý chuyến bay trong hệ thống đặt vé máy bay
 * @author OOP Project Team
 * @date 2024-2025
 */

#ifndef FLIGHT_SERVICE_H
#define FLIGHT_SERVICE_H

#include "../core/entities/Flight.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "../repositories/MySQLRepository/AircraftRepository.h"
#include "../repositories/MySQLRepository/TicketRepository.h"
#include "../utils/Logger.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @class FlightService
 * @brief Lớp dịch vụ chịu trách nhiệm quản lý các thao tác liên quan đến chuyến bay
 * 
 * Lớp này cung cấp các chức năng CRUD cơ bản và các thao tác nghiệp vụ phức tạp
 * cho việc quản lý chuyến bay, bao gồm quản lý ghế ngồi, lịch trình, trạng thái bay.
 */
class FlightService {
private:
    std::shared_ptr<FlightRepository> _flightRepository;        ///< Repository để truy cập dữ liệu chuyến bay
    std::shared_ptr<AircraftRepository> _aircraftRepository;    ///< Repository để truy cập dữ liệu máy bay
    std::shared_ptr<TicketRepository> _ticketRepository;        ///< Repository để truy cập dữ liệu vé
    std::shared_ptr<Logger> _logger;                            ///< Logger để ghi log hệ thống

    /**
     * @brief Lấy thông tin chuyến bay theo ID
     * @param id ID của chuyến bay
     * @return Result<Flight> Kết quả chứa thông tin chuyến bay hoặc lỗi
     */
    Result<Flight> getFlightById(int id);
    
    /**
     * @brief Kiểm tra chuyến bay có tồn tại theo ID
     * @param id ID của chuyến bay cần kiểm tra
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> existsById(const int& id);
    
    /**
     * @brief Xóa chuyến bay theo ID
     * @param id ID của chuyến bay cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deleteById(const int& id);

public:
    /**
     * @brief Constructor khởi tạo FlightService với các dependency
     * @param flightRepository Repository quản lý dữ liệu chuyến bay
     * @param aircraftRepository Repository quản lý dữ liệu máy bay
     * @param ticketRepository Repository quản lý dữ liệu vé
     * @param logger Logger để ghi log (tùy chọn, sẽ sử dụng singleton nếu null)
     */
    FlightService(
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<AircraftRepository> aircraftRepository,
        std::shared_ptr<TicketRepository> ticketRepository,
        std::shared_ptr<Logger> logger = nullptr
    ) : _flightRepository(std::move(flightRepository))
      , _aircraftRepository(std::move(aircraftRepository))
      , _ticketRepository(std::move(ticketRepository))
      , _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    }

    // Core CRUD operations
    /**
     * @brief Lấy thông tin chuyến bay theo số hiệu
     * @param number Số hiệu chuyến bay
     * @return Result<Flight> Kết quả chứa thông tin chuyến bay hoặc lỗi
     */
    Result<Flight> getFlight(const FlightNumber& number);
    
    /**
     * @brief Lấy danh sách tất cả chuyến bay
     * @return Result<std::vector<Flight>> Danh sách tất cả chuyến bay hoặc lỗi
     */
    Result<std::vector<Flight>> getAllFlights();
    
    /**
     * @brief Kiểm tra chuyến bay có tồn tại theo số hiệu
     * @param number Số hiệu chuyến bay
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> flightExists(const FlightNumber& number);
    
    /**
     * @brief Tạo mới một chuyến bay
     * @param flight Đối tượng chuyến bay cần tạo
     * @return Result<Flight> Chuyến bay đã được tạo hoặc lỗi
     */
    Result<Flight> createFlight(const Flight& flight);
    
    /**
     * @brief Cập nhật thông tin chuyến bay
     * @param flight Đối tượng chuyến bay với thông tin mới
     * @return Result<Flight> Chuyến bay đã được cập nhật hoặc lỗi
     */
    Result<Flight> updateFlight(const Flight& flight);
    
    /**
     * @brief Xóa chuyến bay theo số hiệu
     * @param number Số hiệu chuyến bay cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deleteFlight(const FlightNumber& number);
    
    // Business operations
    /**
     * @brief Lấy danh sách chuyến bay theo máy bay
     * @param serial Số serial của máy bay
     * @return Result<std::vector<Flight>> Danh sách chuyến bay hoặc lỗi
     */
    Result<std::vector<Flight>> getFlightsByAircraft(const AircraftSerial& serial);
    
    /**
     * @brief Lấy danh sách chuyến bay theo tuyến đường
     * @param originCode Mã sân bay khởi hành
     * @param destinationCode Mã sân bay đến
     * @return Result<std::vector<Flight>> Danh sách chuyến bay hoặc lỗi
     */
    Result<std::vector<Flight>> getFlightsByRoute(const std::string& originCode, const std::string& destinationCode);
    
    /**
     * @brief Lấy danh sách chuyến bay theo ngày
     * @param date Ngày bay (định dạng chuỗi)
     * @return Result<std::vector<Flight>> Danh sách chuyến bay hoặc lỗi
     */
    Result<std::vector<Flight>> getFlightsByDate(const std::string& date);
    
    /**
     * @brief Kiểm tra ghế có trống trên chuyến bay
     * @param number Số hiệu chuyến bay
     * @param seatNumber Số ghế cần kiểm tra
     * @return Result<bool> true nếu ghế trống, false nếu đã được đặt
     */
    Result<bool> isSeatAvailable(const FlightNumber& number, const std::string& seatNumber);
    
    /**
     * @brief Lấy danh sách ghế trống theo hạng ghế
     * @param number Số hiệu chuyến bay
     * @param seatClass Hạng ghế cần kiểm tra
     * @return Result<std::vector<std::string>> Danh sách số ghế trống hoặc lỗi
     */
    Result<std::vector<std::string>> getAvailableSeats(const FlightNumber& number, const std::string& seatClass);
    
    /**
     * @brief Cập nhật trạng thái chuyến bay
     * @param number Số hiệu chuyến bay
     * @param status Trạng thái mới
     * @return Result<bool> true nếu cập nhật thành công, false nếu thất bại
     */
    Result<bool> updateFlightStatus(const FlightNumber& number, FlightStatus status);

    /**
     * @brief Hủy chuyến bay
     * @param number Số hiệu chuyến bay
     * @param reason Lý do hủy
     * @return Result<bool> true nếu hủy thành công, false nếu thất bại
     */
    Result<bool> cancelFlight(const FlightNumber& number, const std::string& reason);
    
    /**
     * @brief Hoãn chuyến bay
     * @param number Số hiệu chuyến bay
     * @param newDepartureTime Thời gian khởi hành mới
     * @return Result<bool> true nếu hoãn thành công, false nếu thất bại
     */
    Result<bool> delayFlight(const FlightNumber& number, const std::tm& newDepartureTime);
    
    // Seat management business rules
    /**
     * @brief Đặt chỗ ghế trên chuyến bay
     * @param number Số hiệu chuyến bay
     * @param seatNumber Số ghế cần đặt
     * @return Result<bool> true nếu đặt thành công, false nếu thất bại
     */
    Result<bool> reserveSeat(const FlightNumber& number, const std::string& seatNumber);
    
    /**
     * @brief Hủy đặt chỗ ghế trên chuyến bay
     * @param number Số hiệu chuyến bay
     * @param seatNumber Số ghế cần hủy
     * @return Result<bool> true nếu hủy thành công, false nếu thất bại
     */
    Result<bool> releaseSeat(const FlightNumber& number, const std::string& seatNumber);
    
    // Flight capacity management
    /**
     * @brief Lấy số ghế còn trống trên chuyến bay
     * @param number Số hiệu chuyến bay
     * @return Result<int> Số ghế còn trống hoặc lỗi
     */
    Result<int> getRemainingCapacity(const FlightNumber& number);
    
    /**
     * @brief Kiểm tra chuyến bay đã đầy chỗ
     * @param number Số hiệu chuyến bay
     * @return Result<bool> true nếu đầy chỗ, false nếu còn chỗ
     */
    Result<bool> isFlightFull(const FlightNumber& number);
    
    // Business validation
    /**
     * @brief Kiểm tra hành khách có thể lên máy bay
     * @param number Số hiệu chuyến bay
     * @param passport Số hộ chiếu hành khách
     * @return Result<bool> true nếu có thể lên máy bay, false nếu không
     */
    Result<bool> canBoardPassenger(const FlightNumber& number, const PassportNumber& passport);
    
    /**
     * @brief Kiểm tra chuyến bay sắp khởi hành
     * @param number Số hiệu chuyến bay
     * @return Result<bool> true nếu sắp khởi hành, false nếu còn thời gian
     */
    Result<bool> isFlightDepartureImminent(const FlightNumber& number);
    
    // Schedule management
    /**
     * @brief Lấy danh sách chuyến bay xung đột lịch trình
     * @param serial Số serial máy bay
     * @param schedule Lịch trình cần kiểm tra
     * @return Result<std::vector<Flight>> Danh sách chuyến bay xung đột hoặc lỗi
     */
    Result<std::vector<Flight>> getConflictingFlights(const AircraftSerial& serial, const Schedule& schedule);
    
    /**
     * @brief Xác thực lịch trình cho máy bay
     * @param serial Số serial máy bay
     * @param schedule Lịch trình cần xác thực
     * @return Result<bool> true nếu lịch trình hợp lệ, false nếu xung đột
     */
    Result<bool> validateScheduleForAircraft(const AircraftSerial& serial, const Schedule& schedule);
};

#endif // FLIGHT_SERVICE_H