/**
 * @file MySQLXConnection.h
 * @author 
 * @brief Triển khai giao diện kết nối cơ sở dữ liệu sử dụng MySQL X DevAPI (X Protocol).
 * @version 0.1
 * @date 2025-05-02
 * 
 * @details
 * Lớp MySQLXConnection là hiện thực cụ thể của giao diện IDatabaseConnection.
 * Sử dụng thư viện MySQL Connector/C++ 8.x với X DevAPI (dựa trên X Protocol).
 * Hỗ trợ chuẩn hóa việc truy vấn, chuẩn bị statement, và xử lý transaction.
 * Thiết kế chú trọng đến khả năng mở rộng và an toàn luồng (thread-safe).
 */

#ifndef MYSQLX_CONNECTION_H
#define MYSQLX_CONNECTION_H

#include "InterfaceDatabaseConnection.h"
#include <mysqlx/xdevapi.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <ctime>

/**
 * @class MySQLXResult
 * @brief Hiện thực lớp IDatabaseResult cho MySQL X DevAPI.
 *
 * Quản lý và trích xuất dữ liệu từ kết quả trả về của truy vấn SQL SELECT.
 */
class MySQLXResult : public IDatabaseResult {
private:
    mysqlx::RowResult _rowResult;            ///< Kết quả hàng từ truy vấn
    mysqlx::Row _currentRow;                 ///< Hàng hiện tại đang xử lý
    std::vector<std::string> _columnNames;   ///< Danh sách tên cột
    bool _hasData;                           ///< Cờ đánh dấu có dữ liệu
    bool _hasCurrentRow = false;             ///< Cờ kiểm tra hàng hiện tại đã được nạp chưa

public:
    /**
     * @brief Khởi tạo với đối tượng RowResult từ MySQL.
     * @param rowResult Kết quả truy vấn MySQL
     */
    explicit MySQLXResult(mysqlx::RowResult rowResult);

    ~MySQLXResult() override = default;

    Result<bool> next() override;
    Result<std::string> getString(const int& columnIndex) override;
    Result<int> getInt(const int& columnIndex) override;
    Result<double> getDouble(const int& columnIndex) override;
    Result<std::tm> getDateTime(const int& columnIndex) override;

    Result<std::string> getString(const std::string& columnName) override;
    Result<int> getInt(const std::string& columnName) override;
    Result<double> getDouble(const std::string& columnName) override;
    Result<std::tm> getDateTime(const std::string& columnName) override;
};

/**
 * @struct PreparedStatementData
 * @brief Lưu trữ dữ liệu liên quan đến một prepared statement.
 *
 * Bao gồm chuỗi SQL và các giá trị tham số được ánh xạ theo chỉ số.
 */
struct PreparedStatementData {
    std::string query;                                   ///< Chuỗi truy vấn SQL
    std::unordered_map<int, std::string> paramValues;    ///< Giá trị tham số theo chỉ số
};

/**
 * @class MySQLXConnection
 * @brief Hiện thực lớp IDatabaseConnection bằng MySQL X DevAPI.
 * 
 * Sử dụng giao thức X Protocol để kết nối và thao tác với MySQL Server.
 * Cung cấp đầy đủ chức năng truy vấn, statement, transaction và kiểm soát lỗi.
 */
class MySQLXConnection : public IDatabaseConnection {
private:
    static std::shared_ptr<MySQLXConnection> _instance;
    static std::mutex _instanceMutex;

    std::unique_ptr<mysqlx::Session> _session; ///< Phiên làm việc với MySQL
    std::unordered_map<int, PreparedStatementData> _preparedStatements; ///< Bộ nhớ lưu các statement
    std::string _lastError; ///< Mô tả lỗi gần nhất
    int _nextStatementId;   ///< Tạo ID duy nhất cho statement
    std::string _currentSchema; ///< Tên cơ sở dữ liệu đang sử dụng
    std::mutex _mutex; ///< Bảo vệ dữ liệu dùng chung trong môi trường đa luồng

    /**
     * @brief Xây dựng chuỗi SQL từ dữ liệu statement và giá trị tham số.
     * @param data Dữ liệu statement
     * @return Result<std::string> Chuỗi SQL đầy đủ đã thay thế tham số
     */
    Result<std::string> buildPreparedStatement(const PreparedStatementData& data);

    /**
     * @brief Hàm khởi tạo theo mẫu Singleton: Không cho tự phép tạo đối tượng.
     */
    MySQLXConnection();
public:
    /**
     * @brief Hàm hủy.
     */
    ~MySQLXConnection() override;

    MySQLXConnection(const MySQLXConnection&) = delete;
    MySQLXConnection& operator=(const MySQLXConnection&) = delete;

    static std::shared_ptr<MySQLXConnection> getInstance();

    Result<bool> connect(const std::string& host, const std::string& user,
                 const std::string& password, const std::string& database,
                 const int& port = 33060) override;

    VoidResult disconnect() override;

    Result<bool> execute(const std::string& query) override;
    Result<std::unique_ptr<IDatabaseResult>> executeQuery(const std::string& query) override;

    Result<int> prepareStatement(const std::string& query) override;
    VoidResult setString(const int& statementId, const int& paramIndex, const std::string& value) override;
    VoidResult setInt(const int& statementId, const int& paramIndex, const int& value) override;
    VoidResult setDouble(const int& statementId, const int& paramIndex, const double& value) override;
    VoidResult setDateTime(const int& statementId, const int& paramIndex, const std::tm& value) override;

    Result<bool> executeStatement(const int& statementId) override;
    Result<std::unique_ptr<IDatabaseResult>> executeQueryStatement(const int& statementId) override;
    VoidResult freeStatement(const int& statementId) override;

    Result<int> getLastInsertId() override;
    Result<bool> isConnected() const override;
    Result<std::string> getLastError() const override;

    Result<bool> beginTransaction() override;
    Result<bool> commitTransaction() override;
    Result<bool> rollbackTransaction() override;
};

#endif // MYSQLX_CONNECTION_H