#include <gtest/gtest.h>
#include "../../../repositories/MySQLRepository/AircraftRepository.h"
#include "../../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../../core/exceptions/Result.h"
#include "../../../database/MySQLXConnection.h"
#include <memory>

class AircraftRepositoryTest : public ::testing::Test {
protected:
    std::shared_ptr<AircraftRepository> repository;
    std::shared_ptr<MySQLXConnection> db;
    std::shared_ptr<Logger> logger;

    // Test data
    AircraftSerial _serial;
    std::string _model;
    SeatClassMap _seatLayout;

    void SetUp() override {
        // Setup database connection
        db = MySQLXConnection::getInstance();
        auto result = db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " << result.error().message;

        // Setup logger
        logger = Logger::getInstance();
        logger->setMinLevel(LogLevel::DEBUG);

        // Setup repository
        repository = std::make_shared<AircraftRepository>(db, logger);

        // Setup test aircraft data
        auto serialResult = AircraftSerial::create("VN333");
        ASSERT_TRUE(serialResult.has_value());
        _serial = *serialResult;

        _model = "Boeing 737";

        // Create seat layout string
        std::string seatLayoutStr = "E:100,B:20,F:10";
        auto seatLayoutResult = SeatClassMap::create(seatLayoutStr);
        ASSERT_TRUE(seatLayoutResult.has_value());
        _seatLayout = seatLayoutResult.value();
    }

    void TearDown() override {
        // Clean up test data
        auto result = db->execute("DELETE FROM aircraft WHERE serial_number = 'VN333'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        db->disconnect();
    }

    // Helper method to create a test aircraft
    Result<Aircraft> createTestAircraft() {
        return Aircraft::create(_serial, _model, _seatLayout);
    }
};

// Test create operation
TEST_F(AircraftRepositoryTest, CreateAircraft) {
    // Create test aircraft
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    const Aircraft& aircraft = *aircraftResult;

    // Save to database
    auto result = repository->create(aircraft);
    ASSERT_TRUE(result.has_value());
    const Aircraft& savedAircraft = *result;

    // Verify saved aircraft
    EXPECT_GT(savedAircraft.getId(), 0);
    EXPECT_EQ(savedAircraft.getSerial().toString(), _serial.toString());
    EXPECT_EQ(savedAircraft.getModel(), _model);
    EXPECT_EQ(savedAircraft.getSeatLayout().toString(), _seatLayout.toString());
}

// Test findById operation
TEST_F(AircraftRepositoryTest, FindAircraftById) {
    // Create and save test aircraft
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(*aircraftResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Find aircraft by id
    auto result = repository->findById(id);
    ASSERT_TRUE(result.has_value());
    const Aircraft& foundAircraft = *result;

    // Verify found aircraft
    EXPECT_EQ(foundAircraft.getId(), id);
    EXPECT_EQ(foundAircraft.getSerial().toString(), _serial.toString());
    EXPECT_EQ(foundAircraft.getModel(), _model);
    EXPECT_EQ(foundAircraft.getSeatLayout().toString(), _seatLayout.toString());
}

// Test findAll operation
TEST_F(AircraftRepositoryTest, FindAllAircraft) {
    // Create and save test aircraft
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(*aircraftResult);
    ASSERT_TRUE(createResult.has_value());

    // Find all aircraft
    auto result = repository->findAll();
    ASSERT_TRUE(result.has_value());
    const auto& aircrafts = *result;

    // Verify found aircrafts
    EXPECT_FALSE(aircrafts.empty());
    bool found = false;
    for (const auto& aircraft : aircrafts) {
        if (aircraft.getSerial().toString() == _serial.toString()) {
            found = true;
            EXPECT_EQ(aircraft.getModel(), _model);
            EXPECT_EQ(aircraft.getSeatLayout().toString(), _seatLayout.toString());
            break;
        }
    }
    EXPECT_TRUE(found) << "Test aircraft not found in findAll results";
}

// Test exists operation
TEST_F(AircraftRepositoryTest, ExistsAircraft) {
    // Create and save test aircraft
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(*aircraftResult);
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
TEST_F(AircraftRepositoryTest, CountAircraft) {
    // Get initial count
    auto initialCountResult = repository->count();
    ASSERT_TRUE(initialCountResult.has_value());
    size_t initialCount = *initialCountResult;

    // Create and save test aircraft
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(*aircraftResult);
    ASSERT_TRUE(createResult.has_value());

    // Get new count
    auto newCountResult = repository->count();
    ASSERT_TRUE(newCountResult.has_value());
    size_t newCount = *newCountResult;

    // Verify count increased by 1
    EXPECT_EQ(newCount, initialCount + 1);
}

// Test update operation
TEST_F(AircraftRepositoryTest, UpdateAircraft) {
    // Create and save test aircraft
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(*aircraftResult);
    ASSERT_TRUE(createResult.has_value());
    Aircraft aircraft = *createResult;

    // Update aircraft
    aircraft.setModel("Airbus A320");
    auto result = repository->update(aircraft);
    ASSERT_TRUE(result.has_value());
    const Aircraft& updatedAircraft = *result;

    // Verify updated aircraft
    EXPECT_EQ(updatedAircraft.getId(), aircraft.getId());
    EXPECT_EQ(updatedAircraft.getSerial().toString(), _serial.toString());
    EXPECT_EQ(updatedAircraft.getModel(), "Airbus A320");
    EXPECT_EQ(updatedAircraft.getSeatLayout().toString(), _seatLayout.toString());
}

// Test deleteById operation
TEST_F(AircraftRepositoryTest, DeleteAircraftById) {
    // Create and save test aircraft
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    auto createResult = repository->create(*aircraftResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Delete aircraft
    auto result = repository->deleteById(id);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);

    // Verify aircraft is deleted
    auto existsResult = repository->exists(id);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(*existsResult);
}

// Test error handling
TEST_F(AircraftRepositoryTest, ErrorHandling) {
    // Test findById with invalid id
    auto result1 = repository->findById(-1);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "NOT_FOUND");

    // Test update with non-existent id
    auto aircraftResult = createTestAircraft();
    ASSERT_TRUE(aircraftResult.has_value());
    Aircraft aircraft = *aircraftResult;
    aircraft.setId(999999);
    auto result2 = repository->update(aircraft);
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "DB_ERROR");

    // Test deleteById with non-existent id
    auto result3 = repository->deleteById(999999);
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, "DB_ERROR");
} 