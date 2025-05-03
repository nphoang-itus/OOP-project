#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include <sstream>
#include <chrono>

#include "database/MySQLXConnection.h"
#include "utils/Logger.h"

bool usecase_test_databaseconnection() {
    // Khởi tạo logger
    auto logger = Logger::getInstance();
    logger->setMinLevel(LogLevel::DEBUG); // Thiết lập cấp độ log là DEBUG để thấy tất cả thông tin log
    logger->info("Starting database connection test");
    
    try {
        // Lấy thông tin kết nối từ biến môi trường hoặc sử dụng giá trị mặc định
        std::string dbHost = std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "localhost";
        std::string dbUser = std::getenv("DB_USER") ? std::getenv("DB_USER") : "nphoang";
        std::string dbPassword = std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "phucHoang133205";
        std::string dbName = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "airlines_db";
        int dbPort = std::getenv("DB_PORT") ? std::stoi(std::getenv("DB_PORT")) : 33060;

        logger->info("Connecting to MySQL at " + dbHost + ":" + std::to_string(dbPort) + 
                     " with user '" + dbUser + "' and database '" + dbName + "'");

        // Tạo kết nối MySQL
        auto dbConnection = std::make_shared<MySQLXConnection>();

        if (!dbConnection->connect(dbHost, dbUser, dbPassword, dbName, dbPort)) {
            logger->error("Failed to connect to database: " + dbConnection->getLastError());
            std::cerr << "Failed to connect to database: " << dbConnection->getLastError() << "\n";
            return false;
        }

        logger->info("Connected to database successfully!");
        std::cout << "Connected to database successfully!" << std::endl;

        // Kiểm tra dữ liệu trong bảng flight_tb
        std::string query = "SELECT * FROM flight_tb";
        logger->debug("Executing query: " + query);
        auto result = dbConnection->executeQuery(query);

        if (!result) {
            logger->error("Failed to execute query: " + dbConnection->getLastError());
            std::cerr << "Failed to execute query: " << dbConnection->getLastError() << "\n";
            return false;
        }

        logger->info("Query executed successfully");
        
        // Menu loop
        int choice;
        std::cout << "1. Show flight data\n";
        std::cout << "0. Exit\n";
        
        while (true) {
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            
            if (choice == 1) {
                logger->debug("User requested to display flight data");
                std::cout << "Flight data:" << std::endl;
                
                int flightCount = 0;
                while (result->next()) {
                    flightCount++;
                    std::string flightNo = result->getString("f_no");
                    std::string airline = result->getString("f_name");
                    std::string from = result->getString("f_from");
                    std::string to = result->getString("f_destination");
                    std::chrono::sys_time<std::chrono::seconds> departure = result->getDateTime("f_departureTime");

                    std::time_t time = std::chrono::system_clock::to_time_t(departure);
                    std::tm tm = *std::localtime(&time);

                    // Format thành chuỗi "YYYY-MM-DD HH:MM:SS"
                    std::ostringstream oss;
                    oss << std::put_time(&tm, "%F %T"); // %F = YYYY-MM-DD, %T = HH:MM:SS
                    std::string formatted = oss.str();
                    
                    logger->debug("Retrieved flight: " + flightNo + " from " + from + " to " + to);
                    
                    std::cout << "Flight No: " << flightNo
                              << ", Airline: " << airline
                              << ", From: " << from
                              << ", To: " << to
                              << ", Departure: " << departure
                              << std::endl;
                }
                
                logger->info("Displayed " + std::to_string(flightCount) + " flights");
                
                // Cần reset result để hiển thị lại dữ liệu nếu người dùng chọn 1 lần nữa
                result = dbConnection->executeQuery(query);
                if (!result) {
                    logger->error("Failed to re-execute query: " + dbConnection->getLastError());
                }
            } else if (choice == 0) {
                logger->info("User chose to exit");
                break;
            } else {
                logger->warning("Invalid choice: " + std::to_string(choice));
                std::cout << "Invalid choice. Please try again.\n";
            }
        }

        // Ngắt kết nối cơ sở dữ liệu
        logger->info("Disconnecting from database");
        dbConnection->disconnect();
        logger->info("Database connection closed");
        
        std::cout << "Press Enter to exit the program!\n";
        std::cin.ignore();
        std::cin.get();
    }
    catch (const std::exception& e) {
        logger->error("Exception occurred: " + std::string(e.what()));
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }

    logger->info("Database connection test completed");
    return true;
}

int main() {
    std::cout << "Testing database connection and log functionality...\n";
    bool result = usecase_test_databaseconnection();
    std::cout << "Test " << (result ? "succeeded" : "failed") << ".\n";
    return result ? 0 : 1;
}