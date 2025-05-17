#include <gtest/gtest.h>
#include "../../../core/value_objects/seat_class_map/SeatClassMap.h"
#include "BaseValueObjectTest.h"

class SeatClassMapTest : public BaseValueObjectTest<SeatClassMap> {
protected:
    // Constants for error codes
    const std::string EMPTY_MAP = "EMPTY_SEAT_CLASS_MAP";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    const std::string INVALID_SEAT_TYPE = "INVALID_SEAT_TYPE";
    const std::string INVALID_SEAT_COUNT = "INVALID_SEAT_COUNT";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "seatClassMap: Seat class map cannot be empty";
    const std::string FORMAT_MESSAGE = "seatClassMap: Seat class map must be in format 'CLASS_NAME:SEAT_COUNT,CLASS_NAME:SEAT_COUNT,...'";
    const std::string SEAT_TYPE_MESSAGE = "seatClassMap: Seat type name must be one of: ECONOMY, BUSINESS, FIRST";
    const std::string SEAT_COUNT_MESSAGE = "seatClassMap: Seat count must be a positive number";

    // Implement virtual methods
    Result<SeatClassMap> createValueObject(const std::string& value) override {
        return SeatClassMap::create(value);
    }

    std::string getValue(const SeatClassMap& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "seat class map";

        // Setup valid test cases
        validCases = {
            // Single class
            {"V1", {"ECONOMY:100", "ECONOMY:100", "", "", "single economy class"}},
            {"V2", {"BUSINESS:50", "BUSINESS:50", "", "", "single business class"}},
            {"V3", {"FIRST:10", "FIRST:10", "", "", "single first class"}},
            
            // Multiple classes
            {"V4", {"ECONOMY:100,BUSINESS:50", "BUSINESS:50, ECONOMY:100", "", "", "two classes"}},
            {"V5", {"ECONOMY:100,BUSINESS:50,FIRST:10", "FIRST:10, BUSINESS:50, ECONOMY:100", "", "", "three classes"}},
            {"V6", {"first:10", "FIRST:10", "", "", "lowercase name type"}},
            
            // Edge cases
            {"E1", {"ECONOMY:1", "ECONOMY:1", "", "", "minimum seats"}},
            {"E2", {"ECONOMY:999", "ECONOMY:999", "", "", "large number of seats"}},
            {"E3", {"ECONOMY:100,BUSINESS:1,FIRST:1", "FIRST:1, BUSINESS:1, ECONOMY:100", "", "", "mixed seat counts"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_MAP, EMPTY_MESSAGE, "empty string"}},
            
            // Invalid format
            {"I2", {"ECONOMY100", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing colon"}},
            {"I3", {"ECONOMY:100;BUSINESS:50", "", INVALID_FORMAT, FORMAT_MESSAGE, "wrong separator"}},
            {"I4", {"ECONOMY:100,", "", INVALID_FORMAT, FORMAT_MESSAGE, "trailing comma"}},
            {"I5", {",ECONOMY:100", "", INVALID_FORMAT, FORMAT_MESSAGE, "leading comma"}},
            {"I6", {"ECONOMY:100,,BUSINESS:50", "", INVALID_FORMAT, FORMAT_MESSAGE, "double comma"}},
            {"I11", {"ECONOMY:abc", "", INVALID_FORMAT, FORMAT_MESSAGE, "non-numeric seats"}},
            {"I15", {"ECONOMY:1.5", "", INVALID_FORMAT, FORMAT_MESSAGE, "decimal seats"}},
            {"I7", {"ECONOMY1:100", "", INVALID_SEAT_TYPE, SEAT_TYPE_MESSAGE, "invalid economy class"}},
            
            // Invalid class names
            {"I8", {"BUSINESS1:50", "", INVALID_SEAT_TYPE, SEAT_TYPE_MESSAGE, "invalid business class"}},
            {"I9", {"FIRST1:10", "", INVALID_SEAT_TYPE, SEAT_TYPE_MESSAGE, "invalid first class"}},
            {"I10", {"PREMIUM:100", "", INVALID_SEAT_TYPE, SEAT_TYPE_MESSAGE, "unknown class"}},
            
            // Invalid seat counts
            {"I12", {"ECONOMY:0", "", INVALID_SEAT_COUNT, SEAT_COUNT_MESSAGE, "zero seats"}},
            {"I13", {"ECONOMY:-1", "", INVALID_SEAT_COUNT, SEAT_COUNT_MESSAGE, "negative seats"}}
        };
    }
};

// Test all valid cases
TEST_F(SeatClassMapTest, ValidSeatClassMaps) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(SeatClassMapTest, InvalidSeatClassMaps) {
    runAllInvalidTests();
}

// Test specific categories of valid cases
TEST_F(SeatClassMapTest, ValidSeatClassMapsWithSingleClass) {
    runSpecificValidTests({"V1", "V2", "V3"});
}

TEST_F(SeatClassMapTest, ValidSeatClassMapsWithMultipleClasses) {
    runSpecificValidTests({"V4", "V5"});
}

// Test specific categories of invalid cases
TEST_F(SeatClassMapTest, InvalidSeatClassMapsWithEmptyInput) {
    runSpecificInvalidTests({"I1"});
}

TEST_F(SeatClassMapTest, InvalidSeatClassMapsWithInvalidFormat) {
    runSpecificInvalidTests({"I2", "I3", "I4", "I5", "I6"});
}

TEST_F(SeatClassMapTest, InvalidSeatClassMapsWithInvalidClassName) {
    runSpecificInvalidTests({"I7", "I8", "I9", "I10", "I11"});
}

TEST_F(SeatClassMapTest, InvalidSeatClassMapsWithInvalidSeatCount) {
    runSpecificInvalidTests({"I12", "I13", "I14", "I15"});
} 