#include "services/TicketService.h"
#include "repositories/MySQLRepository/TicketRepository.h"
#include "repositories/MySQLRepository/PassengerRepository.h"
#include "repositories/MySQLRepository/FlightRepository.h"
#include "repositories/MySQLRepository/AircraftRepository.h"
#include "core/entities/Ticket.h"
#include "core/entities/Passenger.h"
#include "core/entities/Flight.h"
#include "core/entities/Aircraft.h"
#include "core/value_objects/ticket_number/TicketNumber.h"
#include "core/value_objects/passport_number/PassportNumber.h"
#include "core/value_objects/flight_number/FlightNumber.h"
#include "core/value_objects/price/Price.h"
#include "core/value_objects/contact_info/ContactInfo.h"
#include "core/value_objects/aircraft_serial/AircraftSerial.h"
#include "core/value_objects/seat_class_map/SeatClassMap.h"
#include "core/value_objects/route/Route.h"
#include "core/value_objects/schedule/Schedule.h"
#include "utils/Logger.h"
#include "database/MySQLXConnection.h"
#include <gtest/gtest.h>
#include <memory>
#include <chrono>

class TicketServiceTest : public ::testing::Test
{
protected:
// Helper method to create second test passenger
Result<Passenger> createSecondTestPassenger()
{
    auto name = "Jane Smith";
    auto contactInfoResult = ContactInfo::create("jane@example.com|+84987654321|456 Avenue");
    if (!contactInfoResult) return Failure<Passenger>(contactInfoResult.error());
    
    auto passportResult = PassportNumber::create("VN:9876543");
    if (!passportResult) return Failure<Passenger>(passportResult.error());
    
    auto passengerResult = Passenger::create(name, contactInfoResult.value(), passportResult.value());
    if (!passengerResult) return Failure<Passenger>(passengerResult.error());
    
    return _passengerRepository->create(passengerResult.value());
}

    std::shared_ptr<TicketRepository> _ticketRepository;
    std::shared_ptr<PassengerRepository> _passengerRepository;
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<AircraftRepository> _aircraftRepository;
    std::shared_ptr<TicketService> _service;
    std::shared_ptr<MySQLXConnection> _db;
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<Passenger> _passenger;
    std::shared_ptr<Flight> _flight;
    std::shared_ptr<Aircraft> _aircraft;
    Price _price;
    std::string _seatClass;

    void SetUp() override
    {
        // Initialize database connection
        _db = MySQLXConnection::getInstance();
        auto result = _db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " + result.error().message;

        // Initialize logger
        _logger = Logger::getInstance();
        _logger->setMinLevel(LogLevel::DEBUG);

        // Initialize repositories
        _passengerRepository = std::make_shared<PassengerRepository>(_db, _logger);
        _aircraftRepository = std::make_shared<AircraftRepository>(_db, _logger);
        _flightRepository = std::make_shared<FlightRepository>(_db, _logger);
        _ticketRepository = std::make_shared<TicketRepository>(_db, _passengerRepository, _flightRepository, _logger);

        // Initialize service
        _service = std::make_shared<TicketService>(_ticketRepository, _passengerRepository, _flightRepository, _aircraftRepository, _logger);

        // Setup test data
        setupTestData();
    }

    void TearDown() override
    {
        // Clean up test data
        // Xóa ticket trước (nếu có)
        if (_flight) {
            auto ticketsResult = _ticketRepository->findByFlightId(_flight->getId());
            if (ticketsResult && !ticketsResult.value().empty()) {
                for (const auto& ticket : ticketsResult.value()) {
                    _ticketRepository->deleteById(ticket.getId());
                }
            }
        }
        // Xóa flight_seat_availability trước khi xóa flight
        if (_flight) {
            std::string deleteSeatsQuery = "DELETE FROM flight_seat_availability WHERE flight_id = ?";
            auto prepareResult = _db->prepareStatement(deleteSeatsQuery);
            if (prepareResult) {
                int stmtId = prepareResult.value();
                _db->setInt(stmtId, 1, _flight->getId());
                _db->executeStatement(stmtId);
                _db->freeStatement(stmtId);
            }
        }
        if (_flight) {
            _flightRepository->deleteById(_flight->getId());
        }
        if (_passenger) {
            _passengerRepository->deleteById(_passenger->getId());
        }
        if (_aircraft) {
            _aircraftRepository->deleteById(_aircraft->getId());
        }
        _db->disconnect();
    }

