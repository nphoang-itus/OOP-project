/**
 * @file DatabaseConnection.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <memory>
#include <chrono>

class IDatabaseResult {
public:
    virtual ~IDatabaseResult() = default;

    /**
     * @brief Di chuyển đến hàng tiếp theo trong kết quả
     * 
     * @return true Nếu có hàng tiếp theo
     * @return false Nếu không có hàng tiếp theo (Hết kết quả truy vấn)
     */
    virtual bool next() = 0;
    
    // Lấy giá trị từ cột được chỉ định
    virtual std::string getString(int columnIndex) = 0;
    virtual int getInt(int columnIndex) = 0;
    virtual double getDouble(int columnIndex) = 0;
    virtual std::chrono::sys_time<std::chrono::seconds> getDateTime(int columnIndex) = 0;

    // Lấy giá trị chuỗi từ tên cột
    virtual std::string getString(const std::string& columnName) = 0;
    virtual int getInt(const std::string& columnName) = 0;
    virtual double getDouble(const std::string& columnName) = 0;
    virtual std::chrono::sys_time<std::chrono::seconds> getDateTime(const std::string& columnName) = 0;
};

class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default;

    /**
     * @brief Kết nối đến cơ sở dữ liệu
     * @param host Tên máy chủ
     * @param user Tên người dùng
     * @param password Mật khẩu
     * @param database Tên cơ sở dữ liệu
     * @param port Cổng (mặc định kết nối tới MySQL là 3306)
     * @return true nếu kết nối thành công, false nếu không
     */
    virtual bool connect(const std::string& host, const std::string& user, 
                        const std::string& password, const std::string& database, 
                        int port = 3306) = 0;

    /**
 * @brief Ngắt kết nối từ cơ sở dữ liệu
 */
    virtual void disconnect() = 0;

    /**
 * @brief Thực thi truy vấn không trả về kết quả (INSERT, UPDATE, DELETE)
 * @param query Chuỗi truy vấn SQL
 * @return true nếu thực thi thành công, false nếu không
 */
    virtual bool execute(const std::string& query) = 0;

    /**
 * @brief Thực thi truy vấn trả về kết quả (SELECT)
 * @param query Chuỗi truy vấn SQL
 * @return Con trỏ đến đối tượng DatabaseResult
 */
    virtual std::unique_ptr<IDatabaseResult> executeQuery(const std::string& query) = 0;

    /**
 * @brief Tạo prepared statement - Query với các dấu placeholders(? hoặc :paramName)
 * @param query Chuỗi truy vấn SQL với placeholders
 * @return ID của prepared statement
 */
    virtual int prepareStatement(const std::string& query) = 0;

    /**
 * @brief Thiết lập tham số chuỗi cho prepared statement
 * @param statementId ID của prepared statement
 * @param paramIndex Chỉ số tham số (bắt đầu từ 1)
 * @param value Giá trị chuỗi
 */
    virtual void setString(int statementId, int paramIndex, const std::string& value) = 0;

    /**
 * @brief Thiết lập tham số số nguyên cho prepared statement
 * @param statementId ID của prepared statement
 * @param paramIndex Chỉ số tham số (bắt đầu từ 1)
 * @param value Giá trị số nguyên
 */
    virtual void setInt(int statementId, int paramIndex, int value) = 0;

    /**
 * @brief Thiết lập tham số số thực cho prepared statement
 * @param statementId ID của prepared statement
 * @param paramIndex Chỉ số tham số (bắt đầu từ 1)
 * @param value Giá trị số thực
 */
    virtual void setDouble(int statementId, int paramIndex, double value) = 0;

    virtual void setDateTime(int statementId, int paramIndex, const std::chrono::sys_time<std::chrono::seconds>& value) = 0;

    /**
 * @brief Thực thi prepared statement không trả về kết quả
 * @param statementId ID của prepared statement
 * @return true nếu thực thi thành công, false nếu không
 */
    virtual bool executeStatement(int statementId) = 0;

    /**
 * @brief Thực thi prepared statement trả về kết quả
 * @param statementId ID của prepared statement
 * @return Con trỏ đến đối tượng DatabaseResult
 */
    virtual std::unique_ptr<IDatabaseResult> executeQueryStatement(int statementId) = 0;

    /**
 * @brief Giải phóng prepared statement
 * @param statementId ID của prepared statement
 */
    virtual void freeStatement(int statementId) = 0;

    /**
 * @brief Lấy ID của bản ghi được chèn gần nhất
 * @return ID của bản ghi
 */
    virtual int getLastInsertId() = 0;

    /**
 * @brief Kiểm tra trạng thái kết nối
 * @return true nếu đã kết nối, false nếu không
 */
    virtual bool isConnected() const = 0;

    /**
 * @brief Lấy thông báo lỗi gần nhất
 * @return Chuỗi thông báo lỗi
 */
    virtual std::string getLastError() const = 0;

    /**
 * @brief Bắt đầu một transaction
 * @return true nếu thành công, false nếu không
 */
    virtual bool beginTransaction() = 0;

    /**
 * @brief Commit một transaction
 * @return true nếu thành công, false nếu không
 */
    virtual bool commitTransaction() = 0;

    /**
 * @brief Rollback một transaction (Huỷ giao dịch khi đang thực hiện, xảy ra khi một giao dịch đơn lẻ bị lỗi)
 * @return true nếu thành công, false nếu không
 */
    virtual bool rollbackTransaction() = 0;
};

#endif