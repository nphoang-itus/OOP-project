#include <gtest/gtest.h>
#include "../../../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "BaseValueObjectTest.h"

class AircraftSerialTest : public BaseValueObjectTest<AircraftSerial> {
protected:
    // Constants for error codes
    const std::string EMPTY_SERIAL = "EMPTY_AIRCRAFT_SERIAL";
    const std::string INVALID_LENGTH = "INVALID_LENGTH";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "aircraftSerial: Aircraft serial cannot be empty";
    const std::string LENGTH_MESSAGE = "aircraftSerial: Aircraft serial must be between 3 and 10 characters";
    const std::string FORMAT_MESSAGE = "aircraftSerial: Aircraft serial must start with 2-3 letters followed by 1-7 digits";

    // Implement virtual methods
    Result<AircraftSerial> createValueObject(const std::string& value) override {
        return AircraftSerial::create(value);
    }

    std::string getValue(const AircraftSerial& obj) override {
        return obj.value();
    }

    void setupTestCases() override {
        fieldName = "aircraft serial";

        // Setup valid test cases
        validCases = {
            // Different prefixes
            {"V1", {"VN123", "VN123", "", "", "2-letter prefix"}},
            {"V2", {"VNA123", "VNA123", "", "", "3-letter prefix"}},
            
            // Different lengths
            {"V3", {"VN1", "VN1", "", "", "minimum length"}},
            {"V4", {"VN123", "VN123", "", "", "typical length"}},
            {"V5", {"VNA1234567", "VNA1234567", "", "", "maximum length"}},
            
            // Edge cases
            {"E1", {"VN1234567", "VN1234567", "", "", "min prefix, max digits"}},
            {"E2", {"VNA1", "VNA1", "", "", "max prefix, min digits"}},
            {"E3", {"VN1111111", "VN1111111", "", "", "all same digits"}},
            {"E4", {"VVV111", "VVV111", "", "", "all same letters"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_SERIAL, EMPTY_MESSAGE, "empty string"}},
            
            // Incorrect length
            {"I2", {"V1", "", INVALID_LENGTH, LENGTH_MESSAGE, "too short (2 chars)"}},
            {"I3", {"VNA12345678", "", INVALID_LENGTH, LENGTH_MESSAGE, "too long (11 chars)"}},
            
            // Incorrect format
            {"I4", {"   ", "", INVALID_FORMAT, FORMAT_MESSAGE, "whitespace only"}},
            {"I5", {"1VN23", "", INVALID_FORMAT, FORMAT_MESSAGE, "starts with number"}},
            {"I6", {"VNAA123", "", INVALID_FORMAT, FORMAT_MESSAGE, "more than 3 letters"}},
            {"I7", {"VN12345678", "", INVALID_FORMAT, FORMAT_MESSAGE, "more than 7 digits"}},
            {"I8", {"VN-123", "", INVALID_FORMAT, FORMAT_MESSAGE, "contains special character"}},
            {"I9", {"vn123", "", INVALID_FORMAT, FORMAT_MESSAGE, "lowercase letters"}},
            {"I10", {"VN 123", "", INVALID_FORMAT, FORMAT_MESSAGE, "contains space"}},
            {"I11", {"Vn123", "", INVALID_FORMAT, FORMAT_MESSAGE, "mixed case letters"}}
        };
    }
};

// Test all valid cases
TEST_F(AircraftSerialTest, ValidAircraftSerials) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(AircraftSerialTest, InvalidAircraftSerials) {
    runAllInvalidTests();
}

// Test specific categories of valid cases
TEST_F(AircraftSerialTest, ValidAircraftSerialsWithDifferentPrefixes) {
    runSpecificValidTests({"V1", "V2"});
}

// Test specific categories of invalid cases
TEST_F(AircraftSerialTest, InvalidAircraftSerialsWithEmptyInput) {
    runSpecificInvalidTests({"I1"});
}

TEST_F(AircraftSerialTest, InvalidAircraftSerialsWithIncorrectLength) {
    runSpecificInvalidTests({"I2", "I3"});
}

TEST_F(AircraftSerialTest, InvalidAircraftSerialsWithIncorrectFormat) {
    runSpecificInvalidTests({"I4", "I5", "I6", "I7", "I8", "I9", "I10", "I11"});
} 