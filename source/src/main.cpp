#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include "database/MySQLConnection.h"
#include "repositories/FlightRepository.h"
#include "utils/TableConstants.h"

// Hàm in thông tin một chuyến bay
void printFlight(const Flight& flight) {
    std::cout << "ID: " << flight.getId() << "\n";
    std::cout << "Flight No: " << flight.getNo() << "\n";
    std::cout << "Name: " << flight.getName() << "\n";
    std::cout << "From: " << flight.getFrom() << "\n";
    std::cout << "Destination: " << flight.getDestination() << "\n";
    std::cout << "Departure Time: " << flight.getDepartureTime() << "\n";
    std::cout << "Leave Time: " << flight.getLeaveTime() << "\n";
    std::cout << "Arrival Time: " << flight.getArrivalTime() << "\n";
    std::cout << "Amount: " << flight.getAmount() << "\n";
    std::cout << "Availability: " << (flight.getAvailability() ? "Available" : "Not Available") << "\n";
}

// Hàm in danh sách chuyến bay
void printFlights(const std::vector<Flight>& flights) {
    if (flights.empty()) {
        std::cout << "No flights found.\n";
        return;
    }
    
    std::cout << "Found " << flights.size() << " flights:\n\n";
    
    for (const auto& flight : flights) {
        printFlight(flight);
        std::cout << "----------------------------\n";
    }
}

// Hàm dọn buffer sau khi đọc số
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Hàm nhập thông tin chuyến bay
Flight inputFlightInfo(bool isUpdate = false) {
    Flight flight;
    std::string no, name, from, destination, departureTime, leaveTime, arrivalTime;
    double amount;
    bool availability;
    
    if (isUpdate) {
        int id;
        std::cout << "Enter Flight ID: ";
        std::cin >> id;
        flight.setId(id);
        clearInputBuffer();
    }
    
    std::cout << "Enter Flight No: ";
    std::getline(std::cin, no);
    
    std::cout << "Enter Flight Name: ";
    std::getline(std::cin, name);
    
    std::cout << "Enter From: ";
    std::getline(std::cin, from);
    
    std::cout << "Enter Destination: ";
    std::getline(std::cin, destination);
    
    std::cout << "Enter Departure Time (YYYY-MM-DD HH:MM:SS): ";
    std::getline(std::cin, departureTime);
    
    std::cout << "Enter Leave Time (YYYY-MM-DD HH:MM:SS): ";
    std::getline(std::cin, leaveTime);
    
    std::cout << "Enter Arrival Time (YYYY-MM-DD HH:MM:SS): ";
    std::getline(std::cin, arrivalTime);
    
    std::cout << "Enter Amount: ";
    std::cin >> amount;
    
    char availChar;
    std::cout << "Is Available? (Y/N): ";
    std::cin >> availChar;
    availability = (availChar == 'Y' || availChar == 'y');
    
    flight.setNo(no);
    flight.setName(name);
    flight.setFrom(from);
    flight.setDestination(destination);
    flight.setDepartureTime(departureTime);
    flight.setLeaveTime(leaveTime);
    flight.setArrivalTime(arrivalTime);
    flight.setAmount(amount);
    flight.setAvailability(availability);
    
    return flight;
}

void Menu() {
    std::cout << "\n===== FLIGHT REPOSITORY TEST =====\n";
    std::cout << "Enter your choice:\n";
    std::cout << "1. Find All Flights\n";
    std::cout << "2. Find Flight By ID\n";
    std::cout << "3. Add New Flight\n";
    std::cout << "4. Update Flight\n";
    std::cout << "5. Remove Flight\n";
    std::cout << "6. Find Available Flights\n";
    std::cout << "7. Find Flight By Flight No\n";
    std::cout << "0. Exit\n";
    std::cout << "Your choice: ";
}

bool showAllFlight(std::shared_ptr<MySQLConnection> dbConnection) {
    std::string query = "SELECT * FROM flight_tb";
    auto result = dbConnection->executeQuery(query);

    if (!result) {
        std::cerr << "Failed to execute query: " << dbConnection->getLastError() << "\n";
        return false;
    }

    std::cout << "Flight data:" << std::endl;
    while (result->next()) {
        std::cout << "Flight No: " << result->getString("f_no")
                << ", Airline: " << result->getString("f_name")
                << ", From: " << result->getString("f_from")
                << ", To: " << result->getString("f_destination")
                << ", Departure: " << result->getString("f_departureTime")
                << std::endl;
    }
}

