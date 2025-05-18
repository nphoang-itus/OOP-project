#include <gtest/gtest.h>
#include "../../../core/entities/Flight.h"
#include "../../../core/value_objects/flight_number/FlightNumber.h"
#include "../../../core/value_objects/route/Route.h"
#include "../../../core/value_objects/schedule/Schedule.h"
#include "../../../core/value_objects/seat_number/SeatNumber.h"
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../../core/exceptions/Result.h"

class FlightTest : public ::testing::Test {
protected:
    // Test data
    std::shared_ptr<Aircraft> _aircraft;
    FlightNumber _flightNumber;
    Route _route;
    Schedule _schedule;
    SeatClassMap _seatLayout;

    void SetUp() override {
        // Setup test aircraft with seat layout
        auto seatLayoutResult = SeatClassMap::create({
            {SeatClassRegistry::getByName("ECONOMY").value(), 100},    // E001-E100
            {SeatClassRegistry::getByName("BUSINESS").value(), 20},    // B01-B20
            {SeatClassRegistry::getByName("FIRST").value(), 10}        // F01-F10
        });
        ASSERT_TRUE(seatLayoutResult.has_value());
        _seatLayout = seatLayoutResult.value();

        // Create test aircraft
        auto serialResult = AircraftSerial::create("VN123");
        ASSERT_TRUE(serialResult.has_value());
        auto aircraftResult = Aircraft::create(*serialResult, "Boeing 737", _seatLayout);
        ASSERT_TRUE(aircraftResult.has_value());
        _aircraft = std::make_shared<Aircraft>(*aircraftResult);

        // Create test flight number
        auto flightNumberResult = FlightNumber::create("VN123");
        ASSERT_TRUE(flightNumberResult.has_value());
        _flightNumber = *flightNumberResult;

        // Create test route
        auto routeResult = Route::create("Ho Chi Minh City(SGN)-Ha Noi(HAN)");
        ASSERT_TRUE(routeResult.has_value());
        _route = *routeResult;

        // Create test schedule
        auto scheduleResult = Schedule::create("2024-03-20 10:00|2024-03-20 12:00");
        ASSERT_TRUE(scheduleResult.has_value());
        _schedule = *scheduleResult;
    }

    // Helper method to create a flight
    Result<Flight> createFlight() {
        return Flight::create(_flightNumber, _route, _schedule, _aircraft);
    }
};

// Test flight creation
TEST_F(FlightTest, CreateFlight) {
    auto result = createFlight();
    ASSERT_TRUE(result.has_value());
    const Flight& flight = *result;

    // Test getters
    EXPECT_EQ(flight.getFlightNumber(), _flightNumber);
    EXPECT_EQ(flight.getRoute(), _route);
    EXPECT_EQ(flight.getSchedule(), _schedule);
    EXPECT_EQ(flight.getAircraft(), _aircraft);
}

// Test flight creation with invalid parameters
TEST_F(FlightTest, CreateFlightWithInvalidParameters) {
    // Test with null aircraft
    auto result1 = Flight::create(_flightNumber, _route, _schedule, nullptr);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "INVALID_AIRCRAFT");
    EXPECT_EQ(result1.error().message, "Aircraft cannot be null");

    // Test with invalid flight number
    auto result2 = Flight::create("", _route.toString(), _schedule.toString(), _aircraft);
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "INVALID_FLIGHT_NUMBER");

    // Test with invalid route
    // auto invalidRoute = Route::create("");
    // ASSERT_FALSE(invalidRoute.has_value());
    auto result3 = Flight::create(_flightNumber.toString(), "", _schedule.toString(), _aircraft);
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, "INVALID_ROUTE");

    // Test with invalid schedule
    // auto invalidSchedule = Schedule::create("");
    // ASSERT_FALSE(invalidSchedule.has_value());
    auto result4 = Flight::create(_flightNumber.toString(), _route.toString(), "", _aircraft);
    ASSERT_FALSE(result4.has_value());
    EXPECT_EQ(result4.error().code, "INVALID_SCHEDULE");
}

