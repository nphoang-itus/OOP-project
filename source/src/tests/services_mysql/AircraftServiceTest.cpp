#include <gtest/gtest.h>
#include "../../services/AircraftService.h"
#include "../../repositories/MySQLRepository/AircraftRepository.h"
#include "../../core/entities/Aircraft.h"
#include "../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../core/value_objects/seat_class_map/SeatClass.h"
#include "../../utils/Logger.h"
#include "../../database/MySQLXConnection.h"
#include <memory>

class AircraftServiceTest : public ::testing::Test
{
protected:
    std::shared_ptr<AircraftRepository> repository;
    std::shared_ptr<AircraftService> service;
    std::shared_ptr<MySQLXConnection> db;
    std::shared_ptr<Logger> logger;
    AircraftSerial _serial;
    AircraftModel _model;
    SeatClassMap _seatLayout;

    void SetUp() override
    {
        // Setup database connection
        db = MySQLXConnection::getInstance();
        auto result = db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " << result.error().message;

        // Setup logger
        logger = Logger::getInstance();
        logger->setMinLevel(LogLevel::DEBUG);

        // Setup repositories and service
        repository = std::make_shared<AircraftRepository>(db, logger);
        service = std::make_shared<AircraftService>(repository);

        // Setup test aircraft serial
        auto serialResult = AircraftSerial::create("VN199");
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
        // Clean up test data
        auto result = db->execute("DELETE FROM aircraft WHERE serial_number = 'VN199'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        db->disconnect();
    }

    // Helper method to create a test aircraft
    Result<Aircraft> createTestAircraft()
    {
        return Aircraft::create(_serial, _model, _seatLayout);
    }
};

// Test getById
TEST_F(AircraftServiceTest, GetByIdSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->getById(createResult->getId());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());
}

TEST_F(AircraftServiceTest, GetByIdFailure)
{
    // Act
    auto result = service->getById(999);

    // Assert
    ASSERT_FALSE(result.has_value());
}

// Test getAll
TEST_F(AircraftServiceTest, GetAllSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
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

    // Act
    auto result = service->getAll();

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result.value().size(), 2);
}

// Test create
TEST_F(AircraftServiceTest, CreateSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());

    // Act
    auto result = service->create(aircraftResult.value());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());
}

TEST_F(AircraftServiceTest, CreateFailureDuplicateSerial)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    repository->create(aircraftResult.value());

    // Act
    auto result = service->create(aircraftResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().message, "Aircraft with serial number already exists");
}

// Test update
TEST_F(AircraftServiceTest, UpdateSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
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

    // Act
    auto result = service->update(updatedAircraft);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), "Boeing 737 MAX");
}

TEST_F(AircraftServiceTest, UpdateFailureNotFound)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto aircraft = aircraftResult.value();
    aircraft.setId(999);

    // Act
    auto result = service->update(aircraft);

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().message, "Aircraft not found");
}

// Test deleteById
TEST_F(AircraftServiceTest, DeleteByIdSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->deleteById(createResult->getId());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    // Verify aircraft no longer exists
    auto existsResult = service->exists(createResult->getId());
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(existsResult.value());
}

TEST_F(AircraftServiceTest, DeleteByIdFailure)
{
    // Act
    auto result = service->deleteById(999);

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().message, "Aircraft not found");
}

// Test exists
TEST_F(AircraftServiceTest, ExistsSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->exists(createResult->getId());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(AircraftServiceTest, ExistsFailure)
{
    // Act
    auto result = service->exists(999);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test count
TEST_F(AircraftServiceTest, CountSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    repository->create(aircraftResult.value());

    // Act
    auto result = service->count();

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result.value(), 1);
}

// Test getAircraftBySerialNumber
TEST_F(AircraftServiceTest, GetAircraftBySerialNumberSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->getAircraftBySerialNumber(_serial);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());
}

TEST_F(AircraftServiceTest, GetAircraftBySerialNumberFailure)
{
    // Arrange
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());

    // Act
    auto result = service->getAircraftBySerialNumber(nonExistentSerialResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
}

// Test aircraftExistsBySerialNumber
TEST_F(AircraftServiceTest, AircraftExistsBySerialNumberSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->aircraftExistsBySerialNumber(_serial);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(AircraftServiceTest, AircraftExistsBySerialNumberFailure)
{
    // Arrange
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());

    // Act
    auto result = service->aircraftExistsBySerialNumber(nonExistentSerialResult.value());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test deleteAircraftBySerialNumber
TEST_F(AircraftServiceTest, DeleteAircraftBySerialNumberSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = service->deleteAircraftBySerialNumber(_serial);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    // Verify aircraft no longer exists
    auto existsResult = service->aircraftExistsBySerialNumber(_serial);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(existsResult.value());
}

TEST_F(AircraftServiceTest, DeleteAircraftBySerialNumberFailure)
{
    // Arrange
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());

    // Act
    auto result = service->deleteAircraftBySerialNumber(nonExistentSerialResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().message, "Aircraft not found");
}