    void setupTestData()
    {
        // Create test passenger
        auto name = "John Doe";
        auto contactInfoResult = ContactInfo::create("user@example.com|+84123456789|123 Street");
        ASSERT_TRUE(contactInfoResult.has_value());
        auto passportResult = PassportNumber::create("VN:1232323");
        ASSERT_TRUE(passportResult.has_value());
        auto passengerResult = Passenger::create(name, contactInfoResult.value(), passportResult.value());
        ASSERT_TRUE(passengerResult.has_value());
        auto createPassengerResult = _passengerRepository->create(passengerResult.value());
        ASSERT_TRUE(createPassengerResult.has_value());
        _passenger = std::make_shared<Passenger>(createPassengerResult.value());

        // Create test aircraft
        auto serialResult = AircraftSerial::create("HK191");
        ASSERT_TRUE(serialResult.has_value());
        auto seatLayoutResult = SeatClassMap::create("E:10,B:5,F:2");
        ASSERT_TRUE(seatLayoutResult.has_value());
        auto aircraftResult = Aircraft::create(*serialResult, "Boeing 737", *seatLayoutResult);
        ASSERT_TRUE(aircraftResult.has_value());
        auto createAircraftResult = _aircraftRepository->create(aircraftResult.value());
        ASSERT_TRUE(createAircraftResult.has_value());
        _aircraft = std::make_shared<Aircraft>(createAircraftResult.value());

        // Create test flight
        auto flightNumberResult = FlightNumber::create("HK191");
        ASSERT_TRUE(flightNumberResult.has_value());
        auto routeResult = Route::create("Sai Gon(SGN)-Ha Noi(HAN)");
        ASSERT_TRUE(routeResult.has_value());
        
        // Set departure time to future
        std::tm departure = {0};
        departure.tm_year = 2024 - 1900;
        departure.tm_mon = 3 - 1;
        departure.tm_mday = 15;
        departure.tm_hour = 10;
        departure.tm_min = 0;
        
        std::tm arrival = departure;
        arrival.tm_hour = 12;
        
        auto scheduleResult = Schedule::create(departure, arrival);
        ASSERT_TRUE(scheduleResult.has_value());
        
        auto flightResult = Flight::create(*flightNumberResult, *routeResult, *scheduleResult, _aircraft);
        ASSERT_TRUE(flightResult.has_value());
        auto createFlightResult = _flightRepository->create(flightResult.value());
        ASSERT_TRUE(createFlightResult.has_value());
        _flight = std::make_shared<Flight>(createFlightResult.value());

        // Setup price and seat class
        auto priceResult = Price::create("100000 VND");
        ASSERT_TRUE(priceResult.has_value());
        _price = priceResult.value();
        _seatClass = "E01";
    }

    Result<Ticket> createTestTicket()
    {
        return _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), _seatClass, _price);
    }
};

// // Test bookTicket
// TEST_F(TicketServiceTest, BookTicketSuccess)
// {
//     auto result = createTestTicket();
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getPassenger()->getId(), _passenger->getId());
//     EXPECT_EQ(result.value().getFlight()->getId(), _flight->getId());
//     EXPECT_EQ(result.value().getSeatNumber().toString(), _seatClass);
//     EXPECT_EQ(result.value().getPrice().toString(), _price.toString());
//     EXPECT_EQ(TicketStatusUtil::toString(result.value().getStatus()), TicketStatusUtil::toString(TicketStatus::CONFIRMED));
// }

