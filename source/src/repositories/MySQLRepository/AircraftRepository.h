/**
 * @file AircraftRepository.h
 * @brief Repository class cho entity Aircraft, quản lý thao tác CRUD với bảng aircraft.
 * @version 0.1
 * @date 2025-01-20
 * 
 * @details
 * Lớp AircraftRepository cung cấp interface để thao tác với dữ liệu máy bay
 * trong cơ sở dữ liệu MySQL. Kế thừa từ IRepository và implement các phương thức
 * CRUD cũng như các chức năng đặc biệt riêng cho Aircraft.
 */

#ifndef AIRCRAFT_REPOSITORY_H
#define AIRCRAFT_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Aircraft.h"
#include "../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include "../../utils/Logger.h"
#include "../../utils/TableConstants.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

/**
 * @class AircraftRepository
 * @brief Repository để quản lý dữ liệu máy bay trong cơ sở dữ liệu.
 * 
 * @details
 * Lớp này cung cấp các chức năng:
 * - CRUD operations cơ bản (tạo, đọc, cập nhật, xóa)
 * - Tìm kiếm theo số serial
 * - Kiểm tra sự tồn tại của máy bay
 * - Xóa theo số serial
 * - Mapping dữ liệu từ database row sang Aircraft object
 * 
 * Thiết kế tuân theo:
 * - Repository Pattern
 * - Dependency Injection (connection, logger)
 * - SOLID principles
 * 
 * @note Sử dụng MySQL X DevAPI thông qua IDatabaseConnection
 */
class AircraftRepository : public IRepository<Aircraft> {
private:
    std::shared_ptr<IDatabaseConnection> _connection; ///< Kết nối database được inject
    std::shared_ptr<Logger> _logger;                  ///< Logger để ghi log debug/error

public:
    /**
     * @brief Constructor với dependency injection.
     * 
     * @param connection Shared pointer tới database connection
     * @param logger Optional logger instance (có thể null)
     * 
     * @details
     * Khởi tạo repository với:
     * - Database connection để thực hiện các thao tác SQL
     * - Logger để ghi lại thông tin debug và error
     * 
     * @note Logger có thể null nếu không cần logging
     */
    explicit AircraftRepository(std::shared_ptr<IDatabaseConnection> connection, 
                              std::shared_ptr<Logger> logger = nullptr)
        : _connection(std::move(connection))
        , _logger(std::move(logger)) {}

    /**
     * @brief Tìm máy bay theo ID.
     * 
     * @param id ID của máy bay cần tìm
     * @return Result<Aircraft> Máy bay tìm được hoặc lỗi
     * 
     * @details
     * Thực hiện:
     * 1. Prepare SELECT statement với parameter ID
     * 2. Execute query và lấy kết quả
     * 3. Map row data sang Aircraft object
     * 4. Gán ID cho Aircraft object
     * 
     * @note Sử dụng prepared statement để tránh SQL injection
     */
    Result<Aircraft> findById(const int& id) override;

    /**
     * @brief Lấy tất cả máy bay từ database.
     * 
     * @return Result<std::vector<Aircraft>> Danh sách tất cả máy bay hoặc lỗi
     * 
     * @details
     * Thực hiện:
     * 1. Execute SELECT * query
     * 2. Iterate qua tất cả rows
     * 3. Map mỗi row sang Aircraft object
     * 4. Thêm vào vector kết quả
     * 
     * @warning Có thể tốn memory nếu có nhiều máy bay
     */
    Result<std::vector<Aircraft>> findAll() override;

    /**
     * @brief Kiểm tra máy bay có tồn tại theo ID hay không.
     * 
     * @param id ID cần kiểm tra
     * @return Result<bool> true nếu tồn tại, false nếu không
     * 
     * @details
     * Sử dụng COUNT(*) query để kiểm tra tồn tại hiệu quả
     * mà không cần load toàn bộ data.
     */
    Result<bool> exists(const int& id) override;

