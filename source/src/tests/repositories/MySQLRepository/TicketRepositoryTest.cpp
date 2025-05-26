#include <gtest/gtest.h>
#include "../../../repositories/MySQLRepository/TicketRepository.h"
#include "../../../repositories/MySQLRepository/AircraftRepository.h"
#include "../../../repositories/MySQLRepository/FlightRepository.h"
#include "../../../repositories/MySQLRepository/PassengerRepository.h"
#include "../../../core/value_objects/ticket_number/TicketNumber.h"
#include "../../../core/value_objects/seat_number/SeatNumber.h"
#include "../../../core/value_objects/price/Price.h"
#include "../../../core/value_objects/ticket_status/TicketStatus.h"
#include "../../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "../../../core/value_objects/passport_number/PassportNumber.h"
#include "../../../core/value_objects/contact_info/ContactInfo.h"
#include "../../../core/value_objects/flight_number/FlightNumber.h"
#include "../../../core/value_objects/route/Route.h"
#include "../../../core/value_objects/schedule/Schedule.h"
#include "../../../core/exceptions/Result.h"
#include "../../../database/MySQLXConnection.h"
#include <memory>

class TicketRepositoryTest : public ::testing::Test {
protected:
    std::shared_ptr<TicketRepository> repository;
    std::shared_ptr<AircraftRepository> aircraftRepository;
    std::shared_ptr<FlightRepository> flightRepository;
    std::shared_ptr<PassengerRepository> passengerRepository;
    std::shared_ptr<MySQLXConnection> db;
    std::shared_ptr<Logger> logger;

    // Test data
    TicketNumber _ticketNumber;
    std::shared_ptr<Passenger> _passenger;
    std::shared_ptr<Flight> _flight;
    SeatNumber _seatNumber;
    Price _price;

    void SetUp() override {
        // Setup database connection
        db = MySQLXConnection::getInstance();
        auto result = db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " << result.error().message;

        // Setup logger
        logger = Logger::getInstance();
        logger->setMinLevel(LogLevel::DEBUG);

        // Setup repositories
        aircraftRepository = std::make_shared<AircraftRepository>(db, logger);
        flightRepository = std::make_shared<FlightRepository>(db, logger);
        passengerRepository = std::make_shared<PassengerRepository>(db, logger);
        repository = std::make_shared<TicketRepository>(db, aircraftRepository, flightRepository, passengerRepository, logger);

        // Setup test ticket data
        auto ticketNumberResult = TicketNumber::create("MC1-20240320-0001");
        ASSERT_TRUE(ticketNumberResult.has_value());
        _ticketNumber = *ticketNumberResult;

        auto passengerResult = passengerRepository->findById(1).value();
        _passenger = std::make_shared<Passenger>(passengerResult);

        auto flightResult = flightRepository->findById(1).value();
        _flight = std::make_shared<Flight>(flightResult);

        // // Create test passenger
        // auto passportResult = PassportNumber::create("VN:12121234");
        // ASSERT_TRUE(passportResult.has_value());

        // std::string contactInfoStr = "john.doe@example.com|1234567890|123 Main St";
        // auto contactInfoResult = ContactInfo::create(contactInfoStr);
        // ASSERT_TRUE(contactInfoResult.has_value());

        // auto passengerResult = Passenger::create("John Doe", *contactInfoResult, *passportResult);
        // ASSERT_TRUE(passengerResult.has_value());
        // auto createPassengerResult = passengerRepository->create(*passengerResult);
        // ASSERT_TRUE(createPassengerResult.has_value());
        // _passenger = std::make_shared<Passenger>(*createPassengerResult);

        // // Create test aircraft
        // auto serialResult = AircraftSerial::create("VN226");
        // ASSERT_TRUE(serialResult.has_value());

        // std::string seatLayoutStr = "E:100,B:20,F:10";
        // auto seatLayoutResult = SeatClassMap::create(seatLayoutStr);
        // ASSERT_TRUE(seatLayoutResult.has_value());

        // auto aircraftResult = Aircraft::create(*serialResult, "Boeing 737", *seatLayoutResult);
        // ASSERT_TRUE(aircraftResult.has_value());
        // auto createAircraftResult = aircraftRepository->create(*aircraftResult);
        // ASSERT_TRUE(createAircraftResult.has_value());
        // auto aircraft = std::make_shared<Aircraft>(*createAircraftResult);

        // // Create test flight
        // auto flightNumberResult = FlightNumber::create("VN198");
        // ASSERT_TRUE(flightNumberResult.has_value());

        // auto routeResult = Route::create("Ho Chi Minh City(SGN)-Ha Noi(HAN)");
        // ASSERT_TRUE(routeResult.has_value());

        // auto scheduleResult = Schedule::create("2025-05-22 07:30|2025-05-22 09:30");
        // ASSERT_TRUE(scheduleResult.has_value());

        // auto flightResult = Flight::create(*flightNumberResult, *routeResult, *scheduleResult, aircraft);
        // ASSERT_TRUE(flightResult.has_value());
        // auto createFlightResult = flightRepository->create(*flightResult);
        // ASSERT_TRUE(createFlightResult.has_value());
        // _flight = std::make_shared<Flight>(*createFlightResult);

        // Create seat number
        auto seatNumberResult = SeatNumber::create("E001", _flight->getAircraft()->getSeatLayout());
        ASSERT_TRUE(seatNumberResult.has_value());
        _seatNumber = *seatNumberResult;

        // Create price
        auto priceResult = Price::create(1000000, "VND");
        ASSERT_TRUE(priceResult.has_value());
        _price = *priceResult;
    }

