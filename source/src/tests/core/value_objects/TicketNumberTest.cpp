#include <gtest/gtest.h>
#include "../../../core/value_objects/ticket_number/TicketNumber.h"
#include "BaseValueObjectTest.h"

class TicketNumberTest : public BaseValueObjectTest<TicketNumber> {
protected:
    // Constants for error codes
    const std::string EMPTY_TICKET_NUMBER = "EMPTY_TICKET_NUMBER";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "ticketNumber: Ticket number cannot be empty";
    const std::string FORMAT_MESSAGE = "ticketNumber: Ticket number must be in format 'MCB-YYYYMMDD-XXXX'";

    // Implement virtual methods
    Result<TicketNumber> createValueObject(const std::string& value) override {
        return TicketNumber::create(value);
    }

    std::string getValue(const TicketNumber& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "ticketNumber";

        // Setup valid test cases
        validCases = {
            {"V1", {"MC1-20240320-0001", "MC1-20240320-0001", "", "", "valid ticket number with 1 digit"}},
            {"V2", {"MC12-20240320-0001", "MC12-20240320-0001", "", "", "valid ticket number with 2 digits"}},
            {"V3", {"MC123-20240320-0001", "MC123-20240320-0001", "", "", "valid ticket number with 3 digits"}},
            {"V4", {"MC1234-20240320-0001", "MC1234-20240320-0001", "", "", "valid ticket number with 4 digits"}},
            {"V5", {"MC1-20240320-9999", "MC1-20240320-9999", "", "", "valid ticket number with max sequence"}},
            {"V6", {"MC1-20240320-0001", "MC1-20240320-0001", "", "", "valid ticket number with min sequence"}},
            {"V7", {"MC1-20240320-1234", "MC1-20240320-1234", "", "", "valid ticket number with middle sequence"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_TICKET_NUMBER, EMPTY_MESSAGE, "empty string"}},
            
            // Invalid format
            {"I2", {"MC12345-20240320-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "too many digits after letters"}},
            {"I3", {"MC-20240320-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "no digits after letters"}},
            {"I4", {"M1-20240320-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "only one letter"}},
            {"I5", {"MC1-2024032-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "date too short"}},
            {"I6", {"MC1-202403201-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "date too long"}},
            {"I7", {"MC1-20240320-001", "", INVALID_FORMAT, FORMAT_MESSAGE, "sequence too short"}},
            {"I8", {"MC1-20240320-00001", "", INVALID_FORMAT, FORMAT_MESSAGE, "sequence too long"}},
            {"I9", {"mc1-20240320-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "lowercase letters"}},
            {"I10", {"MC1-20240320-0001 ", "", INVALID_FORMAT, FORMAT_MESSAGE, "trailing space"}},
            {"I11", {" MC1-20240320-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "leading space"}},
            {"I12", {"MC1-20240320-0001-", "", INVALID_FORMAT, FORMAT_MESSAGE, "trailing hyphen"}},
            {"I13", {"-MC1-20240320-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "leading hyphen"}},
            {"I14", {"MC1-20240320-0001-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "extra sequence"}},
            {"I15", {"MC1-20240320-0001-0001-0001", "", INVALID_FORMAT, FORMAT_MESSAGE, "multiple extra sequences"}}
        };
    }
};

// Test all valid cases
TEST_F(TicketNumberTest, ValidTicketNumbers) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(TicketNumberTest, InvalidTicketNumbers) {
    runAllInvalidTests();
}

// Test value object operations
TEST_F(TicketNumberTest, ValueObjectOperations) {
    auto result1 = TicketNumber::create("MC1-20240320-0001");
    auto result2 = TicketNumber::create("MC1-20240320-0001");
    auto result3 = TicketNumber::create("MC1-20240320-0002");

    ASSERT_TRUE(result1.has_value() && result2.has_value() && result3.has_value());

    // Test equality
    EXPECT_TRUE(*result1 == *result2);
    EXPECT_FALSE(*result1 == *result3);

    // Test inequality
    EXPECT_FALSE(*result1 != *result2);
    EXPECT_TRUE(*result1 != *result3);
} 