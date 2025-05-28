#include "services/FlightService.h"
#include "repositories/MySQLRepository/FlightRepository.h"
#include "repositories/MySQLRepository/AircraftRepository.h"
#include "repositories/MySQLRepository/TicketRepository.h"
#include "repositories/MySQLRepository/PassengerRepository.h"
#include "core/entities/Flight.h"
#include "core/entities/Aircraft.h"
#include "core/value_objects/flight_number/FlightNumber.h"
#include "core/value_objects/route/Route.h"
#include "core/value_objects/schedule/Schedule.h"
#include "core/value_objects/aircraft_serial/AircraftSerial.h"
#include "core/value_objects/seat_class_map/SeatClassMap.h"
#include "core/value_objects/seat_class_map/SeatClass.h"
#include "utils/Logger.h"
#include "database/MySQLXConnection.h"
#include <gtest/gtest.h>
#include <memory>
#include <algorithm>

class FlightServiceTest : public ::testing::Test
{
protected:
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<AircraftRepository> _aircraftRepository;
    std::shared_ptr<TicketRepository> _ticketRepository;
    std::shared_ptr<PassengerRepository> _passengerRepository;
    std::shared_ptr<FlightService> _service;
    std::shared_ptr<MySQLXConnection> _db;
    std::shared_ptr<Logger> _logger;
    FlightNumber _flightNumber;
    Route _route;
    Schedule _schedule;
    std::shared_ptr<Aircraft> _aircraft;

    void SetUp() override
    {
        // Initialize database connection
        _db = MySQLXConnection::getInstance();
        auto result = _db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " << result.error().message;

        // Initialize logger
        _logger = Logger::getInstance();
        _logger->setMinLevel(LogLevel::DEBUG);

        // Initialize repositories
        _aircraftRepository = std::make_shared<AircraftRepository>(_db, _logger);
        _passengerRepository = std::make_shared<PassengerRepository>(_db, _logger);
        _flightRepository = std::make_shared<FlightRepository>(_db, _logger);
        _ticketRepository = std::make_shared<TicketRepository>(_db, _passengerRepository, _flightRepository, _logger);

        // Initialize service
        _service = std::make_shared<FlightService>(_flightRepository, _aircraftRepository, _ticketRepository, _logger);

        // Setup test data
        // Create test aircraft
        auto serialResult = AircraftSerial::create("VN119");
        ASSERT_TRUE(serialResult.has_value());
        auto seatLayoutResult = SeatClassMap::create("E:10,B:5,F:2");
        ASSERT_TRUE(seatLayoutResult.has_value());
        auto aircraftResult = Aircraft::create(*serialResult, "Boeing 737", *seatLayoutResult);
        ASSERT_TRUE(aircraftResult.has_value());
        _aircraft = std::make_shared<Aircraft>(*aircraftResult);
        auto createAircraftResult = _aircraftRepository->create(*_aircraft);
        ASSERT_TRUE(createAircraftResult.has_value());
        _aircraft = std::make_shared<Aircraft>(createAircraftResult.value());

        // Setup test flight number
        auto flightNumberResult = FlightNumber::create("VN119");
        ASSERT_TRUE(flightNumberResult.has_value());
        _flightNumber = *flightNumberResult;

        // Setup test route
        auto routeResult = Route::create("Ho Chi Minh City(SGN)-Ha Noi(HAN)");
        ASSERT_TRUE(routeResult.has_value());
        _route = *routeResult;

        // Setup test schedule
        auto scheduleResult = Schedule::create("2024-03-20 10:00|2024-03-20 12:00");
        ASSERT_TRUE(scheduleResult.has_value());
        _schedule = *scheduleResult;
    }

    void TearDown() override
    {
        // Clean up test data
        auto result = _db->execute("DELETE FROM flight WHERE flight_number = 'VN119'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        result = _db->execute("DELETE FROM aircraft WHERE serial_number = 'VN119'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
    }

    Result<Flight> createTestFlight()
    {
        auto flightResult = Flight::create(_flightNumber, _route, _schedule, _aircraft);
        if (!flightResult) return flightResult;
        return _flightRepository->create(*flightResult);
    }
};

// // Core CRUD Tests
// TEST_F(FlightServiceTest, GetFlightSuccess)
// {
//     auto flightResult = createTestFlight();
//     ASSERT_TRUE(flightResult.has_value());

//     auto result = _service->getFlight(_flightNumber);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getFlightNumber().toString(), _flightNumber.toString());
//     EXPECT_EQ(result.value().getRoute().toString(), _route.toString());
//     EXPECT_EQ(result.value().getSchedule().toString(), _schedule.toString());
// }

// TEST_F(FlightServiceTest, GetFlightNotFound)
// {
//     auto result = _service->getFlight(_flightNumber);
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, "NOT_FOUND");
// }

// TEST_F(FlightServiceTest, GetAllFlightsSuccess)
// {
//     auto flightResult = createTestFlight();
//     ASSERT_TRUE(flightResult.has_value());

//     auto result = _service->getAllFlights();
//     ASSERT_TRUE(result.has_value());
//     EXPECT_FALSE(result.value().empty());
    
