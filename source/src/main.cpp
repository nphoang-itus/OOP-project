#include <QApplication>
#include "ui/MainWindow.h"
#include "services/AircraftService.h"
#include "repositories/MySQLRepository/AircraftRepository.h"
#include "repositories/MySQLRepository/FlightRepository.h"
#include "repositories/MySQLRepository/TicketRepository.h"
#include "repositories/MySQLRepository/PassengerRepository.h"
#include "utils/Logger.h"
#include "database/MySQLXConnection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Initialize logger
    auto logger = Logger::getInstance();
    logger->setMinLevel(LogLevel::DEBUG);
    logger->info("Starting application...");

    // Initialize database connection
    auto dbConnection = MySQLXConnection::getInstance();
    auto connectResult = dbConnection->connect(
        "localhost",
        "nphoang",
        "phucHoang133205",
        "airlines_db",
        33060
    );

    if (!connectResult) {
        logger->error("Failed to connect to database: " + connectResult.error().message);
        return 1;
    }
    logger->info("Successfully connected to database");

    // Initialize repositories with database connection
    auto aircraftRepo = std::make_shared<AircraftRepository>(dbConnection, logger);
    auto flightRepo = std::make_shared<FlightRepository>(dbConnection, logger);
    auto passengerRepo = std::make_shared<PassengerRepository>(dbConnection, logger);
    auto ticketRepo = std::make_shared<TicketRepository>(dbConnection, passengerRepo, flightRepo, logger);
    
    // Initialize services
    auto aircraftService = std::make_shared<AircraftService>(
        aircraftRepo,
        flightRepo,
        ticketRepo,
        logger
    );
    
    // Create and show main window
    MainWindow mainWindow(aircraftService);
    mainWindow.show();
    
    int result = app.exec();

    // Cleanup database connection
    dbConnection->disconnect();
    logger->info("Application shutdown complete");
    
    return result;
}
