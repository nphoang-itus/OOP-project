/**
 * @file DatabaseConnection.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Giao diện trừu tượng cho lớp kết nối cơ sở dữ liệu và xử lý kết quả truy vấn.
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 * Giao diện này định nghĩa các phương thức cần thiết cho việc giao tiếp với cơ sở dữ liệu,
 * bao gồm thực thi truy vấn, quản lý prepared statement, và xử lý transaction.
 * Thiết kế tuân theo nguyên lý SOLID, đặc biệt là Dependency Inversion Principle,
 * giúp tách biệt giữa tầng xử lý nghiệp vụ và tầng dữ liệu.
 */

#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <memory>
#include <ctime>
#include "../core/exceptions/Result.h"

/**
* @interface IDatabaseResult
* @brief Đại diện cho kết quả trả về từ truy vấn SELECT trong cơ sở dữ liệu.
*
* Cung cấp các phương thức để duyệt dữ liệu hàng loạt và truy xuất các giá trị theo chỉ số hoặc tên cột.
*/
class IDatabaseResult {
public:
    virtual ~IDatabaseResult() = default;

    /**
    * @brief Di chuyển đến hàng tiếp theo trong kết quả truy vấn.
    * @return Result<bool> true nếu có hàng tiếp theo, false nếu hết kết quả.
    */
    virtual Result<bool> next() = 0;

    /// @name Truy xuất dữ liệu theo chỉ số cột
    /// @{
    virtual Result<std::string> getString(const int& columnIndex) = 0;
    virtual Result<int> getInt(const int& columnIndex) = 0;
    virtual Result<double> getDouble(const int& columnIndex) = 0;
    virtual Result<std::tm> getDateTime(const int& columnIndex) = 0;
    /// @}

    /// @name Truy xuất dữ liệu theo tên cột
    /// @{
    virtual Result<std::string> getString(const std::string& columnName) = 0;
    virtual Result<int> getInt(const std::string& columnName) = 0;
    virtual Result<double> getDouble(const std::string& columnName) = 0;
    virtual Result<std::tm> getDateTime(const std::string& columnName) = 0;
    /// @}
};

/**
* @interface IDatabaseConnection
* @brief Giao diện trừu tượng đại diện cho kết nối đến cơ sở dữ liệu.
* 
* Cho phép quản lý truy vấn SQL, transaction, và các thao tác với prepared statement.
*/
class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default;

    /**
    * @brief Thiết lập kết nối đến cơ sở dữ liệu.
    * @param host Tên máy chủ
    * @param user Tên người dùng
    * @param password Mật khẩu
    * @param database Tên cơ sở dữ liệu
    * @param port Cổng kết nối (mặc định là 33060 cho XAPIDev của MySQL)
    * @return Result<bool> true nếu kết nối thành công
    */
    virtual Result<bool> connect(const std::string& host, const std::string& user,
                         const std::string& password, const std::string& database,
                         const int& port = 33060) = 0;

    /**
    * @brief Ngắt kết nối hiện tại khỏi cơ sở dữ liệu.
    * @return Result<void>
    */
    virtual VoidResult disconnect() = 0;

    /**
    * @brief Thực thi câu lệnh SQL không có kết quả trả về (INSERT, UPDATE, DELETE).
    * @param query Câu truy vấn SQL
    * @return Result<bool> true nếu thực thi thành công
    */
    virtual Result<bool> execute(const std::string& query) = 0;

    /**
    * @brief Thực thi truy vấn SQL có kết quả trả về (SELECT).
    * @param query Câu truy vấn SQL
    * @return Result<std::unique_ptr<IDatabaseResult>> Con trỏ tới đối tượng kết quả
    */
    virtual Result<std::unique_ptr<IDatabaseResult>> executeQuery(const std::string& query) = 0;

    /**
    * @brief Tạo prepared statement với các placeholder (? hoặc :param).
    * @param query Chuỗi SQL có placeholder
    * @return Result<int> ID định danh của prepared statement
    */
    virtual Result<int> prepareStatement(const std::string& query) = 0;

    /// @name Gán giá trị tham số cho prepared statement
    /// @{
    virtual VoidResult setString(const int& statementId, const int& paramIndex, const std::string& value) = 0;
    virtual VoidResult setInt(const int& statementId, const int& paramIndex, const int& value) = 0;
    virtual VoidResult setDouble(const int& statementId, const int& paramIndex, const double& value) = 0;
    virtual VoidResult setDateTime(const int& statementId, const int& paramIndex, const std::tm& value) = 0;
    /// @}

    /**
    * @brief Thực thi prepared statement không trả về kết quả.
    * @param statementId ID của statement
    * @return Result<bool> true nếu thành công
    */
    virtual Result<bool> executeStatement(const int& statementId) = 0;

    /**
    * @brief Thực thi prepared statement có trả về kết quả.
    * @param statementId ID của statement
    * @return Result<std::unique_ptr<IDatabaseResult>> Con trỏ tới kết quả trả về
    */
    virtual Result<std::unique_ptr<IDatabaseResult>> executeQueryStatement(const int& statementId) = 0;

    /**
    * @brief Giải phóng tài nguyên của prepared statement.
    * @param statementId ID của statement
    * @return VoidResult
    */
    virtual VoidResult freeStatement(const int& statementId) = 0;

    /**
    * @brief Lấy ID của bản ghi được chèn gần nhất.
    * @return Result<int> ID của bản ghi vừa thêm
    */
    virtual Result<int> getLastInsertId() = 0;

    /**
    * @brief Kiểm tra trạng thái kết nối.
    * @return Result<bool> true nếu đã kết nối
    */
    virtual Result<bool> isConnected() const = 0;

    /**
    * @brief Trả về thông báo lỗi gần nhất.
    * @return Result<std::string> Chuỗi mô tả lỗi
    */
    virtual Result<std::string> getLastError() const = 0;

    /// @name Transaction Control
    /// @{
    virtual Result<bool> beginTransaction() = 0;
    virtual Result<bool> commitTransaction() = 0;
    virtual Result<bool> rollbackTransaction() = 0;
    /// @}
};

#endif 