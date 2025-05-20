#include <gtest/gtest.h>
#include "../../database/MySQLXConnection.h"
#include "../../core/exceptions/Result.h"
#include <memory>
#include <string>
#include <ctime>
#include <thread>
#include <atomic>

#define ASSERT_RESULT(result) ASSERT_TRUE(result.has_value())
#define EXPECT_RESULT(result) EXPECT_TRUE(result.has_value())

class MySQLXConnectionTest : public ::testing::Test {
protected:
    std::shared_ptr<MySQLXConnection> db;
    std::string host = "localhost";
    std::string user = "nphoang";  // Thay đổi từ "root" sang "nphoang"
    std::string password = "phucHoang133205";  // Thay đổi từ "root" sang "phucHoang133205"
    std::string database = "airlines_db";  // Thay đổi từ "flight_booking" sang "airlines_db"
    int port = 33060;

    void SetUp() override {
        db = MySQLXConnection::getInstance();
        auto result = db->connect(host, user, password, database, port);
        ASSERT_RESULT(result) << "Failed to connect to database: " << result.error().message;
    }

    void TearDown() override {
        db->disconnect();
    }
};

// Test connection
TEST_F(MySQLXConnectionTest, ConnectionTest) {
    // Test successful connection
    auto result = db->isConnected();
    ASSERT_RESULT(result) << "Connection check failed: " << result.error().message;
    EXPECT_TRUE(result.value());

    // Test disconnect
    auto disconnectResult = db->disconnect();
    ASSERT_RESULT(disconnectResult) << "Disconnect failed: " << disconnectResult.error().message;

    // Test reconnection
    auto reconnectResult = db->connect(host, user, password, database, port);
    ASSERT_RESULT(reconnectResult) << "Reconnection failed: " << reconnectResult.error().message;
}

// Test basic query execution
TEST_F(MySQLXConnectionTest, BasicQueryTest) {
    // Test INSERT
    auto insertResult = db->execute(
        "INSERT INTO seat_class (code, name) VALUES ('T', 'TEST')"
    );
    ASSERT_RESULT(insertResult) << "Insert failed: " << insertResult.error().message;
    EXPECT_TRUE(insertResult.value());

    // Test SELECT
    auto queryResult = db->executeQuery(
        "SELECT * FROM seat_class WHERE code = 'T'"
    );
    ASSERT_RESULT(queryResult) << "Query failed: " << queryResult.error().message;
    
    auto result = std::move(queryResult.value());
    ASSERT_TRUE(result->next()) << "No results found";
    
    auto codeResult = result->getString("code");
    ASSERT_RESULT(codeResult) << "Failed to get code: " << codeResult.error().message;
    EXPECT_EQ(codeResult.value(), "T");

    // Test DELETE
    auto deleteResult = db->execute(
        "DELETE FROM seat_class WHERE code = 'T'"
    );
    ASSERT_RESULT(deleteResult) << "Delete failed: " << deleteResult.error().message;
    EXPECT_TRUE(deleteResult.value());
}

// Test prepared statements
TEST_F(MySQLXConnectionTest, PreparedStatementTest) {
    // Prepare statement
    auto prepareResult = db->prepareStatement(
        "INSERT INTO seat_class (code, name) VALUES (?, ?)"
    );
    ASSERT_RESULT(prepareResult) << "Prepare failed: " << prepareResult.error().message;
    int stmtId = prepareResult.value();

    // Set parameters
    auto setStringResult1 = db->setString(stmtId, 1, "P");
    ASSERT_RESULT(setStringResult1) << "Set string failed: " << setStringResult1.error().message;

    auto setStringResult2 = db->setString(stmtId, 2, "PREMIUM");
    ASSERT_RESULT(setStringResult2) << "Set string failed: " << setStringResult2.error().message;

    // Execute statement
    auto executeResult = db->executeStatement(stmtId);
    ASSERT_RESULT(executeResult) << "Execute failed: " << executeResult.error().message;
    EXPECT_TRUE(executeResult.value());

    // Verify insertion
    auto queryResult = db->executeQuery(
        "SELECT * FROM seat_class WHERE code = 'P'"
    );
    ASSERT_RESULT(queryResult) << "Query failed: " << queryResult.error().message;
    
    auto result = std::move(queryResult.value());
    ASSERT_TRUE(result->next()) << "No results found";
    
    auto codeResult = result->getString("code");
    ASSERT_RESULT(codeResult) << "Failed to get code: " << codeResult.error().message;
    EXPECT_EQ(codeResult.value(), "P");

    // Clean up
    auto deleteResult = db->execute(
        "DELETE FROM seat_class WHERE code = 'P'"
    );
    ASSERT_RESULT(deleteResult) << "Delete failed: " << deleteResult.error().message;

    auto freeResult = db->freeStatement(stmtId);
    ASSERT_RESULT(freeResult) << "Free statement failed: " << freeResult.error().message;
}

// // Test transaction handling
// TEST_F(MySQLXConnectionTest, TransactionTest) {
//     // Begin transaction
//     auto beginResult = db->beginTransaction();
//     ASSERT_RESULT(beginResult) << "Begin transaction failed: " << beginResult.error().message;
//     EXPECT_TRUE(beginResult.value());

//     // Insert test data
//     auto insertResult = db->execute(
//         "INSERT INTO seat_class (code, name) VALUES ('X', 'TEST')"
//     );
//     ASSERT_RESULT(insertResult) << "Insert failed: " << insertResult.error().message;

