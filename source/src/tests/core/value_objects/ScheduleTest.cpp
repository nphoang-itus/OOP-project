#include <gtest/gtest.h>
#include "../../../core/value_objects/schedule/Schedule.h"
#include "BaseValueObjectTest.h"

class ScheduleTest : public BaseValueObjectTest<Schedule> {
protected:
    // Constants for error codes
    const std::string EMPTY_SCHEDULE = "EMPTY_SCHEDULE";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    const std::string INVALID_DEPARTURE_TIME = "INVALID_DEPARTURE_TIME";
    const std::string INVALID_ARRIVAL_TIME = "INVALID_ARRIVAL_TIME";
    const std::string ARRIVAL_BEFORE_DEPARTURE = "ARRIVAL_BEFORE_DEPARTURE";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "schedule: Schedule cannot be empty";
    const std::string FORMAT_MESSAGE = "schedule: Schedule must be in format 'YYYY-MM-DD HH:mm'";
    const std::string DEPARTURE_TIME_MESSAGE = "schedule: Invalid departure time";
    const std::string ARRIVAL_TIME_MESSAGE = "schedule: Invalid arrival time";
    const std::string ARRIVAL_MESSAGE = "schedule: Arrival time must be after departure time";

    // Implement virtual methods
    Result<Schedule> createValueObject(const std::string& value) override {
        return Schedule::create(value);
    }