// Test seat management
TEST_F(FlightTest, SeatManagement) {
    auto result = createFlight();
    ASSERT_TRUE(result.has_value());
    Flight& flight = *result;

    // Test initial seat availability
    EXPECT_TRUE(flight.isSeatAvailable("E001"));
    EXPECT_TRUE(flight.isSeatAvailable("B01"));
    EXPECT_TRUE(flight.isSeatAvailable("F01"));

    // Test reserving a seat
    EXPECT_TRUE(flight.reserveSeat("E001"));
    EXPECT_FALSE(flight.isSeatAvailable("E001"));

    // Test reserving an already reserved seat
    EXPECT_FALSE(flight.reserveSeat("E001"));

    // Test releasing a seat
    EXPECT_TRUE(flight.releaseSeat("E001"));
    EXPECT_TRUE(flight.isSeatAvailable("E001"));

    // Test releasing an already available seat
    EXPECT_FALSE(flight.releaseSeat("E001"));

    // Test invalid seat numbers
    EXPECT_FALSE(flight.isSeatAvailable("X001"));  // Invalid class
    EXPECT_FALSE(flight.isSeatAvailable("E101"));  // Invalid sequence
    EXPECT_FALSE(flight.reserveSeat("X001"));
    EXPECT_FALSE(flight.releaseSeat("X001"));
}

// Test value object operations
TEST_F(FlightTest, ValueObjectOperations) {
    auto result1 = createFlight();
    auto result2 = createFlight();
    ASSERT_TRUE(result1.has_value() && result2.has_value());
    const Flight& flight1 = *result1;
    const Flight& flight2 = *result2;

    // Test equality
    EXPECT_TRUE(flight1.equals(flight2));

    // Test clone
    auto clone = flight1.clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_TRUE(flight1.equals(*clone));

    // Test toString
    std::string expected = "Flight{flightNumber=" + _flightNumber.toString() + 
                          ", route=" + _route.toString() + 
                          ", schedule=" + _schedule.toString() + 
                          ", aircraft=" + _aircraft->toString() + "}";
    EXPECT_EQ(flight1.toString(), expected);
}

// Test seat availability map
TEST_F(FlightTest, SeatAvailabilityMap) {
    auto result = createFlight();
    ASSERT_TRUE(result.has_value());
    const Flight& flight = *result;

    // Get seat availability map
    const auto& seatAvailability = flight.getSeatAvailability();

    // Verify all seats are initially available
    for (const auto& [seatNumber, isAvailable] : seatAvailability) {
        EXPECT_TRUE(isAvailable) << "Seat " << seatNumber.toString() << " should be available initially";
    }

    // Verify seat counts match the layout
    EXPECT_EQ(seatAvailability.size(), 130);  // 100 economy + 20 business + 10 first class
}

// Test flight with different seat layouts
TEST_F(FlightTest, DifferentSeatLayouts) {
    // Create a different seat layout
    auto seatLayoutResult = SeatClassMap::create({
        {SeatClassRegistry::getByName("ECONOMY").value(), 50},     // E01-E50
        {SeatClassRegistry::getByName("BUSINESS").value(), 100}     // B001-B100
    });
    ASSERT_TRUE(seatLayoutResult.has_value());
    SeatClassMap differentLayout = seatLayoutResult.value();

    // Create aircraft with different layout
    auto serialResult2 = AircraftSerial::create("VN456");
    ASSERT_TRUE(serialResult2.has_value());
    auto aircraftResult = Aircraft::create(*serialResult2, "Airbus A320", differentLayout);
    ASSERT_TRUE(aircraftResult.has_value());
    auto differentAircraft = std::make_shared<Aircraft>(*aircraftResult);

    // Create flight with different aircraft
    auto result = Flight::create(_flightNumber, _route, _schedule, differentAircraft);
    ASSERT_TRUE(result.has_value());
    const Flight& flight = *result;

    // Verify seat availability
    EXPECT_TRUE(flight.isSeatAvailable("E01"));
    EXPECT_TRUE(flight.isSeatAvailable("B001"));
    EXPECT_FALSE(flight.isSeatAvailable("F01"));  // First class not available in this layout
    EXPECT_FALSE(flight.isSeatAvailable("E51")); // Economy seats beyond 50 not available
    EXPECT_FALSE(flight.isSeatAvailable("B111"));  // Business seats beyond 10 not available
} 