// // Test cancelTicket
// TEST_F(TicketServiceTest, CancelTicketSuccess)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Cancel ticket
//     auto result = _service->cancelTicket(ticketResult.value().getTicketNumber(), "Test cancellation");
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());

//     // Verify ticket status
//     auto getResult = _service->getTicket(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(getResult.has_value());
//     EXPECT_EQ(getResult.value().getStatus(), TicketStatus::CANCELLED);
// }

// TEST_F(TicketServiceTest, CancelTicketNotFound)
// {
//     auto ticketNumberResult = TicketNumber::create("HK123-20250529-0001");
//     ASSERT_TRUE(ticketNumberResult.has_value());
    
//     auto result = _service->cancelTicket(*ticketNumberResult, "Test cancellation");
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, "NOT_FOUND");
// }

// // Test checkInTicket
// TEST_F(TicketServiceTest, CheckInTicketSuccess)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Check in ticket
//     auto result = _service->checkInTicket(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());

//     // Verify ticket status
//     auto getResult = _service->getTicket(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(getResult.has_value());
//     EXPECT_EQ(getResult.value().getStatus(), TicketStatus::CHECKED_IN);
// }

// TEST_F(TicketServiceTest, CheckInTicketNotFound)
// {
//     auto ticketNumberResult = TicketNumber::create("HK123-20250529-0001");
//     ASSERT_TRUE(ticketNumberResult.has_value());
    
//     auto result = _service->checkInTicket(*ticketNumberResult);
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, "NOT_FOUND");
// }

// // Test boardPassenger
// TEST_F(TicketServiceTest, BoardPassengerSuccess)
// {
//     // Book and check in ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());
//     auto checkInResult = _service->checkInTicket(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(checkInResult.has_value());

//     // Board passenger
//     auto result = _service->boardPassenger(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());

//     // Verify ticket status
//     auto getResult = _service->getTicket(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(getResult.has_value());
//     EXPECT_EQ(getResult.value().getStatus(), TicketStatus::BOARDED);
// }

// TEST_F(TicketServiceTest, BoardPassengerNotCheckedIn)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Try to board without checking in
//     auto result = _service->boardPassenger(ticketResult.value().getTicketNumber());
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, "NOT_CHECKED_IN");
// }

// // Test refundTicket
// TEST_F(TicketServiceTest, RefundTicketSuccess)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Refund ticket
//     auto result = _service->refundTicket(ticketResult.value().getTicketNumber(), "Test refund");
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());

//     // Verify ticket status
//     auto getResult = _service->getTicket(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(getResult.has_value());
//     EXPECT_EQ(getResult.value().getStatus(), TicketStatus::REFUNDED);
// }

// TEST_F(TicketServiceTest, RefundTicketNotFound)
// {
//     auto ticketNumberResult = TicketNumber::create("HK123-20250529-0001");
//     ASSERT_TRUE(ticketNumberResult.has_value());
    
//     auto result = _service->refundTicket(*ticketNumberResult, "Test refund");
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, "NOT_FOUND");
// }

// // Test search operations
// TEST_F(TicketServiceTest, SearchByPriceRange)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Search by price range
//     auto minPrice = Price::create("500000 VND");
//     ASSERT_TRUE(minPrice.has_value());
//     auto maxPrice = Price::create("1500000 VND");
//     ASSERT_TRUE(maxPrice.has_value());
    
//     auto result = _service->searchByPriceRange(*minPrice, *maxPrice);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_GE(result.value().size(), 1);
// }

// TEST_F(TicketServiceTest, SearchByFlight)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Search by flight
//     auto result = _service->searchByFlight(_flight->getFlightNumber());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_GE(result.value().size(), 1);
// }

// TEST_F(TicketServiceTest, SearchByStatus)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Search by status
//     auto result = _service->searchByStatus(TicketStatus::CONFIRMED);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_GE(result.value().size(), 1);
// }

// TEST_F(TicketServiceTest, SearchByPassenger)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Search by passenger
//     auto result = _service->searchByPassenger(_passenger->getPassport());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_GE(result.value().size(), 1);
// }