    std::string getValue(const Schedule& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "schedule";

        // Setup valid test cases
        validCases = {
            // Basic valid cases
            {"V1", {"2024-03-20 10:00|2024-03-20 12:00", 
                    "2024-03-20 10:00|2024-03-20 12:00", "", "", "basic valid case"}},
            {"V2", {"2024-03-20 10:00|2024-03-21 10:00", 
                    "2024-03-20 10:00|2024-03-21 10:00", "", "", "next day arrival"}},
            
            // Different time formats
            {"V3", {"2024-03-20 00:00|2024-03-20 23:59", 
                    "2024-03-20 00:00|2024-03-20 23:59", "", "", "full day range"}},
            {"V4", {"2024-03-20 10:30|2024-03-20 11:45", 
                    "2024-03-20 10:30|2024-03-20 11:45", "", "", "partial hours"}},
            {"V5", {"2024-03-20 10:00|2024-03-20 10:01", 
                    "2024-03-20 10:00|2024-03-20 10:01", "", "", "one minute difference"}},
            
            // Edge cases
            {"E1", {"2024-02-29 10:00|2024-02-29 12:00", 
                    "2024-02-29 10:00|2024-02-29 12:00", "", "", "leap year"}},
            {"E2", {"2024-12-31 23:59|2025-01-01 00:00", 
                    "2024-12-31 23:59|2025-01-01 00:00", "", "", "year change"}},
            {"E3", {"2024-03-20 10:00|2024-03-20 10:01", 
                    "2024-03-20 10:00|2024-03-20 10:01", "", "", "one minute difference"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_SCHEDULE, EMPTY_MESSAGE, "empty string"}},
            
            // Invalid format
            {"I2", {"2024-03-20 10:00", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing arrival time"}},
            {"I3", {"2024-03-20 10:00;2024-03-20 12:00", "", INVALID_FORMAT, FORMAT_MESSAGE, "wrong separator"}},
            // {"I6", {"2024-03-20 10:00||2024-03-20 12:00", "", INVALID_FORMAT, FORMAT_MESSAGE, "double separator"}},
            
            // Invalid time format
            {"I7", {"2024-13-20 10:00|2024-03-20 12:00", "", INVALID_DEPARTURE_TIME, DEPARTURE_TIME_MESSAGE, "invalid month"}},
            {"I8", {"2024-03-32 10:00|2024-03-20 12:00", "", INVALID_DEPARTURE_TIME, DEPARTURE_TIME_MESSAGE, "invalid day"}},
            {"I9", {"2024-03-20 25:00|2024-03-20 12:00", "", INVALID_DEPARTURE_TIME, DEPARTURE_TIME_MESSAGE, "invalid hour"}},
            {"I5", {"|2024-03-20 12:00", "", INVALID_DEPARTURE_TIME, DEPARTURE_TIME_MESSAGE, "leading separator"}},
            {"I10", {"2024-03-20 10:60|2024-03-20 12:00", "", INVALID_DEPARTURE_TIME, DEPARTURE_TIME_MESSAGE, "invalid minute"}},
            {"I11", {"2024-03-20 10:00|2024-13-20 12:00", "", INVALID_ARRIVAL_TIME, ARRIVAL_TIME_MESSAGE, "invalid arrival month"}},
            {"I12", {"2024-03-20 10:00|2024-03-32 12:00", "", INVALID_ARRIVAL_TIME, ARRIVAL_TIME_MESSAGE, "invalid arrival day"}},
            {"I13", {"2024-03-20 10:00|2024-03-20 25:00", "", INVALID_ARRIVAL_TIME, ARRIVAL_TIME_MESSAGE, "invalid arrival hour"}},
            {"I14", {"2024-03-20 10:00|2024-03-20 10:60", "", INVALID_ARRIVAL_TIME, ARRIVAL_TIME_MESSAGE, "invalid arrival minute"}},
            {"I4", {"2024-03-20 10:00|", "", INVALID_ARRIVAL_TIME, ARRIVAL_TIME_MESSAGE, "trailing separator"}},
            
            // Arrival before departure
            {"I15", {"2024-03-20 12:00|2024-03-20 10:00", "", 
                    ARRIVAL_BEFORE_DEPARTURE, ARRIVAL_MESSAGE, "arrival before departure"}},
            {"I16", {"2024-03-21 10:00|2024-03-20 12:00", "", 
                    ARRIVAL_BEFORE_DEPARTURE, ARRIVAL_MESSAGE, "arrival day before departure"}},
            {"I17", {"2024-03-20 10:00|2024-03-20 10:00", "", 
                    ARRIVAL_BEFORE_DEPARTURE, ARRIVAL_MESSAGE, "same time"}}
        };
    }

    // Additional test methods for create with individual parameters
    void testCreateWithIndividualParams(
        const std::tm& departure,
        const std::tm& arrival,
        bool shouldSucceed,
        const std::string& expectedErrorCode = "",
        const std::string& expectedErrorMessage = "") {
        auto result = Schedule::create(departure, arrival);
        if (shouldSucceed) {
            ASSERT_TRUE(result.has_value()) 
                << "Failed to create Schedule with departure: " << std::asctime(&departure)
                << ", arrival: " << std::asctime(&arrival);
            EXPECT_EQ(result->getDeparture().tm_year, departure.tm_year);
            EXPECT_EQ(result->getDeparture().tm_mon, departure.tm_mon);
            EXPECT_EQ(result->getDeparture().tm_mday, departure.tm_mday);
            EXPECT_EQ(result->getDeparture().tm_hour, departure.tm_hour);
            EXPECT_EQ(result->getDeparture().tm_min, departure.tm_min);
            EXPECT_EQ(result->getArrival().tm_year, arrival.tm_year);
            EXPECT_EQ(result->getArrival().tm_mon, arrival.tm_mon);
            EXPECT_EQ(result->getArrival().tm_mday, arrival.tm_mday);
            EXPECT_EQ(result->getArrival().tm_hour, arrival.tm_hour);
            EXPECT_EQ(result->getArrival().tm_min, arrival.tm_min);
        } else {
            ASSERT_FALSE(result.has_value());
            EXPECT_EQ(result.error().code, expectedErrorCode);
            EXPECT_EQ(result.error().message, expectedErrorMessage);
        }
    }
};

// Test all valid cases
TEST_F(ScheduleTest, ValidSchedules) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(ScheduleTest, InvalidSchedules) {
    runAllInvalidTests();
}

// Test create with individual parameters
TEST_F(ScheduleTest, CreateWithIndividualParams) {
    // Valid cases
    std::tm departure1 = {0, 0, 10, 20, 2, 124}; // 2024-03-20 10:00
    std::tm arrival1 = {0, 0, 12, 20, 2, 124};  // 2024-03-20 12:00
    testCreateWithIndividualParams(departure1, arrival1, true);

    std::tm departure2 = {0, 0, 10, 20, 2, 124}; // 2024-03-20 10:00
    std::tm arrival2 = {0, 0, 10, 21, 2, 124};  // 2024-03-21 10:00
    testCreateWithIndividualParams(departure2, arrival2, true);

    // Invalid cases - Arrival before departure
    std::tm departure3 = {0, 0, 12, 20, 2, 124}; // 2024-03-20 12:00
    std::tm arrival3 = {0, 0, 10, 20, 2, 124};  // 2024-03-20 10:00
    testCreateWithIndividualParams(departure3, arrival3, false, 
        ARRIVAL_BEFORE_DEPARTURE, ARRIVAL_MESSAGE);

    // Invalid cases - Same time
    std::tm departure4 = {0, 0, 10, 20, 2, 124}; // 2024-03-20 10:00
    std::tm arrival4 = {0, 0, 10, 20, 2, 124};  // 2024-03-20 10:00
    testCreateWithIndividualParams(departure4, arrival4, false, 
        ARRIVAL_BEFORE_DEPARTURE, ARRIVAL_MESSAGE);
}

// Test create with two string parameters
TEST_F(ScheduleTest, CreateWithTwoStringParams) {
    // Valid cases
    auto result1 = Schedule::create("2024-03-20 10:00", "2024-03-20 12:00");
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->toString(), "2024-03-20 10:00|2024-03-20 12:00");

    auto result2 = Schedule::create("2024-03-20 10:00", "2024-03-21 10:00");
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->toString(), "2024-03-20 10:00|2024-03-21 10:00");