    void TearDown() override {
        // Clean up test data
        auto result = db->execute("DELETE FROM ticket WHERE ticket_number = 'MC1-20240320-0001'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        db->disconnect();
    }

    // Helper method to create a test ticket
    Result<Ticket> createTestTicket() {
        return Ticket::create(_ticketNumber, _passenger, _flight, _seatNumber, _price);
    }
};

// Test create operation
TEST_F(TicketRepositoryTest, CreateTicket) {
    // Create test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    const Ticket& ticket = *ticketResult;

    // Save to database
    auto result = repository->create(ticket);
    ASSERT_TRUE(result.has_value());
    const Ticket& savedTicket = *result;

    // Verify saved ticket
    EXPECT_GT(savedTicket.getId(), 0);
    EXPECT_EQ(savedTicket.getTicketNumber().toString(), _ticketNumber.toString());
    EXPECT_EQ(savedTicket.getPassenger()->getId(), _passenger->getId());
    EXPECT_EQ(savedTicket.getFlight()->getId(), _flight->getId());
    EXPECT_EQ(savedTicket.getSeatNumber().toString(), _seatNumber.toString());
    EXPECT_EQ(savedTicket.getPrice().toString(), _price.toString());
}

// Test findById operation
TEST_F(TicketRepositoryTest, FindTicketById) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Find ticket by id
    auto result = repository->findById(id);
    ASSERT_TRUE(result.has_value());
    const Ticket& foundTicket = *result;

    // Verify found ticket
    EXPECT_EQ(foundTicket.getId(), id);
    EXPECT_EQ(foundTicket.getTicketNumber().toString(), _ticketNumber.toString());
    EXPECT_EQ(foundTicket.getPassenger()->getId(), _passenger->getId());
    EXPECT_EQ(foundTicket.getFlight()->getId(), _flight->getId());
    EXPECT_EQ(foundTicket.getSeatNumber().toString(), _seatNumber.toString());
    EXPECT_EQ(foundTicket.getPrice().toString(), _price.toString());
}

// Test findAll operation
TEST_F(TicketRepositoryTest, FindAllTickets) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());

    // Find all tickets
    auto result = repository->findAll();
    ASSERT_TRUE(result.has_value());
    const auto& tickets = *result;

    // Verify found tickets
    EXPECT_FALSE(tickets.empty());
    bool found = false;
    for (const auto& ticket : tickets) {
        if (ticket.getTicketNumber().toString() == _ticketNumber.toString()) {
            found = true;
            EXPECT_EQ(ticket.getPassenger()->getId(), _passenger->getId());
            EXPECT_EQ(ticket.getFlight()->getId(), _flight->getId());
            EXPECT_EQ(ticket.getSeatNumber().toString(), _seatNumber.toString());
            EXPECT_EQ(ticket.getPrice().toString(), _price.toString());
            break;
        }
    }
    EXPECT_TRUE(found) << "Test ticket not found in findAll results";
}

// Test exists operation
TEST_F(TicketRepositoryTest, ExistsTicket) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
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
TEST_F(TicketRepositoryTest, CountTickets) {
    // Get initial count
    auto initialCountResult = repository->count();
    ASSERT_TRUE(initialCountResult.has_value());
    size_t initialCount = *initialCountResult;

    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());

    // Get new count
    auto newCountResult = repository->count();
    ASSERT_TRUE(newCountResult.has_value());
    size_t newCount = *newCountResult;

    // Verify count increased by 1
    EXPECT_EQ(newCount, initialCount + 1);
}

// Test update operation
TEST_F(TicketRepositoryTest, UpdateTicket) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());
    Ticket ticket = *createResult;

    // Update ticket
    auto newPriceResult = Price::create(2000000, "VND");
    ASSERT_TRUE(newPriceResult.has_value());
    ticket.setPrice(*newPriceResult);
    ticket.setStatus(TicketStatus::CONFIRMED);

    auto result = repository->update(ticket);
    ASSERT_TRUE(result.has_value());
    const Ticket& updatedTicket = *result;

    // Verify updated ticket
    EXPECT_EQ(updatedTicket.getId(), ticket.getId());
    EXPECT_EQ(updatedTicket.getTicketNumber().toString(), _ticketNumber.toString());
    EXPECT_EQ(updatedTicket.getPassenger()->getId(), _passenger->getId());
    EXPECT_EQ(updatedTicket.getFlight()->getId(), _flight->getId());
    EXPECT_EQ(updatedTicket.getSeatNumber().toString(), _seatNumber.toString());
    EXPECT_EQ(updatedTicket.getPrice().toString(), newPriceResult->toString());
    EXPECT_EQ(updatedTicket.getStatus(), TicketStatus::CONFIRMED);
}