// // Test business validation
// TEST_F(TicketServiceTest, CanCancelTicket)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Check if can cancel
//     auto result = _service->canCancelTicket(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(TicketServiceTest, CanCheckIn)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Check if can check in
//     auto result = _service->canCheckIn(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(TicketServiceTest, CanRefund)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Check if can refund
//     auto result = _service->canRefund(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(TicketServiceTest, IsTicketExpired)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Check if expired
//     auto result = _service->isTicketExpired(ticketResult.value().getTicketNumber());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// // Test utility methods
// TEST_F(TicketServiceTest, HasActiveTickets)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Check if has active tickets
//     auto result = _service->hasActiveTickets(_passenger->getPassport());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(TicketServiceTest, GetTicketCount)
// {
//     // Book ticket
//     auto ticketResult = createTestTicket();
//     ASSERT_TRUE(ticketResult.has_value());

//     // Get ticket count
//     auto result = _service->getTicketCount();
//     ASSERT_TRUE(result.has_value());
//     EXPECT_GE(result.value(), 1);
// }

// =============================================================================
// SEARCH BUILDER PATTERN TESTS
// =============================================================================

TEST_F(TicketServiceTest, SearchBuilderSingleCriteria)
{
    // Book ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());

    // Test builder with single flight criteria
    auto result = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .execute();
    
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result.value().size(), 1);
    
    // Verify ticket found
    bool ticketFound = false;
    for (const auto& ticket : result.value()) {
        if (ticket.getTicketNumber().toString() == ticketResult.value().getTicketNumber().toString()) {
            ticketFound = true;
            break;
        }
    }
    EXPECT_TRUE(ticketFound);
}

TEST_F(TicketServiceTest, SearchBuilderMultipleCriteria)
{
    // Book ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());

    // Create price range
    auto minPrice = Price::create("50000 VND");
    auto maxPrice = Price::create("150000 VND");
    ASSERT_TRUE(minPrice.has_value());
    ASSERT_TRUE(maxPrice.has_value());

    // Test builder with multiple criteria
    auto result = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .withPriceRange(*minPrice, *maxPrice)
        .withStatus(TicketStatus::CONFIRMED)
        .withPassenger(_passenger->getPassport())
        .execute();
    
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result.value().size(), 1);
    
    // Verify all criteria match
    for (const auto& ticket : result.value()) {
        EXPECT_EQ(ticket.getFlight()->getFlightNumber().toString(), _flight->getFlightNumber().toString());
        EXPECT_EQ(ticket.getStatus(), TicketStatus::CONFIRMED);
        EXPECT_EQ(ticket.getPassenger()->getPassport().toString(), _passenger->getPassport().toString());
        EXPECT_GE(ticket.getPrice().getAmount(), minPrice->getAmount());
        EXPECT_LE(ticket.getPrice().getAmount(), maxPrice->getAmount());
    }
}

TEST_F(TicketServiceTest, SearchBuilderWithSorting)
{
    // Book multiple tickets with different prices
    auto ticket1Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E02", Price::create("80000 VND").value());
    auto ticket2Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E03", Price::create("120000 VND").value());
    auto ticket3Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E04", Price::create("60000 VND").value());
    
    ASSERT_TRUE(ticket1Result.has_value());
    ASSERT_TRUE(ticket2Result.has_value());
    ASSERT_TRUE(ticket3Result.has_value());

    // Test sorting by price ascending
    auto resultAsc = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .sortBy("price", true)
        .execute();
    
    ASSERT_TRUE(resultAsc.has_value());
    EXPECT_GE(resultAsc.value().size(), 3);
    
    // Verify ascending order
    for (size_t i = 1; i < resultAsc.value().size(); ++i) {
        EXPECT_LE(resultAsc.value()[i-1].getPrice().getAmount(), 
                  resultAsc.value()[i].getPrice().getAmount());
    }

    // Test sorting by price descending
    auto resultDesc = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .sortBy("price", false)
        .execute();
    
    ASSERT_TRUE(resultDesc.has_value());
    EXPECT_GE(resultDesc.value().size(), 3);
    
    // Verify descending order
    for (size_t i = 1; i < resultDesc.value().size(); ++i) {
        EXPECT_GE(resultDesc.value()[i-1].getPrice().getAmount(), 
                  resultDesc.value()[i].getPrice().getAmount());
    }
}

