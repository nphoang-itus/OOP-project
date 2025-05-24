#include <gtest/gtest.h>
#include "../../../repositories/MySQLRepository/FlightRepository.h"
#include "../../../repositories/MySQLRepository/AircraftRepository.h"
#include "../../../core/value_objects/flight_number/FlightNumber.h"
#include "../../../core/value_objects/route/Route.h"
#include "../../../core/value_objects/schedule/Schedule.h"
#include "../../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../../core/exceptions/Result.h"
#include "../../../database/MySQLXConnection.h"
#include <memory>

class FlightRepositoryTest : public ::testing::Test {
protected:
    std::shared_ptr<AircraftRepository> aircraftRepository;
    std::shared_ptr<FlightRepository> repository;
    std::shared_ptr<MySQLXConnection> db;
    std::shared_ptr<Logger> logger;

    // Test data
    FlightNumber _flightNumber;
    Route _route;
    Schedule _schedule;
    std::shared_ptr<Aircraft> _aircraft;

    void SetUp() override {
        // Setup database connection
        db = MySQLXConnection::getInstance();
        auto result = db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " << result.error().message;

        // Setup logger
        logger = Logger::getInstance();
        logger->setMinLevel(LogLevel::DEBUG);

        // Setup repository
        repository = std::make_shared<FlightRepository>(db, logger);
        aircraftRepository = std::make_shared<AircraftRepository>(db, logger);

        // Setup test flight data
        auto flightNumberResult = FlightNumber::create("VN999");
        ASSERT_TRUE(flightNumberResult.has_value());
        _flightNumber = *flightNumberResult;

        auto routeResult = Route::create("Ho Chi Minh City(SGN)-Ha Noi(HAN)");
        ASSERT_TRUE(routeResult.has_value());
        _route = *routeResult;

        auto scheduleResult = Schedule::create("2025-05-22 07:30|2025-05-22 09:30");
        ASSERT_TRUE(scheduleResult.has_value());
        _schedule = *scheduleResult;

        // Create test aircraft
        auto serialResult = AircraftSerial::create("VN333");
        ASSERT_TRUE(serialResult.has_value());

        std::string seatLayoutStr = "E:100,B:20,F:10";
        auto seatLayoutResult = SeatClassMap::create(seatLayoutStr);
        ASSERT_TRUE(seatLayoutResult.has_value());

        auto aircraftResult = Aircraft::create(*serialResult, "Boeing 737", *seatLayoutResult);
        ASSERT_TRUE(aircraftResult.has_value());
        // Tạo Aircraft mới từ kết quả tạo
        _aircraft = std::make_shared<Aircraft>(*aircraftResult);

        // Lưu vào database nếu chưa tồn tại
        if (!aircraftRepository->existsAircraft(*serialResult).value()) {
            auto saveResult = aircraftRepository->create(*_aircraft);
            if (saveResult.has_value()) {
                _aircraft = std::make_shared<Aircraft>(saveResult.value());
            }
        } 
        // Nếu tồn tại trong database
        else {
            auto existingAircraft = aircraftRepository->findBySerialNumber(*serialResult);
            if (existingAircraft.has_value()) {
                _aircraft = std::make_shared<Aircraft>(existingAircraft.value());
            }
        }
    }

