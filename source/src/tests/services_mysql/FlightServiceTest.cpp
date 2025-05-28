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
    std::shared_ptr<Flight> _flight;

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
        auto serialResult = AircraftSerial::create("VN414");
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
        auto flightNumberResult = FlightNumber::create("VN414");
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
        // Delete seat availability records first
        if (_flight) {
            std::string deleteSeatsQuery = "DELETE FROM flight_seat_availability WHERE flight_id = ?";
            auto prepareResult = _db->prepareStatement(deleteSeatsQuery);
            if (prepareResult) {
                int stmtId = prepareResult.value();
                _db->setInt(stmtId, 1, _flight->getId());
                _db->executeStatement(stmtId);
                _db->freeStatement(stmtId);
            }
        }

        // Delete test flight
        if (_flight) {
            _service->deleteFlight(_flight->getFlightNumber());
        }

        // Delete test aircraft
        if (_aircraft) {
            _aircraftRepository->deleteById(_aircraft->getId());
        }
    }

    Result<Flight> createTestFlight()
    {
        auto flightResult = Flight::create(_flightNumber, _route, _schedule, _aircraft);
        if (!flightResult) return flightResult;
        return _flightRepository->create(*flightResult);
    }
};

TEST_F(FlightServiceTest, CreateFlightSuccess)
{
    // Create test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult);
    _flight = std::make_shared<Flight>(flightResult.value());

    // Verify flight was created
    auto getFlightResult = _service->getFlight(_flight->getFlightNumber());
    ASSERT_TRUE(getFlightResult);
    EXPECT_EQ(getFlightResult.value().getFlightNumber().toString(), _flight->getFlightNumber().toString());
    EXPECT_EQ(getFlightResult.value().getRoute().toString(), _flight->getRoute().toString());
    EXPECT_EQ(getFlightResult.value().getSchedule().toString(), _flight->getSchedule().toString());
    EXPECT_EQ(getFlightResult.value().getAircraft()->getId(), _flight->getAircraft()->getId());
    EXPECT_EQ(getFlightResult.value().getStatus(), _flight->getStatus());

    // Verify seat availability records were created
    auto availableSeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "ECONOMY");
    ASSERT_TRUE(availableSeatsResult);
    EXPECT_EQ(availableSeatsResult.value().size(), 10); // 10 economy seats

    availableSeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "BUSINESS");
    ASSERT_TRUE(availableSeatsResult);
    EXPECT_EQ(availableSeatsResult.value().size(), 5); // 5 business seats

    availableSeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "FIRST");
    ASSERT_TRUE(availableSeatsResult);
    EXPECT_EQ(availableSeatsResult.value().size(), 2); // 2 first class seats
}

TEST_F(FlightServiceTest, CreateFlightDuplicateNumber)
{
    // Create first flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult);
    _flight = std::make_shared<Flight>(flightResult.value());

    // Try to create another flight with same number
    auto duplicateResult = createTestFlight();
    ASSERT_FALSE(duplicateResult);
    EXPECT_EQ(duplicateResult.error().code, "DUPLICATE_FLIGHT_NUMBER");
}

TEST_F(FlightServiceTest, ReserveSeatSuccess)
{
    // Create test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult);
    _flight = std::make_shared<Flight>(flightResult.value());

    // Reserve a seat
    auto reserveResult = _service->reserveSeat(_flight->getFlightNumber(), "E01");
    ASSERT_TRUE(reserveResult);
    EXPECT_TRUE(reserveResult.value());

    // Verify seat is no longer available
    auto availableSeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "ECONOMY");
    ASSERT_TRUE(availableSeatsResult);
    EXPECT_EQ(availableSeatsResult.value().size(), 9); // One less available seat

    // Try to reserve same seat again
    auto duplicateReserveResult = _service->reserveSeat(_flight->getFlightNumber(), "E01");
    ASSERT_FALSE(duplicateReserveResult);
    EXPECT_EQ(duplicateReserveResult.error().code, "SEAT_NOT_AVAILABLE");
}

