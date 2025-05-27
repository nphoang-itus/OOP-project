#ifndef PASSENGER_MOCK_REPOSITORY_TEST_H
#define PASSENGER_MOCK_REPOSITORY_TEST_H

#include <gtest/gtest.h>
#include "repositories/MockRepository/PassengerMockRepository.h"
#include "core/entities/Passenger.h"
#include "core/value_objects/passport_number/PassportNumber.h"
#include "core/value_objects/contact_info/ContactInfo.h"
#include "utils/Logger.h"
#include <memory>

class PassengerMockRepositoryTest : public ::testing::Test
{
protected:
    std::shared_ptr<PassengerMockRepository> repository;
    std::string _name;
    ContactInfo _contactInfo;
    PassportNumber _passport;

    void SetUp() override
    {
        repository = std::make_shared<PassengerMockRepository>(Logger::getInstance());

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
        repository->clear();
    }

    // Helper method to create a passenger
    Result<Passenger> createPassenger()
    {
        return Passenger::create(_name, _contactInfo, _passport);
    }
};

// Test create operation
TEST_F(PassengerMockRepositoryTest, CreatePassenger)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());

    auto result = repository->create(passengerResult.value());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), _name);
    EXPECT_EQ(result.value().getContactInfo().toString(), _contactInfo.toString());
    EXPECT_EQ(result.value().getPassport().toString(), _passport.toString());
    EXPECT_EQ(result.value().getId(), 1);
}

// Test findById operation
TEST_F(PassengerMockRepositoryTest, FindById)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->findById(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), _name);
    EXPECT_EQ(result.value().getContactInfo().toString(), _contactInfo.toString());
    EXPECT_EQ(result.value().getPassport().toString(), _passport.toString());

    auto notFoundResult = repository->findById(999);
    ASSERT_FALSE(notFoundResult.has_value());
}

// Test findAll operation
TEST_F(PassengerMockRepositoryTest, FindAll)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    repository->create(passengerResult.value());

    // Create another passenger
    std::string name2 = "Jane Doe";
    auto contactInfoResult = ContactInfo::create("jane@example.com|+84999888777|456 Street");
    ASSERT_TRUE(contactInfoResult.has_value());
    auto passportResult = PassportNumber::create("VN:654321");
    ASSERT_TRUE(passportResult.has_value());

    auto passengerResult2 = Passenger::create(name2, contactInfoResult.value(), passportResult.value());
    ASSERT_TRUE(passengerResult2.has_value());
    repository->create(passengerResult2.value());

    auto result = repository->findAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

// Test update operation
TEST_F(PassengerMockRepositoryTest, UpdatePassenger)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());

    // Create a new passenger with updated data
    std::string name2 = "Jane Doe";
    auto contactInfoResult = ContactInfo::create("jane@example.com|+84999888777|456 Street");
    ASSERT_TRUE(contactInfoResult.has_value());

    auto updatedPassengerResult = Passenger::create(name2, contactInfoResult.value(), _passport);
    ASSERT_TRUE(updatedPassengerResult.has_value());
    auto updatedPassenger = updatedPassengerResult.value();
    updatedPassenger.setId(createResult.value().getId());

    auto result = repository->update(updatedPassenger);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), name2);
    EXPECT_EQ(result.value().getContactInfo().toString(), contactInfoResult.value().toString());

    auto findResult = repository->findById(1);
    ASSERT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult.value().getName(), name2);
    EXPECT_EQ(findResult.value().getContactInfo().toString(), contactInfoResult.value().toString());
}

// Test deleteById operation
TEST_F(PassengerMockRepositoryTest, DeleteById)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto deleteResult = repository->deleteById(1);
    ASSERT_TRUE(deleteResult.has_value());
    EXPECT_TRUE(deleteResult.value());

    auto findResult = repository->findById(1);
    ASSERT_FALSE(findResult.has_value());
}

// Test exists operation
TEST_F(PassengerMockRepositoryTest, Exists)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto existsResult = repository->exists(1);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_TRUE(existsResult.value());

    auto notExistsResult = repository->exists(999);
    ASSERT_TRUE(notExistsResult.has_value());
    EXPECT_FALSE(notExistsResult.value());
}

// Test count operation
TEST_F(PassengerMockRepositoryTest, Count)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    repository->create(passengerResult.value());
    EXPECT_TRUE(repository->count().has_value());
    EXPECT_EQ(repository->count().value(), 1);
}

// Test findByPassport operation
TEST_F(PassengerMockRepositoryTest, FindByPassport)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->findByPassportNumber(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), _name);
    EXPECT_EQ(result.value().getContactInfo().toString(), _contactInfo.toString());
    EXPECT_EQ(result.value().getPassport().toString(), _passport.toString());

    // Test with non-existent passport
    auto nonExistentPassport = PassportNumber::create("VN:999999");
    ASSERT_TRUE(nonExistentPassport.has_value());
    auto notFoundResult = repository->findByPassportNumber(nonExistentPassport.value());
    ASSERT_FALSE(notFoundResult.has_value());
}

// Test existsPassport operation
TEST_F(PassengerMockRepositoryTest, ExistsPassport)
{
    auto passengerResult = createPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(passengerResult.value());
    ASSERT_TRUE(createResult.has_value());

    auto result = repository->existsPassport(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    // Test with non-existent passport
    auto nonExistentPassport = PassportNumber::create("VN:999999");
    ASSERT_TRUE(nonExistentPassport.has_value());
    auto notFoundResult = repository->existsPassport(nonExistentPassport.value());
    ASSERT_TRUE(notFoundResult.has_value());
    EXPECT_FALSE(notFoundResult.value());
}

#endif // PASSENGER_MOCK_REPOSITORY_TEST_H