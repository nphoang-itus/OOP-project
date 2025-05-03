/**
 * @file MySQLXConnection.h
 * @author Your Name
 * @brief Implementation of DatabaseConnection interface using MySQL X DevAPI
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef MYSQLX_CONNECTION_H
#define MYSQLX_CONNECTION_H

#include "DatabaseConnection.h"
#include <mysqlx/xdevapi.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>

/**
 * @class MySQLXResult
 * @brief Implementation of DatabaseResult for MySQL X DevAPI
 */
class MySQLXResult : public IDatabaseResult {
private:
    mysqlx::RowResult _rowResult;
    mysqlx::Row _currentRow;
    std::vector<std::string> _columnNames;
    bool _hasData;
    bool _hasCurrentRow = false;

public:
    explicit MySQLXResult(mysqlx::RowResult rowResult);
    ~MySQLXResult() override = default;
    
    bool next() override;
    std::string getString(int columnIndex) override;
    int getInt(int columnIndex) override;
    double getDouble(int columnIndex) override;
    std::chrono::sys_time<std::chrono::seconds> getDateTime(int columnIndex) override;
    
    std::string getString(const std::string& columnName) override;
    int getInt(const std::string& columnName) override;
    double getDouble(const std::string& columnName) override;
    std::chrono::sys_time<std::chrono::seconds> getDateTime(const std::string& columnName) override;
};

/**
 * @struct PreparedStatementData
 * @brief Structure to hold prepared statement data
 */
struct PreparedStatementData {
    std::string query;
    std::unordered_map<int, std::string> paramValues;
};

/**
 * @class MySQLXConnection
 * @brief Implementation of DatabaseConnection using MySQL X DevAPI
 */
class MySQLXConnection : public IDatabaseConnection {
private:
    std::unique_ptr<mysqlx::Session> _session;
    std::unordered_map<int, PreparedStatementData> _preparedStatements;
    std::string _lastError;
    int _nextStatementId;
    std::string _currentSchema;
    std::mutex _mutex; // For thread safety
    
    /**
     * @brief Build a SQL query with parameters replaced
     * @param data The prepared statement data
     * @return SQL query with parameters replaced
     */
    std::string buildPreparedStatement(const PreparedStatementData& data);

public:
    MySQLXConnection();
    ~MySQLXConnection() override;
    
    bool connect(const std::string& host, const std::string& user, 
                const std::string& password, const std::string& database, 
                int port = 33060) override; // X Protocol default port is 33060
    void disconnect() override;
    
    bool execute(const std::string& query) override;
    std::unique_ptr<IDatabaseResult> executeQuery(const std::string& query) override;
    
    int prepareStatement(const std::string& query) override;
    void setString(int statementId, int paramIndex, const std::string& value) override;
    void setInt(int statementId, int paramIndex, int value) override;
    void setDouble(int statementId, int paramIndex, double value) override;
    void setDateTime(int statementId, int paramIndex, const std::chrono::sys_time<std::chrono::seconds>& value) override;
    
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

#endif // MYSQLX_CONNECTION_H