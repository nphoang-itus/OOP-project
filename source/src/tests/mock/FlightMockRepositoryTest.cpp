#ifndef FLIGHT_MOCK_REPOSITORY_TEST_H
#define FLIGHT_MOCK_REPOSITORY_TEST_H

#include <gtest/gtest.h>
#include "repositories/MockRepository/FlightMockRepository.h"
#include "core/entities/Flight.h"
#include "core/value_objects/flight_number/FlightNumber.h"
#include "core/value_objects/route/Route.h"
#include "core/value_objects/schedule/Schedule.h"
#include <memory>

class FlightMockRepositoryTest : public ::testing::Test
{
protected:
    std::shared_ptr<FlightMockRepository> repository;
    FlightNumber _flightNumber;
    Route _route;
    Schedule _schedule;
    int _aircraftId;

    void SetUp() override
    {
        repository = std::make_shared<FlightMockRepository>();

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

        // Setup test aircraft id
        _aircraftId = 1;
    }

    void TearDown() override
    {
        repository->clear();
    }

    // Helper method to create a flight
    Result<Flight> createFlight()
    {
        auto aircraftResult = Aircraft::create("VN123", "A320", "E:100,B:20,F:10");
        if (!aircraftResult.has_value())
        {
            return std::unexpected(aircraftResult.error());
        }
        auto aircraft = std::make_shared<Aircraft>(aircraftResult.value());
        aircraft->setId(_aircraftId);
        return Flight::create(_flightNumber, _route, _schedule, aircraft);
    }
};

// Test create operation
TEST_F(FlightMockRepositoryTest, CreateFlight)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "CreateFlight failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());

    auto result = repository->create(flightResult.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), _flightNumber);
    EXPECT_EQ(result.value().getRoute(), _route);
    EXPECT_EQ(result.value().getSchedule(), _schedule);
    EXPECT_EQ(result.value().getAircraft()->getId(), _aircraftId);
    EXPECT_EQ(result.value().getId(), 1);
}

// Test findById operation
TEST_F(FlightMockRepositoryTest, FindById)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "FindById (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(flightResult.value());
    if (!createResult.has_value())
    {
        std::cerr << "FindById (repository->create) failed: " << createResult.error().message << std::endl;
    }
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->findById(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), _flightNumber);
    EXPECT_EQ(result.value().getRoute(), _route);
    EXPECT_EQ(result.value().getSchedule(), _schedule);
    EXPECT_EQ(result.value().getAircraft()->getId(), _aircraftId);

    auto notFoundResult = repository->findById(999);
    ASSERT_FALSE(notFoundResult.has_value());
}

// Test findAll operation
TEST_F(FlightMockRepositoryTest, FindAll)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "FindAll (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    repository->create(flightResult.value());

    // Create another flight
    auto flightNumberResult = FlightNumber::create("VN456");
    if (!flightNumberResult.has_value())
    {
        std::cerr << "FindAll (FlightNumber::create) failed" << std::endl;
    }
    ASSERT_TRUE(flightNumberResult.has_value());

    auto aircraftResult = Aircraft::create("VN123", "A320", "E:100,B:20,F:10");
    if (!aircraftResult.has_value())
    {
        std::cerr << "FindAll (Aircraft::create) failed: " << aircraftResult.error().message << std::endl;
    }
    ASSERT_TRUE(aircraftResult.has_value());
    auto aircraft = std::make_shared<Aircraft>(aircraftResult.value());
    aircraft->setId(_aircraftId);
    auto flightResult2 = Flight::create(
        flightNumberResult.value(),
        _route,
        _schedule,
        aircraft);
    if (!flightResult2.has_value())
    {
        std::cerr << "FindAll (Flight::create) failed: " << flightResult2.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult2.has_value());

    repository->create(flightResult2.value());

    auto result = repository->findAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