    void TearDown() override {
        // Clean up test data
        auto result = db->execute("DELETE FROM flight WHERE flight_number = 'VN999'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        db->disconnect();
    }

    // Helper method to create a test flight
    Result<Flight> createTestFlight() {
        return Flight::create(_flightNumber, _route, _schedule, _aircraft);
    }
};

// Test create operation
TEST_F(FlightRepositoryTest, CreateFlight) {
    // Create test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    const Flight& flight = *flightResult;

    // Save to database
    auto result = repository->create(flight);
    ASSERT_TRUE(result.has_value());
    const Flight& savedFlight = *result;

    // Verify saved flight
    EXPECT_GT(savedFlight.getId(), 0);
    EXPECT_EQ(savedFlight.getFlightNumber().toString(), _flightNumber.toString());
    EXPECT_EQ(savedFlight.getRoute().toString(), _route.toString());
    EXPECT_EQ(savedFlight.getSchedule().toString(), _schedule.toString());
    EXPECT_EQ(savedFlight.getAircraft()->getSerial().toString(), _aircraft->getSerial().toString());
}

// Test findById operation
TEST_F(FlightRepositoryTest, FindFlightById) {
    // Create and save test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(*flightResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Find flight by id
    auto result = repository->findById(id);
    ASSERT_TRUE(result.has_value());
    const Flight& foundFlight = *result;

    // Verify found flight
    EXPECT_EQ(foundFlight.getId(), id);
    EXPECT_EQ(foundFlight.getFlightNumber().toString(), _flightNumber.toString());
    EXPECT_EQ(foundFlight.getRoute().toString(), _route.toString());
    EXPECT_EQ(foundFlight.getSchedule().toString(), _schedule.toString());
    EXPECT_EQ(foundFlight.getAircraft()->getSerial().toString(), _aircraft->getSerial().toString());
}

// Test findAll operation
TEST_F(FlightRepositoryTest, FindAllFlights) {
    // Create and save test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(*flightResult);
    ASSERT_TRUE(createResult.has_value());

    // Find all flights
    auto result = repository->findAll();
    ASSERT_TRUE(result.has_value());
    const auto& flights = *result;

    // Verify found flights
    EXPECT_FALSE(flights.empty());
    bool found = false;
    for (const auto& flight : flights) {
        if (flight.getFlightNumber().toString() == _flightNumber.toString()) {
            found = true;
            EXPECT_EQ(flight.getRoute().toString(), _route.toString());
            EXPECT_EQ(flight.getSchedule().toString(), _schedule.toString());
            EXPECT_EQ(flight.getAircraft()->getSerial().toString(), _aircraft->getSerial().toString());
            break;
        }
    }
    EXPECT_TRUE(found) << "Test flight not found in findAll results";
}

// Test exists operation
TEST_F(FlightRepositoryTest, ExistsFlight) {
    // Create and save test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(*flightResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Check existence
    auto result = repository->exists(id);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);

    // Check non-existent id
    auto nonExistentResult = repository->exists(999999);
    ASSERT_TRUE(nonExistentResult.has_value());
    EXPECT_FALSE(*nonExistentResult);
}

// Test count operation
TEST_F(FlightRepositoryTest, CountFlights) {
    // Get initial count
    auto initialCountResult = repository->count();
    ASSERT_TRUE(initialCountResult.has_value());
    size_t initialCount = *initialCountResult;

    // Create and save test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(*flightResult);
    ASSERT_TRUE(createResult.has_value());

    // Get new count
    auto newCountResult = repository->count();
    ASSERT_TRUE(newCountResult.has_value());
    size_t newCount = *newCountResult;

    // Verify count increased by 1
    EXPECT_EQ(newCount, initialCount + 1);
}

// Test update operation
TEST_F(FlightRepositoryTest, UpdateFlight) {
    // Create and save test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(*flightResult);
    ASSERT_TRUE(createResult.has_value());
    Flight flight = *createResult;

    // Update flight
    auto newScheduleResult = Schedule::create("2025-05-23 07:30|2025-05-23 09:30");
    ASSERT_TRUE(newScheduleResult.has_value());
    flight.setSchedule(*newScheduleResult);

    auto result = repository->update(flight);
    ASSERT_TRUE(result.has_value());
    const Flight& updatedFlight = *result;

    // Verify updated flight
    EXPECT_EQ(updatedFlight.getId(), flight.getId());
    EXPECT_EQ(updatedFlight.getFlightNumber().toString(), _flightNumber.toString());
    EXPECT_EQ(updatedFlight.getRoute().toString(), _route.toString());
    EXPECT_EQ(updatedFlight.getSchedule().toString(), newScheduleResult->toString());
    EXPECT_EQ(updatedFlight.getAircraft()->getSerial().toString(), _aircraft->getSerial().toString());
}

// Test deleteById operation
TEST_F(FlightRepositoryTest, DeleteFlightById) {
    // Create and save test flight
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(*flightResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Delete flight
    auto result = repository->deleteById(id);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);

    // Verify flight is deleted
    auto existsResult = repository->exists(id);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(*existsResult);
}

// Test error handling
TEST_F(FlightRepositoryTest, ErrorHandling) {
    // Test findById with invalid id
    auto result1 = repository->findById(-1);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "NOT_FOUND");

    // Test update with non-existent id
    auto flightResult = createTestFlight();
    ASSERT_TRUE(flightResult.has_value());
    Flight flight = *flightResult;
    flight.setId(999999);
    auto result2 = repository->update(flight);
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "DB_ERROR");

    // Test deleteById with non-existent id
    auto result3 = repository->deleteById(999999);
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, "DB_ERROR");
} 