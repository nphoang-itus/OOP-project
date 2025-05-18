#include <gtest/gtest.h>
#include "../../../core/entities/Aircraft.h"
#include "../../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../../core/exceptions/Result.h"

class AircraftTest : public ::testing::Test {
protected:
    // Test data
    AircraftSerial _serial;
    AircraftModel _model;
    SeatClassMap _seatLayout;

    void SetUp() override {
        // Setup test aircraft serial
        auto serialResult = AircraftSerial::create("VN123");
        ASSERT_TRUE(serialResult.has_value());
        _serial = *serialResult;

        // Setup test model
        _model = "Boeing 737";

        // Setup test seat layout
        auto seatLayoutResult = SeatClassMap::create({
            {SeatClassRegistry::getByName("ECONOMY").value(), 100},    // E001-E100
            {SeatClassRegistry::getByName("BUSINESS").value(), 20},    // B01-B20
            {SeatClassRegistry::getByName("FIRST").value(), 10}        // F01-F10
        });
        ASSERT_TRUE(seatLayoutResult.has_value());
        _seatLayout = seatLayoutResult.value();
    }

    // Helper method to create an aircraft
    Result<Aircraft> createAircraft() {
        return Aircraft::create(_serial, _model, _seatLayout);
    }
};

// Test aircraft creation
TEST_F(AircraftTest, CreateAircraft) {
    auto result = createAircraft();
    ASSERT_TRUE(result.has_value());
    const Aircraft& aircraft = *result;

    // Test getters
    EXPECT_EQ(aircraft.getSerial().toString(), _serial.toString());
    EXPECT_EQ(aircraft.getModel(), _model);
    EXPECT_EQ(aircraft.getSeatLayout().toString(), _seatLayout.toString());
}

// Test aircraft creation with invalid parameters
TEST_F(AircraftTest, CreateAircraftWithInvalidParameters) {
    // Test with empty model
    auto result1 = Aircraft::create(_serial, "", _seatLayout);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "INVALID_MODEL");
    EXPECT_EQ(result1.error().message, "Aircraft model cannot be empty");

    auto result2 = Aircraft::create("", _model, _seatLayout.toString());
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "INVALID_SERIAL");

    // Test with invalid seat layout
    auto result3 = Aircraft::create(_serial.toString(), _model, "");
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, "INVALID_SEAT_LAYOUT");
}

// Test value object operations
TEST_F(AircraftTest, ValueObjectOperations) {
    auto result1 = createAircraft();
    auto result2 = createAircraft();
    ASSERT_TRUE(result1.has_value() && result2.has_value());
    const Aircraft& aircraft1 = *result1;
    const Aircraft& aircraft2 = *result2;

    // Test equality
    EXPECT_TRUE(aircraft1.equals(aircraft2));

    // Test clone
    auto clone = aircraft1.clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_TRUE(aircraft1.equals(*clone));

    // Test toString
    std::string expected = "Aircraft{serial=" + _serial.toString() + 
                          ", model=" + _model + 
                          ", seatLayout=" + _seatLayout.toString() + "}";
    EXPECT_EQ(aircraft1.toString(), expected);
}

// Test string-based creation
TEST_F(AircraftTest, StringBasedCreation) {
    auto result = Aircraft::create("VN123", "Boeing 737", _seatLayout.toString());
    ASSERT_TRUE(result.has_value());
    const Aircraft& aircraft = *result;

    EXPECT_EQ(aircraft.getSerial().toString(), "VN123");
    EXPECT_EQ(aircraft.getModel(), "Boeing 737");
    EXPECT_EQ(aircraft.getSeatLayout(), _seatLayout);
}

// Test different aircraft models
TEST_F(AircraftTest, DifferentAircraftModels) {
    // Test with Airbus A320
    auto result1 = Aircraft::create(_serial, "Airbus A320", _seatLayout);
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->getModel(), "Airbus A320");

    // Test with Boeing 777
    auto result2 = Aircraft::create(_serial, "Boeing 777", _seatLayout);
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->getModel(), "Boeing 777");
}

// Test different seat layouts
TEST_F(AircraftTest, DifferentSeatLayouts) {
    // Create a different seat layout
    auto seatLayoutResult = SeatClassMap::create({
        {SeatClassRegistry::getByName("ECONOMY").value(), 50},     // E001-E050
        {SeatClassRegistry::getByName("BUSINESS").value(), 10}     // B01-B10
    });
    ASSERT_TRUE(seatLayoutResult.has_value());
    SeatClassMap differentLayout = seatLayoutResult.value();

    // Create aircraft with different layout
    auto result = Aircraft::create(_serial, _model, differentLayout);
    ASSERT_TRUE(result.has_value());
    const Aircraft& aircraft = *result;

    // Verify seat layout
    EXPECT_EQ(aircraft.getSeatLayout(), differentLayout);
    EXPECT_NE(aircraft.getSeatLayout(), _seatLayout);
} 