// Test update operation
TEST_F(FlightMockRepositoryTest, UpdateFlight)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "UpdateFlight (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(flightResult.value());
    if (!createResult.has_value())
    {
        std::cerr << "UpdateFlight (repository->create) failed: " << createResult.error().message << std::endl;
    }
    ASSERT_TRUE(createResult.has_value());

    // Create a new flight with updated flight number
    auto newFlightNumberResult = FlightNumber::create("VN789");
    if (!newFlightNumberResult.has_value())
    {
        std::cerr << "UpdateFlight (FlightNumber::create) failed" << std::endl;
    }
    ASSERT_TRUE(newFlightNumberResult.has_value());

    auto aircraftResult = Aircraft::create("VN123", "A320", "E:100,B:20,F:10");
    if (!aircraftResult.has_value())
    {
        std::cerr << "UpdateFlight (Aircraft::create) failed: " << aircraftResult.error().message << std::endl;
    }
    ASSERT_TRUE(aircraftResult.has_value());
    auto aircraft = std::make_shared<Aircraft>(aircraftResult.value());
    aircraft->setId(_aircraftId);
    auto updatedFlightResult = Flight::create(
        newFlightNumberResult.value(),
        _route,
        _schedule,
        aircraft);
    if (!updatedFlightResult.has_value())
    {
        std::cerr << "UpdateFlight (Flight::create) failed: " << updatedFlightResult.error().message << std::endl;
    }
    ASSERT_TRUE(updatedFlightResult.has_value());
    auto updatedFlight = updatedFlightResult.value();
    updatedFlight.setId(createResult.value().getId());

    auto result = repository->update(updatedFlight);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), newFlightNumberResult.value());

    auto findResult = repository->findById(1);
    ASSERT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult.value().getFlightNumber(), newFlightNumberResult.value());
}

// Test deleteById operation
TEST_F(FlightMockRepositoryTest, DeleteById)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "DeleteById (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(flightResult.value());
    if (!createResult.has_value())
    {
        std::cerr << "DeleteById (repository->create) failed: " << createResult.error().message << std::endl;
    }
    ASSERT_TRUE(createResult.has_value());

    auto deleteResult = repository->deleteById(1);
    ASSERT_TRUE(deleteResult.has_value());
    EXPECT_TRUE(deleteResult.value());

    auto findResult = repository->findById(1);
    ASSERT_FALSE(findResult.has_value());
}

// Test exists operation
TEST_F(FlightMockRepositoryTest, Exists)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "Exists (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(flightResult.value());
    if (!createResult.has_value())
    {
        std::cerr << "Exists (repository->create) failed: " << createResult.error().message << std::endl;
    }
    ASSERT_TRUE(createResult.has_value());

    auto existsResult = repository->exists(1);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_TRUE(existsResult.value());

    auto notExistsResult = repository->exists(999);
    ASSERT_TRUE(notExistsResult.has_value());
    EXPECT_FALSE(notExistsResult.value());
}

// Test count operation
TEST_F(FlightMockRepositoryTest, Count)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "Count (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    repository->create(flightResult.value());
    EXPECT_TRUE(repository->count().has_value());
    EXPECT_EQ(repository->count().value(), 1);
}

// Test findByFlightNumber operation
TEST_F(FlightMockRepositoryTest, FindByFlightNumber)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "FindByFlightNumber (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(flightResult.value());
    if (!createResult.has_value())
    {
        std::cerr << "FindByFlightNumber (repository->create) failed: " << createResult.error().message << std::endl;
    }
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->findByFlightNumber(_flightNumber);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFlightNumber(), _flightNumber);
    EXPECT_EQ(result.value().getRoute(), _route);
    EXPECT_EQ(result.value().getSchedule(), _schedule);
    EXPECT_EQ(result.value().getAircraft()->getId(), _aircraftId);

    // Test with non-existent flight number
    auto nonExistentFlightNumberResult = FlightNumber::create("VN999");
    ASSERT_TRUE(nonExistentFlightNumberResult.has_value());
    auto notFoundResult = repository->findByFlightNumber(nonExistentFlightNumberResult.value());
    ASSERT_FALSE(notFoundResult.has_value());
}

// Test existsFlight operation
TEST_F(FlightMockRepositoryTest, ExistsFlight)
{
    auto flightResult = createFlight();
    if (!flightResult.has_value())
    {
        std::cerr << "ExistsFlight (createFlight) failed: " << flightResult.error().message << std::endl;
    }
    ASSERT_TRUE(flightResult.has_value());
    auto createResult = repository->create(flightResult.value());
    if (!createResult.has_value())
    {
        std::cerr << "ExistsFlight (repository->create) failed: " << createResult.error().message << std::endl;
    }
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->existsFlight(_flightNumber);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    // Test with non-existent flight number
    auto nonExistentFlightNumberResult = FlightNumber::create("VN999");
    ASSERT_TRUE(nonExistentFlightNumberResult.has_value());
    auto notFoundResult = repository->existsFlight(nonExistentFlightNumberResult.value());
    ASSERT_TRUE(notFoundResult.has_value());
    EXPECT_FALSE(notFoundResult.value());
}

#endif // FLIGHT_MOCK_REPOSITORY_TEST_H
