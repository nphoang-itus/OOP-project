#include <gtest/gtest.h>
#include "../../../core/value_objects/seat_number/SeatNumber.h"
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "BaseValueObjectTest.h"

class SeatNumberTest : public BaseValueObjectTest<SeatNumber> {
protected:
    // Constants for error codes
    const std::string EMPTY_SEAT_NUMBER = "EMPTY_SEAT_NUMBER";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    const std::string INVALID_CLASS_CODE = "INVALID_CLASS_CODE";
    const std::string INVALID_SEQUENCE_NUMBER = "INVALID_SEQUENCE_NUMBER";
    const std::string INVALID_SEAT_CLASS = "INVALID_SEAT_CLASS";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "seatNumber: Seat number cannot be empty";
    const std::string FORMAT_MESSAGE = "seatNumber: Seat number must be in format 'CLASS_CODE[SEQUENCE_NUMBER]'";
    const std::string CLASS_CODE_MESSAGE = "seatNumber: Invalid seat class code";
    const std::string SEQUENCE_MESSAGE = "seatNumber: Invalid sequence number";
    const std::string SEAT_CLASS_MESSAGE = "seatNumber: Seat class not found in aircraft layout";

    // Test seat layout
    SeatClassMap _seatLayout;

    void SetUp() override {
        // Setup test seat layout
        auto result = SeatClassMap::create({
            {SeatClassRegistry::getByName("ECONOMY").value(), 100},    // E001-E100
            {SeatClassRegistry::getByName("BUSINESS").value(), 20},    // B01-B20
            {SeatClassRegistry::getByName("FIRST").value(), 10}        // F01-F10
        });
        ASSERT_TRUE(result.has_value());
        _seatLayout = result.value();
    }

    // Implement virtual methods
    Result<SeatNumber> createValueObject(const std::string& value) override {
        return SeatNumber::create(value, _seatLayout);
    }

    std::string getValue(const SeatNumber& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "seatNumber";

        // Setup valid test cases
        validCases = {
            // Economy class
            {"V1", {"E001", "E001", "", "", "economy first seat"}},
            {"V2", {"E100", "E100", "", "", "economy last seat"}},
            {"V3", {"E050", "E050", "", "", "economy middle seat"}},
            
            // Business class
            {"V4", {"B01", "B01", "", "", "business first seat"}},
            {"V5", {"B20", "B20", "", "", "business last seat"}},
            {"V6", {"B10", "B10", "", "", "business middle seat"}},
            
            // First class
            {"V7", {"F01", "F01", "", "", "first class first seat"}},
            {"V8", {"F10", "F10", "", "", "first class last seat"}},
            {"V9", {"F05", "F05", "", "", "first class middle seat"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_SEAT_NUMBER, EMPTY_MESSAGE, "empty string"}},
            
            // Invalid format
            {"I2", {"A1", "", INVALID_FORMAT, FORMAT_MESSAGE, "single digit"}},
            {"I3", {"E1", "", INVALID_FORMAT, FORMAT_MESSAGE, "single digit"}},
            {"I4", {"E0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "too many digits"}},
            {"I5", {"1E", "", INVALID_FORMAT, FORMAT_MESSAGE, "wrong order"}},
            {"I6", {"E 01", "", INVALID_FORMAT, FORMAT_MESSAGE, "space in number"}},
            
            // Invalid class code
            {"I7", {"X001", "", INVALID_SEAT_CLASS, SEAT_CLASS_MESSAGE, "non-existent class"}},
            {"I8", {"e001", "", INVALID_FORMAT, FORMAT_MESSAGE, "lowercase class"}},
            {"I9", {"1E01", "", INVALID_FORMAT, FORMAT_MESSAGE, "number in class"}},
            
            // Invalid sequence number
            {"I10", {"E000", "", INVALID_SEQUENCE_NUMBER, SEQUENCE_MESSAGE, "zero sequence"}},
            {"I11", {"E101", "", INVALID_SEQUENCE_NUMBER, SEQUENCE_MESSAGE, "sequence too high"}},
            {"I12", {"B21", "", INVALID_SEQUENCE_NUMBER, SEQUENCE_MESSAGE, "sequence too high"}},
            {"I13", {"F11", "", INVALID_SEQUENCE_NUMBER, SEQUENCE_MESSAGE, "sequence too high"}},
            {"I14", {"E-01", "", INVALID_FORMAT, FORMAT_MESSAGE, "negative sequence"}}
        };
    }

    // Additional test methods for getters
    void testGetters(const std::string& value, char expectedClass, int expectedSequence) {
        auto result = SeatNumber::create(value, _seatLayout);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result->getClassCode(), expectedClass);
        EXPECT_EQ(result->getSequenceNumber(), expectedSequence);
    }
};

// Test all valid cases
TEST_F(SeatNumberTest, ValidSeatNumbers) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(SeatNumberTest, InvalidSeatNumbers) {
    runAllInvalidTests();
}

// Test getters
TEST_F(SeatNumberTest, Getters) {
    // Test Economy class
    testGetters("E001", 'E', 1);
    testGetters("E100", 'E', 100);
    testGetters("E050", 'E', 50);

    // Test Business class
    testGetters("B01", 'B', 1);
    testGetters("B20", 'B', 20);
    testGetters("B10", 'B', 10);

    // Test First class
    testGetters("F01", 'F', 1);
    testGetters("F10", 'F', 10);
    testGetters("F05", 'F', 5);
}

// Test value object operations
TEST_F(SeatNumberTest, ValueObjectOperations) {
    auto result1 = SeatNumber::create("E001", _seatLayout);
    auto result2 = SeatNumber::create("E001", _seatLayout);
    auto result3 = SeatNumber::create("E002", _seatLayout);

    ASSERT_TRUE(result1.has_value() && result2.has_value() && result3.has_value());

    // Test equality
    EXPECT_TRUE(*result1 == *result2);
    EXPECT_FALSE(*result1 == *result3);

    // Test inequality
    EXPECT_FALSE(*result1 != *result2);
    EXPECT_TRUE(*result1 != *result3);
} 