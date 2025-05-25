#include <gtest/gtest.h>
#include "repositories/MockRepository/TicketMockRepository.h"
#include "core/entities/Ticket.h"
#include "core/entities/Passenger.h"
#include "core/entities/Flight.h"
#include "core/value_objects/ticket_number/TicketNumber.h"
#include "core/value_objects/seat_number/SeatNumber.h"
#include "core/value_objects/seat_class_map/SeatClassMap.h"
#include "core/value_objects/price/Price.h"
#include "core/value_objects/flight_number/FlightNumber.h"
#include "core/value_objects/route/Route.h"
#include "core/value_objects/schedule/Schedule.h"
#include "core/entities/Aircraft.h"
#include "utils/Logger.h"
#include <memory>
#include <ctime>

class TicketMockRepositoryTest : public ::testing::Test
{
protected:
    std::shared_ptr<TicketMockRepository> repository;
    std::shared_ptr<Passenger> passenger;
    std::shared_ptr<Flight> flight;
    TicketNumber ticketNumber;
    SeatNumber seatNumber;
    Price price;

    void SetUp() override
    {
        repository = std::make_shared<TicketMockRepository>(Logger::getInstance());

        // Passenger
        auto passengerResult = Passenger::create("John Doe", "john.doe123@abc.com|0123456789|123 Main St", "VN:1234567");
        ASSERT_TRUE(passengerResult.has_value());
        passenger = std::make_shared<Passenger>(passengerResult.value());
        passenger->setId(1);

        // Aircraft
        auto aircraftResult = Aircraft::create("VN123", "A320", "E:100,B:20,F:10");
        ASSERT_TRUE(aircraftResult.has_value());
        auto aircraft = std::make_shared<Aircraft>(aircraftResult.value());
        aircraft->setId(1);

        // Flight
        auto flightNumberResult = FlightNumber::create("VN123");
        ASSERT_TRUE(flightNumberResult.has_value());
        auto routeResult = Route::create("Noi Bai International Airport", "HAN", "Tan Son Nhat International Airport", "SGN");
        ASSERT_TRUE(routeResult.has_value());
        std::time_t now = std::time(nullptr);
        auto departure = *std::localtime(&now);
        auto arrival = departure;
        arrival.tm_hour += 2;
        auto scheduleResult = Schedule::create(departure, arrival);
        ASSERT_TRUE(scheduleResult.has_value());
        auto flightResult = Flight::create(flightNumberResult.value(), routeResult.value(), scheduleResult.value(), aircraft);
        ASSERT_TRUE(flightResult.has_value());
        flight = std::make_shared<Flight>(flightResult.value());
        flight->setId(1);

        // TicketNumber, SeatClassMap, SeatNumber, Price
        auto ticketNumberResult = TicketNumber::create("AB12-20230525-0001");
        ASSERT_TRUE(ticketNumberResult.has_value());
        ticketNumber = ticketNumberResult.value();

        auto seatClassMapResult = SeatClassMap::create("E:100,B:20,F:10");
        ASSERT_TRUE(seatClassMapResult.has_value());
        auto seatNumberResult = SeatNumber::create("E001", seatClassMapResult.value());
        ASSERT_TRUE(seatNumberResult.has_value());
        seatNumber = seatNumberResult.value();

        auto priceResult = Price::create(100.0, "USD");
        ASSERT_TRUE(priceResult.has_value());
        price = priceResult.value();
    }

    void TearDown() override
    {
        repository->clear();
    }

    Result<Ticket> createTicket()
    {
        return Ticket::create(ticketNumber, passenger, flight, seatNumber, price);
    }
};

TEST_F(TicketMockRepositoryTest, CreateAndFindAndDeleteTicket)
{
    // Create ticket
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(ticketResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto createdTicket = createResult.value();

    // Find by id
    auto findResult = repository->findById(createdTicket.getId());
    ASSERT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult.value().getTicketNumber().toString(), ticketNumber.toString());
    EXPECT_EQ(findResult.value().getPassenger()->getId(), passenger->getId());
    EXPECT_EQ(findResult.value().getFlight()->getId(), flight->getId());

    // Count tickets
    auto countResult = repository->count();
    ASSERT_TRUE(countResult.has_value());
    EXPECT_EQ(countResult.value(), 1);

    // Delete ticket
    auto deleteResult = repository->deleteById(createdTicket.getId());
    ASSERT_TRUE(deleteResult.has_value());
    EXPECT_TRUE(deleteResult.value());

    // Count after delete
    countResult = repository->count();
    ASSERT_TRUE(countResult.has_value());
    EXPECT_EQ(countResult.value(), 0);
}