TEST_F(FlightServiceTest, ReleaseSeatSuccess)
{
    // Create test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult);
    _flight = std::make_shared<Flight>(flightResult.value());

    // Reserve a seat
    auto reserveResult = _service->reserveSeat(_flight->getFlightNumber(), "E01");
    ASSERT_TRUE(reserveResult);
    EXPECT_TRUE(reserveResult.value());

    // Release the seat
    auto releaseResult = _service->releaseSeat(_flight->getFlightNumber(), "E01");
    ASSERT_TRUE(releaseResult);
    EXPECT_TRUE(releaseResult.value());

    // Verify seat is available again
    auto availableSeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "ECONOMY");
    ASSERT_TRUE(availableSeatsResult);
    EXPECT_EQ(availableSeatsResult.value().size(), 10); // All seats available again

    // Try to release an already available seat
    auto duplicateReleaseResult = _service->releaseSeat(_flight->getFlightNumber(), "E01");
    ASSERT_FALSE(duplicateReleaseResult);
    EXPECT_EQ(duplicateReleaseResult.error().code, "SEAT_NOT_RESERVED");
}

TEST_F(FlightServiceTest, InvalidSeatNumber)
{
    // Create test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult);
    _flight = std::make_shared<Flight>(flightResult.value());

    // Try to reserve invalid seat
    auto reserveResult = _service->reserveSeat(_flight->getFlightNumber(), "X99");
    ASSERT_FALSE(reserveResult);
    EXPECT_EQ(reserveResult.error().code, "INVALID_SEAT_NUMBER");

    // Try to release invalid seat
    auto releaseResult = _service->releaseSeat(_flight->getFlightNumber(), "X99");
    ASSERT_FALSE(releaseResult);
    EXPECT_EQ(releaseResult.error().code, "INVALID_SEAT_NUMBER");
}

TEST_F(FlightServiceTest, GetAvailableSeatsByClass)
{
    // Create test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult);
    _flight = std::make_shared<Flight>(flightResult.value());

    // Get available seats for each class
    auto economySeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "ECONOMY");
    ASSERT_TRUE(economySeatsResult);
    EXPECT_EQ(economySeatsResult.value().size(), 10);

    auto businessSeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "BUSINESS");
    ASSERT_TRUE(businessSeatsResult);
    EXPECT_EQ(businessSeatsResult.value().size(), 5);

    auto firstSeatsResult = _service->getAvailableSeats(_flight->getFlightNumber(), "FIRST");
    ASSERT_TRUE(firstSeatsResult);
    EXPECT_EQ(firstSeatsResult.value().size(), 2);

    // Try invalid class
    auto invalidClassResult = _service->getAvailableSeats(_flight->getFlightNumber(), "INVALID");
    ASSERT_FALSE(invalidClassResult);
    EXPECT_EQ(invalidClassResult.error().code, "INVALID_SEAT_CLASS");
}

TEST_F(FlightServiceTest, GetRemainingCapacity)
{
    // Create test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult);
    _flight = std::make_shared<Flight>(flightResult.value());

    // Check initial capacity
    auto initialCapacityResult = _service->getRemainingCapacity(_flight->getFlightNumber());
    ASSERT_TRUE(initialCapacityResult);
    EXPECT_EQ(initialCapacityResult.value(), 17); // Total seats: 10 + 5 + 2

    // Reserve some seats
    auto reserveResult1 = _service->reserveSeat(_flight->getFlightNumber(), "E01");
    ASSERT_TRUE(reserveResult1);
    auto reserveResult2 = _service->reserveSeat(_flight->getFlightNumber(), "B01");
    ASSERT_TRUE(reserveResult2);

    // Check remaining capacity
    auto remainingCapacityResult = _service->getRemainingCapacity(_flight->getFlightNumber());
    ASSERT_TRUE(remainingCapacityResult);
    EXPECT_EQ(remainingCapacityResult.value(), 15); // 17 - 2 reserved seats
}
