#include <gtest/gtest.h>
#include "../../../core/value_objects/route/Route.h"
#include "BaseValueObjectTest.h"

class RouteTest : public BaseValueObjectTest<Route> {
protected:
    // Constants for error codes
    const std::string EMPTY_ROUTE = "EMPTY_ROUTE";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    const std::string INVALID_ORIGIN_CODE = "INVALID_ORIGIN_CODE";
    const std::string INVALID_DESTINATION_CODE = "INVALID_DESTINATION_CODE";
    const std::string SAME_ORIGIN_DESTINATION = "SAME_ORIGIN_DESTINATION";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "route: Route cannot be empty";
    const std::string FORMAT_MESSAGE = "route: Route must be in format 'ORIGIN(ORIGIN_CODE)-DESTINATION(DESTINATION_CODE)'";
    const std::string ORIGIN_CODE_MESSAGE = "route: Origin code must be a valid 3-letter IATA code";
    const std::string DESTINATION_CODE_MESSAGE = "route: Destination code must be a valid 3-letter IATA code";
    const std::string SAME_ORIGIN_DESTINATION_MESSAGE = "route: Origin and destination cannot be the same";

    // Implement virtual methods
    Result<Route> createValueObject(const std::string& value) override {
        return Route::create(value);
    }

    std::string getValue(const Route& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "route";

        // Setup valid test cases
        validCases = {
            // Basic valid cases
            {"V1", {"Ho Chi Minh City(SGN)-Ha Noi(HAN)", 
                    "Ho Chi Minh City(SGN)-Ha Noi(HAN)", "", "", "basic valid case"}},
            {"V2", {"Da Nang(DAD)-Hai Phong(HPH)", 
                    "Da Nang(DAD)-Hai Phong(HPH)", "", "", "another valid case"}},
            
            // Different city names
            {"V3", {"New York(JFK)-London(LHR)", 
                    "New York(JFK)-London(LHR)", "", "", "international route"}},
            {"V4", {"Tokyo(HND)-Seoul(ICN)", 
                    "Tokyo(HND)-Seoul(ICN)", "", "", "Asian route"}},
            {"V5", {"Paris(CDG)-Berlin(BER)", 
                    "Paris(CDG)-Berlin(BER)", "", "", "European route"}},
            
            // Edge cases
            {"E1", {"A(AAB)-B(BBB)", "A(AAB)-B(BBB)", "", "", "minimal city names"}},
            {"E2", {"Very Long City Name(VLC)-Another Long City Name(ALC)", 
                    "Very Long City Name(VLC)-Another Long City Name(ALC)", "", "", "long city names"}},
            {"E3", {"City1(AAA)-City2(BBB)", 
                    "City1(AAA)-City2(BBB)", "", "", "city names with numbers"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_ROUTE, EMPTY_MESSAGE, "empty string"}},
            
            // Invalid format
            {"I2", {"Ho Chi Minh City-SGN-Ha Noi-HAN", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing parentheses"}},
            {"I3", {"Ho Chi Minh City[SGN]-Ha Noi[HAN]", "", INVALID_FORMAT, FORMAT_MESSAGE, "wrong brackets"}},
            {"I4", {"Ho Chi Minh City(SGN)Ha Noi(HAN)", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing dash"}},
            {"I5", {"(SGN)-Ha Noi(HAN)", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing origin city"}},
            {"I6", {"Ho Chi Minh City(SGN)-", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing destination"}},
            
            // Invalid airport codes
            {"I7", {"Ho Chi Minh City(SG)-Ha Noi(HAN)", "", INVALID_ORIGIN_CODE, ORIGIN_CODE_MESSAGE, "short origin code"}},
            {"I8", {"Ho Chi Minh City(SGNN)-Ha Noi(HAN)", "", INVALID_ORIGIN_CODE, ORIGIN_CODE_MESSAGE, "long origin code"}},
            {"I9", {"Ho Chi Minh City(123)-Ha Noi(HAN)", "", INVALID_ORIGIN_CODE, ORIGIN_CODE_MESSAGE, "numeric origin code"}},
            {"I10", {"Ho Chi Minh City(SGN)-Ha Noi(HA)", "", INVALID_DESTINATION_CODE, DESTINATION_CODE_MESSAGE, "short destination code"}},
            {"I11", {"Ho Chi Minh City(SGN)-Ha Noi(HANN)", "", INVALID_DESTINATION_CODE, DESTINATION_CODE_MESSAGE, "long destination code"}},
            {"I12", {"Ho Chi Minh City(SGN)-Ha Noi(123)", "", INVALID_DESTINATION_CODE, DESTINATION_CODE_MESSAGE, "numeric destination code"}},
            
            // Same origin and destination
            {"I13", {"Ho Chi Minh City(SGN)-Ho Chi Minh City(SGN)", "", 
                    SAME_ORIGIN_DESTINATION, SAME_ORIGIN_DESTINATION_MESSAGE, "same city and code"}},
            {"I14", {"Ha Noi(HAN)-Ha Noi(HAN)", "", 
                    SAME_ORIGIN_DESTINATION, SAME_ORIGIN_DESTINATION_MESSAGE, "same city and code"}}
        };
    }

    // Additional test methods for create with individual parameters
    void testCreateWithIndividualParams(
        const std::string& origin,
        const std::string& originCode,
        const std::string& destination,
        const std::string& destinationCode,
        bool shouldSucceed,
        const std::string& expectedErrorCode = "",
        const std::string& expectedErrorMessage = "") {
        auto result = Route::create(origin, originCode, destination, destinationCode);
        if (shouldSucceed) {
            ASSERT_TRUE(result.has_value()) 
                << "Failed to create Route with origin: " << origin 
                << ", originCode: " << originCode
                << ", destination: " << destination
                << ", destinationCode: " << destinationCode;
            EXPECT_EQ(result->getOrigin(), origin);
            EXPECT_EQ(result->getOriginCode(), originCode);
            EXPECT_EQ(result->getDestination(), destination);
            EXPECT_EQ(result->getDestinationCode(), destinationCode);
        } else {
            ASSERT_FALSE(result.has_value());
            EXPECT_EQ(result.error().code, expectedErrorCode);
            EXPECT_EQ(result.error().message, expectedErrorMessage);
        }
    }
};

// Test all valid cases
TEST_F(RouteTest, ValidRoutes) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(RouteTest, InvalidRoutes) {
    runAllInvalidTests();
}

// Test create with individual parameters
TEST_F(RouteTest, CreateWithIndividualParams) {
    // Valid cases
    testCreateWithIndividualParams(
        "Ho Chi Minh City",
        "SGN",
        "Ha Noi",
        "HAN",
        true
    );

    testCreateWithIndividualParams(
        "Da Nang",
        "DAD",
        "Hai Phong",
        "HPH",
        true
    );

    // Invalid cases - Empty fields
    testCreateWithIndividualParams(
        "",
        "SGN",
        "Ha Noi",
        "HAN",
        false,
        EMPTY_ROUTE,
        EMPTY_MESSAGE
    );

    testCreateWithIndividualParams(
        "Ho Chi Minh City",
        "",
        "Ha Noi",
        "HAN",
        false,
        EMPTY_ROUTE,
        EMPTY_MESSAGE
    );

    // Invalid cases - Invalid airport codes
    testCreateWithIndividualParams(
        "Ho Chi Minh City",
        "SG",
        "Ha Noi",
        "HAN",
        false,
        INVALID_ORIGIN_CODE,
        ORIGIN_CODE_MESSAGE
    );

    testCreateWithIndividualParams(
        "Ho Chi Minh City",
        "SGN",
        "Ha Noi",
        "HA",
        false,
        INVALID_DESTINATION_CODE,
        DESTINATION_CODE_MESSAGE
    );

    // Invalid cases - Same origin and destination
    testCreateWithIndividualParams(
        "Ho Chi Minh City",
        "SGN",
        "Ho Chi Minh City",
        "SGN",
        false,
        SAME_ORIGIN_DESTINATION,
        SAME_ORIGIN_DESTINATION_MESSAGE
    );
}

// Test formatting variations
TEST_F(RouteTest, FormattingVariations) {
    auto result = Route::create("Ho Chi Minh City", "SGN", "Ha Noi", "HAN");
    ASSERT_TRUE(result.has_value());
    const Route& route = *result;

    // Test default format
    EXPECT_EQ(route.toString(), "Ho Chi Minh City(SGN)-Ha Noi(HAN)");

    // Test getters
    EXPECT_EQ(route.getOrigin(), "Ho Chi Minh City");
    EXPECT_EQ(route.getOriginCode(), "SGN");
    EXPECT_EQ(route.getDestination(), "Ha Noi");
    EXPECT_EQ(route.getDestinationCode(), "HAN");
}

// Test value object operations
TEST_F(RouteTest, ValueObjectOperations) {
    auto result1 = Route::create("Ho Chi Minh City", "SGN", "Ha Noi", "HAN");
    auto result2 = Route::create("Ho Chi Minh City", "SGN", "Ha Noi", "HAN");
    auto result3 = Route::create("Da Nang", "DAD", "Hai Phong", "HPH");

    ASSERT_TRUE(result1.has_value() && result2.has_value() && result3.has_value());

    // Test equality
    EXPECT_TRUE(*result1 == *result2);
    EXPECT_FALSE(*result1 == *result3);

    // Test inequality
    EXPECT_FALSE(*result1 != *result2);
    EXPECT_TRUE(*result1 != *result3);
} 