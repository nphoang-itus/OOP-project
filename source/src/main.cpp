#include "ui/MainUI.h"
#include "services/FlightService.h"
#include "services/PassengerService.h"
#include "services/ReservationService.h"
#include "repositories/FlightRepository.h"
#include "repositories/PassengerRepository.h"
#include "repositories/ReservationRepository.h"
#include "database/MySQLXConnection.h"
#include "utils/Logger.h"

#include <wx/wx.h>
#include <memory>

class AirlinesApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        // Initialize Logger
        auto logger = Logger::getInstance();
        logger->setMinLevel(LogLevel::DEBUG);

        // Initialize Database Connection
        auto dbConnection = MySQLXConnection::getInstance();
        if (!dbConnection->connect("localhost", "cuogne", "cuong116", "cuogne"))
        {
            logger->error("Failed to connect to the database: " + dbConnection->getLastError());
            return false;
        }

        // Initialize Repositories
        auto flightRepo = std::make_shared<FlightRepository>(dbConnection);
        auto passengerRepo = std::make_shared<PassengerRepository>(dbConnection);
        auto reservationRepo = std::make_shared<ReservationRepository>(dbConnection);

        // Initialize Services
        auto flightService = std::make_shared<FlightService>(flightRepo);
        auto passengerService = std::make_shared<PassengerService>(passengerRepo);
        auto reservationService = std::make_shared<ReservationService>(reservationRepo, flightService, passengerService);

        // Create and show main window
        auto mainWindow = new MainWindow("Hệ thống quản lý đặt vé máy bay",
                                         flightService,
                                         passengerService,
                                         reservationService);
        mainWindow->Show();

        return true;
    }
};

wxIMPLEMENT_APP(AirlinesApp);

// #include "repositories/PassengerRepository.h"
// #include "database/MySQLXConnection.h"
// #include "utils/Logger.h"
// #include "utils/utils.h"

// #include <iostream>
// #include <memory>

// int main()
// {
//     // Initialize Logger
//     auto logger = Logger::getInstance();
//     logger->setMinLevel(LogLevel::DEBUG);

//     // Initialize Database Connection
//     auto dbConnection = std::make_shared<MySQLXConnection>();
//     //         if (!dbConnection->connect("localhost", "cuogne", "cuong116", "cuogne"))
//     if (!dbConnection->connect("localhost", "cuogne", "cuong116", "airlines_db"))
//     {
//         logger->error("Failed to connect to the database: " + dbConnection->getLastError());
//         return 1;
//     }

//     // Initialize FlightRepository
//     PassengerRepository passRepo(dbConnection);

//     // Test findAll
//     std::cout << "Testing findAll()..." << std::endl;
//     auto passengers = passRepo.findAll();
//     for (const auto &pass : passengers)
//     {
//         std::cout << "Flight ID: " << pass.getId() << ", Name: " << pass.getName() << std::endl;
//     }

//     // Test findById
//     // std::cout << "Testing findById()..." << std::endl;
//     // auto passOpt = passRepo.findById(5);
//     // if (passOpt) {
//     //     std::cout << "Found Flight ID: " << passOpt->getId() << ", Name: " << passOpt->getName() << std::endl;
//     // } else {
//     //     std::cout << "Flight not found!" << std::endl;
//     // }

//     // Test save
//     // std::cout << "Testing save()..." << std::endl;
//     // std::cout << "\n";
//     // Passenger newPass("Nguyen Phuc Hoang", "0915513951", "067205001201", "TP Thu Duc, TP HCM");
//     // if (passRepo.save(newPass)) {
//     //     std::cout << "New flight saved successfully with name = " << newPass.getName() << std::endl;
//     // } else {
//     //     std::cout << "Failed to save new flight!" << std::endl;
//     // }
//     // std::cout << "\n";

//     // // Test findAll
//     // std::cout << "Testing findAll()..." << std::endl;
//     // auto passengers = passRepo.findAll();
//     // for (const auto& pass : passengers) {
//     //     std::cout << "Flight ID: " << pass.getId() << ", Name: " << pass.getName() << std::endl;
//     // }

//     // // Test update
//     // std::cout << "\n";
//     // std::cout << "Testing update()..." << std::endl;
//     // Passenger newPass;
//     // newPass.setId(11);
//     // newPass.setName("Khuc Khoang");
//     // if (passRepo.update(newPass)) {
//     //     std::cout << "Flight updated successfully with id = " << newPass.getId() << std::endl;
//     // } else {
//     //     std::cout << "Failed to update flight!" << std::endl;
//     // }
//     // std::cout << "\n";

//     // std::cout << "Testing findAll()..." << std::endl;
//     // auto passengers = passRepo.findAll();
//     // for (const auto& pass : passengers) {
//     //     std::cout << "Flight ID: " << pass.getId() << ", Name: " << pass.getName() << std::endl;
//     // }

//     // // Test remove
//     // std::cout << "Testing remove()..." << std::endl;
//     // if (passRepo.remove(newPass.getId())) {
//     //     std::cout << "Flight removed successfully!" << std::endl;
//     // } else {
//     //     std::cout << "Failed to remove flight!" << std::endl;

//     // }

//     // std::cout << "Testing findAll()..." << std::endl;
//     // auto passengers = passRepo.findAll();
//     // for (const auto& pass : passengers) {
//     //     std::cout << "Flight ID: " << pass.getId() << ", Name: " << pass.getName() << std::endl;
//     // }

//     // Test findAvailableFlights
//     // std::cout << "Testing findAvailableFlights()..." << std::endl;
//     // auto findName = passRepo.findByName("Nguyen Thi F");
//     // for (const auto& flight : findName) {
//     //     std::cout << "Available Flight ID: " << flight.getId() << ", Name: " << flight.getName() << std::endl;
//     // }

//     // Test findByFlightNo
//     // std::cout << "Testing findByFlightNo()..." << std::endl;
//     // auto flightByNo = passRepo.findByPassport("A12345678");
//     // if (flightByNo)
//     // {
//     //     std::cout << "Found Flight No: " << flightByNo->getId() << ", Name: " << flightByNo->getName() << std::endl;
//     // }
//     // else
//     // {
//     //     std::cout << "Flight not found by No!" << std::endl;
//     // }

//     // // Disconnect from database
//     // std::cout << "Disconnected from database." << std::endl;

//     // Test passenger operations
//     // try
//     // {
//     //     std::cout << "\n=== Testing Passenger Operations ===" << std::endl;

//     //     // Create a new passenger
//     //     Passenger newPass;
//     //     newPass.setId(11);
//     //     newPass.setPassengerNo("P0011");
//     //     newPass.setName("Test Passenger");
//     //     newPass.setPhone("1234567890");
//     //     newPass.setPassport("P123456");
//     //     newPass.setAddress("Test Address");

//     //     // Add passenger
//     //     std::cout << "Adding new passenger..." << std::endl;
//     //     passengerService->addPassenger(newPass);
//     //     std::cout << "Passenger added successfully!" << std::endl;

//     //     // ... existing code ...
//     // }
//     // catch (const std::exception &e)
//     // {
//     //     std::cerr << "Error: " << e.what() << std::endl;
//     //     return 1;
//     // }

//     return 0;
// }