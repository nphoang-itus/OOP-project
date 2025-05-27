#include "services/TicketService.h"
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
#include <gtest/gtest.h>
#include <memory>
#include <ctime>

class TicketServiceTest : public ::testing::Test
{
protected:
    std::shared_ptr<TicketMockRepository> mockRepository;
    std::shared_ptr<TicketService> service;
    std::shared_ptr<Logger> logger;
    std::shared_ptr<Passenger> passenger;
    std::shared_ptr<Flight> flight;
    TicketNumber ticketNumber;
    SeatNumber seatNumber;
    Price price;

    void SetUp() override
    {
        mockRepository = std::make_shared<TicketMockRepository>(Logger::getInstance());
        logger = Logger::getInstance();
        service = std::make_shared<TicketService>(mockRepository, logger);

        // Setup Passenger
        auto passengerResult = Passenger::create("John Doe", "john.doe123@abc.com|0123456789|123 Main St", "VN:1234567");
        ASSERT_TRUE(passengerResult.has_value());
        passenger = std::make_shared<Passenger>(passengerResult.value());
        passenger->setId(1);

        // Setup Aircraft
        auto aircraftResult = Aircraft::create("VN123", "A320", "E:100,B:20,F:10");
        ASSERT_TRUE(aircraftResult.has_value());
        auto aircraft = std::make_shared<Aircraft>(aircraftResult.value());
        aircraft->setId(1);

        // Setup Flight
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

        // Setup TicketNumber, SeatNumber, Price
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
        mockRepository->clear();
    }

    Result<Ticket> createTicket()
    {
        return Ticket::create(ticketNumber, passenger, flight, seatNumber, price);
    }
};

// Test getById
TEST_F(TicketServiceTest, GetByIdSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = service->create(ticketResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->getById(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getTicketNumber(), ticketNumber);
}

TEST_F(TicketServiceTest, GetByIdFailure)
{
    auto result = service->getById(999);
    ASSERT_FALSE(result.has_value());
}

// Test getAll
TEST_F(TicketServiceTest, GetAllSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    service->create(ticketResult.value());
    service->create(ticketResult.value());
    auto result = service->getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

// Test create
TEST_F(TicketServiceTest, CreateSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto result = service->create(ticketResult.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getTicketNumber(), ticketNumber);
    EXPECT_EQ(result.value().getPassenger()->getId(), passenger->getId());
    EXPECT_EQ(result.value().getFlight()->getId(), flight->getId());
}

// Test update
TEST_F(TicketServiceTest, UpdateSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = service->create(ticketResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto ticket = createResult.value();

    // Create new ticket with updated price
    Price newPrice = Price::create(150.0, "USD").value();
    auto newTicketResult = Ticket::create(
        ticket.getTicketNumber(),
        ticket.getPassenger(),
        ticket.getFlight(),
        ticket.getSeatNumber(),
        newPrice);
    ASSERT_TRUE(newTicketResult.has_value());
    auto newTicket = newTicketResult.value();
    newTicket.setId(ticket.getId());
    newTicket.setStatus(ticket.getStatus());

    auto result = service->update(newTicket);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getPrice(), newPrice);
}

// Test deleteById
TEST_F(TicketServiceTest, DeleteByIdSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = service->create(ticketResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->deleteById(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
    auto findResult = service->getById(1);
    ASSERT_FALSE(findResult.has_value());
}

// Test exists
TEST_F(TicketServiceTest, ExistsSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = service->create(ticketResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->exists(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(TicketServiceTest, ExistsFailure)
{
    auto result = service->exists(999);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test count
TEST_F(TicketServiceTest, CountSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    service->create(ticketResult.value());
    service->create(ticketResult.value());
    auto result = service->count();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 2);
}

// Test findByTicketNumber
TEST_F(TicketServiceTest, FindByTicketNumberSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    service->create(ticketResult.value());
    auto result = service->findByTicketNumber(ticketNumber);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getTicketNumber(), ticketNumber);
}

TEST_F(TicketServiceTest, FindByTicketNumberFailure)
{
    auto nonExistentTicketNumber = TicketNumber::create("ZZ99-99999999-9999");
    ASSERT_TRUE(nonExistentTicketNumber.has_value());
    auto result = service->findByTicketNumber(nonExistentTicketNumber.value());
    ASSERT_FALSE(result.has_value());
}

// Test existsTicket
TEST_F(TicketServiceTest, ExistsTicketSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    service->create(ticketResult.value());
    auto result = service->existsTicket(ticketNumber);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(TicketServiceTest, ExistsTicketFailure)
{
    auto nonExistentTicketNumber = TicketNumber::create("ZZ99-99999999-9999");
    ASSERT_TRUE(nonExistentTicketNumber.has_value());
    auto result = service->existsTicket(nonExistentTicketNumber.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test findByPassengerId
TEST_F(TicketServiceTest, FindByPassengerIdSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    service->create(ticketResult.value());
    auto result = service->findByPassengerId(passenger->getId());
    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(result.value().empty());
    for (const auto &t : result.value())
    {
        EXPECT_EQ(t.getPassenger()->getId(), passenger->getId());
    }
}

// Test findBySerialNumber
TEST_F(TicketServiceTest, FindBySerialNumberSuccess)
{
    auto ticketResult = createTicket();
    ASSERT_TRUE(ticketResult.has_value());
    service->create(ticketResult.value());
    auto result = service->findBySerialNumber(flight->getAircraft()->getSerial());
    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(result.value().empty());
    for (const auto &t : result.value())
    {
        EXPECT_EQ(t.getFlight()->getAircraft()->getSerial(), flight->getAircraft()->getSerial());
    }
}
