#include "services/PassengerService.h"
#include "repositories/MockRepository/PassengerMockRepository.h"
#include "core/entities/Passenger.h"
#include "core/value_objects/passport_number/PassportNumber.h"
#include "core/value_objects/contact_info/ContactInfo.h"
#include "utils/Logger.h"
#include <gtest/gtest.h>
#include <memory>

class PassengerServiceTest : public ::testing::Test
{
protected:
    std::shared_ptr<PassengerMockRepository> mockRepository;
    std::shared_ptr<PassengerService> service;
    std::shared_ptr<Logger> logger;
    std::string _name;
    ContactInfo _contactInfo;
    PassportNumber _passport;

    void SetUp() override
    {
        mockRepository = std::make_shared<PassengerMockRepository>(Logger::getInstance());
        logger = Logger::getInstance();
        service = std::make_shared<PassengerService>(mockRepository, logger);

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
        mockRepository->clear();
    }

    // Helper method to create a passenger
    Result<Passenger> createPassenger()
    {
        return Passenger::create(_name, _contactInfo, _passport);
    }
};

// Test createPassenger
TEST_F(PassengerServiceTest, CreatePassengerSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto result = service->createPassenger(passengerResult.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), _name);
    EXPECT_EQ(result.value().getContactInfo().toString(), _contactInfo.toString());
    EXPECT_EQ(result.value().getPassport().toString(), _passport.toString());
}

// Test getPassengerById
TEST_F(PassengerServiceTest, GetPassengerByIdSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = service->createPassenger(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->getPassengerById(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), _name);
}

TEST_F(PassengerServiceTest, GetPassengerByIdFailure)
{
    auto result = service->getPassengerById(999);
    ASSERT_FALSE(result.has_value());
}

// Test getAllPassengers
TEST_F(PassengerServiceTest, GetAllPassengersSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    service->createPassenger(passengerResult.value());
    // Create another passenger
    std::string name2 = "Jane Doe";
    auto contactInfoResult = ContactInfo::create("jane@example.com|+84999888777|456 Street");
    ASSERT_TRUE(contactInfoResult.has_value());
    auto passportResult = PassportNumber::create("VN:654321");
    ASSERT_TRUE(passportResult.has_value());
    auto passengerResult2 = Passenger::create(name2, contactInfoResult.value(), passportResult.value());
    ASSERT_TRUE(passengerResult2.has_value());
    service->createPassenger(passengerResult2.value());
    auto result = service->getAllPassengers();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

// Test updatePassenger
TEST_F(PassengerServiceTest, UpdatePassengerSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = service->createPassenger(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());
    // Update data
    std::string name2 = "Jane Doe";
    auto contactInfoResult = ContactInfo::create("jane@example.com|+84999888777|456 Street");
    ASSERT_TRUE(contactInfoResult.has_value());
    auto updatedPassengerResult = Passenger::create(name2, contactInfoResult.value(), _passport);
    ASSERT_TRUE(updatedPassengerResult.has_value());
    auto updatedPassenger = updatedPassengerResult.value();
    updatedPassenger.setId(createResult.value().getId());
    auto result = service->updatePassenger(updatedPassenger);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), name2);
    EXPECT_EQ(result.value().getContactInfo().toString(), contactInfoResult.value().toString());
}

// Test deletePassenger
TEST_F(PassengerServiceTest, DeletePassengerSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = service->createPassenger(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->deletePassenger(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
    // Verify not found
    auto findResult = service->getPassengerById(1);
    ASSERT_FALSE(findResult.has_value());
}

// Test getPassengerByPassport
TEST_F(PassengerServiceTest, GetPassengerByPassportSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = service->createPassenger(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->getPassengerByPassport(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), _name);
}

TEST_F(PassengerServiceTest, GetPassengerByPassportFailure)
{
    auto nonExistentPassport = PassportNumber::create("VN:999999");
    ASSERT_TRUE(nonExistentPassport.has_value());
    auto result = service->getPassengerByPassport(nonExistentPassport.value());
    ASSERT_FALSE(result.has_value());
}

// Test passengerExists
TEST_F(PassengerServiceTest, PassengerExistsSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = service->createPassenger(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->passengerExists(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(PassengerServiceTest, PassengerExistsFailure)
{
    auto result = service->passengerExists(999);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test passportExists
TEST_F(PassengerServiceTest, PassportExistsSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = service->createPassenger(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());
    auto result = service->passportExists(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(PassengerServiceTest, PassportExistsFailure)
{
    auto nonExistentPassport = PassportNumber::create("VN:999999");
    ASSERT_TRUE(nonExistentPassport.has_value());
    auto result = service->passportExists(nonExistentPassport.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// Test getPassengerCount
TEST_F(PassengerServiceTest, GetPassengerCountSuccess)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    service->createPassenger(passengerResult.value());
    auto result = service->getPassengerCount();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 1);
}
