#ifndef AIRCRAFT_MOCK_REPOSITORY_TEST_H
#define AIRCRAFT_MOCK_REPOSITORY_TEST_H

#include <gtest/gtest.h>
#include "repositories/MockRepository/AircraftMockRepository.h"
#include "core/entities/Aircraft.h"
#include "core/value_objects/aircraft_serial/AircraftSerial.h"
#include "core/value_objects/seat_class_map/SeatClassMap.h"
#include "core/value_objects/seat_class_map/SeatClass.h"
#include <memory>

class AircraftMockRepositoryTest : public ::testing::Test
{
protected:
    std::shared_ptr<AircraftMockRepository> repository;
    AircraftSerial _serial;
    AircraftModel _model;
    SeatClassMap _seatLayout;

    void SetUp() override
    {
        repository = std::make_shared<AircraftMockRepository>();

        // Setup test aircraft serial
        auto serialResult = AircraftSerial::create("VN123");
        ASSERT_TRUE(serialResult.has_value());
        _serial = *serialResult;

        // Setup test model
        _model = "Boeing 737";

        // Setup test seat layout
        auto seatLayoutResult = SeatClassMap::create({
            {SeatClassRegistry::getByName("ECONOMY").value(), 100}, // E001-E100
            {SeatClassRegistry::getByName("BUSINESS").value(), 20}, // B01-B20
            {SeatClassRegistry::getByName("FIRST").value(), 10}     // F01-F10
        });
        ASSERT_TRUE(seatLayoutResult.has_value());
        _seatLayout = seatLayoutResult.value();
    }

    void TearDown() override
    {
        repository->clear();
    }

    // Helper method to create an aircraft
    Result<Aircraft> createAircraft()
    {
        return Aircraft::create(_serial, _model, _seatLayout);
    }
};

// Test create operation
TEST_F(AircraftMockRepositoryTest, CreateAircraft)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());

    auto result = repository->create(aircraftResult.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());
    EXPECT_EQ(result.value().getId(), 1);
}

// Test findById operation
TEST_F(AircraftMockRepositoryTest, FindById)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->findById(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());

    auto notFoundResult = repository->findById(999);
    ASSERT_FALSE(notFoundResult.has_value());
}

// Test findAll operation
TEST_F(AircraftMockRepositoryTest, FindAll)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    repository->create(aircraftResult.value());

    // Create another aircraft
    auto serialResult = AircraftSerial::create("VN320");
    ASSERT_TRUE(serialResult.has_value());

    auto seatLayoutResult = SeatClassMap::create({
        {SeatClassRegistry::getByName("ECONOMY").value(), 50}, // E001-E050
        {SeatClassRegistry::getByName("BUSINESS").value(), 10} // B01-B10
    });
    ASSERT_TRUE(seatLayoutResult.has_value());

    auto aircraftResult2 = Aircraft::create(
        serialResult.value(),
        "Airbus A320",
        seatLayoutResult.value());
    ASSERT_TRUE(aircraftResult2.has_value());

    repository->create(aircraftResult2.value());

    auto result = repository->findAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

// Test update operation
TEST_F(AircraftMockRepositoryTest, UpdateAircraft)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Create a new aircraft with updated model
    auto updatedAircraftResult = Aircraft::create(
        createResult.value().getSerial(),
        "Boeing 737 MAX",
        createResult.value().getSeatLayout());
    ASSERT_TRUE(updatedAircraftResult.has_value());
    auto updatedAircraft = updatedAircraftResult.value();
    updatedAircraft.setId(createResult.value().getId());

    auto result = repository->update(updatedAircraft);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), "Boeing 737 MAX");

    auto findResult = repository->findById(1);
    ASSERT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult.value().getModel(), "Boeing 737 MAX");
}

// Test deleteById operation
TEST_F(AircraftMockRepositoryTest, DeleteById)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto deleteResult = repository->deleteById(1);
    ASSERT_TRUE(deleteResult.has_value());
    EXPECT_TRUE(deleteResult.value());

    auto findResult = repository->findById(1);
    ASSERT_FALSE(findResult.has_value());
}