//     bool foundTargetFlight = false;
//     for (const auto& flight : result.value()) {
//         if (flight.getFlightNumber().toString() == _flightNumber.toString()) {
//             foundTargetFlight = true;
//             EXPECT_EQ(flight.getFlightNumber().toString(), _flightNumber.toString());
//             EXPECT_EQ(flight.getRoute().toString(), _route.toString());
//             EXPECT_EQ(flight.getSchedule().toString(), _schedule.toString());
//             break;
//         }
//     }
// }

// TEST_F(FlightServiceTest, FlightExistsSuccess)
// {
//     auto flightResult = createTestFlight();
//     ASSERT_TRUE(flightResult.has_value());

//     auto result = _service->flightExists(_flightNumber);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(FlightServiceTest, FlightExistsNotFound)
// {
//     auto result = _service->flightExists(_flightNumber);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_FALSE(result.value());
// }

// TEST_F(FlightServiceTest, CreateFlightSuccess)
// {
//     auto flightResult = Flight::create(_flightNumber, _route, _schedule, _aircraft);
//     ASSERT_TRUE(flightResult.has_value());

//     auto result = _service->createFlight(*flightResult);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getFlightNumber().toString(), _flightNumber.toString());
//     EXPECT_EQ(result.value().getRoute().toString(), _route.toString());
//     EXPECT_EQ(result.value().getSchedule().toString(), _schedule.toString());
// }

// TEST_F(FlightServiceTest, CreateFlightDuplicateFlightNumber)
// {
//     auto flightResult = createTestFlight();
//     ASSERT_TRUE(flightResult.has_value());

//     auto result = _service->createFlight(flightResult.value());
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, "DUPLICATE_FLIGHT_NUMBER");
// }

// TEST_F(FlightServiceTest, UpdateFlightSuccess)
// {
//     auto flightResult = createTestFlight();
//     ASSERT_TRUE(flightResult.has_value());

//     auto flight = flightResult.value();
//     auto newScheduleResult = Schedule::create("2024-03-21 10:00|2024-03-21 12:00");
//     ASSERT_TRUE(newScheduleResult.has_value());
//     flight.setSchedule(*newScheduleResult);

//     auto result = _service->updateFlight(flight);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getSchedule().toString(), newScheduleResult->toString());
// }

// TEST_F(FlightServiceTest, UpdateFlightNotFound)
// {
//     auto flightResult = Flight::create(_flightNumber, _route, _schedule, _aircraft);
//     ASSERT_TRUE(flightResult.has_value());

//     auto result = _service->updateFlight(*flightResult);
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, "NOT_FOUND");
// }

TEST_F(FlightServiceTest, DeleteFlightSuccess)
{
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());

    auto result = _service->deleteFlight(_flightNumber);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto existsResult = _service->flightExists(_flightNumber);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(existsResult.value());
}

TEST_F(FlightServiceTest, DeleteFlightNotFound)
{
    auto result = _service->deleteFlight(_flightNumber);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_FOUND");
}

// Business Operations Tests
TEST_F(FlightServiceTest, GetFlightsByAircraftSuccess)
{
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());

    auto result = _service->getFlightsByAircraft(_aircraft->getSerial());
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value().empty());
    EXPECT_EQ(result.value()[0].getFlightNumber().toString(), _flightNumber.toString());
}

TEST_F(FlightServiceTest, GetFlightsByAircraftNotFound)
{
    auto serialResult = AircraftSerial::create("VN888");
    ASSERT_TRUE(serialResult.has_value());

    auto result = _service->getFlightsByAircraft(*serialResult);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_FOUND");
}

TEST_F(FlightServiceTest, GetFlightsByRouteNotImplemented)
{
    auto result = _service->getFlightsByRoute("SGN", "HAN");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_IMPLEMENTED");
}

TEST_F(FlightServiceTest, GetFlightsByDateNotImplemented)
{
    auto result = _service->getFlightsByDate("2024-03-20");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_IMPLEMENTED");
}

TEST_F(FlightServiceTest, IsSeatAvailableSuccess)
{
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());

    auto result = _service->isSeatAvailable(_flightNumber, "E01");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(FlightServiceTest, IsSeatAvailableInvalidSeat)
{
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());

    auto result = _service->isSeatAvailable(_flightNumber, "X01");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "INVALID_SEAT_NUMBER");
}

TEST_F(FlightServiceTest, GetAvailableSeatsSuccess)
{
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());

    auto result = _service->getAvailableSeats(_flightNumber, "ECONOMY");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value().empty());
    EXPECT_EQ(result.value().size(), 10); // 10 economy seats
}

TEST_F(FlightServiceTest, GetAvailableSeatsInvalidClass)
{
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());

    auto result = _service->getAvailableSeats(_flightNumber, "INVALID");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "INVALID_SEAT_CLASS");
}

TEST_F(FlightServiceTest, UpdateFlightStatusSuccess)
{
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());

    auto result = _service->updateFlightStatus(_flightNumber, FlightStatus::DELAYED);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto flight = _service->getFlight(_flightNumber);
    ASSERT_TRUE(flight.has_value());
    EXPECT_EQ(flight.value().getStatus(), FlightStatus::DELAYED);
}

TEST_F(FlightServiceTest, UpdateFlightStatusNotFound)
{
    auto result = _service->updateFlightStatus(_flightNumber, FlightStatus::DELAYED);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_FOUND");
}
