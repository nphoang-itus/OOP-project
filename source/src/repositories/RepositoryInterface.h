/**
 * @file RepositoryInterface.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Định nghĩa giao diện cho các repository thực hiện các thao tác CRUD.
 * @version 0.1
 * @date 2025-05-01
 * 
 * @details
 * Giao diện IRepository định nghĩa các phương thức chung để thực hiện các thao tác CRUD (Create, 
 * Read, Update, Delete) cho các thực thể trong ứng dụng. Các repository sẽ kế thừa từ giao diện này
 * và cài đặt các phương thức theo cách phù hợp với nguồn dữ liệu mà chúng tương tác (ví dụ: cơ sở dữ liệu).
 */

#ifndef REPOSITORY_INTERFACE_H
#define REPOSITORY_INTERFACE_H

#include <vector>
#include <optional>

/**
 * @class IRepository
 * @brief Giao diện chung cho các repository thực hiện các thao tác CRUD.
 * 
 * Giao diện này định nghĩa các phương thức cơ bản như `findAll`, `findById`, `save`, `update`, `remove`
 * cho các thực thể trong ứng dụng. Các repository cụ thể sẽ kế thừa giao diện này và cài đặt các phương 
 * thức để tương tác với nguồn dữ liệu cụ thể (như cơ sở dữ liệu, file hệ thống, v.v).
 * 
 * @tparam T Loại đối tượng thực thể mà repository làm việc
 * @tparam IdType Kiểu dữ liệu của khoá chính (mặc định là `int`)
 */
template <typename T, typename IdType = int>
class IRepository {
public:
    virtual ~IRepository() = default;
    
    /**
     * @brief Lấy tất cả các thuộc tính của thực thể và lưu vào vector.
     * 
     * @return std::vector<T> Một vector chứa tất cả các đối tượng của loại T
     */
    virtual std::vector<T> findAll() = 0;

    /**
     * @brief Tìm các giá trị thuộc tính theo Id.
     * 
     * @param id Khoá chính của thực thể
     * @return std::optional<T> Một đối tượng `T` nếu tìm thấy, hoặc `std::nullopt` nếu không tìm thấy
     */
    virtual std::optional<T> findById(const IdType& id) = 0;
    
    /**
     * @brief Lưu một thực thể vào kho lưu trữ.
     * 
     * @param entity Thực thể cần lưu
     * @return true Nếu thao tác lưu thành công
     * @return false Nếu thao tác lưu thất bại
     */
    virtual bool save(T& entity) = 0;
    
    /**
     * @brief Cập nhật thông tin của một thực thể trong kho lưu trữ.
     * 
     * @param entity Thực thể cần cập nhật
     * @return true Nếu thao tác cập nhật thành công
     * @return false Nếu thao tác cập nhật thất bại
     */
    virtual bool update(const T& entity) = 0;

    /**
     * @brief Xóa một thực thể khỏi kho lưu trữ theo Id.
     * 
     * @param id Khoá chính của thực thể cần xóa
     * @return true Nếu thao tác xóa thành công
     * @return false Nếu thao tác xóa thất bại
     */
    virtual bool remove(const IdType& id) = 0;
};

#endif // REPOSITORY_INTERFACE_H