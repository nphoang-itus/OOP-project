/**
 * @file MySQLXConnection.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Cài đặt các hàm phục vụ chức năng kết nối và trả về kết quả từ Database sử dụng XAPIDev của MySQL
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "MySQLXConnection.h"
#include "../utils/Logger.h"
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <ctime>

// MySQLXResult implementation
MySQLXResult::MySQLXResult(mysqlx::RowResult rowResult) 
    : _rowResult(std::move(rowResult)), _hasData(false) {
    
    auto logger = Logger::getInstance();
    logger->debug("Creating result object from MySQL X DevAPI RowResult");
    
    try {
        // Get column names
        _columnNames.clear();
        const auto& columns = _rowResult.getColumns();
        for (auto col : columns) {
            _columnNames.push_back(col.getColumnName());
        }
        
        logger->debug("Retrieved " + std::to_string(_columnNames.size()) + " column names");
    }
    catch (const std::exception& e) {
        logger->error("Error while retrieving column metadata: " + std::string(e.what()));
    }
}

bool MySQLXResult::next() {
    auto logger = Logger::getInstance();
    
    try {
        if (_hasCurrentRow) {
            // We already fetched a row last time, get next one
            _currentRow = _rowResult.fetchOne();
            if (_currentRow) {
                logger->debug("Retrieved next row from result set");
                return true;
            }
            else {
                _hasCurrentRow = false;
                logger->debug("No more rows in result set");
                return false;
            }
        }
        else {
            // First call to next(), fetch first row
            _currentRow = _rowResult.fetchOne();
            if (_currentRow) {
                _hasCurrentRow = true;
                logger->debug("Retrieved first row from result set");
                return true;
            }
            else {
                logger->debug("Result set is empty");
                return false;
            }
        }
    }
    catch (const mysqlx::Error& e) {
        logger->error("MySQL error in result set navigation: " + std::string(e.what()));
        throw std::runtime_error("Error in result set: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        logger->error("Unexpected error in result set navigation: " + std::string(e.what()));
        throw std::runtime_error("Error in result set: " + std::string(e.what()));
    }
}

std::string MySQLXResult::getString(const int& columnIndex) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get string data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(_currentRow.colCount())) {
            logger->error("Column index out of range: " + std::to_string(columnIndex) + 
                        " (max: " + std::to_string(_currentRow.colCount() - 1) + ")");
            throw std::out_of_range("Column index out of range");
        }

        std::string value = _currentRow[columnIndex].get<std::string>();
        logger->debug("Retrieved string value from column " + std::to_string(columnIndex));
        return value;
    }
    catch (const std::exception& e) {
        logger->error("Error getting string data from column " + std::to_string(columnIndex) + 
                    ": " + std::string(e.what()));
        throw std::runtime_error("Error getting string data: " + std::string(e.what()));
    }
}

int MySQLXResult::getInt(const int& columnIndex) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get integer data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(_currentRow.colCount())) {
            logger->error("Column index out of range: " + std::to_string(columnIndex) + 
                        " (max: " + std::to_string(_currentRow.colCount() - 1) + ")");
            throw std::out_of_range("Column index out of range");
        }
        
        int value = _currentRow[columnIndex].get<int>();
        logger->debug("Retrieved integer value " + std::to_string(value) + 
                    " from column " + std::to_string(columnIndex));
        return value;
    }
    catch (const std::exception& e) {
        logger->error("Error getting integer data from column " + std::to_string(columnIndex) + 
                    ": " + std::string(e.what()));
        throw std::runtime_error("Error getting integer data: " + std::string(e.what()));
    }
}

double MySQLXResult::getDouble(const int& columnIndex) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get double data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(_currentRow.colCount())) {
            logger->error("Column index out of range: " + std::to_string(columnIndex) + 
                        " (max: " + std::to_string(_currentRow.colCount() - 1) + ")");
            throw std::out_of_range("Column index out of range");
        }
        
        double value = _currentRow[columnIndex].get<double>();
        logger->debug("Retrieved double value " + std::to_string(value) + 
                    " from column " + std::to_string(columnIndex));
        return value;
    }
    catch (const std::exception& e) {
        logger->error("Error getting double data from column " + std::to_string(columnIndex) + 
                    ": " + std::string(e.what()));
        throw std::runtime_error("Error getting double data: " + std::string(e.what()));
    }
}

std::tm MySQLXResult::getDateTime(const int& columnIndex) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get double data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(_currentRow.colCount())) {
            logger->error("Column index out of range: " + std::to_string(columnIndex) + 
                        " (max: " + std::to_string(_currentRow.colCount() - 1) + ")");
            throw std::out_of_range("Column index out of range");
        }
        
        // Lấy raw bytes
        mysqlx::bytes raw = _currentRow[columnIndex].getRawBytes();
        const unsigned char* data = raw.begin();
        size_t len = raw.size();

        int year = ((data[1] << 8) | data[0]) - 2048;
        int month = data[2];
        int day = data[3];
        int hour = data[4];
        int minute = data[5];
        int second = (len >= 7) ? data[6] : 0;

        std::tm tm_time{};
        tm_time.tm_year = year - 1900;
        tm_time.tm_mon  = month - 1;
        tm_time.tm_mday = day;
        tm_time.tm_hour = hour;
        tm_time.tm_min  = minute;
        tm_time.tm_sec  = second;

        std::time_t time_c = std::mktime(&tm_time);
        if (time_c == -1) {
            logger->error("Failed to convert decoded datetime to std::time_t");
            throw std::runtime_error("Invalid datetime components");
        }

        return tm_time;
    }
    catch (const std::exception& e) {
        logger->error("Error getting double data from column " + std::to_string(columnIndex) + 
                    ": " + std::string(e.what()));
        throw std::runtime_error("Error getting double data: " + std::string(e.what()));
    }
}

std::string MySQLXResult::getString(const std::string& columnName) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get string data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        // Find column index by name
        auto it = std::find(_columnNames.begin(), _columnNames.end(), columnName);
        if (it == _columnNames.end()) {
            logger->error("Column '" + columnName + "' not found in result set");
            throw std::out_of_range("Column '" + columnName + "' not found");
        }
        
        int index = std::distance(_columnNames.begin(), it);
        logger->debug("Found column '" + columnName + "' at index " + std::to_string(index));
        return getString(index);
    }
    catch (const std::exception& e) {
        logger->error("Error getting string data from column '" + columnName + "': " + 
                    std::string(e.what()));
        throw std::runtime_error("Error getting string data: " + std::string(e.what()));
    }
}

int MySQLXResult::getInt(const std::string& columnName) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get integer data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        // Find column index by name
        auto it = std::find(_columnNames.begin(), _columnNames.end(), columnName);
        if (it == _columnNames.end()) {
            logger->error("Column '" + columnName + "' not found in result set");
            throw std::out_of_range("Column '" + columnName + "' not found");
        }
        
        int index = std::distance(_columnNames.begin(), it);
        logger->debug("Found column '" + columnName + "' at index " + std::to_string(index));
        return getInt(index);
    }
    catch (const std::exception& e) {
        logger->error("Error getting integer data from column '" + columnName + "': " + 
                    std::string(e.what()));
        throw std::runtime_error("Error getting integer data: " + std::string(e.what()));
    }
}

double MySQLXResult::getDouble(const std::string& columnName) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get double data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        // Find column index by name
        auto it = std::find(_columnNames.begin(), _columnNames.end(), columnName);
        if (it == _columnNames.end()) {
            logger->error("Column '" + columnName + "' not found in result set");
            throw std::out_of_range("Column '" + columnName + "' not found");
        }
        
        int index = std::distance(_columnNames.begin(), it);
        logger->debug("Found column '" + columnName + "' at index " + std::to_string(index));
        return getDouble(index);
    }
    catch (const std::exception& e) {
        logger->error("Error getting double data from column '" + columnName + "': " + 
                    std::string(e.what()));
        throw std::runtime_error("Error getting double data: " + std::string(e.what()));
    }
}

std::tm MySQLXResult::getDateTime(const std::string& columnName) {
    auto logger = Logger::getInstance();
    
    if (!_hasCurrentRow) {
        logger->error("Attempted to get datetime data without an active row");
        throw std::runtime_error("No current row available");
    }
    
    try {
        // Find column index by name
        auto it = std::find(_columnNames.begin(), _columnNames.end(), columnName);
        if (it == _columnNames.end()) {
            logger->error("Column '" + columnName + "' not found in result set");
            throw std::out_of_range("Column '" + columnName + "' not found");
        }
        
        int index = std::distance(_columnNames.begin(), it);
        logger->debug("Found column '" + columnName + "' at index " + std::to_string(index));
        return getDateTime(index);
    }
    catch (const std::exception& e) {
        logger->error("Error getting datetime data from column '" + columnName + "': " + 
                    std::string(e.what()));
        throw std::runtime_error("Error getting datetime data: " + std::string(e.what()));
    }
}

// MySQLXConnection implementation
MySQLXConnection::MySQLXConnection() : _nextStatementId(1) {
    auto logger = Logger::getInstance();
    logger->debug("MySQLXConnection instance created");
}

MySQLXConnection::~MySQLXConnection() {
    auto logger = Logger::getInstance();
    logger->debug("MySQLXConnection instance being destroyed");
    disconnect();
}

bool MySQLXConnection::connect( const std::string& host, const std::string& user, 
                                const std::string& password, const std::string& database, 
                                const int& port) {
    auto logger = Logger::getInstance();
    logger->info("Attempting to connect to MySQL server at " + host + ":" + std::to_string(port) + 
                " with user '" + user + "' and database '" + database + "'");
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        // Create session with connection options
        _session = std::make_unique<mysqlx::Session>(
            mysqlx::SessionOption::HOST, host,
            mysqlx::SessionOption::PORT, port,
            mysqlx::SessionOption::USER, user,
            mysqlx::SessionOption::PWD, password
        );
        
        // Set the default schema
        _session->sql("USE " + database).execute();
        _currentSchema = database;
        
        logger->info("Successfully connected to MySQL database '" + database + "'");
        return true;
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL connection error: " + std::string(e.what()));
        return false;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error while connecting to MySQL: " + std::string(e.what()));
        return false;
    }
}

void MySQLXConnection::disconnect() {
    auto logger = Logger::getInstance();
    logger->info("Disconnecting from MySQL database");
    
    std::lock_guard<std::mutex> lock(_mutex);
    
    try {
        // Clear all prepared statements
        _preparedStatements.clear();
        logger->debug("Cleared all prepared statements");
        
        // Close the session
        if (_session) {
            _session.reset();
            logger->info("Successfully disconnected from MySQL database");
        } else {
            logger->warning("Disconnect called when no active connection exists");
        }
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Error during disconnect: " + std::string(e.what()));
    }
}

bool MySQLXConnection::execute(const std::string& query) {
    auto logger = Logger::getInstance();
    logger->debug("Executing SQL: " + query);
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (!_session) {
            _lastError = "Not connected to database";
            logger->error("Cannot execute SQL: No active database connection");
            return false;
        }
        
        _session->sql(query).execute();
        logger->debug("SQL executed successfully");
        return true;
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error executing SQL: " + std::string(e.what()));
        return false;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error executing SQL: " + std::string(e.what()));
        return false;
    }
}

std::unique_ptr<IDatabaseResult> MySQLXConnection::executeQuery(const std::string& query) {
    auto logger = Logger::getInstance();
    logger->debug("Executing query: " + query);
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (!_session) {
            _lastError = "Not connected to database";
            logger->error("Attempted to execute query without an active database connection");
            return nullptr;
        }
        
        mysqlx::SqlResult result = _session->sql(query).execute();
        logger->debug("Query executed successfully");
        
        // Check if this is a result-producing query
        if (result.hasData()) {
            return std::make_unique<MySQLXResult>(std::move(result));
        } else {
            logger->debug("Query did not return any data");
            return nullptr;
        }
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error executing query: " + std::string(e.what()));
        return nullptr;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error executing query: " + std::string(e.what()));
        return nullptr;
    }
}

int MySQLXConnection::prepareStatement(const std::string& query) {
    auto logger = Logger::getInstance();
    logger->debug("Preparing SQL statement: " + query);
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (!_session) {
            _lastError = "Not connected to database";
            logger->error("Cannot prepare statement: No active database connection");
            return -1;
        }
        
        // Store prepared statement data
        PreparedStatementData data;
        data.query = query;
        data.paramValues.clear();
        
        int id = _nextStatementId++;
        _preparedStatements[id] = data;
        
        logger->debug("Statement prepared successfully with ID: " + std::to_string(id));
        return id;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Error preparing statement: " + std::string(e.what()));
        return -1;
    }
}

void MySQLXConnection::setString(const int& statementId, const int& paramIndex, const std::string& value) {
    auto logger = Logger::getInstance();
    logger->debug("Binding string parameter at index " + std::to_string(paramIndex) + 
                " for statement ID " + std::to_string(statementId));
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            logger->error("Cannot bind parameter: Invalid statement ID " + std::to_string(statementId));
            return;
        }
        
        // Store parameter value
        PreparedStatementData& data = it->second;
        data.paramValues[paramIndex] = value;
        logger->debug("String parameter bound successfully");
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Error binding string parameter: " + std::string(e.what()));
    }
}

void MySQLXConnection::setInt(const int& statementId, const int& paramIndex, const int& value) {
    auto logger = Logger::getInstance();
    logger->debug("Binding integer parameter " + std::to_string(value) + " at index " + 
                std::to_string(paramIndex) + " for statement ID " + std::to_string(statementId));
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            logger->error("Cannot bind parameter: Invalid statement ID " + std::to_string(statementId));
            return;
        }
        
        // Store parameter value
        PreparedStatementData& data = it->second;
        data.paramValues[paramIndex] = std::to_string(value);
        logger->debug("Integer parameter bound successfully");
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Error binding integer parameter: " + std::string(e.what()));
    }
}

void MySQLXConnection::setDouble(const int& statementId, const int& paramIndex, const double& value) {
    auto logger = Logger::getInstance();
    logger->debug("Binding double parameter " + std::to_string(value) + " at index " + 
                std::to_string(paramIndex) + " for statement ID " + std::to_string(statementId));
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            logger->error("Cannot bind parameter: Invalid statement ID " + std::to_string(statementId));
            return;
        }
        
        // Store parameter value
        PreparedStatementData& data = it->second;
        data.paramValues[paramIndex] = std::to_string(value);
        logger->debug("Double parameter bound successfully");
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Error binding double parameter: " + std::string(e.what()));
    }
}

void MySQLXConnection::setDateTime(const int& statementId, const int& paramIndex, const std::tm& value) {
    auto logger = Logger::getInstance();
    logger->debug(  "Binding datetime parameter at index " + std::to_string(paramIndex) +
                    " for statement ID " + std::to_string(statementId));
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            logger->error("Cannot bind parameter: Invalid statement ID " + std::to_string(statementId));
            return;
        }

        // Format thành chuỗi "YYYY-MM-DD HH:MM:SS"
        std::ostringstream oss;
        oss << std::put_time(&value, "%Y-%m-%d %H:%M:%S");
        std::string formatted = oss.str();

        // Store parameter value
        PreparedStatementData& data = it->second;
        data.paramValues[paramIndex] = formatted;
        logger->debug("Datetime parameter bound successfully as string: " + formatted);
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Error binding datetime parameter: " + std::string(e.what()));
    }
}

std::string MySQLXConnection::buildPreparedStatement(const PreparedStatementData& data) {
    auto logger = Logger::getInstance();
    std::string query = data.query;
    logger->debug("Building prepared statement from: " + query);
    
    // Đếm số lượng parameters (dấu ?) trong query
    int questionMarkCount = std::count(query.begin(), query.end(), '?');
    int maxParamIndex = 0;
    
    for (const auto& param : data.paramValues) {
        maxParamIndex = std::max(maxParamIndex, param.first);
    }
    
    if (maxParamIndex > questionMarkCount) {
        logger->warning("Query has fewer placeholders than parameters provided");
    }
    
    // Replace ? with actual values từ cuối lên đầu để tránh việc thay đổi chỉ số
    for (int i = maxParamIndex; i >= 1; i--) {
        auto paramIt = data.paramValues.find(i);
        if (paramIt == data.paramValues.end()) {
            logger->warning("Missing parameter for index " + std::to_string(i));
            continue;
        }
        
        std::string value = paramIt->second;
        
        // Tìm dấu ? thứ i trong query
        size_t pos = 0;
        for (int j = 0; j < i; j++) {
            pos = query.find('?', pos);
            if (pos == std::string::npos) {
                break;
            }
            if (j < i - 1) {
                pos++;
            }
        }
        
        if (pos != std::string::npos) {
            // Thay thế ? với giá trị được quote
            query.replace(pos, 1, "'" + value + "'");
        }
    }
    
    logger->debug("Built final query: " + query);
    return query;
}

bool MySQLXConnection::executeStatement(const int& statementId) {
    auto logger = Logger::getInstance();
    logger->debug("Executing prepared statement with ID: " + std::to_string(statementId));
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            logger->error("Cannot execute statement: Invalid statement ID " + std::to_string(statementId));
            return false;
        }
        
        const PreparedStatementData& data = it->second;
        
        // Xây dựng câu lệnh SQL cuối cùng từ prepared statement
        std::string finalQuery = buildPreparedStatement(data);
        logger->debug("Executing prepared statement: " + finalQuery);
        
        // Thực thi câu lệnh SQL đã xây dựng trực tiếp
        _session->sql(finalQuery).execute();
        
        logger->debug("Statement executed successfully");
        return true;
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error executing statement: " + std::string(e.what()));
        return false;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error executing statement: " + std::string(e.what()));
        return false;
    }
}

std::unique_ptr<IDatabaseResult> MySQLXConnection::executeQueryStatement(const int& statementId) {
    auto logger = Logger::getInstance();
    logger->debug("Executing query prepared statement with ID: " + std::to_string(statementId));
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            logger->error("Cannot execute query statement: Invalid statement ID " + std::to_string(statementId));
            return nullptr;
        }
        
        // Build the statement with parameters
        std::string finalQuery = buildPreparedStatement(it->second);
        logger->debug("Executing prepared query: " + finalQuery);
        
        // Execute the statement as a query
        mysqlx::SqlResult result = _session->sql(finalQuery).execute();
        logger->debug("Query statement executed successfully");
        
        // Check if this is a result-producing query
        if (result.hasData()) {
            return std::make_unique<MySQLXResult>(std::move(result));
        } else {
            logger->debug("Query did not return any data");
            return nullptr;
        }
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error executing query statement: " + std::string(e.what()));
        return nullptr;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error executing query statement: " + std::string(e.what()));
        return nullptr;
    }
}

void MySQLXConnection::freeStatement(const int& statementId) {
    auto logger = Logger::getInstance();
    logger->debug("Freeing prepared statement with ID: " + std::to_string(statementId));
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            logger->warning("Attempted to free non-existent statement with ID: " + std::to_string(statementId));
        } else {
            _preparedStatements.erase(statementId);
            logger->debug("Statement freed successfully");
        }
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Error freeing statement: " + std::string(e.what()));
    }
}

int MySQLXConnection::getLastInsertId() {
    auto logger = Logger::getInstance();
    logger->debug("Getting last insert ID");
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (!_session) {
            _lastError = "Not connected to database";
            logger->error("Cannot get last insert ID: No active database connection");
            return -1;
        }
        
        // Execute query to get last insert ID
        mysqlx::SqlResult result = _session->sql("SELECT LAST_INSERT_ID()").execute();
        
        if (result.hasData()) {
            mysqlx::Row row = result.fetchOne();
            if (row) {
                int lastId = row[0].get<int>();
                logger->debug("Last insert ID: " + std::to_string(lastId));
                return lastId;
            }
        }
        
        logger->warning("No last insert ID available");
        return -1;
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error getting last insert ID: " + std::string(e.what()));
        return -1;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error getting last insert ID: " + std::string(e.what()));
        return -1;
    }
}

bool MySQLXConnection::isConnected() const {
    return _session != nullptr;
}

std::string MySQLXConnection::getLastError() const {
    return _lastError;
}

bool MySQLXConnection::beginTransaction() {
    auto logger = Logger::getInstance();
    logger->debug("Beginning database transaction");
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (!_session) {
            _lastError = "Not connected to database";
            logger->error("Cannot begin transaction: No active database connection");
            return false;
        }
        
        _session->startTransaction();
        logger->debug("Transaction started successfully");
        return true;
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error starting transaction: " + std::string(e.what()));
        return false;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error starting transaction: " + std::string(e.what()));
        return false;
    }
}

bool MySQLXConnection::commitTransaction() {
    auto logger = Logger::getInstance();
    logger->debug("Committing database transaction");
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (!_session) {
            _lastError = "Not connected to database";
            logger->error("Cannot commit transaction: No active database connection");
            return false;
        }
        
        _session->commit();
        logger->debug("Transaction committed successfully");
        return true;
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error committing transaction: " + std::string(e.what()));
        return false;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error committing transaction: " + std::string(e.what()));
        return false;
    }
}

bool MySQLXConnection::rollbackTransaction() {
    auto logger = Logger::getInstance();
    logger->debug("Rolling back database transaction");
    
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (!_session) {
            _lastError = "Not connected to database";
            logger->error("Cannot rollback transaction: No active database connection");
            return false;
        }
        
        _session->rollback();
        logger->debug("Transaction rolled back successfully");
        return true;
    }
    catch (const mysqlx::Error& e) {
        _lastError = e.what();
        logger->error("MySQL error rolling back transaction: " + std::string(e.what()));
        return false;
    }
    catch (const std::exception& e) {
        _lastError = e.what();
        logger->error("Unexpected error rolling back transaction: " + std::string(e.what()));
        return false;
    }
}