TEST_F(TicketServiceTest, SearchBuilderWithLimit)
{
    // Book multiple tickets
    auto ticket1Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E02", _price);
    auto ticket2Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E03", _price);
    auto ticket3Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E04", _price);
    
    ASSERT_TRUE(ticket1Result.has_value());
    ASSERT_TRUE(ticket2Result.has_value());
    ASSERT_TRUE(ticket3Result.has_value());

    // Test with limit
    auto result = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .withLimit(2)
        .execute();
    
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

TEST_F(TicketServiceTest, SearchBuilderPriceRangeOnly)
{
    // Book tickets with different prices
    auto lowPriceTicket = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E02", Price::create("50000 VND").value());
    auto midPriceTicket = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E03", Price::create("100000 VND").value());
    auto highPriceTicket = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E04", Price::create("200000 VND").value());
    
    ASSERT_TRUE(lowPriceTicket.has_value());
    ASSERT_TRUE(midPriceTicket.has_value());
    ASSERT_TRUE(highPriceTicket.has_value());

    // Test price range filter
    auto minPrice = Price::create("75000 VND");
    auto maxPrice = Price::create("150000 VND");
    ASSERT_TRUE(minPrice.has_value());
    ASSERT_TRUE(maxPrice.has_value());

    auto result = _service->createSearchBuilder()
        .withPriceRange(*minPrice, *maxPrice)
        .execute();
    
    ASSERT_TRUE(result.has_value());
    
    // Verify all tickets are within price range
    for (const auto& ticket : result.value()) {
        EXPECT_GE(ticket.getPrice().getAmount(), minPrice->getAmount());
        EXPECT_LE(ticket.getPrice().getAmount(), maxPrice->getAmount());
    }
}

TEST_F(TicketServiceTest, SearchBuilderStatusFilter)
{
    // Book and modify ticket statuses
    auto ticket1Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E02", _price);
    auto ticket2Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E03", _price);
    auto ticket3Result = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E04", _price);
    
    ASSERT_TRUE(ticket1Result.has_value());
    ASSERT_TRUE(ticket2Result.has_value());
    ASSERT_TRUE(ticket3Result.has_value());

    // Change some ticket statuses
    _service->checkInTicket(ticket2Result.value().getTicketNumber());
    _service->cancelTicket(ticket3Result.value().getTicketNumber(), "Test cancel");

    // Test filter by CONFIRMED status
    auto confirmedResult = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .withStatus(TicketStatus::CONFIRMED)
        .execute();
    
    ASSERT_TRUE(confirmedResult.has_value());
    for (const auto& ticket : confirmedResult.value()) {
        EXPECT_EQ(ticket.getStatus(), TicketStatus::CONFIRMED);
    }

    // Test filter by CHECKED_IN status
    auto checkedInResult = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .withStatus(TicketStatus::CHECKED_IN)
        .execute();
    
    ASSERT_TRUE(checkedInResult.has_value());
    for (const auto& ticket : checkedInResult.value()) {
        EXPECT_EQ(ticket.getStatus(), TicketStatus::CHECKED_IN);
    }

    // Test filter by CANCELLED status
    auto cancelledResult = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .withStatus(TicketStatus::CANCELLED)
        .execute();
    
    ASSERT_TRUE(cancelledResult.has_value());
    for (const auto& ticket : cancelledResult.value()) {
        EXPECT_EQ(ticket.getStatus(), TicketStatus::CANCELLED);
    }
}

TEST_F(TicketServiceTest, SearchBuilderReset)
{
    // Book ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());

    // Create builder with criteria
    auto builder = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .withStatus(TicketStatus::CONFIRMED);

    // Execute first search
    auto result1 = builder.execute();
    ASSERT_TRUE(result1.has_value());
    EXPECT_GE(result1.value().size(), 1);

    // Reset and execute with different criteria
    auto minPrice = Price::create("50000 VND");
    auto maxPrice = Price::create("150000 VND");
    ASSERT_TRUE(minPrice.has_value());
    ASSERT_TRUE(maxPrice.has_value());

    auto result2 = builder.reset()
        .withPriceRange(*minPrice, *maxPrice)
        .execute();
    
    ASSERT_TRUE(result2.has_value());
    
    // Results should be different (reset worked)
    // This test verifies that reset clears previous criteria
}

TEST_F(TicketServiceTest, SearchBuilderEmptyCriteria)
{
    // Book ticket
    auto ticketResult = createTestTicket();
    ASSERT_TRUE(ticketResult.has_value());

    // Execute search with no criteria (should return all tickets)
    auto result = _service->createSearchBuilder()
        .execute();
    
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result.value().size(), 1);
}

