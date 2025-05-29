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
 * 
 * @details
 * Lớp này wrapper cho mysqlx::RowResult và cung cấp interface thống nhất
 * để truy xuất dữ liệu theo các kiểu khác nhau. Hỗ trợ cả truy xuất theo
 * chỉ số cột và tên cột.
 * 
 * Đặc điểm:
 * - Thread-safe trong phạm vi đơn result object
 * - Tự động chuyển đổi kiểu dữ liệu
 * - Xử lý giá trị NULL một cách an toàn
 * - Hỗ trợ lazy loading cho hiệu suất tốt
 */
class MySQLXResult : public IDatabaseResult {
private:
    mysqlx::RowResult _rowResult;            ///< Kết quả hàng từ truy vấn MySQL X DevAPI
    mysqlx::Row _currentRow;                 ///< Hàng hiện tại đang được xử lý
    std::vector<std::string> _columnNames;   ///< Danh sách tên cột từ metadata
    bool _hasData;                           ///< Cờ đánh dấu có dữ liệu trong result set
    bool _hasCurrentRow = false;             ///< Cờ kiểm tra hàng hiện tại đã được nạp chưa

public:
    /**
     * @brief Khởi tạo với đối tượng RowResult từ MySQL X DevAPI.
     * 
     * @param rowResult Kết quả truy vấn MySQL được truyền từ executeQuery
     * 
     * @details
     * Constructor sẽ:
     * - Lưu trữ RowResult object
     * - Trích xuất metadata của các cột
     * - Khởi tạo trạng thái ban đầu
     * 
     * @note RowResult được move vào object này để tránh copy không cần thiết
     */
    explicit MySQLXResult(mysqlx::RowResult rowResult);

    /**
     * @brief Destructor mặc định.
     * 
     * @details
     * Tự động giải phóng tài nguyên của MySQL X DevAPI.
     * Không cần xử lý đặc biệt do sử dụng RAII.
     */
    ~MySQLXResult() override = default;

    /**
     * @brief Di chuyển con trỏ đến hàng kết quả tiếp theo.
     * 
     * @return Result<bool> 
     *         - true nếu thành công và còn dữ liệu
     *         - false nếu đã đến cuối kết quả
     * 
     * @details
     * Phương thức này cập nhật _currentRow và trạng thái _hasData.
     * Nếu không còn hàng nào, _hasData được đặt là false.
     */
    Result<bool> next() override;

    /**
     * @brief Lấy giá trị kiểu chuỗi từ cột hiện tại.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<std::string> 
     *         - Giá trị chuỗi của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Phương thức này thực hiện:
     * - Kiểm tra chỉ số cột hợp lệ
     * - Truy xuất giá trị từ _currentRow
     * - Chuyển đổi kiểu nếu cần thiết
     * 
     * @note Xử lý cả trường hợp giá trị NULL
     */
    Result<std::string> getString(const int& columnIndex) override;

    /**
     * @brief Lấy giá trị kiểu nguyên từ cột hiện tại.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<int> 
     *         - Giá trị nguyên của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Tương tự như getString nhưng cho kiểu int.
     */
    Result<int> getInt(const int& columnIndex) override;

    /**
     * @brief Lấy giá trị kiểu số thực từ cột hiện tại.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<double> 
     *         - Giá trị số thực của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Tương tự như getString nhưng cho kiểu double.
     */
    Result<double> getDouble(const int& columnIndex) override;

    /**
     * @brief Lấy giá trị kiểu thời gian từ cột hiện tại.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<std::tm> 
     *         - Giá trị thời gian của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Tương tự như getString nhưng cho kiểu std::tm.
     */
    Result<std::tm> getDateTime(const int& columnIndex) override;

    /**
     * @brief Lấy giá trị kiểu chuỗi từ cột hiện tại theo tên cột.
     * 
     * @param columnName Tên cột
     * @return Result<std::string> 
     *         - Giá trị chuỗi của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Tương tự như getString nhưng cho phép truy xuất bằng tên cột.
     */
    Result<std::string> getString(const std::string& columnName) override;

    /**
     * @brief Lấy giá trị kiểu nguyên từ cột hiện tại theo tên cột.
     * 
     * @param columnName Tên cột
     * @return Result<int> 
     *         - Giá trị nguyên của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Tương tự như getInt nhưng cho phép truy xuất bằng tên cột.
     */
    Result<int> getInt(const std::string& columnName) override;

    /**
     * @brief Lấy giá trị kiểu số thực từ cột hiện tại theo tên cột.
     * 
     * @param columnName Tên cột
     * @return Result<double> 
     *         - Giá trị số thực của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Tương tự như getDouble nhưng cho phép truy xuất bằng tên cột.
     */
    Result<double> getDouble(const std::string& columnName) override;

    /**
     * @brief Lấy giá trị kiểu thời gian từ cột hiện tại theo tên cột.
     * 
     * @param columnName Tên cột
     * @return Result<std::tm> 
     *         - Giá trị thời gian của cột
     *         - Thông tin lỗi nếu có
     * 
     * @details
     * Tương tự như getDateTime nhưng cho phép truy xuất bằng tên cột.
     */
    Result<std::tm> getDateTime(const std::string& columnName) override;
};

