#include <gtest/gtest.h>
#include "../../../core/entities/Passenger.h"
#include "../../../core/value_objects/contact_info/ContactInfo.h"
#include "../../../core/value_objects/passport_number/PassportNumber.h"
#include "../../../core/exceptions/Result.h"

class PassengerTest : public ::testing::Test {
protected:
    // Test data
    Name _name;
    ContactInfo _contactInfo;
    PassportNumber _passport;

    void SetUp() override {
        // Setup test name
        _name = "John Doe";

        // Setup test contact info
        auto contactInfoResult = ContactInfo::create("user@example.com|+84123456789|123 Street");
        ASSERT_TRUE(contactInfoResult.has_value());
        _contactInfo = *contactInfoResult;

        // Setup test passport
        auto passportResult = PassportNumber::create("US:123456");
        ASSERT_TRUE(passportResult.has_value());
        _passport = *passportResult;
    }

    // Helper method to create a passenger
    Result<Passenger> createPassenger() {
        return Passenger::create(_name, _contactInfo, _passport);
    }
};

// Test passenger creation
TEST_F(PassengerTest, CreatePassenger) {
    auto result = createPassenger();
    ASSERT_TRUE(result.has_value());
    const Passenger& passenger = *result;

    // Test getters
    EXPECT_EQ(passenger.getName(), _name);
    EXPECT_EQ(passenger.getContactInfo().toString(), _contactInfo.toString());
    EXPECT_EQ(passenger.getPassport().toString(), _passport.toString());
}

// Test passenger creation with invalid parameters
TEST_F(PassengerTest, CreatePassengerWithInvalidParameters) {
    // Test with empty name
    auto result1 = Passenger::create("", _contactInfo, _passport);
    ASSERT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error().code, "INVALID_NAME");
    EXPECT_EQ(result1.error().message, "Passenger name cannot be empty");

    // Test with invalid contact info
    auto result2 = Passenger::create(_name, "invalid-contact", _passport.toString());
    ASSERT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error().code, "INVALID_CONTACT_INFO");

    // Test with invalid passport
    auto result3 = Passenger::create(_name, _contactInfo.toString(), "invalid-passport");
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, "INVALID_PASSPORT");
}

// Test value object operations
TEST_F(PassengerTest, ValueObjectOperations) {
    auto result1 = createPassenger();
    auto result2 = createPassenger();
    ASSERT_TRUE(result1.has_value() && result2.has_value());
    const Passenger& passenger1 = *result1;
    const Passenger& passenger2 = *result2;

    // Test equality
    EXPECT_TRUE(passenger1.equals(passenger2));

    // Test clone
    auto clone = passenger1.clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_TRUE(passenger1.equals(*clone));

    // Test toString
    std::string expected = "Passenger{name=" + _name + 
                          ", contactInfo=" + _contactInfo.toString() + 
                          ", passport=" + _passport.toString() + "}";
    EXPECT_EQ(passenger1.toString(), expected);
}

// Test string-based creation
TEST_F(PassengerTest, StringBasedCreation) {
    auto result = Passenger::create("Jane Doe", "user@example.com|+84123456789|123 Street", "VN:123456789");
    ASSERT_TRUE(result.has_value());
    const Passenger& passenger = *result;

    EXPECT_EQ(passenger.getName(), "Jane Doe");
    EXPECT_EQ(passenger.getContactInfo().toString(), "user@example.com|+84123456789|123 Street");
    EXPECT_EQ(passenger.getPassport().toString(), "VN:123456789");
}

// Test different contact info formats
TEST_F(PassengerTest, DifferentContactInfoFormats) {
    // Test with different email and phone
    auto result1 = Passenger::create(_name, "duser@example.com|+84123456789|123 Street", _passport.toString());
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->getContactInfo().toString(), "duser@example.com|+84123456789|123 Street");

    // Test with different phone format
    auto result2 = Passenger::create(_name, "same@email.com|+84999888777|123 Blabla", _passport.toString());
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->getContactInfo().toString(), "same@email.com|+84999888777|123 Blabla");
}

// Test different passport formats
TEST_F(PassengerTest, DifferentPassportFormats) {
    // Test with different passport number
    auto result1 = Passenger::create(_name, _contactInfo.toString(), "VN:99999999");
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->getPassport().toString(), "VN:99999999");

    // Test with another passport number
    auto result2 = Passenger::create(_name, _contactInfo.toString(), "VN:11111111");
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->getPassport().toString(), "VN:11111111");
} 