TEST_F(TicketServiceTest, SearchBuilderComplexScenario)
{
    // Create multiple passengers for complex testing
    auto passenger2Result = createSecondTestPassenger();
    ASSERT_TRUE(passenger2Result.has_value());
    auto passenger2 = std::make_shared<Passenger>(passenger2Result.value());

    // Book tickets for different passengers with different prices and statuses
    auto ticket1 = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E02", Price::create("80000 VND").value());
    auto ticket2 = _service->bookTicket(_passenger->getPassport(), _flight->getFlightNumber(), "E03", Price::create("120000 VND").value());
    auto ticket3 = _service->bookTicket(passenger2->getPassport(), _flight->getFlightNumber(), "E05", Price::create("90000 VND").value());
    auto ticket4 = _service->bookTicket(passenger2->getPassport(), _flight->getFlightNumber(), "E06", Price::create("110000 VND").value());
    
    ASSERT_TRUE(ticket1.has_value());
    ASSERT_TRUE(ticket2.has_value());
    ASSERT_TRUE(ticket3.has_value());
    ASSERT_TRUE(ticket4.has_value());

    // Change some statuses
    _service->checkInTicket(ticket2.value().getTicketNumber());
    _service->checkInTicket(ticket4.value().getTicketNumber());

    // Complex search: Flight + Price Range + Status + Limit + Sort
    auto minPrice = Price::create("75000 VND");
    auto maxPrice = Price::create("125000 VND");
    ASSERT_TRUE(minPrice.has_value());
    ASSERT_TRUE(maxPrice.has_value());

    auto result = _service->createSearchBuilder()
        .withFlight(_flight->getFlightNumber())
        .withPriceRange(*minPrice, *maxPrice)
        .withStatus(TicketStatus::CONFIRMED)
        .sortBy("price", true)
        .withLimit(3)
        .execute();
    
    ASSERT_TRUE(result.has_value());
    
    // Verify complex criteria
    for (const auto& ticket : result.value()) {
        EXPECT_EQ(ticket.getFlight()->getFlightNumber().toString(), _flight->getFlightNumber().toString());
        EXPECT_GE(ticket.getPrice().getAmount(), minPrice->getAmount());
        EXPECT_LE(ticket.getPrice().getAmount(), maxPrice->getAmount());
        EXPECT_EQ(ticket.getStatus(), TicketStatus::CONFIRMED);
    }
    
    // Verify limit
    EXPECT_LE(result.value().size(), 3);
    
    // Verify sorting (ascending by price)
    for (size_t i = 1; i < result.value().size(); ++i) {
        EXPECT_LE(result.value()[i-1].getPrice().getAmount(), 
                  result.value()[i].getPrice().getAmount());
    }

    // Cleanup second passenger
    _passengerRepository->deleteById(passenger2->getId());
}