//     // Rollback transaction
//     auto rollbackResult = db->rollbackTransaction();
//     ASSERT_RESULT(rollbackResult) << "Rollback failed: " << rollbackResult.error().message;
//     EXPECT_TRUE(rollbackResult.value());

//     // Verify rollback
//     auto queryResult = db->executeQuery(
//         "SELECT * FROM seat_class WHERE code = 'X'"
//     );
//     ASSERT_RESULT(queryResult) << "Query failed: " << queryResult.error().message;
    
//     auto result = std::move(queryResult.value());
//     EXPECT_FALSE(result->next()) << "Rollback did not work";

//     // Test commit
//     beginResult = db->beginTransaction();
//     ASSERT_RESULT(beginResult) << "Begin transaction failed: " << beginResult.error().message;

//     insertResult = db->execute(
//         "INSERT INTO seat_class (code, name) VALUES ('Y', 'TEST')"
//     );
//     ASSERT_RESULT(insertResult) << "Insert failed: " << insertResult.error().message;

//     auto commitResult = db->commitTransaction();
//     ASSERT_RESULT(commitResult) << "Commit failed: " << commitResult.error().message;
//     EXPECT_TRUE(commitResult.value());

//     // Verify commit
//     queryResult = db->executeQuery(
//         "SELECT * FROM seat_class WHERE code = 'Y'"
//     );
//     ASSERT_RESULT(queryResult) << "Query failed: " << queryResult.error().message;
    
//     result = std::move(queryResult.value());
//     ASSERT_TRUE(result->next()) << "Commit did not work";

//     // Clean up
//     auto deleteResult = db->execute(
//         "DELETE FROM seat_class WHERE code = 'Y'"
//     );
//     ASSERT_RESULT(deleteResult) << "Delete failed: " << deleteResult.error().message;
// }

// Test error handling
TEST_F(MySQLXConnectionTest, ErrorHandlingTest) {
    // Test invalid query
    auto invalidQueryResult = db->execute("INVALID SQL");
    ASSERT_FALSE(invalidQueryResult.has_value()) << "Invalid query should fail";

    // Test invalid prepared statement
    auto invalidStmtResult = db->executeStatement(999);
    ASSERT_FALSE(invalidStmtResult.has_value()) << "Invalid statement should fail";

    // Test invalid parameter setting
    auto invalidParamResult = db->setString(999, 1, "test");
    ASSERT_FALSE(invalidParamResult.has_value()) << "Invalid parameter setting should fail";
}

// Test data type handling
TEST_F(MySQLXConnectionTest, DataTypeTest) {
    // Create test table
    auto createResult = db->execute(
        "CREATE TABLE IF NOT EXISTS test_types ("
        "id INT AUTO_INCREMENT PRIMARY KEY,"
        "str_val VARCHAR(50),"
        "int_val INT,"
        "double_val DOUBLE,"
        "date_val DATETIME"
        ")"
    );
    ASSERT_RESULT(createResult) << "Create table failed: " << createResult.error().message;

    // Insert test data
    auto insertResult = db->execute(
        "INSERT INTO test_types (str_val, int_val, double_val, date_val) "
        "VALUES ('test', 42, 3.14, '2024-03-14 12:00:00')"
    );
    ASSERT_RESULT(insertResult) << "Insert failed: " << insertResult.error().message;

    // Query and verify data
    auto queryResult = db->executeQuery("SELECT * FROM test_types");
    ASSERT_RESULT(queryResult) << "Query failed: " << queryResult.error().message;
    
    auto result = std::move(queryResult.value());
    ASSERT_TRUE(result->next()) << "No results found";

    // Test string
    auto strResult = result->getString("str_val");
    ASSERT_RESULT(strResult) << "Failed to get string: " << strResult.error().message;
    EXPECT_EQ(strResult.value(), "test");

    // Test integer
    auto intResult = result->getInt("int_val");
    ASSERT_RESULT(intResult) << "Failed to get integer: " << intResult.error().message;
    EXPECT_EQ(intResult.value(), 42);

    // Test double
    auto doubleResult = result->getDouble("double_val");
    ASSERT_RESULT(doubleResult) << "Failed to get double: " << doubleResult.error().message;
    EXPECT_DOUBLE_EQ(doubleResult.value(), 3.14);

    // Test datetime
    auto dateResult = result->getDateTime("date_val");
    ASSERT_RESULT(dateResult) << "Failed to get datetime: " << dateResult.error().message;
    auto tm = dateResult.value();
    EXPECT_EQ(tm.tm_year + 1900, 2024);
    EXPECT_EQ(tm.tm_mon + 1, 3);
    EXPECT_EQ(tm.tm_mday, 14);
    EXPECT_EQ(tm.tm_hour, 12);

    // Clean up
    auto dropResult = db->execute("DROP TABLE test_types");
    ASSERT_RESULT(dropResult) << "Drop table failed: " << dropResult.error().message;
}

// Test concurrent access
TEST_F(MySQLXConnectionTest, ConcurrentAccessTest) {
    const int numThreads = 5;
    std::vector<std::thread> threads;
    std::atomic<int> successCount(0);

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this, i, &successCount]() {
            auto queryResult = db->executeQuery(
                "SELECT * FROM seat_class WHERE code = 'E'"
            );
            if (queryResult.has_value() && queryResult.value()->next()) {
                successCount++;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(successCount, numThreads) << "Concurrent access failed";
}