// Test exists operation
TEST_F(AircraftMockRepositoryTest, Exists)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto existsResult = repository->exists(1);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_TRUE(existsResult.value());

    auto notExistsResult = repository->exists(999);
    ASSERT_TRUE(notExistsResult.has_value());
    EXPECT_FALSE(notExistsResult.value());
}

// Test count operation
TEST_F(AircraftMockRepositoryTest, Count)
{
    EXPECT_TRUE(repository->count().has_value());
    EXPECT_EQ(repository->count().value(), 0);

    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    repository->create(aircraftResult.value());
    EXPECT_TRUE(repository->count().has_value());
    EXPECT_EQ(repository->count().value(), 1);
}

// Test different aircraft models
TEST_F(AircraftMockRepositoryTest, DifferentAircraftModels)
{
    // Test with Airbus A320
    auto serialResult = AircraftSerial::create("VN320");
    ASSERT_TRUE(serialResult.has_value());

    auto aircraftResult = Aircraft::create(
        serialResult.value(),
        "Airbus A320",
        _seatLayout);
    ASSERT_TRUE(aircraftResult.has_value());

    auto result = repository->create(aircraftResult.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), "Airbus A320");

    // Test with Boeing 777
    auto serialResult2 = AircraftSerial::create("VN777");
    ASSERT_TRUE(serialResult2.has_value());

    auto aircraftResult2 = Aircraft::create(
        serialResult2.value(),
        "Boeing 777",
        _seatLayout);
    ASSERT_TRUE(aircraftResult2.has_value());

    auto result2 = repository->create(aircraftResult2.value());
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2.value().getModel(), "Boeing 777");
}

// Test different seat layouts
TEST_F(AircraftMockRepositoryTest, DifferentSeatLayouts)
{
    // Create a different seat layout
    auto seatLayoutResult = SeatClassMap::create({
        {SeatClassRegistry::getByName("ECONOMY").value(), 50}, // E001-E050
        {SeatClassRegistry::getByName("BUSINESS").value(), 10} // B01-B10
    });
    ASSERT_TRUE(seatLayoutResult.has_value());
    SeatClassMap differentLayout = seatLayoutResult.value();

    // Create aircraft with different layout
    auto aircraftResult = Aircraft::create(_serial, _model, differentLayout);
    ASSERT_TRUE(aircraftResult.has_value());

    auto result = repository->create(aircraftResult.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getSeatLayout(), differentLayout);
    EXPECT_NE(result.value().getSeatLayout(), _seatLayout);
}

// Test findBySerialNumber operation
TEST_F(AircraftMockRepositoryTest, FindBySerialNumber)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->findBySerialNumber(_serial);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());

    // Test with non-existent serial number
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());
    auto notFoundResult = repository->findBySerialNumber(nonExistentSerialResult.value());
    ASSERT_FALSE(notFoundResult.has_value());
}

// Test existsAircraft operation
TEST_F(AircraftMockRepositoryTest, ExistsAircraft)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->existsAircraft(_serial);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    // Test with non-existent serial number
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());
    auto notFoundResult = repository->existsAircraft(nonExistentSerialResult.value());
    ASSERT_TRUE(notFoundResult.has_value());
    EXPECT_FALSE(notFoundResult.value());
}

// Test deleteBySerialNumber operation
TEST_F(AircraftMockRepositoryTest, DeleteBySerialNumber)
{
    auto aircraftResult = createAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Test successful deletion
    auto deleteResult = repository->deleteBySerialNumber(_serial);
    ASSERT_TRUE(deleteResult.has_value());
    EXPECT_TRUE(deleteResult.value());

    // Verify aircraft no longer exists
    auto existsResult = repository->existsAircraft(_serial);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(existsResult.value());

    // Test deletion of non-existent aircraft
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());
    auto notFoundResult = repository->deleteBySerialNumber(nonExistentSerialResult.value());
    ASSERT_TRUE(notFoundResult.has_value());
    EXPECT_FALSE(notFoundResult.value());
}

#endif