#ifndef FLIGHT_REPOSITORY_H
#define FLIGHT_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Flight.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include <memory>
#include <vector>

/**
 * @brief Lớp repository để quản lý các thao tác cơ sở dữ liệu cho thực thể Flight
 * 
 * Lớp này cung cấp các phương thức để thực hiện các thao tác CRUD (Create, Read, Update, Delete)
 * trên bảng flights trong cơ sở dữ liệu MySQL. Ngoài ra còn hỗ trợ quản lý ghế ngồi và các
 * truy vấn đặc biệt cho chuyến bay.
 */
class FlightRepository : public IRepository<Flight> {
private:
    std::shared_ptr<IDatabaseConnection> _connection; ///< Kết nối cơ sở dữ liệu
    std::shared_ptr<Logger> _logger; ///< Logger để ghi log

    /**
     * @brief Ánh xạ một hàng dữ liệu từ cơ sở dữ liệu thành đối tượng Flight
     * @param row Map chứa dữ liệu hàng với key là tên cột và value là giá trị
     * @return Đối tượng Flight được tạo từ dữ liệu
     */
    Flight mapRowToFlight(const std::map<std::string, std::string>& row) const;
    
    /**
     * @brief Lấy thông tin tình trạng ghế ngồi cho một chuyến bay
     * @param flight Chuyến bay cần lấy thông tin ghế
     * @return Map với key là SeatNumber và value là trạng thái available (true/false)
     */
    std::map<SeatNumber, bool> getSeatAvailability(const Flight& flight) const;

public:
    /**
     * @brief Constructor tạo FlightRepository với kết nối cơ sở dữ liệu và logger
     * @param connection Shared pointer đến interface kết nối cơ sở dữ liệu
     * @param logger Shared pointer đến logger (tùy chọn, mặc định sẽ dùng singleton)
     */
    FlightRepository(std::shared_ptr<IDatabaseConnection> connection, 
                    std::shared_ptr<Logger> logger = nullptr)
    : _connection(std::move(connection)), _logger(std::move(logger)) {
        if (!_logger) {
            _logger = Logger::getInstance();
        }
    };

    /**
     * @brief Tìm kiếm chuyến bay theo ID
     * @param id ID của chuyến bay cần tìm
     * @return Result chứa đối tượng Flight nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Flight> findById(const int& id) override;
    
    /**
     * @brief Lấy tất cả chuyến bay từ cơ sở dữ liệu
     * @return Result chứa vector các đối tượng Flight nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<std::vector<Flight>> findAll() override;
    
    /**
     * @brief Kiểm tra xem chuyến bay có tồn tại theo ID hay không
     * @param id ID của chuyến bay cần kiểm tra
     * @return Result chứa bool (true nếu tồn tại, false nếu không) hoặc lỗi nếu thất bại
     */
    Result<bool> exists(const int& id) override;
    
    /**
     * @brief Đếm tổng số chuyến bay trong cơ sở dữ liệu
     * @return Result chứa số lượng chuyến bay nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<size_t> count() override;
    
    /**
     * @brief Tạo mới một chuyến bay trong cơ sở dữ liệu
     * @param flight Đối tượng Flight cần tạo
     * @return Result chứa đối tượng Flight đã được tạo (có ID) nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Flight> create(const Flight& flight) override;
    
    /**
     * @brief Cập nhật thông tin chuyến bay trong cơ sở dữ liệu
     * @param flight Đối tượng Flight chứa thông tin cần cập nhật
     * @return Result chứa đối tượng Flight đã cập nhật nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Flight> update(const Flight& flight) override;
    
    /**
     * @brief Xóa chuyến bay theo ID
     * @param id ID của chuyến bay cần xóa
     * @return Result chứa bool (true nếu xóa thành công) hoặc lỗi nếu thất bại
     */
    Result<bool> deleteById(const int& id) override;

    // Phương thức riêng của Flight
    
    /**
     * @brief Tìm kiếm chuyến bay theo số hiệu chuyến bay
     * @param number Số hiệu chuyến bay cần tìm
     * @return Result chứa đối tượng Flight nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Flight> findByFlightNumber(const FlightNumber& number);
    
    /**
     * @brief Kiểm tra xem chuyến bay có tồn tại theo số hiệu hay không
     * @param number Số hiệu chuyến bay cần kiểm tra
     * @return Result chứa bool (true nếu tồn tại, false nếu không) hoặc lỗi nếu thất bại
     */
    Result<bool> existsFlight(const FlightNumber& number);
    
    /**
     * @brief Tìm kiếm các chuyến bay theo số seri máy bay
     * @param serial Số seri của máy bay
     * @return Result chứa vector các chuyến bay sử dụng máy bay này, hoặc lỗi nếu thất bại
     */
    Result<std::vector<Flight>> findFlightByAircraft(const AircraftSerial& serial);

    // Phương thức quản lý seat availability
    
    /**
     * @brief Đặt trước một ghế ngồi cho chuyến bay
     * @param flight Chuyến bay cần đặt ghế
     * @param seatNumber Số ghế cần đặt
     * @return Result chứa bool (true nếu đặt thành công) hoặc lỗi nếu thất bại
     */
    Result<bool> reserveSeat(const Flight& flight, const SeatNumber& seatNumber);
    
    /**
     * @brief Hủy đặt ghế ngồi cho chuyến bay
     * @param flight Chuyến bay cần hủy đặt ghế
     * @param seatNumber Số ghế cần hủy đặt
     * @return Result chứa bool (true nếu hủy thành công) hoặc lỗi nếu thất bại
     */
    Result<bool> releaseSeat(const Flight& flight, const SeatNumber& seatNumber);
    
    /**
     * @brief Lấy danh sách các ghế còn trống cho chuyến bay
     * @param flight Chuyến bay cần kiểm tra
     * @return Result chứa vector các SeatNumber còn trống, hoặc lỗi nếu thất bại
     */
    Result<std::vector<SeatNumber>> getAvailableSeats(const Flight& flight);
    
    /**
     * @brief Lấy danh sách các ghế đã được đặt cho chuyến bay
     * @param flight Chuyến bay cần kiểm tra
     * @return Result chứa vector các SeatNumber đã được đặt, hoặc lỗi nếu thất bại
     */
    Result<std::vector<SeatNumber>> getReservedSeats(const Flight& flight);
    
    /**
     * @brief Kiểm tra xem một ghế có còn trống hay không
     * @param flight Chuyến bay cần kiểm tra
     * @param seatNumber Số ghế cần kiểm tra
     * @return Result chứa bool (true nếu ghế còn trống) hoặc lỗi nếu thất bại
     */
    Result<bool> isSeatAvailable(const Flight& flight, const SeatNumber& seatNumber);
};

#endif // FLIGHT_REPOSITORY_H