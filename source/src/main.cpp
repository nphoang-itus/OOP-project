#include <wx/wx.h>
#include "ui/MainUI.h"
#include "services/AircraftService.h"
#include "services/FlightService.h"
#include "services/PassengerService.h"
#include "repositories/MySQLRepository/AircraftRepository.h"
#include "repositories/MySQLRepository/FlightRepository.h"
#include "repositories/MySQLRepository/PassengerRepository.h"
#include "repositories/MySQLRepository/TicketRepository.h"
#include "database/MySQLXConnection.h"
#include "utils/Logger.h"

class AirlinesApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        // Initialize Logger
        auto logger = Logger::getInstance();
        logger->setMinLevel(LogLevel::DEBUG);

        // Initialize Database Connection
        auto connection = MySQLXConnection::getInstance();
        if (!connection->connect("localhost", "cuong116", "1162005", "airlines_db", 33060))
        {
            auto err = connection->getLastError();
            if (err.has_value())
            {
                logger->error(std::string("Failed to connect to the database: ") + err.value());
            }
            else
            {
                logger->error("Failed to connect to the database: Unknown error");
            }
            return false;
        }

        // Create repositories
        auto aircraftRepo = std::make_shared<AircraftRepository>(connection, logger);
        auto flightRepo = std::make_shared<FlightRepository>(connection, logger);
        auto passengerRepo = std::make_shared<PassengerRepository>(connection, logger);
        auto ticketRepo = std::make_shared<TicketRepository>(connection, passengerRepo, flightRepo, logger);

        // Create services with their repositories
        auto aircraftService = std::make_shared<AircraftService>(aircraftRepo, flightRepo, ticketRepo, logger);
        auto flightService = std::make_shared<FlightService>(flightRepo, aircraftRepo, ticketRepo, logger);
        auto passengerService = std::make_shared<PassengerService>(passengerRepo, ticketRepo, flightRepo, logger);

        // Create and show main window
        MainWindow *mainWindow = new MainWindow("Quản lý hãng hàng không",
                                                aircraftService,
                                                flightService,
                                                passengerService);
        mainWindow->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(AirlinesApp);
