#include <gtest/gtest.h>
#include "../../../core/entities/Ticket.h"
#include "../../../core/entities/Flight.h"
#include "../../../core/entities/Passenger.h"
#include "../../../core/value_objects/seat_number/SeatNumber.h"
#include "../../../core/value_objects/ticket_number/TicketNumber.h"
#include "../../../core/value_objects/price/Price.h"
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../../core/exceptions/Result.h"

class TicketTest : public ::testing::Test {
protected:
    // Test data
    std::shared_ptr<Flight> _flight;
    std::shared_ptr<Passenger> _passenger;
    SeatNumber _seatNumber;
    TicketNumber _ticketNumber;
    Price _price;
    SeatClassMap _seatLayout;

    void SetUp() override {
        // Create seat layout
        auto seatLayoutResult = SeatClassMap::create({
            {SeatClassRegistry::getByName("ECONOMY").value(), 100},    // E001-E100
            {SeatClassRegistry::getByName("BUSINESS").value(), 20},    // B01-B20
            {SeatClassRegistry::getByName("FIRST").value(), 10}        // F01-F10
        });
        ASSERT_TRUE(seatLayoutResult.has_value());
        _seatLayout = *seatLayoutResult;

        // Create test aircraft
        auto serialResult = AircraftSerial::create("VN123");
        ASSERT_TRUE(serialResult.has_value());
        auto aircraftResult = Aircraft::create(*serialResult, "Boeing 737", _seatLayout);
        ASSERT_TRUE(aircraftResult.has_value());
        auto aircraft = std::make_shared<Aircraft>(*aircraftResult);

        // Create test flight
        auto flightNumberResult = FlightNumber::create("VN123");
        ASSERT_TRUE(flightNumberResult.has_value());
        auto routeResult = Route::create("Ho Chi Minh City(SGN)-Ha Noi(HAN)");
        ASSERT_TRUE(routeResult.has_value());
        auto scheduleResult = Schedule::create("2024-03-20 10:00|2024-03-20 12:00");
        ASSERT_TRUE(scheduleResult.has_value());
        auto flightResult = Flight::create(*flightNumberResult, *routeResult, *scheduleResult, aircraft);
        ASSERT_TRUE(flightResult.has_value());
        _flight = std::make_shared<Flight>(*flightResult);

        // Create test passenger
        auto passengerResult = Passenger::create("John Doe", "john@example.com|+84123456789|123 Tp Ho Chi Minh", "VN:12345678");
        ASSERT_TRUE(passengerResult.has_value());
        _passenger = std::make_shared<Passenger>(*passengerResult);

        // Create test seat number
        auto seatNumberResult = SeatNumber::create("E001", _seatLayout);
        ASSERT_TRUE(seatNumberResult.has_value());
        _seatNumber = *seatNumberResult;

        // Create test ticket number
        auto ticketNumberResult = TicketNumber::create("MC1-20240320-0001");
        ASSERT_TRUE(ticketNumberResult.has_value());
        _ticketNumber = *ticketNumberResult;

        // Create test price
        auto priceResult = Price::create(1000000, "VND");
        ASSERT_TRUE(priceResult.has_value());
        _price = *priceResult;
    }

    // Helper method to create a ticket
    Result<Ticket> createTicket() {
        return Ticket::create(_flight, _passenger, _seatNumber, _ticketNumber, _price);
    }
};

// Test ticket creation
TEST_F(TicketTest, CreateTicket) {
    auto result = createTicket();
    ASSERT_TRUE(result.has_value());
    const Ticket& ticket = *result;

    // Test getters
    EXPECT_EQ(ticket.getFlight(), _flight);
    EXPECT_EQ(ticket.getPassenger(), _passenger);
    EXPECT_EQ(ticket.getSeatNumber().toString(), _seatNumber.toString());
    EXPECT_EQ(ticket.getTicketNumber().toString(), _ticketNumber.toString());
    EXPECT_EQ(ticket.getPrice().toString(), _price.toString());
    EXPECT_FALSE(ticket.isCheckedIn());
}

