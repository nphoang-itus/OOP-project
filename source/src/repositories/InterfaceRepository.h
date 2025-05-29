/**
 * @file InterfaceRepository.h
 * @brief Giao diện trừu tượng cho các lớp Repository theo mẫu thiết kế Repository Pattern.
 * @version 0.1
 * @date 2025-01-20
 * 
 * @details
 * Định nghĩa interface chung cho tất cả các repository trong hệ thống.
 * Cung cấp các thao tác CRUD cơ bản và tuân theo nguyên lý SOLID.
 * Hỗ trợ generic type để tái sử dụng cho nhiều entity khác nhau.
 */

#ifndef INTERFACE_REPOSITORY_H
#define INTERFACE_REPOSITORY_H

#include <vector>
#include "../core/exceptions/Result.h"

/**
 * @interface IRepository
 * @brief Giao diện repository cơ bản cho các thao tác CRUD.
 * 
 * @tparam T Kiểu dữ liệu của entity được quản lý
 * @tparam IdType Kiểu dữ liệu của khóa chính (mặc định là int)
 * 
 * @details
 * Interface này định nghĩa các phương thức chuẩn cho:
 * - Create: Tạo mới entity
 * - Read: Đọc entity (theo ID, tất cả, kiểm tra tồn tại, đếm)
 * - Update: Cập nhật entity
 * - Delete: Xóa entity
 * 
 * Thiết kế theo Repository Pattern giúp:
 * - Tách biệt logic business và data access
 * - Dễ dàng thay đổi implementation (MySQL, PostgreSQL, etc.)
 * - Hỗ trợ unit testing với mock objects
 * - Tuân theo Dependency Inversion Principle
 */
template<typename T, typename IdType = int>
class IRepository {
public:
    /**
     * @brief Destructor ảo để đảm bảo polymorphism hoạt động đúng.
     */
    virtual ~IRepository() = default;
    
    /**
     * @brief Tìm entity theo ID.
     * 
     * @param id Khóa chính của entity cần tìm
     * @return Result<T> Entity tìm được hoặc thông báo lỗi
     * 
     * @details
     * Tìm kiếm entity dựa trên khóa chính.
     * Trả về Success(entity) nếu tìm thấy, Failure nếu không tồn tại.
     */
    virtual Result<T> findById(const IdType& id) = 0;

    /**
     * @brief Lấy tất cả entities trong repository.
     * 
     * @return Result<std::vector<T>> Danh sách tất cả entities hoặc thông báo lỗi
     * 
     * @details
     * Trả về tất cả records trong bảng tương ứng.
     * Có thể trả về vector rỗng nếu không có dữ liệu.
     * 
     * @warning Cẩn thận với hiệu suất khi bảng có nhiều dữ liệu
     */
    virtual Result<std::vector<T>> findAll() = 0;

    /**
     * @brief Kiểm tra entity có tồn tại hay không.
     * 
     * @param id Khóa chính cần kiểm tra
     * @return Result<bool> true nếu tồn tại, false nếu không
     * 
     * @details
     * Phương thức tối ưu để kiểm tra sự tồn tại mà không cần load toàn bộ entity.
     * Thường được implement bằng SELECT COUNT(*) để hiệu suất cao.
     */
    virtual Result<bool> exists(const IdType& id) = 0;

    /**
     * @brief Đếm tổng số entities trong repository.
     * 
     * @return Result<size_t> Số lượng entities hoặc thông báo lỗi
     * 
     * @details
     * Trả về tổng số records trong bảng.
     * Hữu ích cho pagination và báo cáo thống kê.
     */
    virtual Result<size_t> count() = 0;

    /**
     * @brief Tạo mới một entity.
     * 
     * @param entity Entity cần tạo (không bao gồm ID)
     * @return Result<T> Entity đã được tạo với ID mới hoặc thông báo lỗi
     * 
     * @details
     * Thêm entity mới vào database và trả về entity với ID được assign.
     * ID thường được tự động generate bởi database.
     * 
     * @pre Entity phải hợp lệ và chưa có ID
     * @post Entity được persist vào database với ID mới
     */
    virtual Result<T> create(const T& entity) = 0;

    /**
     * @brief Cập nhật một entity đã tồn tại.
     * 
     * @param entity Entity cần cập nhật (phải có ID hợp lệ)
     * @return Result<T> Entity đã được cập nhật hoặc thông báo lỗi
     * 
     * @details
     * Cập nhật toàn bộ thông tin của entity dựa trên ID.
     * Thường implement theo phương pháp "update all fields".
     * 
     * @pre Entity phải có ID hợp lệ và tồn tại trong database
     * @post Entity được cập nhật với thông tin mới
     */
    virtual Result<T> update(const T& entity) = 0;

    /**
     * @brief Xóa entity theo ID.
     * 
     * @param id Khóa chính của entity cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu không tìm thấy
     * 
     * @details
     * Xóa vĩnh viễn entity khỏi database.
     * Kiểm tra constraint và foreign key trước khi xóa.
     * 
     * @pre Entity với ID này phải tồn tại
     * @post Entity bị xóa khỏi database
     * @warning Thao tác không thể hoàn tác
     */
    virtual Result<bool> deleteById(const IdType& id) = 0;
};

#endif // INTERFACE_REPOSITORY_H
