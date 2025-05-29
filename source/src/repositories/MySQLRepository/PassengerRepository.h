#ifndef PASSENGER_REPOSITORY_H
#define PASSENGER_REPOSITORY_H

#include "../InterfaceRepository.h"
#include "../../core/entities/Passenger.h"
#include "../../database/InterfaceDatabaseConnection.h"
#include "../../utils/Logger.h"
#include <memory>
#include <vector>

/**
 * @brief Lớp repository để quản lý các thao tác cơ sở dữ liệu cho thực thể Passenger
 * 
 * Lớp này cung cấp các phương thức để thực hiện các thao tác CRUD trên bảng passengers
 * trong cơ sở dữ liệu MySQL. Bao gồm cả các truy vấn đặc biệt để tìm kiếm hành khách
 * theo số hộ chiếu.
 */
class PassengerRepository : public IRepository<Passenger> {
private:
    std::shared_ptr<IDatabaseConnection> _connection; ///< Kết nối cơ sở dữ liệu
    std::shared_ptr<Logger> _logger; ///< Logger để ghi log

public:
    /**
     * @brief Constructor tạo PassengerRepository với kết nối cơ sở dữ liệu và logger
     * @param connection Shared pointer đến interface kết nối cơ sở dữ liệu
     * @param logger Shared pointer đến logger
     */
    PassengerRepository(std::shared_ptr<IDatabaseConnection> connection, std::shared_ptr<Logger> logger)
        : _connection(connection), _logger(logger) {}

    /**
     * @brief Tìm kiếm hành khách theo ID
     * @param id ID của hành khách cần tìm
     * @return Result chứa đối tượng Passenger nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Passenger> findById(const int& id) override;
    
    /**
     * @brief Lấy tất cả hành khách từ cơ sở dữ liệu
     * @return Result chứa vector các đối tượng Passenger nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<std::vector<Passenger>> findAll() override;
    
    /**
     * @brief Kiểm tra xem hành khách có tồn tại theo ID hay không
     * @param id ID của hành khách cần kiểm tra
     * @return Result chứa bool (true nếu tồn tại, false nếu không) hoặc lỗi nếu thất bại
     */
    Result<bool> exists(const int& id) override;
    
    /**
     * @brief Đếm tổng số hành khách trong cơ sở dữ liệu
     * @return Result chứa số lượng hành khách nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<size_t> count() override;
    
    /**
     * @brief Tạo mới một hành khách trong cơ sở dữ liệu
     * @param passenger Đối tượng Passenger cần tạo
     * @return Result chứa đối tượng Passenger đã được tạo (có ID) nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Passenger> create(const Passenger& passenger) override;
    
    /**
     * @brief Cập nhật thông tin hành khách trong cơ sở dữ liệu
     * @param passenger Đối tượng Passenger chứa thông tin cần cập nhật
     * @return Result chứa đối tượng Passenger đã cập nhật nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Passenger> update(const Passenger& passenger) override;
    
    /**
     * @brief Xóa hành khách theo ID
     * @param id ID của hành khách cần xóa
     * @return Result chứa bool (true nếu xóa thành công) hoặc lỗi nếu thất bại
     */
    Result<bool> deleteById(const int& id) override;
    
    /**
     * @brief Xóa hành khách theo số hộ chiếu
     * @param passport Số hộ chiếu của hành khách cần xóa
     * @return Result chứa bool (true nếu xóa thành công) hoặc lỗi nếu thất bại
     */
    Result<bool> deleteByPassportNumber(const PassportNumber& passport);
    
    /**
     * @brief Tìm kiếm hành khách theo số hộ chiếu
     * @param passport Số hộ chiếu của hành khách cần tìm
     * @return Result chứa đối tượng Passenger nếu thành công, hoặc lỗi nếu thất bại
     */
    Result<Passenger> findByPassportNumber(const PassportNumber& passport);
    
    /**
     * @brief Kiểm tra xem hành khách có tồn tại theo số hộ chiếu hay không
     * @param passport Số hộ chiếu cần kiểm tra
     * @return Result chứa bool (true nếu tồn tại, false nếu không) hoặc lỗi nếu thất bại
     */
    Result<bool> existsPassport(const PassportNumber& passport);
};

#endif