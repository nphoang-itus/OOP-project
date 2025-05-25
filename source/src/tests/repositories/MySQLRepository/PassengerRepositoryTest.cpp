#include <gtest/gtest.h>
#include "../../../repositories/MySQLRepository/PassengerRepository.h"
#include "../../../core/value_objects/passport_number/PassportNumber.h"
#include "../../../core/value_objects/contact_info/ContactInfo.h"
#include "../../../core/exceptions/Result.h"
#include "../../../database/MySQLXConnection.h"
#include <memory>

class PassengerRepositoryTest : public ::testing::Test {
protected:
    std::shared_ptr<PassengerRepository> repository;
    std::shared_ptr<MySQLXConnection> db;
    std::shared_ptr<Logger> logger;

    // Test data
    PassportNumber _passport;
    std::string _name;
    ContactInfo _contactInfo;

    void SetUp() override {
        // Setup database connection
        db = MySQLXConnection::getInstance();
        auto result = db->connect("localhost", "nphoang", "phucHoang133205", "airlines_db", 33060);
        ASSERT_TRUE(result.has_value()) << "Failed to connect to database: " << result.error().message;

        // Setup logger
        logger = Logger::getInstance();
        logger->setMinLevel(LogLevel::DEBUG);

        // Setup repository
        repository = std::make_shared<PassengerRepository>(db, logger);

        // Setup test passenger data
        auto passportResult = PassportNumber::create("VN:12345678");
        ASSERT_TRUE(passportResult.has_value());
        _passport = *passportResult;

        _name = "John Doe";

        // Create contact info string
        std::string contactInfoStr = "john.doe@example.com|1234567890|123 Main St";
        auto contactInfoResult = ContactInfo::create(contactInfoStr);
        ASSERT_TRUE(contactInfoResult.has_value());
        _contactInfo = *contactInfoResult;
    }

    void TearDown() override {
        // Clean up test data
        auto result = db->execute("DELETE FROM passenger WHERE passport_number = 'VN:12345678'");
        ASSERT_TRUE(result.has_value()) << "Failed to clean up test data: " << result.error().message;
        
        db->disconnect();
    }

    // Helper method to create a test passenger
    Result<Passenger> createTestPassenger() {
        return Passenger::create(_name, _contactInfo, _passport);
    }
};

// Test create operation
TEST_F(PassengerRepositoryTest, CreatePassenger) {
    // Create test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    const Passenger& passenger = *passengerResult;

    // Save to database
    auto result = repository->create(passenger);
    ASSERT_TRUE(result.has_value());
    const Passenger& savedPassenger = *result;

    // Verify saved passenger
    EXPECT_GT(savedPassenger.getId(), 0);
    EXPECT_EQ(savedPassenger.getPassport().toString(), _passport.toString());
    EXPECT_EQ(savedPassenger.getName(), _name);
    EXPECT_EQ(savedPassenger.getContactInfo().getEmail(), _contactInfo.getEmail());
    EXPECT_EQ(savedPassenger.getContactInfo().getPhone(), _contactInfo.getPhone());
    EXPECT_EQ(savedPassenger.getContactInfo().getAddress(), _contactInfo.getAddress());
}

// Test findById operation
TEST_F(PassengerRepositoryTest, FindPassengerById) {
    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Find passenger by id
    auto result = repository->findById(id);
    ASSERT_TRUE(result.has_value());
    const Passenger& foundPassenger = *result;

    // Verify found passenger
    EXPECT_EQ(foundPassenger.getId(), id);
    EXPECT_EQ(foundPassenger.getPassport().toString(), _passport.toString());
    EXPECT_EQ(foundPassenger.getName(), _name);
    EXPECT_EQ(foundPassenger.getContactInfo().getEmail(), _contactInfo.getEmail());
    EXPECT_EQ(foundPassenger.getContactInfo().getPhone(), _contactInfo.getPhone());
    EXPECT_EQ(foundPassenger.getContactInfo().getAddress(), _contactInfo.getAddress());
}

// Test findAll operation
TEST_F(PassengerRepositoryTest, FindAllPassengers) {
    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
    ASSERT_TRUE(createResult.has_value());

    // Find all passengers
    auto result = repository->findAll();
    ASSERT_TRUE(result.has_value());
    const auto& passengers = *result;

    // Verify found passengers
    EXPECT_FALSE(passengers.empty());
    bool found = false;
    for (const auto& passenger : passengers) {
        if (passenger.getPassport().toString() == _passport.toString()) {
            found = true;
            EXPECT_EQ(passenger.getName(), _name);
            EXPECT_EQ(passenger.getContactInfo().getEmail(), _contactInfo.getEmail());
            EXPECT_EQ(passenger.getContactInfo().getPhone(), _contactInfo.getPhone());
            EXPECT_EQ(passenger.getContactInfo().getAddress(), _contactInfo.getAddress());
            break;
        }
    }
    EXPECT_TRUE(found) << "Test passenger not found in findAll results";
}

