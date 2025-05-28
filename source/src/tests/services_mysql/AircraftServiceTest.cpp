#include <gtest/gtest.h>
#include "../../services/AircraftService.h"
#include "../../repositories/MySQLRepository/AircraftRepository.h"
#include "../../repositories/MySQLRepository/FlightRepository.h"
#include "../../repositories/MySQLRepository/TicketRepository.h"
#include "../../repositories/MySQLRepository/PassengerRepository.h"
#include "../../core/entities/Aircraft.h"
#include "../../utils/Logger.h"
#include "../../database/MySQLXConnection.h"
#include <memory>

class AircraftServiceTest : public ::testing::Test
{
protected:
    std::shared_ptr<AircraftRepository> _aircraftRepository;
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<TicketRepository> _ticketRepository;
    std::shared_ptr<PassengerRepository> _passengerRepository;
    std::shared_ptr<AircraftService> _service;
    std::shared_ptr<MySQLXConnection> _db;
    std::shared_ptr<Logger> _logger;
    AircraftSerial _serial;
    AircraftModel _model;
    SeatClassMap _seatLayout;

    void SetUp() override
    {
        // Setup database connection
        _db = MySQLXConnection::getInstance();
        auto result = _db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " << result.error().message;

        // Setup logger
        _logger = Logger::getInstance();
        _logger->setMinLevel(LogLevel::DEBUG);

        // Setup repositories and service
        _aircraftRepository = std::make_shared<AircraftRepository>(_db, _logger);
        _flightRepository = std::make_shared<FlightRepository>(_db, _logger);
        _passengerRepository = std::make_shared<PassengerRepository>(_db, _logger);
        _ticketRepository = std::make_shared<TicketRepository>(_db, _passengerRepository, _flightRepository, _logger);
        _service = std::make_shared<AircraftService>(_aircraftRepository, _flightRepository, _ticketRepository, _logger);

        // Setup test aircraft serial
        auto serialResult = AircraftSerial::create("VN113");
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
        auto result = _db->execute("DELETE FROM aircraft WHERE serial_number = 'VN113'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        _db->disconnect();
    }

    // Helper method to create a test aircraft
    Result<Aircraft> createTestAircraft()
    {
        return Aircraft::create(_serial, _model, _seatLayout);
    }
};

// Test getAircraft
TEST_F(AircraftServiceTest, GetAircraftSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->getAircraft(_serial);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());
}

TEST_F(AircraftServiceTest, GetAircraftNotFound)
{
    // Arrange
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());

    // Act
    auto result = _service->getAircraft(nonExistentSerialResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_FOUND");
}

// Test getAllAircraft
TEST_F(AircraftServiceTest, GetAllAircraftSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->getAllAircraft();

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result.value().size(), 1);
}

// Test aircraftExists
TEST_F(AircraftServiceTest, AircraftExistsSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->aircraftExists(_serial);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(AircraftServiceTest, AircraftExistsNotFound)
{
    // Arrange
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());

    // Act
    auto result = _service->aircraftExists(nonExistentSerialResult.value());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test createAircraft
TEST_F(AircraftServiceTest, CreateAircraftSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());

    // Act
    auto result = _service->createAircraft(aircraftResult.value());

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), _model);
    EXPECT_EQ(result.value().getSerial().toString(), _serial.toString());
    EXPECT_EQ(result.value().getSeatLayout().toString(), _seatLayout.toString());
}

TEST_F(AircraftServiceTest, CreateAircraftDuplicateSerial)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->createAircraft(aircraftResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "DUPLICATE_SERIAL");
}

// Test updateAircraft
TEST_F(AircraftServiceTest, UpdateAircraftSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
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
    auto result = _service->updateAircraft(updatedAircraft);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getModel(), "Boeing 737 MAX");
}

TEST_F(AircraftServiceTest, UpdateAircraftNotFound)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto aircraft = aircraftResult.value();
    aircraft.setId(999);

    // Act
    auto result = _service->updateAircraft(aircraft);

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_FOUND");
}

// Test deleteAircraft
TEST_F(AircraftServiceTest, DeleteAircraftSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->deleteAircraft(_serial);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    // Verify aircraft no longer exists
    auto existsResult = _service->aircraftExists(_serial);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(existsResult.value());
}

TEST_F(AircraftServiceTest, DeleteAircraftNotFound)
{
    // Arrange
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());

    // Act
    auto result = _service->deleteAircraft(nonExistentSerialResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_FOUND");
}

// Test getAvailableSeatClasses
TEST_F(AircraftServiceTest, GetAvailableSeatClassesSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->getAvailableSeatClasses(_serial);

    // Assert
    ASSERT_TRUE(result.has_value());
    auto seatClasses = result.value();
    EXPECT_EQ(seatClasses.size(), 3);
    EXPECT_TRUE(std::find(seatClasses.begin(), seatClasses.end(), "ECONOMY") != seatClasses.end());
    EXPECT_TRUE(std::find(seatClasses.begin(), seatClasses.end(), "BUSINESS") != seatClasses.end());
    EXPECT_TRUE(std::find(seatClasses.begin(), seatClasses.end(), "FIRST") != seatClasses.end());
}

TEST_F(AircraftServiceTest, GetAvailableSeatClassesNotFound)
{
    // Arrange
    auto nonExistentSerialResult = AircraftSerial::create("VN999");
    ASSERT_TRUE(nonExistentSerialResult.has_value());

    // Act
    auto result = _service->getAvailableSeatClasses(nonExistentSerialResult.value());

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "NOT_FOUND");
}

// Test getAvailableSeats
TEST_F(AircraftServiceTest, GetAvailableSeatsSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->getAvailableSeats(_serial, "ECONOMY");

    // Assert
    ASSERT_TRUE(result.has_value());
    auto availableSeats = result.value();
    EXPECT_EQ(availableSeats.size(), 100); // All economy seats should be available
    EXPECT_TRUE(std::find(availableSeats.begin(), availableSeats.end(), "E001") != availableSeats.end());
    EXPECT_TRUE(std::find(availableSeats.begin(), availableSeats.end(), "E100") != availableSeats.end());
}

TEST_F(AircraftServiceTest, GetAvailableSeatsInvalidClass)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->getAvailableSeats(_serial, "INVALID");

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "INVALID_SEAT_CLASS");
}

// Test isSeatAvailable
TEST_F(AircraftServiceTest, IsSeatAvailableSuccess)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->isSeatAvailable(_serial, "E001");

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(AircraftServiceTest, IsSeatAvailableInvalidSeat)
{
    // Arrange
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = _aircraftRepository->create(aircraftResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Act
    auto result = _service->isSeatAvailable(_serial, "INVALID");

    // Assert
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, "INVALID_SEAT_NUMBER");
}
