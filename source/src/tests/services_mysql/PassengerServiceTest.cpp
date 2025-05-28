#include "services/PassengerService.h"
#include "repositories/MySQLRepository/PassengerRepository.h"
#include "repositories/MySQLRepository/TicketRepository.h"
#include "repositories/MySQLRepository/FlightRepository.h"
#include "core/entities/Passenger.h"
#include "core/value_objects/passport_number/PassportNumber.h"
#include "core/value_objects/contact_info/ContactInfo.h"
#include "utils/Logger.h"
#include "database/MySQLXConnection.h"
#include <gtest/gtest.h>
#include <memory>

class PassengerServiceTest : public ::testing::Test
{
protected:
    std::shared_ptr<PassengerRepository> repository;
    std::shared_ptr<TicketRepository> ticketRepository;
    std::shared_ptr<FlightRepository> flightRepository;
    std::shared_ptr<PassengerService> service;
    std::shared_ptr<MySQLXConnection> db;
    std::shared_ptr<Logger> logger;
    std::string _name;
    ContactInfo _contactInfo;
    PassportNumber _passport;

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
        repository = std::make_shared<PassengerRepository>(db, logger);
        ticketRepository = std::make_shared<TicketRepository>(db, repository, flightRepository, logger);
        flightRepository = std::make_shared<FlightRepository>(db, logger);
        service = std::make_shared<PassengerService>(repository, ticketRepository, flightRepository, logger);

        // Setup test data
        _name = "John Doe";
        auto contactInfoResult = ContactInfo::create("user@example.com|+84123456789|123 Street");
        ASSERT_TRUE(contactInfoResult.has_value());
        _contactInfo = contactInfoResult.value();
        auto passportResult = PassportNumber::create("VN:123456");
        ASSERT_TRUE(passportResult.has_value());
        _passport = passportResult.value();
    }

    void TearDown() override
    {
        // Clean up test data
        auto result = db->execute("DELETE FROM passenger WHERE passport_number = 'VN:123456'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        db->disconnect();
    }

    // Helper method to create a test passenger
    Result<Passenger> createTestPassenger()
    {
        return Passenger::create(_name, _contactInfo, _passport);
    }
};

// // Test createPassenger
// TEST_F(PassengerServiceTest, CreatePassengerSuccess)
// {
//     auto passengerResult = createTestPassenger();
//     ASSERT_TRUE(passengerResult.has_value());
//     auto result = service->createPassenger(passengerResult.value());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getName(), _name);
//     EXPECT_EQ(result.value().getContactInfo().toString(), _contactInfo.toString());
//     EXPECT_EQ(result.value().getPassport().toString(), _passport.toString());
// }

// // Test getAllPassengers
// TEST_F(PassengerServiceTest, GetAllPassengersSuccess)
// {
//     auto passengerResult = createTestPassenger();
//     ASSERT_TRUE(passengerResult.has_value());
//     service->createPassenger(passengerResult.value());
//     // Create another passenger
//     std::string name2 = "Jane Doe";
//     auto contactInfoResult = ContactInfo::create("jane@example.com|+84999888777|456 Street");
//     ASSERT_TRUE(contactInfoResult.has_value());
//     auto passportResult = PassportNumber::create("VN:654321");
//     ASSERT_TRUE(passportResult.has_value());
//     auto passengerResult2 = Passenger::create(name2, contactInfoResult.value(), passportResult.value());
//     ASSERT_TRUE(passengerResult2.has_value());
//     service->createPassenger(passengerResult2.value());
//     auto result = service->getAllPassengers();
//     ASSERT_TRUE(result.has_value());
//     EXPECT_GE(result.value().size(), 2);
// }

// // Test updatePassenger
// TEST_F(PassengerServiceTest, UpdatePassengerSuccess)
// {
//     auto passengerResult = createTestPassenger();
//     ASSERT_TRUE(passengerResult.has_value());
//     auto createResult = service->createPassenger(passengerResult.value());
//     ASSERT_TRUE(createResult.has_value());
//     // Update data
//     std::string name2 = "Jane Doe";
//     auto contactInfoResult = ContactInfo::create("jane@example.com|+84999888777|456 Street");
//     ASSERT_TRUE(contactInfoResult.has_value());
//     auto updatedPassengerResult = Passenger::create(name2, contactInfoResult.value(), _passport);
//     ASSERT_TRUE(updatedPassengerResult.has_value());
//     auto updatedPassenger = updatedPassengerResult.value();
//     updatedPassenger.setId(createResult->getId());
//     auto result = service->updatePassenger(updatedPassenger);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getName(), name2);
//     EXPECT_EQ(result.value().getContactInfo().toString(), contactInfoResult.value().toString());
// }

// // Test deletePassenger
// TEST_F(PassengerServiceTest, DeletePassengerSuccess)
// {
//     auto passengerResult = createTestPassenger();
//     ASSERT_TRUE(passengerResult.has_value());
//     auto createResult = service->createPassenger(passengerResult.value());
//     ASSERT_TRUE(createResult.has_value());
//     auto result = service->deletePassenger(createResult.value().getPassport());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
//     // Verify not found
//     auto findResult = service->getPassenger(createResult.value().getPassport());
//     ASSERT_FALSE(findResult.has_value());
// }

// // Test getPassengerByPassport
// TEST_F(PassengerServiceTest, GetPassengerByPassportSuccess)
// {
//     auto passengerResult = createTestPassenger();
//     ASSERT_TRUE(passengerResult.has_value());
//     auto createResult = service->createPassenger(passengerResult.value());
//     ASSERT_TRUE(createResult.has_value());
//     auto result = service->getPassenger(_passport);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getName(), _name);
// }

// TEST_F(PassengerServiceTest, GetPassengerByPassportFailure)
// {
//     auto nonExistentPassport = PassportNumber::create("VN:999999");
//     ASSERT_TRUE(nonExistentPassport.has_value());
//     auto result = service->getPassenger(nonExistentPassport.value());
//     ASSERT_FALSE(result.has_value());
// }

// // Test passengerExists
// TEST_F(PassengerServiceTest, PassengerExistsSuccess)
// {
//     auto passengerResult = createTestPassenger();
//     ASSERT_TRUE(passengerResult.has_value());
//     auto createResult = service->createPassenger(passengerResult.value());
//     ASSERT_TRUE(createResult.has_value());
//     auto result = service->passengerExists(createResult.value().getPassport());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(PassengerServiceTest, PassengerExistsFailure)
// {
//     auto nonExistentPassport = PassportNumber::create("VN:999999");
//     auto result = service->passengerExists(nonExistentPassport.value());
//     ASSERT_TRUE(result.has_value());
//     EXPECT_FALSE(result.value());
// }

TEST_F(PassengerServiceTest, CanDeletePassengerSuccess)
{
    auto createResult = createTestPassenger();
    ASSERT_TRUE(createResult.has_value());
    auto PassengerResult = service->createPassenger(createResult.value());
    
    auto result = service->canDeletePassenger(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(PassengerServiceTest, HasActiveBookingsSuccess)
{
    auto createResult = createTestPassenger();
    ASSERT_TRUE(createResult.has_value());
    auto PassengerResult = service->createPassenger(createResult.value());
    
    auto result = service->hasActiveBookings(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

TEST_F(PassengerServiceTest, GetTotalFlightCountSuccess)
{
    auto createResult = createTestPassenger();
    ASSERT_TRUE(createResult.has_value());
    auto PassengerResult = service->createPassenger(createResult.value());
    auto result = service->getTotalFlightCount(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 0);
}