    // Invalid cases - Empty strings
    auto result3 = Schedule::create("", "2024-03-20 12:00");
    ASSERT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error().code, INVALID_DEPARTURE_TIME);
    EXPECT_EQ(result3.error().message, DEPARTURE_TIME_MESSAGE);

    auto result4 = Schedule::create("2024-03-20 10:00", "");
    ASSERT_FALSE(result4.has_value());
    EXPECT_EQ(result4.error().code, INVALID_ARRIVAL_TIME);
    EXPECT_EQ(result4.error().message, ARRIVAL_TIME_MESSAGE);

    // Invalid cases - Invalid format
    auto result5 = Schedule::create("2024-13-20 10:00", "2024-03-20 12:00");
    ASSERT_FALSE(result5.has_value());
    EXPECT_EQ(result5.error().code, INVALID_DEPARTURE_TIME);
    EXPECT_EQ(result5.error().message, DEPARTURE_TIME_MESSAGE);

    auto result6 = Schedule::create("2024-03-20 10:00", "2024-03-20 25:00");
    ASSERT_FALSE(result6.has_value());
    EXPECT_EQ(result6.error().code, INVALID_ARRIVAL_TIME);
    EXPECT_EQ(result6.error().message, ARRIVAL_TIME_MESSAGE);

    // Invalid cases - Arrival before departure
    auto result7 = Schedule::create("2024-03-20 12:00", "2024-03-20 10:00");
    ASSERT_FALSE(result7.has_value());
    EXPECT_EQ(result7.error().code, ARRIVAL_BEFORE_DEPARTURE);
    EXPECT_EQ(result7.error().message, ARRIVAL_MESSAGE);

    auto result8 = Schedule::create("2024-03-21 10:00", "2024-03-20 12:00");
    ASSERT_FALSE(result8.has_value());
    EXPECT_EQ(result8.error().code, ARRIVAL_BEFORE_DEPARTURE);
    EXPECT_EQ(result8.error().message, ARRIVAL_MESSAGE);

    // Invalid cases - Same time
    auto result9 = Schedule::create("2024-03-20 10:00", "2024-03-20 10:00");
    ASSERT_FALSE(result9.has_value());
    EXPECT_EQ(result9.error().code, ARRIVAL_BEFORE_DEPARTURE);
    EXPECT_EQ(result9.error().message, ARRIVAL_MESSAGE);
}