int main() {
    try {
        // Lấy thông tin kết nối từ biến môi trường hoặc sử dụng giá trị mặc định
        std::string dbHost = std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "mysql";
        std::string dbUser = std::getenv("DB_USER") ? std::getenv("DB_USER") : "nphoang";
        std::string dbPassword = std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "phucHoang133205";
        std::string dbName = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "airlines_db";
        int dbPort = std::getenv("DB_PORT") ? std::stoi(std::getenv("DB_PORT")) : 3306;

        // Tạo kết nối MySQL
        auto dbConnection = std::make_shared<MySQLConnection>();

        if (!dbConnection->connect(dbHost, dbUser, dbPassword, dbName, dbPort)) {
            std::cerr << "Failed to connect to database: " << dbConnection->getLastError() << "\n";
            return 1;
        }

        std::cout << "Connected to database successfully!" << std::endl;

        showAllFlight(dbConnection);

        // Tạo repository
        auto flightRepo = std::make_shared<FlightRepository>(dbConnection);

        int choice = 0;
        while (true) {
            Menu();
            std::cin >> choice;
            clearInputBuffer();

            switch (choice) {
                case 0: {
                    std::cout << "Exiting program...\n";
                    return 0;
                }
                case 1: {
                    std::cout << "\n===== FINDING ALL FLIGHTS =====\n";
                    auto flights = flightRepo->findAll();
                    printFlights(flights);
                    break;
                }
                case 2: {
                    std::cout << "\n===== FIND FLIGHT BY ID =====\n";
                    int id;
                    std::cout << "Enter Flight ID: ";
                    std::cin >> id;
                    clearInputBuffer();
                    
                    auto flightOpt = flightRepo->findById(id);
                    if (flightOpt.has_value()) {
                        std::cout << "Flight found:\n";
                        printFlight(flightOpt.value());
                    } else {
                        std::cout << "Flight not found with ID: " << id << "\n";
                    }
                    break;
                }
                case 3: {
                    std::cout << "\n===== ADD NEW FLIGHT =====\n";
                    Flight newFlight = inputFlightInfo();
                    
                    if (flightRepo->save(newFlight)) {
                        std::cout << "Flight added successfully with ID: " << newFlight.getId() << "\n";
                    } else {
                        std::cout << "Failed to add flight!\n";
                    }
                    break;
                }
                case 4: {
                    std::cout << "\n===== UPDATE FLIGHT =====\n";
                    Flight updateFlight = inputFlightInfo(true);
                    
                    if (flightRepo->update(updateFlight)) {
                        std::cout << "Flight updated successfully!\n";
                    } else {
                        std::cout << "Failed to update flight!\n";
                    }
                    break;
                }
                case 5: {
                    std::cout << "\n===== REMOVE FLIGHT =====\n";
                    int id;
                    std::cout << "Enter Flight ID to remove: ";
                    std::cin >> id;
                    clearInputBuffer();
                    
                    // Kiểm tra xem flight có tồn tại không
                    auto flightOpt = flightRepo->findById(id);
                    if (!flightOpt.has_value()) {
                        std::cout << "Flight not found with ID: " << id << "\n";
                        break;
                    }
                    
                    // Xác nhận xóa
                    char confirm;
                    std::cout << "Are you sure you want to delete this flight? (Y/N): ";
                    std::cin >> confirm;
                    clearInputBuffer();
                    
                    if (confirm == 'Y' || confirm == 'y') {
                        if (flightRepo->remove(id)) {
                            std::cout << "Flight removed successfully!\n";
                        } else {
                            std::cout << "Failed to remove flight!\n";
                        }
                    } else {
                        std::cout << "Flight removal cancelled.\n";
                    }
                    break;
                }
                case 6: {
                    std::cout << "\n===== FIND AVAILABLE FLIGHTS =====\n";
                    auto flights = flightRepo->findAvailableFlights();
                    printFlights(flights);
                    break;
                }
                case 7: {
                    std::cout << "\n===== FIND FLIGHT BY FLIGHT NO =====\n";
                    std::string flightNo;
                    std::cout << "Enter Flight No: ";
                    std::getline(std::cin, flightNo);
                    
                    auto flightOpt = flightRepo->findByFlightNo(flightNo);
                    if (flightOpt.has_value()) {
                        std::cout << "Flight found:\n";
                        printFlight(flightOpt.value());
                    } else {
                        std::cout << "Flight not found with Flight No: " << flightNo << "\n";
                    }
                    break;
                }
                default: {
                    std::cout << "Invalid choice! Please try again.\n";
                    break;
                }
            }
            
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}