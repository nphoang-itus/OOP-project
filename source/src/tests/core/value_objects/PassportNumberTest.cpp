#include <gtest/gtest.h>
#include "../../../core/value_objects/passport_number/PassportNumber.h"
#include "BaseValueObjectTest.h"

class PassportNumberTest : public BaseValueObjectTest<PassportNumber> {
protected:
    // Constants for error codes
    const std::string EMPTY_PASSPORT = "EMPTY_PASSPORT_NUMBER";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    const std::string INVALID_COUNTRY = "INVALID_ISSUING_COUNTRY";
    const std::string INVALID_NUMBER_LENGTH = "INVALID_NUMBER_LENGTH";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "passportNumber: Passport number cannot be empty";
    const std::string FORMAT_MESSAGE = "passportNumber: Passport number must be in format 'COUNTRY:NUMBER'";
    const std::string COUNTRY_MESSAGE = "passportNumber: Issuing country must be a valid 2-letter country code";
    const std::string NUMBER_LENGTH_MESSAGE = "passportNumber: Passport number must be between 6-9 digits";

    // Implement virtual methods
    Result<PassportNumber> createValueObject(const std::string& value) override {
        return PassportNumber::create(value);
    }

    std::string getValue(const PassportNumber& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "passport number";

        // Setup valid test cases
        validCases = {
            // Different country codes - uppercase
            {"V1", {"US:123456", "US:123456", "", "", "US passport"}},
            {"V2", {"GB:123456", "GB:123456", "", "", "UK passport"}},
            {"V3", {"VN:123456", "VN:123456", "", "", "Vietnam passport"}},
            
            // Different country codes - lowercase
            {"V4", {"us:123456", "US:123456", "", "", "US passport lowercase"}},
            {"V5", {"gb:123456", "GB:123456", "", "", "UK passport lowercase"}},
            {"V6", {"vn:123456", "VN:123456", "", "", "Vietnam passport lowercase"}},
            
            // Different number lengths
            {"V7", {"US:123456", "US:123456", "", "", "6 digits"}},
            {"V8", {"US:1234567", "US:1234567", "", "", "7 digits"}},
            {"V9", {"US:12345678", "US:12345678", "", "", "8 digits"}},
            {"V10", {"US:123456789", "US:123456789", "", "", "9 digits"}},
            
            // Edge cases
            {"E1", {"US:000000", "US:000000", "", "", "all zeros"}},
            {"E2", {"US:999999", "US:999999", "", "", "all nines"}},
            {"E3", {"US:123456789", "US:123456789", "", "", "maximum length"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_PASSPORT, EMPTY_MESSAGE, "empty string"}},
            
            // Invalid format
            {"I2", {"US123456", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing colon"}},
            {"I3", {"US-123456", "", INVALID_FORMAT, FORMAT_MESSAGE, "wrong separator"}},
            {"I4", {"US:123456:", "", INVALID_FORMAT, FORMAT_MESSAGE, "extra colon"}},
            {"I5", {":123456", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing country"}},
            {"I6", {"US:", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing number"}},
            {"I13", {"US:12345A", "", INVALID_FORMAT, FORMAT_MESSAGE, "non-numeric characters"}},
            {"I14", {"US:12345.6", "", INVALID_FORMAT, FORMAT_MESSAGE, "decimal number"}},
            {"I15", {"US:-123456", "", INVALID_FORMAT, FORMAT_MESSAGE, "negative number"}},
            
            // Invalid country codes
            // {"I7", {"USA:123456", "", INVALID_COUNTRY, COUNTRY_MESSAGE, "3-letter country code"}},
            {"I8", {"U:123456", "", INVALID_COUNTRY, COUNTRY_MESSAGE, "1-letter country code"}},
            {"I9", {"12:123456", "", INVALID_COUNTRY, COUNTRY_MESSAGE, "numeric country code"}},
            
            // Invalid number lengths
            {"I11", {"US:12345", "", INVALID_NUMBER_LENGTH, NUMBER_LENGTH_MESSAGE, "too short (5 digits)"}},
            {"I12", {"US:1234567890", "", INVALID_NUMBER_LENGTH, NUMBER_LENGTH_MESSAGE, "too long (10 digits)"}}
        };
    }

    // Additional test methods for create with individual parameters
    void testCreateWithIndividualParams(
        const std::string& countryCode,
        const std::string& number,
        bool shouldSucceed,
        const std::string& expectedErrorCode = "",
        const std::string& expectedErrorMessage = "") {
        auto result = PassportNumber::create(countryCode, number);
        if (shouldSucceed) {
            ASSERT_TRUE(result.has_value()) 
                << "Failed to create PassportNumber with country: " << countryCode 
                << ", number: " << number;

        std::string upperCountryCode = countryCode;
        
        // Chuyển đổi countryCode thành chữ hoa
        std::transform(upperCountryCode.begin(), upperCountryCode.end(), upperCountryCode.begin(),
                      [](unsigned char c) { return std::toupper(c); });
            
            EXPECT_EQ(result->getIssuingCountry(), upperCountryCode);
            EXPECT_EQ(result->getNumber(), number);
        } else {
            ASSERT_FALSE(result.has_value());
            EXPECT_EQ(result.error().code, expectedErrorCode);
            EXPECT_EQ(result.error().message, expectedErrorMessage);
        }
    }
};

// Test all valid cases
TEST_F(PassportNumberTest, ValidPassportNumbers) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(PassportNumberTest, InvalidPassportNumbers) {
    runAllInvalidTests();
}

// Test create with individual parameters
TEST_F(PassportNumberTest, CreateWithIndividualParams) {
    // Valid cases - uppercase
    testCreateWithIndividualParams(
        "US",
        "123456",
        true
    );

    // Valid cases - lowercase
    testCreateWithIndividualParams(
        "us",
        "123456",
        true
    );

    testCreateWithIndividualParams(
        "gb",
        "123456789",
        true
    );

    // Invalid cases - Empty fields
    testCreateWithIndividualParams(
        "",
        "123456",
        false,
        EMPTY_PASSPORT,
        EMPTY_MESSAGE
    );

    testCreateWithIndividualParams(
        "US",
        "",
        false,
        EMPTY_PASSPORT,
        EMPTY_MESSAGE
    );

    // Invalid cases - Number validation
    testCreateWithIndividualParams(
        "US",
        "12345",
        false,
        INVALID_NUMBER_LENGTH,
        NUMBER_LENGTH_MESSAGE
    );

    testCreateWithIndividualParams(
        "US",
        "1234567890",
        false,
        INVALID_NUMBER_LENGTH,
        NUMBER_LENGTH_MESSAGE
    );
}

// Test formatting variations
TEST_F(PassportNumberTest, FormattingVariations) {
    // Test uppercase
    auto result1 = PassportNumber::create("US", "123456");
    ASSERT_TRUE(result1.has_value());
    const PassportNumber& passport1 = *result1;
    EXPECT_EQ(passport1.toString(), "US:123456");
    EXPECT_EQ(passport1.getIssuingCountry(), "US");
    EXPECT_EQ(passport1.getNumber(), "123456");

    // Test lowercase
    auto result2 = PassportNumber::create("us", "123456");
    ASSERT_TRUE(result2.has_value());
    const PassportNumber& passport2 = *result2;
    EXPECT_EQ(passport2.toString(), "US:123456");
    EXPECT_EQ(passport2.getIssuingCountry(), "US");
    EXPECT_EQ(passport2.getNumber(), "123456");
}

// Test value object operations
TEST_F(PassportNumberTest, ValueObjectOperations) {
    auto result1 = PassportNumber::create("US", "123456");
    auto result2 = PassportNumber::create("us", "123456");
    auto result3 = PassportNumber::create("GB", "123456");

    ASSERT_TRUE(result1.has_value() && result2.has_value() && result3.has_value());

    // Test equality (case insensitive)
    EXPECT_TRUE(*result1 == *result2);
    EXPECT_FALSE(*result1 == *result3);

    // Test inequality
    EXPECT_FALSE(*result1 != *result2);
    EXPECT_TRUE(*result1 != *result3);
} 