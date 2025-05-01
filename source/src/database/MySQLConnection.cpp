#include "MySQLConnection.h"

#include <sstream>

// Triển khai MySQLResult
MySQLResult::MySQLResult(std::shared_ptr<sql::ResultSet> resultSet) : _resultSet(std::move(resultSet)) {}

bool MySQLResult::next() {
    return _resultSet->next();
}

std::string MySQLResult::getString(int columnIndex) {
    return _resultSet->getString(columnIndex);
}

int MySQLResult::getInt(int columnIndex) {
    return _resultSet->getInt(columnIndex);
}

double MySQLResult::getDouble(int columnIndex) {
    return _resultSet->getDouble(columnIndex);
}

std::string MySQLResult::getString(const std::string& columnName) {
    return _resultSet->getString(columnName);
}

int MySQLResult::getInt(const std::string& columnName) {
    return _resultSet->getInt(columnName);
}

double MySQLResult::getDouble(const std::string& columnName) {
    return _resultSet->getDouble(columnName);
}

// Triển khai MySQLConnection
MySQLConnection::MySQLConnection() : _nextStatementId(1) {
    try {
        _driver = get_driver_instance();
    } catch (sql::SQLException& e) {
        _lastError = e.what();
    }
}

MySQLConnection::~MySQLConnection() {
    disconnect(); 
}

bool MySQLConnection::connect(const std::string& host, const std::string& user, 
                              const std::string& password, const std::string& database, 
                              int port) {
    try {
        std::ostringstream hostUrl;
        hostUrl << "tcp://" << host << ":" << port;
        
        _connection.reset(_driver->connect(hostUrl.str(), user, password));
        _connection->setSchema(database);
        
        return true;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return false;
    }
}

void MySQLConnection::disconnect() {
    // Giải phóng tất cả prepared statements
    _preparedStatements.clear();

    // Đóng kết nối
    if (_connection) {
        _connection.reset();
    }
}

bool MySQLConnection::execute(const std::string& query) {
    try {
        if (!_connection) {
            _lastError = "Not connected to database";
            return false;
        }
        
        std::unique_ptr<sql::Statement> statement(_connection->createStatement());
        statement->execute(query);
        
        return true;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return false;
    }
}

std::unique_ptr<IDatabaseResult> MySQLConnection::executeQuery(const std::string& query) {
    try {
        if (!_connection) {
            _lastError = "Not connected to database";
            return nullptr;
        }
        
        std::unique_ptr<sql::Statement> statement(_connection->createStatement());
        std::shared_ptr<sql::ResultSet> resultSet(statement->executeQuery(query));
        
        return std::make_unique<MySQLResult>(resultSet);
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return nullptr;
}
}

int MySQLConnection::prepareStatement(const std::string& query) {
    try {
        if (!_connection) {
            _lastError = "Not connected to database";
            return -1;
        }
        
        std::shared_ptr<sql::PreparedStatement> pstmt(_connection->prepareStatement(query));
        int id = _nextStatementId++;
        _preparedStatements[id] = pstmt;
        
        return id;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return -1;
    }
}

void MySQLConnection::setString(int statementId, int paramIndex, const std::string& value) {
    try {
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            return;
        }
        
        it->second->setString(paramIndex, value);
    } catch (sql::SQLException& e) {
        _lastError = e.what();
    }
}

void MySQLConnection::setInt(int statementId, int paramIndex, int value) {
    try {
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            return;
        }
        
        it->second->setInt(paramIndex, value);
    } catch (sql::SQLException& e) {
        _lastError = e.what();
    }
}

void MySQLConnection::setDouble(int statementId, int paramIndex, double value) {
    try {
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            return;
        }
        
        it->second->setDouble(paramIndex, value);
    } catch (sql::SQLException& e) {
        _lastError = e.what();
    }
}

bool MySQLConnection::executeStatement(int statementId) {
    try {
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            return false;
        }
        
        it->second->execute();
        return true;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return false;
    }
}

std::unique_ptr<IDatabaseResult> MySQLConnection::executeQueryStatement(int statementId) {
    try {
        auto it = _preparedStatements.find(statementId);
        if (it == _preparedStatements.end()) {
            _lastError = "Invalid statement ID";
            return nullptr;
        }
        
        std::shared_ptr<sql::ResultSet> resultSet(it->second->executeQuery());
        return std::make_unique<MySQLResult>(resultSet);
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return nullptr;
    }
}

void MySQLConnection::freeStatement(int statementId) {
    _preparedStatements.erase(statementId);
}

int MySQLConnection::getLastInsertId() {
    try {
        if (!_connection) {
            _lastError = "Not connected to database";
            return -1;
        }
        
        std::unique_ptr<sql::Statement> statement(_connection->createStatement());
        std::unique_ptr<sql::ResultSet> resultSet(statement->executeQuery("SELECT LAST_INSERT_ID()"));
        
        if (resultSet->next()) {
            return resultSet->getInt(1);
        }
        
        return -1;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return -1;
    }
}

bool MySQLConnection::isConnected() const {
    return (_connection != nullptr) 
        && (!_connection->isClosed());
}

std::string MySQLConnection::getLastError() const {
    return _lastError;
}

bool MySQLConnection::beginTransaction() {
    try {
        if (!_connection) {
            _lastError = "Not connected to database";
            return false;
        }
        
        _connection->setAutoCommit(false);
        return true;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return false;
    }
}

bool MySQLConnection::commitTransaction() {
    try {
        if (!_connection) {
            _lastError = "Not connected to database";
            return false;
        }
        
        _connection->commit();
        _connection->setAutoCommit(true);
        return true;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return false;
    }
}

bool MySQLConnection::rollbackTransaction() {
    try {
        if (!_connection) {
            _lastError = "Not connected to database";
            return false;
        }
        
        _connection->rollback();
        _connection->setAutoCommit(true);
        return true;
    } catch (sql::SQLException& e) {
        _lastError = e.what();
        return false;
    }
}