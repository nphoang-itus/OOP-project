/**
 * @file MySQLConnection.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "DatabaseConnection.h"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <memory>
#include <unordered_map>

#ifndef MYSQL_CONNECTION_H
#define MYSQL_CONNECTION_H

class MySQLResult : public IDatabaseResult {
private:
    std::shared_ptr<sql::ResultSet> _resultSet;

public:
     explicit MySQLResult(std::shared_ptr<sql::ResultSet> resultSet);
     ~MySQLResult() override = default;
     
     bool next() override;

     std::string getString(int columnIndex) override;
     int getInt(int columnIndex) override;
     double getDouble(int columnIndex) override;
     
     std::string getString(const std::string& columnName) override;
     int getInt(const std::string& columnName) override;
     double getDouble(const std::string& columnName) override;
};

/**
 * @class MySQLConnection
 * @brief Triển khai DatabaseResult cho MySQL
 */
class MySQLConnection : public IDatabaseConnection {
private:
    sql::Driver* _driver; ///< Driver MySQL
    std::unique_ptr<sql::Connection> _connection; ///< Kết nối MySQL
    std::unordered_map<int, std::shared_ptr<sql::PreparedStatement>> _preparedStatements; ///< Map lưu trữ prepared statements
    std::string _lastError; ///< Thông báo lỗi gần nhất
    int _nextStatementId; ///< ID cho prepared statement tiếp theo

public:
    MySQLConnection();
    ~MySQLConnection() override;
    
    bool connect(const std::string& host, const std::string& user, 
                 const std::string& password, const std::string& database, 
                 int port = 3306) override;
    void disconnect() override;

    bool execute(const std::string& query) override;
    std::unique_ptr<IDatabaseResult> executeQuery(const std::string& query) override;
    int prepareStatement(const std::string& query) override;

    void setString(int statementId, int paramIndex, const std::string& value) override;
    void setInt(int statementId, int paramIndex, int value) override;
    void setDouble(int statementId, int paramIndex, double value) override;
    bool executeStatement(int statementId) override;
    std::unique_ptr<IDatabaseResult> executeQueryStatement(int statementId) override;
    void freeStatement(int statementId) override;
    int getLastInsertId() override;
    bool isConnected() const override;
    std::string getLastError() const override;
    bool beginTransaction() override;
    bool commitTransaction() override;
    bool rollbackTransaction() override;
};

#endif