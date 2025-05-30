/**
 * @file IEntity.h
 * @brief Interface cơ sở cho tất cả các thực thể domain trong hệ thống quản lý chuyến bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>

/**
 * @brief Lớp cơ sở trừu tượng cho tất cả các thực thể domain
 * 
 * Interface này định nghĩa hợp đồng chung mà tất cả các thực thể domain phải thực hiện.
 * Nó cung cấp cả định danh cấp độ cơ sở dữ liệu (qua ID số nguyên) và định danh
 * cấp độ nghiệp vụ (qua ID chuỗi) để hỗ trợ cả lưu trữ và logic domain.
 * 
 * Tất cả các thực thể được mong đợi là các value object bất biến với phương thức factory
 * để tạo và hỗ trợ nhân bản để sao chép an toàn.
 */
class IEntity {
protected:
    int _id;  ///< Định danh duy nhất được tạo bởi cơ sở dữ liệu

public:
    /**
     * @brief Destructor ảo để dọn dẹp đúng cách các lớp dẫn xuất
     */
    virtual ~IEntity() = default;

    /**
     * @brief Lấy định danh duy nhất được tạo bởi cơ sở dữ liệu
     * @return ID số nguyên được gán bởi cơ sở dữ liệu
     */
    virtual int getId() const = 0;

    /**
     * @brief Lấy định danh cấp độ nghiệp vụ dưới dạng chuỗi
     * @return Biểu diễn chuỗi của định danh nghiệp vụ (ví dụ: số hiệu chuyến bay, số hộ chiếu)
     */
    virtual std::string getStringId() const = 0;

    /**
     * @brief Lấy biểu diễn chuỗi của thực thể
     * @return Chuỗi có thể đọc được chứa tất cả thuộc tính của thực thể
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Kiểm tra tính bằng nhau với thực thể khác
     * @param other Thực thể để so sánh
     * @return true nếu các thực thể bằng nhau dựa trên ID của chúng, false nếu ngược lại
     */
    virtual bool equals(const IEntity& other) const = 0;

    /**
     * @brief Tạo bản sao sâu của thực thể
     * @return Unique pointer đến instance thực thể mới với giá trị giống hệt
     */
    virtual std::unique_ptr<IEntity> clone() const = 0;

    /**
     * @brief Đặt ID được tạo bởi cơ sở dữ liệu
     * @param id ID cơ sở dữ liệu để gán (thường được gọi sau khi chèn vào cơ sở dữ liệu)
     */
    virtual void setId(int id) = 0;

protected:
    /**
     * @brief Constructor được bảo vệ khởi tạo ID thành 0
     * Chỉ các lớp dẫn xuất mới có thể tạo instance
     */
    IEntity() : _id(0) {}
};

using EntityPtr = std::unique_ptr<IEntity>; ///< Type alias cho unique pointer thực thể

#endif