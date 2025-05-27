#include "services/FlightService.h"
#include "repositories/MockRepository/FlightMockRepository.h"
#include "core/entities/Flight.h"
#include "core/entities/Aircraft.h"
#include "core/value_objects/flight_number/FlightNumber.h"
#include "core/value_objects/route/Route.h"
#include "core/value_objects/schedule/Schedule.h"
#include "core/value_objects/aircraft_serial/AircraftSerial.h"
#include "core/value_objects/seat_class_map/SeatClassMap.h"
#include "core/value_objects/seat_class_map/SeatClass.h"
#include "utils/Logger.h"
#include <gtest/gtest.h>
#include <memory>

class FlightServiceTest : public ::testing::Test
{
protected:
    std::shared_ptr<FlightMockRepository> mockRepository;
    std::shared_ptr<FlightService> service;
    std::shared_ptr<Logger> logger;
    FlightNumber _flightNumber;
    Route _route;
    Schedule _schedule;
    std::shared_ptr<Aircraft> _aircraft;

    void SetUp() override
    {
        mockRepository = std::make_shared<FlightMockRepository>();
        logger = Logger::getInstance();
        service = std::make_shared<FlightService>(mockRepository);

        // Setup test flight number
        auto flightNumberResult = FlightNumber::create("VN123");
        ASSERT_TRUE(flightNumberResult.has_value());
        _flightNumber = *flightNumberResult;

        // Setup test route
        auto routeResult = Route::create("Noi Bai International Airport", "HAN",
                                         "Tan Son Nhat International Airport", "SGN");
        ASSERT_TRUE(routeResult.has_value());
        _route = *routeResult;

        // Setup test schedule
        auto now = std::time(nullptr);
        auto departure = *std::localtime(&now);
        auto arrival = *std::localtime(&now);
        arrival.tm_hour += 2; // 2 hours later
        auto scheduleResult = Schedule::create(departure, arrival);
        ASSERT_TRUE(scheduleResult.has_value());
        _schedule = *scheduleResult;

        // Setup test aircraft
        auto serialResult = AircraftSerial::create("VN123");
        ASSERT_TRUE(serialResult.has_value());

        auto seatLayoutResult = SeatClassMap::create({{SeatClassRegistry::getByName("ECONOMY").value(), 100},
                                                      {SeatClassRegistry::getByName("BUSINESS").value(), 20},
                                                      {SeatClassRegistry::getByName("FIRST").value(), 10}});
        ASSERT_TRUE(seatLayoutResult.has_value());

        auto aircraftResult = Aircraft::create(
            serialResult.value(),
            "Boeing 737",
            seatLayoutResult.value());
        ASSERT_TRUE(aircraftResult.has_value());
        _aircraft = std::make_shared<Aircraft>(aircraftResult.value());
        _aircraft->setId(1);
    }

    void TearDown() override
    {
        mockRepository->clear();
    }

    // Helper method to create a flight
    Result<Flight> createFlight()
    {
        return Flight::create(_flightNumber, _route, _schedule, _aircraft);
    }
};

// Test getById
TEST_F(FlightServiceTest, GetByIdSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = mockRepository->create(flightResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->getById(1);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), _flightNumber);
    EXPECT_EQ(result.value().getRoute(), _route);
    EXPECT_EQ(result.value().getSchedule(), _schedule);
    EXPECT_EQ(result.value().getAircraft()->getId(), _aircraft->getId());
}

TEST_F(FlightServiceTest, GetByIdFailure)
{
    // Act
    auto result = service->getById(999);

    // Assert
    ASSERT_FALSE(result.has_value());
}

// Test getAll
TEST_F(FlightServiceTest, GetAllSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    mockRepository->create(flightResult.value());

    // Create another flight
    auto flightNumberResult = FlightNumber::create("VN456");
    ASSERT_TRUE(flightNumberResult.has_value());

    auto flightResult2 = Flight::create(
        flightNumberResult.value(),
        _route,
        _schedule,
        _aircraft);
    ASSERT_TRUE(flightResult2.has_value());

    mockRepository->create(flightResult2.value());

    // Act
    auto result = service->getAll();

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

// Test create
TEST_F(FlightServiceTest, CreateSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());

    // Act
    auto result = service->create(flightResult.value());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), _flightNumber);
    EXPECT_EQ(result.value().getRoute(), _route);
    EXPECT_EQ(result.value().getSchedule(), _schedule);
    EXPECT_EQ(result.value().getAircraft()->getId(), _aircraft->getId());
}

TEST_F(FlightServiceTest, CreateFailureDuplicateFlightNumber)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    mockRepository->create(flightResult.value());

    // Act
    auto result = service->create(flightResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().message, "Flight with flight number already exists");
}

// Test update
TEST_F(FlightServiceTest, UpdateSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = mockRepository->create(flightResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Create a new flight with updated flight number
    auto newFlightNumberResult = FlightNumber::create("VN789");
    ASSERT_TRUE(newFlightNumberResult.has_value());

    auto updatedFlightResult = Flight::create(
        newFlightNumberResult.value(),
        _route,
        _schedule,
        _aircraft);
    ASSERT_TRUE(updatedFlightResult.has_value());
    auto updatedFlight = updatedFlightResult.value();
    updatedFlight.setId(createResult.value().getId());

    // Act
    auto result = service->update(updatedFlight);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), newFlightNumberResult.value());
}

TEST_F(FlightServiceTest, UpdateFailureNotFound)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto flight = flightResult.value();
    flight.setId(999);

    // Act
    auto result = service->update(flight);

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().message, "Flight not found");
}

// Test deleteById
TEST_F(FlightServiceTest, DeleteByIdSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = mockRepository->create(flightResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->deleteById(1);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    // Verify flight no longer exists
    auto existsResult = service->exists(1);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(existsResult.value());
}

TEST_F(FlightServiceTest, DeleteByIdFailure)
{
    // Act
    auto result = service->deleteById(999);

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().message, "Flight not found");
}

// Test exists
TEST_F(FlightServiceTest, ExistsSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = mockRepository->create(flightResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->exists(1);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(FlightServiceTest, ExistsFailure)
{
    // Act
    auto result = service->exists(999);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test count
TEST_F(FlightServiceTest, CountSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    mockRepository->create(flightResult.value());

    // Act
    auto result = service->count();

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 1);
}

// Test getFlightByNumber
TEST_F(FlightServiceTest, GetFlightByNumberSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = mockRepository->create(flightResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->getFlightByNumber(_flightNumber);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), _flightNumber);
    EXPECT_EQ(result.value().getRoute(), _route);
    EXPECT_EQ(result.value().getSchedule(), _schedule);
    EXPECT_EQ(result.value().getAircraft()->getId(), _aircraft->getId());
}

TEST_F(FlightServiceTest, GetFlightByNumberFailure)
{
    // Arrange
    auto nonExistentFlightNumberResult = FlightNumber::create("VN999");
    ASSERT_TRUE(nonExistentFlightNumberResult.has_value());

    // Act
    auto result = service->getFlightByNumber(nonExistentFlightNumberResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
}

// Test flightExistsByNumber
TEST_F(FlightServiceTest, FlightExistsByNumberSuccess)
{
    // Arrange
    auto flightResult = createFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = mockRepository->create(flightResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->flightExistsByNumber(_flightNumber);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(FlightServiceTest, FlightExistsByNumberFailure)
{
    // Arrange
    auto nonExistentFlightNumberResult = FlightNumber::create("VN999");
    ASSERT_TRUE(nonExistentFlightNumberResult.has_value());

    // Act
    auto result = service->flightExistsByNumber(nonExistentFlightNumberResult.value());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}
