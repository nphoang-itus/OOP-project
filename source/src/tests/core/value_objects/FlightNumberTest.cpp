#include <gtest/gtest.h>
#include "../../../core/value_objects/flight_number/FlightNumber.h"
#include "BaseValueObjectTest.h"

class FlightNumberTest : public BaseValueObjectTest<FlightNumber> {
protected:
    // Constants for error codes
    const std::string EMPTY_FLIGHT = "EMPTY_FLIGHT_NUMBER";
    const std::string INVALID_LENGTH = "INVALID_LENGTH";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "flightNumber: Flight number cannot be empty";
    const std::string LENGTH_MESSAGE = "flightNumber: Flight number must be between 3 and 6 characters";
    const std::string FORMAT_MESSAGE = "flightNumber: Flight number must start with 2 letters followed by 1-4 digits";

    // Implement virtual methods
    Result<FlightNumber> createValueObject(const std::string& value) override {
        return FlightNumber::create(value);
    }

    std::string getValue(const FlightNumber& obj) override {
        return obj.value();
    }

    void setupTestCases() override {
        fieldName = "flight number";

        // Setup valid test cases
        validCases = {
            // Different prefixes
            {"V1", {"VN123", "VN123", "", "", "2-letter prefix"}},
            {"V2", {"VN123", "VN123", "", "", "3-letter prefix"}},
            
            // Different lengths
            {"V3", {"VN1", "VN1", "", "", "minimum length"}},
            {"V4", {"VN123", "VN123", "", "", "typical length"}},
            {"V5", {"VN1234", "VN1234", "", "", "maximum length"}},
            
            // Edge cases
            {"E1", {"VN1234", "VN1234", "", "", "max digits"}},
            {"E2", {"VN1", "VN1", "", "", "min digits"}},
            {"E3", {"VN1111", "VN1111", "", "", "all same digits"}},
            {"E4", {"VV111", "VV111", "", "", "all same letters"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_FLIGHT, EMPTY_MESSAGE, "empty string"}},
            
            // Incorrect length
            {"I2", {"V1", "", INVALID_LENGTH, LENGTH_MESSAGE, "too short (2 chars)"}},
            {"I3", {"VNA12345", "", INVALID_LENGTH, LENGTH_MESSAGE, "too long (7 chars)"}},
            {"I6", {"VNAA123", "", INVALID_LENGTH, LENGTH_MESSAGE, "more than 3 letters"}},
            {"I7", {"VN12345", "", INVALID_LENGTH, LENGTH_MESSAGE, "more than 4 digits"}},
            
            // Incorrect format
            {"I4", {"   ", "", INVALID_FORMAT, FORMAT_MESSAGE, "whitespace only"}},
            {"I5", {"1VN23", "", INVALID_FORMAT, FORMAT_MESSAGE, "starts with number"}},
            {"I8", {"VN-123", "", INVALID_FORMAT, FORMAT_MESSAGE, "contains special character"}},
            {"I9", {"vn123", "", INVALID_FORMAT, FORMAT_MESSAGE, "lowercase letters"}},
            {"I10", {"VN 123", "", INVALID_FORMAT, FORMAT_MESSAGE, "contains space"}},
            {"I11", {"Vn123", "", INVALID_FORMAT, FORMAT_MESSAGE, "mixed case letters"}}
        };
    }
};

// Test all valid cases
TEST_F(FlightNumberTest, ValidFlightNumbers) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(FlightNumberTest, InvalidFlightNumbers) {
    runAllInvalidTests();
}

// Test value object operations
TEST_F(FlightNumberTest, ValueObjectOperations) {
    auto result1 = FlightNumber::create("VN123");
    auto result2 = FlightNumber::create("VN123");
    auto result3 = FlightNumber::create("VN456");

    ASSERT_TRUE(result1.has_value() && result2.has_value() && result3.has_value());

    // Test equality
    EXPECT_TRUE(*result1 == *result2);
    EXPECT_FALSE(*result1 == *result3);

    // Test inequality
    EXPECT_FALSE(*result1 != *result2);
    EXPECT_TRUE(*result1 != *result3);

    // Test toString
    EXPECT_EQ(result1->toString(), "VN123");
}