/**
 * @struct PreparedStatementData
 * @brief Lưu trữ dữ liệu liên quan đến một prepared statement.
 *
 * Bao gồm chuỗi SQL và các giá trị tham số được ánh xạ theo chỉ số.
 * 
 * @details
 * Cấu trúc này được sử dụng nội bộ để quản lý prepared statements
 * trong MySQLXConnection. Mỗi statement có một ID duy nhất và
 * được lưu trữ trong map để tái sử dụng.
 */
struct PreparedStatementData {
    std::string query;                                   ///< Chuỗi truy vấn SQL gốc với placeholder
    std::unordered_map<int, std::string> paramValues;    ///< Giá trị tham số theo chỉ số (1-based)
};

/**
 * @class MySQLXConnection
 * @brief Hiện thực lớp IDatabaseConnection bằng MySQL X DevAPI.
 * 
 * Sử dụng giao thức X Protocol để kết nối và thao tác với MySQL Server.
 * Cung cấp đầy đủ chức năng truy vấn, statement, transaction và kiểm soát lỗi.
 * 
 * @details
 * Lớp này implements Singleton pattern để đảm bảo chỉ có một connection
 * active trong toàn bộ application. Điều này giúp:
 * - Tránh connection leak
 * - Quản lý resource hiệu quả
 * - Đảm bảo transaction consistency
 * 
 * Đặc điểm kỹ thuật:
 * - Thread-safe với std::mutex
 * - Connection pooling thông qua singleton
 * - Automatic resource management với RAII
 * - Comprehensive error handling với Result<T>
 * - Support cho prepared statements với parameter binding
 * - Full transaction control (ACID properties)
 */
class MySQLXConnection : public IDatabaseConnection {
private:
    static std::shared_ptr<MySQLXConnection> _instance; ///< Singleton instance duy nhất
    static std::mutex _instanceMutex;                   ///< Mutex bảo vệ việc tạo singleton

    std::unique_ptr<mysqlx::Session> _session; ///< Phiên làm việc với MySQL Server
    std::unordered_map<int, PreparedStatementData> _preparedStatements; ///< Bộ nhớ lưu các prepared statement
    std::string _lastError; ///< Mô tả lỗi gần nhất để debugging
    int _nextStatementId;   ///< Bộ đếm tạo ID duy nhất cho statement
    std::string _currentSchema; ///< Tên cơ sở dữ liệu đang sử dụng
    std::mutex _mutex; ///< Bảo vệ dữ liệu dùng chung trong môi trường đa luồng

    /**
     * @brief Xây dựng chuỗi SQL từ dữ liệu statement và giá trị tham số.
     * 
     * @param data Dữ liệu prepared statement chứa query và parameters
     * 
     * @return Result<std::string> 
     *         - Success: Chuỗi SQL đầy đủ đã thay thế tham số
     *         - Failure: Thiếu parameter hoặc lỗi format
     * 
     * @details
     * Phương thức này thực hiện:
     * - Thay thế các placeholder '?' bằng giá trị thực
     * - Escape các ký tự đặc biệt để tránh SQL injection
     * - Validate số lượng parameters với số placeholder
     * 
     * @note Parameter index bắt đầu từ 1, không phải 0
     */
    Result<std::string> buildPreparedStatement(const PreparedStatementData& data);

    /**
     * @brief Hàm khởi tạo riêng tư theo mẫu Singleton.
     * 
     * @details
     * Constructor được đặt private để ngăn việc tạo object trực tiếp.
     * Chỉ có thể tạo instance thông qua getInstance().
     * 
     * Khởi tạo:
     * - _nextStatementId = 1
     * - Các container rỗng
     * - Mutex trong trạng thái unlocked
     */
    MySQLXConnection();

public:
    /**
     * @brief Hàm hủy tự động disconnect khi object bị destroy.
     * 
     * @details
     * Đảm bảo tất cả tài nguyên được giải phóng:
     * - Đóng connection với MySQL
     * - Clear tất cả prepared statements
     * - Rollback transaction chưa commit
     */
    ~MySQLXConnection() override;

    // Ngăn copy constructor và assignment operator
    MySQLXConnection(const MySQLXConnection&) = delete;
    MySQLXConnection& operator=(const MySQLXConnection&) = delete;

    /**
     * @brief Lấy instance duy nhất của MySQLXConnection (Singleton pattern).
     * 
     * @return std::shared_ptr<MySQLXConnection> Smart pointer tới instance duy nhất
     * 
     * @details
     * Thread-safe implementation của Singleton pattern sử dụng double-checked locking.
     * Instance sẽ được tạo lần đầu tiên khi gọi phương thức này.
     * 
     * @note Sử dụng shared_ptr để automatic memory management
     * @note Thread-safe với std::mutex
     */
    static std::shared_ptr<MySQLXConnection> getInstance();

    // Implementation của IDatabaseConnection interface
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