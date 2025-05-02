/**
 * @file MySQLConnection.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "MySQLConnection.h"
 #include "../utils/Logger.h"
 #include <sstream>
 
 // Helper method để cắt ngắn câu truy vấn dài khi log
 std::string MySQLConnection::_truncateQueryForLog(const std::string& query, size_t maxLength) const {
     if (query.length() <= maxLength) {
         return query;
     }
     return query.substr(0, maxLength) + "... [truncated, " + 
         std::to_string(query.length() - maxLength) + " more characters]";
 }
 
 // Triển khai MySQLResult
 MySQLResult::MySQLResult(std::shared_ptr<sql::ResultSet> resultSet) : _resultSet(std::move(resultSet)) {
     auto logger = Logger::getInstance();
     logger->debug("Created MySQLResult object");
 }
 
 bool MySQLResult::next() {
     bool hasNext = _resultSet->next();
     auto logger = Logger::getInstance();
     logger->debug("MySQLResult::next() returned " + std::string(hasNext ? "true" : "false"));
     return hasNext;
 }
 
 std::string MySQLResult::getString(int columnIndex) {
     try {
         std::string value = _resultSet->getString(columnIndex);
         auto logger = Logger::getInstance();
         logger->debug("Retrieved string from column index " + std::to_string(columnIndex) + 
                     ": '" + (value.length() > 50 ? value.substr(0, 50) + "..." : value) + "'");
         return value;
     } catch (sql::SQLException& e) {
         auto logger = Logger::getInstance();
         logger->error("Error retrieving string from column index " + std::to_string(columnIndex) + 
                     ": " + e.what());
         throw;
     }
 }
 
 int MySQLResult::getInt(int columnIndex) {
     try {
         int value = _resultSet->getInt(columnIndex);
         auto logger = Logger::getInstance();
         logger->debug("Retrieved int from column index " + std::to_string(columnIndex) + 
                     ": " + std::to_string(value));
         return value;
     } catch (sql::SQLException& e) {
         auto logger = Logger::getInstance();
         logger->error("Error retrieving int from column index " + std::to_string(columnIndex) + 
                     ": " + e.what());
         throw;
     }
 }
 
 double MySQLResult::getDouble(int columnIndex) {
     try {
         double value = _resultSet->getDouble(columnIndex);
         auto logger = Logger::getInstance();
         logger->debug("Retrieved double from column index " + std::to_string(columnIndex) + 
                     ": " + std::to_string(value));
         return value;
     } catch (sql::SQLException& e) {
         auto logger = Logger::getInstance();
         logger->error("Error retrieving double from column index " + std::to_string(columnIndex) + 
                     ": " + e.what());
         throw;
     }
 }
 
 std::string MySQLResult::getString(const std::string& columnName) {
     try {
         std::string value = _resultSet->getString(columnName);
         auto logger = Logger::getInstance();
         logger->debug("Retrieved string from column '" + columnName + "': '" + 
                     (value.length() > 50 ? value.substr(0, 50) + "..." : value) + "'");
         return value;
     } catch (sql::SQLException& e) {
         auto logger = Logger::getInstance();
         logger->error("Error retrieving string from column '" + columnName + "': " + e.what());
         throw;
     }
 }
 
 int MySQLResult::getInt(const std::string& columnName) {
     try {
         int value = _resultSet->getInt(columnName);
         auto logger = Logger::getInstance();
         logger->debug("Retrieved int from column '" + columnName + "': " + std::to_string(value));
         return value;
     } catch (sql::SQLException& e) {
         auto logger = Logger::getInstance();
         logger->error("Error retrieving int from column '" + columnName + "': " + e.what());
         throw;
     }
 }
 
 double MySQLResult::getDouble(const std::string& columnName) {
     try {
         double value = _resultSet->getDouble(columnName);
         auto logger = Logger::getInstance();
         logger->debug("Retrieved double from column '" + columnName + "': " + std::to_string(value));
         return value;
     } catch (sql::SQLException& e) {
         auto logger = Logger::getInstance();
         logger->error("Error retrieving double from column '" + columnName + "': " + e.what());
         throw;
     }
 }
 
 // Triển khai MySQLConnection
 MySQLConnection::MySQLConnection() : _nextStatementId(1) {
     auto logger = Logger::getInstance();
     logger->debug("Creating new MySQLConnection instance");
     
     try {
         _driver = get_driver_instance();
         logger->debug("MySQL driver instance obtained successfully");
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("Failed to obtain MySQL driver instance: " + std::string(e.what()));
     }
 }
 
 MySQLConnection::~MySQLConnection() {
     auto logger = Logger::getInstance();
     logger->debug("Destroying MySQLConnection instance");
     disconnect();
 }
 
 bool MySQLConnection::connect(const std::string& host, const std::string& user, 
                             const std::string& password, const std::string& database, 
                             int port) {
     auto logger = Logger::getInstance();
     logger->info("Attempting to connect to MySQL at " + host + ":" + std::to_string(port) + 
                 " with user '" + user + "' and database '" + database + "'");
     
     try {
         std::ostringstream hostUrl;
         hostUrl << "tcp://" << host << ":" << port;
         
         auto startTime = std::chrono::high_resolution_clock::now();
         _connection.reset(_driver->connect(hostUrl.str(), user, password));
         _connection->setSchema(database);
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->info("Successfully connected to MySQL database '" + database + "' in " + 
                 std::to_string(duration) + "ms");
         return true;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("Failed to connect to MySQL: " + std::string(e.what()));
         return false;
     }
 }
 
 void MySQLConnection::disconnect() {
     auto logger = Logger::getInstance();
     
     if (!isConnected()) {
         logger->debug("disconnect() called but connection was already closed");
         return;
     }
     
     logger->info("Disconnecting from MySQL database");
     
     // Giải phóng tất cả prepared statements
     int stmtCount = _preparedStatements.size();
     _preparedStatements.clear();
     logger->debug("Cleared " + std::to_string(stmtCount) + " prepared statements");
 
     // Đóng kết nối
     if (_connection) {
         _connection.reset();
         logger->info("MySQL connection closed successfully");
     }
 }
 
 bool MySQLConnection::execute(const std::string& query) {
     auto logger = Logger::getInstance();
     logger->debug("Executing SQL statement: " + _truncateQueryForLog(query));
     
     try {
         if (!_connection) {
             _lastError = "Not connected to database";
             logger->error("Failed to execute statement - not connected to database");
             return false;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         std::unique_ptr<sql::Statement> statement(_connection->createStatement());
         statement->execute(query);
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Statement executed successfully in " + std::to_string(duration) + "ms");
         
         return true;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in execute: " + std::string(e.what()) + " for statement: " + 
                     _truncateQueryForLog(query));
         return false;
     }
 }
 
 std::unique_ptr<IDatabaseResult> MySQLConnection::executeQuery(const std::string& query) {
     auto logger = Logger::getInstance();
     logger->debug("Executing SQL query: " + _truncateQueryForLog(query));
     
     try {
         if (!_connection) {
             _lastError = "Not connected to database";
             logger->error("Failed to execute query - not connected to database");
             return nullptr;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         std::unique_ptr<sql::Statement> statement(_connection->createStatement());
         std::shared_ptr<sql::ResultSet> resultSet(statement->executeQuery(query));
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Query executed in " + std::to_string(duration) + "ms");
         
         return std::make_unique<MySQLResult>(resultSet);
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in executeQuery: " + std::string(e.what()) + " for query: " + 
                     _truncateQueryForLog(query));
         return nullptr;
     }
 }
 
 int MySQLConnection::prepareStatement(const std::string& query) {
     auto logger = Logger::getInstance();
     logger->debug("Preparing SQL statement: " + _truncateQueryForLog(query));
     
     try {
         if (!_connection) {
             _lastError = "Not connected to database";
             logger->error("Failed to prepare statement - not connected to database");
             return -1;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         std::shared_ptr<sql::PreparedStatement> pstmt(_connection->prepareStatement(query));
         auto endTime = std::chrono::high_resolution_clock::now();
         
         int id = _nextStatementId++;
         _preparedStatements[id] = pstmt;
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Statement #" + std::to_string(id) + " prepared in " + 
                     std::to_string(duration) + "ms");
         
         return id;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in prepareStatement: " + std::string(e.what()) + " for query: " + 
                     _truncateQueryForLog(query));
         return -1;
     }
 }
 
 void MySQLConnection::setString(int statementId, int paramIndex, const std::string& value) {
     auto logger = Logger::getInstance();
     std::string logValue = (value.length() > 100) ? value.substr(0, 100) + "..." : value;
     logger->debug("Setting string parameter #" + std::to_string(paramIndex) + 
                 " for statement #" + std::to_string(statementId) + ": '" + logValue + "'");
     
     try {
         auto it = _preparedStatements.find(statementId);
         if (it == _preparedStatements.end()) {
             _lastError = "Invalid statement ID";
             logger->warning("Invalid statement ID " + std::to_string(statementId) + 
                         " in setString");
             return;
         }
         
         it->second->setString(paramIndex, value);
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in setString: " + std::string(e.what()));
     }
 }
 
 void MySQLConnection::setInt(int statementId, int paramIndex, int value) {
     auto logger = Logger::getInstance();
     logger->debug("Setting int parameter #" + std::to_string(paramIndex) + 
                 " for statement #" + std::to_string(statementId) + ": " + 
                 std::to_string(value));
     
     try {
         auto it = _preparedStatements.find(statementId);
         if (it == _preparedStatements.end()) {
             _lastError = "Invalid statement ID";
             logger->warning("Invalid statement ID " + std::to_string(statementId) + 
                         " in setInt");
             return;
         }
         
         it->second->setInt(paramIndex, value);
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in setInt: " + std::string(e.what()));
     }
 }
 
 void MySQLConnection::setDouble(int statementId, int paramIndex, double value) {
     auto logger = Logger::getInstance();
     logger->debug("Setting double parameter #" + std::to_string(paramIndex) + 
                 " for statement #" + std::to_string(statementId) + ": " + 
                 std::to_string(value));
     
     try {
         auto it = _preparedStatements.find(statementId);
         if (it == _preparedStatements.end()) {
             _lastError = "Invalid statement ID";
             logger->warning("Invalid statement ID " + std::to_string(statementId) + 
                         " in setDouble");
             return;
         }
         
         it->second->setDouble(paramIndex, value);
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in setDouble: " + std::string(e.what()));
     }
 }
 
 bool MySQLConnection::executeStatement(int statementId) {
     auto logger = Logger::getInstance();
     logger->debug("Executing prepared statement #" + std::to_string(statementId));
     
     try {
         auto it = _preparedStatements.find(statementId);
         if (it == _preparedStatements.end()) {
             _lastError = "Invalid statement ID";
             logger->warning("Invalid statement ID " + std::to_string(statementId) + 
                         " in executeStatement");
             return false;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         it->second->execute();
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Prepared statement #" + std::to_string(statementId) + 
                     " executed in " + std::to_string(duration) + "ms");
         
         return true;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in executeStatement: " + std::string(e.what()) + 
                     " for statement #" + std::to_string(statementId));
         return false;
     }
 }
 
 std::unique_ptr<IDatabaseResult> MySQLConnection::executeQueryStatement(int statementId) {
     auto logger = Logger::getInstance();
     logger->debug("Executing query prepared statement #" + std::to_string(statementId));
     
     try {
         auto it = _preparedStatements.find(statementId);
         if (it == _preparedStatements.end()) {
             _lastError = "Invalid statement ID";
             logger->warning("Invalid statement ID " + std::to_string(statementId) + 
                         " in executeQueryStatement");
             return nullptr;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         std::shared_ptr<sql::ResultSet> resultSet(it->second->executeQuery());
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Prepared query statement #" + std::to_string(statementId) + 
                     " executed in " + std::to_string(duration) + "ms");
         
         return std::make_unique<MySQLResult>(resultSet);
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in executeQueryStatement: " + std::string(e.what()) + 
                     " for statement #" + std::to_string(statementId));
         return nullptr;
     }
 }
 
 void MySQLConnection::freeStatement(int statementId) {
     auto logger = Logger::getInstance();
     logger->debug("Freeing prepared statement #" + std::to_string(statementId));
     
     auto it = _preparedStatements.find(statementId);
     if (it == _preparedStatements.end()) {
         logger->warning("Attempting to free nonexistent statement #" + 
                     std::to_string(statementId));
         return;
     }
     
     _preparedStatements.erase(statementId);
     logger->debug("Prepared statement #" + std::to_string(statementId) + " freed successfully");
 }
 
 int MySQLConnection::getLastInsertId() {
     auto logger = Logger::getInstance();
     logger->debug("Getting last insert ID");
     
     try {
         if (!_connection) {
             _lastError = "Not connected to database";
             logger->error("Failed to get last insert ID - not connected to database");
             return -1;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         std::unique_ptr<sql::Statement> statement(_connection->createStatement());
         std::unique_ptr<sql::ResultSet> resultSet(statement->executeQuery("SELECT LAST_INSERT_ID()"));
         auto endTime = std::chrono::high_resolution_clock::now();
         
         int lastId = -1;
         if (resultSet->next()) {
             lastId = resultSet->getInt(1);
         }
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Got last insert ID: " + std::to_string(lastId) + " in " + 
                     std::to_string(duration) + "ms");
         
         return lastId;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in getLastInsertId: " + std::string(e.what()));
         return -1;
     }
 }
 
 bool MySQLConnection::isConnected() const {
     bool connected = (_connection != nullptr) && (!_connection->isClosed());
     auto logger = Logger::getInstance();
     logger->debug("isConnected() check: " + std::string(connected ? "true" : "false"));
     return connected;
 }
 
 std::string MySQLConnection::getLastError() const {
     auto logger = Logger::getInstance();
     if (!_lastError.empty()) {
         logger->debug("getLastError() returned: " + _lastError);
     }
     return _lastError;
 }
 
 bool MySQLConnection::beginTransaction() {
     auto logger = Logger::getInstance();
     logger->debug("Beginning new transaction");
     
     try {
         if (!_connection) {
             _lastError = "Not connected to database";
             logger->error("Failed to begin transaction - not connected to database");
             return false;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         _connection->setAutoCommit(false);
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Transaction started successfully in " + std::to_string(duration) + "ms");
         return true;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in beginTransaction: " + std::string(e.what()));
         return false;
     }
 }
 
 bool MySQLConnection::commitTransaction() {
     auto logger = Logger::getInstance();
     logger->debug("Committing transaction");
     
     try {
         if (!_connection) {
             _lastError = "Not connected to database";
             logger->error("Failed to commit transaction - not connected to database");
             return false;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         _connection->commit();
         _connection->setAutoCommit(true);
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Transaction committed successfully in " + std::to_string(duration) + "ms");
         return true;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in commitTransaction: " + std::string(e.what()));
         return false;
     }
 }
 
 bool MySQLConnection::rollbackTransaction() {
     auto logger = Logger::getInstance();
     logger->debug("Rolling back transaction");
     
     try {
         if (!_connection) {
             _lastError = "Not connected to database";
             logger->error("Failed to rollback transaction - not connected to database");
             return false;
         }
         
         auto startTime = std::chrono::high_resolution_clock::now();
         _connection->rollback();
         _connection->setAutoCommit(true);
         auto endTime = std::chrono::high_resolution_clock::now();
         
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
         logger->debug("Transaction rolled back successfully in " + std::to_string(duration) + "ms");
         return true;
     } catch (sql::SQLException& e) {
         _lastError = e.what();
         logger->error("SQL error in rollbackTransaction: " + std::string(e.what()));
         return false;
     }
 }