// Test exists operation
TEST_F(PassengerRepositoryTest, ExistsPassenger) {
    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
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
TEST_F(PassengerRepositoryTest, CountPassengers) {
    // Get initial count
    auto initialCountResult = repository->count();
    ASSERT_TRUE(initialCountResult.has_value());
    size_t initialCount = *initialCountResult;

    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
    ASSERT_TRUE(createResult.has_value());

    // Get new count
    auto newCountResult = repository->count();
    ASSERT_TRUE(newCountResult.has_value());
    size_t newCount = *newCountResult;

    // Verify count increased by 1
    EXPECT_EQ(newCount, initialCount + 1);
}

// Test update operation
TEST_F(PassengerRepositoryTest, UpdatePassenger) {
    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
    ASSERT_TRUE(createResult.has_value());
    Passenger passenger = *createResult;

    // Update passenger
    passenger.setName("Jane Doe");
    auto result = repository->update(passenger);
    ASSERT_TRUE(result.has_value());
    const Passenger& updatedPassenger = *result;

    // Verify updated passenger
    EXPECT_EQ(updatedPassenger.getId(), passenger.getId());
    EXPECT_EQ(updatedPassenger.getPassport().toString(), _passport.toString());
    EXPECT_EQ(updatedPassenger.getName(), "Jane Doe");
    EXPECT_EQ(updatedPassenger.getContactInfo().getEmail(), _contactInfo.getEmail());
    EXPECT_EQ(updatedPassenger.getContactInfo().getPhone(), _contactInfo.getPhone());
    EXPECT_EQ(updatedPassenger.getContactInfo().getAddress(), _contactInfo.getAddress());
}

// Test deleteById operation
TEST_F(PassengerRepositoryTest, DeletePassengerById) {
    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
    ASSERT_TRUE(createResult.has_value());
    int id = createResult->getId();

    // Delete passenger
    auto result = repository->deleteById(id);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);

    // Verify passenger is deleted
    auto existsResult = repository->exists(id);
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_FALSE(*existsResult);
}

// Test findByPassportNumber operation
TEST_F(PassengerRepositoryTest, FindPassengerByPassportNumber) {
    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
    ASSERT_TRUE(createResult.has_value());

    // Find passenger by passport number
    auto result = repository->findByPassportNumber(_passport);
    ASSERT_TRUE(result.has_value());
    const Passenger& foundPassenger = *result;

    // Verify found passenger
    EXPECT_EQ(foundPassenger.getPassport().toString(), _passport.toString());
    EXPECT_EQ(foundPassenger.getName(), _name);
    EXPECT_EQ(foundPassenger.getContactInfo().getEmail(), _contactInfo.getEmail());
    EXPECT_EQ(foundPassenger.getContactInfo().getPhone(), _contactInfo.getPhone());
    EXPECT_EQ(foundPassenger.getContactInfo().getAddress(), _contactInfo.getAddress());
}

// Test existsPassport operation
TEST_F(PassengerRepositoryTest, ExistsPassport) {
    // Create and save test passenger
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    auto createResult = repository->create(*passengerResult);
    ASSERT_TRUE(createResult.has_value());

    // Check passport existence
    auto result = repository->existsPassport(_passport);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);

    // Check non-existent passport
    auto nonExistentPassportResult = PassportNumber::create("VN:99999999");
    ASSERT_TRUE(nonExistentPassportResult.has_value());
    auto nonExistentResult = repository->existsPassport(*nonExistentPassportResult);
    ASSERT_TRUE(nonExistentResult.has_value());
    EXPECT_FALSE(*nonExistentResult);
}

// Test error handling
TEST_F(PassengerRepositoryTest, ErrorHandling) {
    // Test findById with invalid id
    auto result1 = repository->findById(-1);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "NOT_FOUND");

    // Test update with non-existent id
    auto passengerResult = createTestPassenger();
    ASSERT_TRUE(passengerResult.has_value());
    Passenger passenger = *passengerResult;
    passenger.setId(999999);
    auto result2 = repository->update(passenger);
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "DB_ERROR");

    // Test deleteById with non-existent id
    auto result3 = repository->deleteById(999999);
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, "DB_ERROR");

    // Test findByPassportNumber with non-existent passport
    auto nonExistentPassportResult = PassportNumber::create("VN:99999999");
    ASSERT_TRUE(nonExistentPassportResult.has_value());
    auto result4 = repository->findByPassportNumber(*nonExistentPassportResult);
    ASSERT_FALSE(result4.has_value());
    EXPECT_EQ(result4.error().code, "NOT_FOUND");
} 