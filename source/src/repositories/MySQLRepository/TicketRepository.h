#ifndef TICKET_REPOSITORY_H
#define TICKET_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Ticket.h"
#include "../../core/value_objects/ticket_number/TicketNumber.h"
#include "../../core/value_objects/seat_number/SeatNumber.h"
#include "../../core/value_objects/price/Price.h"
#include "../../core/value_objects/ticket_status/TicketStatus.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include "AircraftRepository.h"
#include "FlightRepository.h"
#include "PassengerRepository.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <optional>

/**
 * @brief Lớp repository để quản lý các thao tác cơ sở dữ liệu cho thực thể Ticket
 * 
 * Lớp này cung cấp các phương thức để thực hiện các thao tác CRUD trên bảng tickets
 * trong cơ sở dữ liệu MySQL. Bao gồm cả các truy vấn đặc biệt để tìm kiếm vé theo
 * nhiều tiêu chí khác nhau.
 */
class TicketRepository : public IRepository<Ticket> {
private:
    std::shared_ptr<IDatabaseConnection> _connection; ///< Kết nối cơ sở dữ liệu
    std::shared_ptr<Logger> _logger; ///< Logger để ghi log
    std::shared_ptr<AircraftRepository> _aircraftRepository; ///< Repository để truy vấn máy bay
    std::shared_ptr<FlightRepository> _flightRepository; ///< Repository để truy vấn chuyến bay
    std::shared_ptr<PassengerRepository> _passengerRepository; ///< Repository để truy vấn hành khách

public:
    /**
     * @brief Constructor tạo TicketRepository với các dependencies cần thiết
     * @param connection Shared pointer đến interface kết nối cơ sở dữ liệu
     * @param passengerRepository Repository để truy vấn thông tin hành khách
     * @param flightRepository Repository để truy vấn thông tin chuyến bay
     * @param logger Shared pointer đến logger (tùy chọn)
     */
    explicit TicketRepository(
        std::shared_ptr<IDatabaseConnection> connection,
        std::shared_ptr<PassengerRepository> passengerRepository,
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<Logger> logger = nullptr
    )
        : _connection(std::move(connection))
        , _passengerRepository(std::move(passengerRepository))
        , _flightRepository(std::move(flightRepository))
        , _logger(std::move(logger))
    {}

    /**
     * @brief Tìm kiếm vé theo ID
     * @param id ID của vé cần tìm
     * @return Result chứa đối tượng Ticket nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Ticket> findById(const int& id) override;
    
    /**
     * @brief Lấy tất cả vé từ cơ sở dữ liệu
     * @return Result chứa vector các đối tượng Ticket nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<std::vector<Ticket>> findAll() override;
    
    /**
     * @brief Kiểm tra xem vé có tồn tại theo ID hay không
     * @param id ID của vé cần kiểm tra
     * @return Result chứa bool (true nếu tồn tại, false nếu không) hoặc lỗi nếu thất bại
     */
    Result<bool> exists(const int& id) override;
    
    /**
     * @brief Đếm tổng số vé trong cơ sở dữ liệu
     * @return Result chứa số lượng vé nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<size_t> count() override;
    
    /**
     * @brief Tạo mới một vé trong cơ sở dữ liệu
     * @param ticket Đối tượng Ticket cần tạo
     * @return Result chứa đối tượng Ticket đã được tạo (có ID) nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Ticket> create(const Ticket& ticket) override;
    
    /**
     * @brief Cập nhật thông tin vé trong cơ sở dữ liệu
     * @param ticket Đối tượng Ticket chứa thông tin cần cập nhật
     * @return Result chứa đối tượng Ticket đã cập nhật nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Ticket> update(const Ticket& ticket) override;
    
    /**
     * @brief Xóa vé theo ID
     * @param id ID của vé cần xóa
     * @return Result chứa bool (true nếu xóa thành công) hoặc lỗi nếu thất bại
     */
    Result<bool> deleteById(const int& id) override;
    
    /**
     * @brief Tìm kiếm vé theo mã số vé
     * @param ticketNumber Mã số vé cần tìm
     * @return Result chứa đối tượng Ticket nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Ticket> findByTicketNumber(const TicketNumber& ticketNumber);
    
    /**
     * @brief Kiểm tra xem vé có tồn tại theo mã số vé hay không
     * @param ticketNumber Mã số vé cần kiểm tra
     * @return Result chứa bool (true nếu tồn tại, false nếu không) hoặc lỗi nếu thất bại
     */
    Result<bool> existsTicket(const TicketNumber& ticketNumber);
    
    /**
     * @brief Tìm kiếm các vé theo ID hành khách
     * @param passengerId ID của hành khách
     * @return Result chứa vector các vé của hành khách hoặc lỗi nếu thất bại
     */
    Result<std::vector<Ticket>> findByPassengerId(int passengerId);
    
    /**
     * @brief Tìm kiếm các vé theo ID chuyến bay
     * @param flightId ID của chuyến bay
     * @return Result chứa vector các vé của chuyến bay hoặc lỗi nếu thất bại
     */
    Result<std::vector<Ticket>> findByFlightId(int flightId);
    
    /**
     * @brief Tìm kiếm các vé theo số seri máy bay
     * @param serial Số seri của máy bay
     * @return Result chứa vector các vé trên máy bay này hoặc lỗi nếu thất bại
     */
    Result<std::vector<Ticket>> findBySerialNumber(const AircraftSerial& serial);
    
    /**
     * @brief Tìm kiếm vé theo nhiều tiêu chí với tùy chọn sắp xếp và giới hạn
     * @param params Map chứa các tham số tìm kiếm (minPrice, maxPrice, flightNumber, status, passport)
     * @param limit Số lượng kết quả tối đa (tùy chọn)
     * @param sortBy Trường để sắp xếp (tùy chọn)
     * @param sortAscending True để sắp xếp tăng dần, false để giảm dần
     * @return Result chứa vector các vé thỏa mãn điều kiện hoặc lỗi nếu thất bại
     */
    Result<std::vector<Ticket>> findByCriteria(const std::map<std::string, std::string>& params, 
                                             std::optional<int> limit = std::nullopt,
                                             std::optional<std::string> sortBy = std::nullopt,
                                             bool sortAscending = true);
};

#endif