    /**
     * @brief Đếm tổng số máy bay trong database.
     * 
     * @return Result<size_t> Số lượng máy bay hoặc lỗi
     * 
     * @details
     * Sử dụng COUNT(*) query để đếm hiệu quả.
     * Hữu ích cho pagination và thống kê.
     */
    Result<size_t> count() override;

    /**
     * @brief Tạo mới một máy bay.
     * 
     * @param aircraft Máy bay cần tạo (không có ID)
     * @return Result<Aircraft> Máy bay đã tạo với ID mới hoặc lỗi
     * 
     * @details
     * Thực hiện trong transaction:
     * 1. Begin transaction
     * 2. Insert aircraft data
     * 3. Get last insert ID
     * 4. Commit transaction
     * 5. Return aircraft với ID mới
     * 
     * @note Rollback nếu có lỗi xảy ra
     */
    Result<Aircraft> create(const Aircraft& aircraft) override;

    /**
     * @brief Cập nhật thông tin máy bay.
     * 
     * @param aircraft Máy bay với thông tin mới (phải có ID)
     * @return Result<Aircraft> Máy bay đã cập nhật hoặc lỗi
     * 
     * @details
     * Thực hiện:
     * 1. Kiểm tra sự tồn tại
     * 2. Begin transaction
     * 3. Update với prepared statement
     * 4. Commit transaction
     * 
     * @pre Aircraft phải có ID hợp lệ và tồn tại
     */
    Result<Aircraft> update(const Aircraft& aircraft) override;

    /**
     * @brief Xóa máy bay theo ID.
     * 
     * @param id ID của máy bay cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu không tìm thấy
     * 
     * @details
     * Thực hiện:
     * 1. Kiểm tra sự tồn tại
     * 2. Begin transaction
     * 3. Delete với prepared statement
     * 4. Commit transaction
     * 
     * @warning Kiểm tra foreign key constraints trước khi xóa
     */
    Result<bool> deleteById(const int& id) override;

    // Phương thức riêng của Aircraft

    /**
     * @brief Tìm máy bay theo số serial.
     * 
     * @param serial Số serial của máy bay
     * @return Result<Aircraft> Máy bay tìm được hoặc lỗi
     * 
     * @details
     * Serial number là unique identifier của máy bay.
     * Tương tự findById nhưng search theo serial thay vì ID.
     */
    Result<Aircraft> findBySerialNumber(const AircraftSerial& serial);

    /**
     * @brief Kiểm tra máy bay có tồn tại theo số serial hay không.
     * 
     * @param serial Số serial cần kiểm tra
     * @return Result<bool> true nếu tồn tại, false nếu không
     * 
     * @details
     * Hữu ích để validation trước khi tạo máy bay mới
     * với serial number không trùng lặp.
     */
    Result<bool> existsAircraft(const AircraftSerial& serial);

    /**
     * @brief Xóa máy bay theo số serial.
     * 
     * @param serial Số serial của máy bay cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu không tìm thấy
     * 
     * @details
     * Alternative delete method sử dụng serial thay vì ID.
     * Tiện lợi khi chỉ biết serial number.
     */
    Result<bool> deleteBySerialNumber(const AircraftSerial& serial);

private:
    /**
     * @brief Chuyển đổi dữ liệu từ database row sang Aircraft object.
     * 
     * @param row Map chứa dữ liệu từ database row
     * @return Aircraft object đã được tạo
     * 
     * @details
     * Thực hiện mapping:
     * 1. Extract dữ liệu từ row map
     * 2. Tạo value objects (AircraftSerial, SeatClassMap)
     * 3. Tạo Aircraft object từ value objects
     * 4. Set ID cho Aircraft
     * 
     * @note Method này giả định dữ liệu trong row hợp lệ
     */
    Aircraft mapRowToAircraft(const std::map<std::string, std::string>& row) const;
};

#endif // AIRCRAFT_REPOSITORY_H