// Test deleteById operation
TEST_F(TicketRepositoryTest, DeleteTicketById) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Delete ticket
    auto result = repository->deleteById(id);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);

    // Verify ticket is deleted
    auto existsResult = repository->exists(id);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(*existsResult);
}

// Test findByTicketNumber operation
TEST_F(TicketRepositoryTest, FindTicketByTicketNumber) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());

    // Find ticket by ticket number
    auto result = repository->findByTicketNumber(_ticketNumber);
    ASSERT_TRUE(result.has_value());
    const Ticket& foundTicket = *result;

    // Verify found ticket
    EXPECT_EQ(foundTicket.getTicketNumber().toString(), _ticketNumber.toString());
    EXPECT_EQ(foundTicket.getPassenger()->getId(), _passenger->getId());
    EXPECT_EQ(foundTicket.getFlight()->getId(), _flight->getId());
    EXPECT_EQ(foundTicket.getSeatNumber().toString(), _seatNumber.toString());
    EXPECT_EQ(foundTicket.getPrice().toString(), _price.toString());
}

// Test existsTicket operation
TEST_F(TicketRepositoryTest, ExistsTicketNumber) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());

    // Check ticket number existence
    auto result = repository->existsTicket(_ticketNumber);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);

    // Check non-existent ticket number
    auto nonExistentTicketNumberResult = TicketNumber::create("MC2-20240320-0001");
    ASSERT_TRUE(nonExistentTicketNumberResult.has_value());
    auto nonExistentResult = repository->existsTicket(*nonExistentTicketNumberResult);
    ASSERT_TRUE(nonExistentResult.has_value());
    EXPECT_FALSE(*nonExistentResult);
}

// Test findByPassengerId operation
TEST_F(TicketRepositoryTest, FindTicketsByPassengerId) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());

    // Find tickets by passenger id
    auto result = repository->findByPassengerId(_passenger->getId());
    ASSERT_TRUE(result.has_value());
    const auto& tickets = *result;

    // Verify found tickets
    EXPECT_FALSE(tickets.empty());
    bool found = false;
    for (const auto& ticket : tickets) {
        if (ticket.getTicketNumber().toString() == _ticketNumber.toString()) {
            found = true;
            EXPECT_EQ(ticket.getPassenger()->getId(), _passenger->getId());
            EXPECT_EQ(ticket.getFlight()->getId(), _flight->getId());
            EXPECT_EQ(ticket.getSeatNumber().toString(), _seatNumber.toString());
            EXPECT_EQ(ticket.getPrice().toString(), _price.toString());
            break;
        }
    }
    EXPECT_TRUE(found) << "Test ticket not found in findByPassengerId results";
}

// Test findBySerialNumber operation
TEST_F(TicketRepositoryTest, FindTicketsBySerialNumber) {
    // Create and save test ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    auto createResult = repository->create(*ticketResult);
    ASSERT_TRUE(createResult.has_value());

    // Find tickets by aircraft serial number
    auto aircraftResult = aircraftRepository->findById(1).value();
    auto serialResult = aircraftResult.getSerial();
    auto result = repository->findBySerialNumber(serialResult);
    ASSERT_TRUE(result.has_value());
    const auto& tickets = *result;

    // Verify found tickets
    EXPECT_FALSE(tickets.empty());
    bool found = false;
    for (const auto& ticket : tickets) {
        if (ticket.getTicketNumber().toString() == _ticketNumber.toString()) {
            found = true;
            EXPECT_EQ(ticket.getPassenger()->getId(), _passenger->getId());
            EXPECT_EQ(ticket.getFlight()->getId(), _flight->getId());
            EXPECT_EQ(ticket.getSeatNumber().toString(), _seatNumber.toString());
            EXPECT_EQ(ticket.getPrice().toString(), _price.toString());
            break;
        }
    }
    EXPECT_TRUE(found) << "Test ticket not found in findBySerialNumber results";
}

// Test error handling
TEST_F(TicketRepositoryTest, ErrorHandling) {
    // Test findById with invalid id
    auto result1 = repository->findById(-1);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "NOT_FOUND");

    // Test update with non-existent id
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());
    Ticket ticket = *ticketResult;
    ticket.setId(999999);
    auto result2 = repository->update(ticket);
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "DB_ERROR");

    // Test deleteById with non-existent id
    auto result3 = repository->deleteById(999999);
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, "DB_ERROR");

    // Test findByTicketNumber with non-existent ticket number
    auto nonExistentTicketNumberResult = TicketNumber::create("MC3-20240320-0001");
    ASSERT_TRUE(nonExistentTicketNumberResult.has_value());
    auto result4 = repository->findByTicketNumber(*nonExistentTicketNumberResult);
    ASSERT_FALSE(result4.has_value());
    EXPECT_EQ(result4.error().code, "NOT_FOUND");
}
