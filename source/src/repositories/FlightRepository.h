/**
 * @file FlightRepository.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Lớp quản lý các thao tác với dữ liệu chuyến bay (Flight) trong cơ sở dữ liệu.
 * @version 0.1
 * @date 2025-05-01
 * 
 * @details
 * FlightRepository là lớp thực hiện các thao tác CRUD (Create, Read, Update, Delete) với dữ liệu
 * của chuyến bay trong cơ sở dữ liệu. Lớp này sử dụng đối tượng IDatabaseConnection để kết nối và
 * thao tác với cơ sở dữ liệu, đồng thời sử dụng Logger để ghi lại các thông tin log liên quan.
 */

#ifndef FLIGHT_REPOSITORY_H
#define FLIGHT_REPOSITORY_H

#include "RepositoryInterface.h"
#include "../core/Flight.h"
#include "../database/DatabaseConnection.h"
#include "../utils/Logger.h"

#include <memory>
#include <vector>
#include <optional>

/**
 * @class FlightRepository
 * @brief Quản lý các thao tác với dữ liệu chuyến bay trong cơ sở dữ liệu.
 *
 * FlightRepository thực hiện các thao tác CRUD (Create, Read, Update, Delete) cho đối tượng
 * chuyến bay (Flight), tương tác với cơ sở dữ liệu thông qua giao diện IDatabaseConnection.
 * Lớp này cung cấp các phương thức để tìm kiếm, lưu trữ, và cập nhật dữ liệu chuyến bay.
 */
class FlightRepository : public IRepository<Flight> {
private:
    std::shared_ptr<IDatabaseConnection> _dbConnection; ///< Đối tượng kết nối cơ sở dữ liệu
    std::shared_ptr<Logger> _logger;                    ///< Đối tượng ghi log
    
    /**
     * @brief Chuyển đổi kết quả truy vấn thành đối tượng Flight.
     * @param result Kết quả trả về từ truy vấn cơ sở dữ liệu
     * @return Đối tượng Flight tương ứng với kết quả
     */
    Flight _mapResultToFlight(IDatabaseResult& result);

public:
    /**
     * @brief Khởi tạo đối tượng FlightRepository.
     * @param dbConnection Đối tượng kết nối cơ sở dữ liệu
     */
    explicit FlightRepository(std::shared_ptr<IDatabaseConnection> dbConnection);

    std::vector<Flight> findAll() override;
    std::optional<Flight> findById(const int& id) override;
    bool save(Flight& flight) override;
    bool update(const Flight& flight) override;
    bool remove(const int& id) override;

    /**
     * @brief Tìm kiếm các chuyến bay có sẵn (chưa được đặt chỗ).
     * @return Một vector chứa các đối tượng Flight có sẵn
     */
    std::vector<Flight> findAvailableFlights();

    /**
     * @brief Tìm chuyến bay theo số hiệu chuyến bay.
     * @param flightNo Số hiệu chuyến bay
     * @return Đối tượng Flight nếu tìm thấy, ngược lại trả về std::nullopt
     */
    std::optional<Flight> findByFlightNo(const std::string& flightNo);
};

#endif // FLIGHT_REPOSITORY_H
 