// Test ticket creation with invalid parameters
TEST_F(TicketTest, CreateTicketWithInvalidParameters) {
    // Test with null flight
    auto result1 = Ticket::create(nullptr, _passenger, _seatNumber, _ticketNumber, _price);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "INVALID_FLIGHT");
    EXPECT_EQ(result1.error().message, "Flight cannot be null");

    // Test with null passenger
    auto result2 = Ticket::create(_flight, nullptr, _seatNumber, _ticketNumber, _price);
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "INVALID_PASSENGER");
    EXPECT_EQ(result2.error().message, "Passenger cannot be null");
}

// Test check-in operations
TEST_F(TicketTest, CheckInOperations) {
    auto result = createTicket();
    ASSERT_TRUE(result.has_value());
    Ticket ticket = *result;

    // Test initial state
    EXPECT_FALSE(ticket.isCheckedIn());

    // Test check-in
    ticket.checkIn();
    EXPECT_TRUE(ticket.isCheckedIn());

    // Test double check-in
    EXPECT_THROW(ticket.checkIn(), CoreError);

    // Test cancel check-in
    ticket.cancelCheckIn();
    EXPECT_FALSE(ticket.isCheckedIn());

    // Test cancel check-in when not checked in
    EXPECT_THROW(ticket.cancelCheckIn(), CoreError);
}

// Test value object operations
TEST_F(TicketTest, ValueObjectOperations) {
    auto result1 = createTicket();
    auto result2 = createTicket();
    ASSERT_TRUE(result1.has_value() && result2.has_value());
    const Ticket& ticket1 = *result1;
    const Ticket& ticket2 = *result2;

    // Test equality
    EXPECT_TRUE(ticket1.equals(ticket2));

    // Test clone
    auto clone = ticket1.clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_TRUE(ticket1.equals(*clone));

    // Test toString
    std::string expected = "Ticket[" + _ticketNumber.toString() + "]";
    EXPECT_EQ(ticket1.toString(), expected);
}

// Test different seat numbers
TEST_F(TicketTest, DifferentSeatNumbers) {
    // Test with business class seat
    auto seatNumberResult = SeatNumber::create("B01", _seatLayout);
    ASSERT_TRUE(seatNumberResult.has_value());
    auto result1 = Ticket::create(_flight, _passenger, *seatNumberResult, _ticketNumber, _price);
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->getSeatNumber().toString(), "B01");

    // Test with first class seat
    auto seatNumberResult2 = SeatNumber::create("F01", _seatLayout);
    ASSERT_TRUE(seatNumberResult2.has_value());
    auto result2 = Ticket::create(_flight, _passenger, *seatNumberResult2, _ticketNumber, _price);
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->getSeatNumber().toString(), "F01");
}

// Test different prices
TEST_F(TicketTest, DifferentPrices) {
    // Test with different amount
    auto priceResult1 = Price::create(2000000, "VND");
    ASSERT_TRUE(priceResult1.has_value());
    auto result1 = Ticket::create(_flight, _passenger, _seatNumber, _ticketNumber, *priceResult1);
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->getPrice().toString(), "2000000.00 VND");

    // Test with different currency
    auto priceResult2 = Price::create(100, "USD");
    ASSERT_TRUE(priceResult2.has_value());
    auto result2 = Ticket::create(_flight, _passenger, _seatNumber, _ticketNumber, *priceResult2);
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->getPrice().toString(), "100.00 USD");
}

// Test different ticket numbers
TEST_F(TicketTest, DifferentTicketNumbers) {
    // Test with different ticket number
    auto ticketNumberResult = TicketNumber::create("MC1-20240320-0001");
    ASSERT_TRUE(ticketNumberResult.has_value());
    auto result = Ticket::create(_flight, _passenger, _seatNumber, *ticketNumberResult, _price);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTicketNumber().toString(), "MC1-20240320-0001");
}