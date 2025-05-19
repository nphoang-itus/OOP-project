#ifndef BASE_VALUE_OBJECT_TEST_H
#define BASE_VALUE_OBJECT_TEST_H

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>
#include "../../../core/exceptions/Result.h"

template<typename T>
class BaseValueObjectTest : public ::testing::Test {
protected:
    // Common test data structure
    struct TestData {
        std::string input;
        std::string expected;
        std::string errorCode;
        std::string errorMessage;
        std::string description;
    };

    // Test cases with numbering
    std::map<std::string, TestData> validCases;
    std::map<std::string, TestData> invalidCases;
    std::map<std::string, TestData> edgeCases;

    // Constants for common error codes
    const std::string EMPTY_VALUE = "EMPTY_VALUE";
    const std::string INVALID_LENGTH = "INVALID_LENGTH";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    
    // Field name for error messages
    std::string fieldName;

    // Virtual methods to be implemented by derived classes
    virtual Result<T> createValueObject(const std::string& value) = 0;
    virtual std::string getValue(const T& obj) = 0;
    virtual void setupTestCases() = 0;

    void SetUp() override {
        setupTestCases();
    }

    void TearDown() override {
        validCases.clear();
        invalidCases.clear();
        edgeCases.clear();
    }

    // Helper function to assert valid value object
    void assertValidValueObject(const std::pair<std::string, TestData>& testCase) {
        const auto& [id, testData] = testCase;
        auto result = createValueObject(testData.input);
        ASSERT_TRUE(result.has_value()) 
            << "Test case " << id << " (" << testData.description << ") failed: "
            << "Failed to create valid " << fieldName << ": " << testData.input;
        EXPECT_EQ(getValue(result.value()), testData.expected) 
            << "Test case " << id << " (" << testData.description << ") failed: "
            << "Expected " << fieldName << ": " << testData.expected 
            << ", but got: " << getValue(result.value());
    }

    // Helper function to assert invalid value object
    void assertInvalidValueObject(const std::pair<std::string, TestData>& testCase) {
        const auto& [id, testData] = testCase;
        auto result = createValueObject(testData.input);
        ASSERT_FALSE(result.has_value()) 
            << "Test case " << id << " (" << testData.description << ") failed: "
            << "Expected invalid " << fieldName << " but got valid: " << testData.input;
        EXPECT_EQ(result.error().code, testData.errorCode) 
            << "Test case " << id << " (" << testData.description << ") failed: "
            << "Expected error code: " << testData.errorCode 
            << ", but got: " << result.error().code;
        EXPECT_EQ(result.error().message, testData.errorMessage)
            << "Test case " << id << " (" << testData.description << ") failed: "
            << "Expected error message: " << testData.errorMessage 
            << ", but got: " << result.error().message;
    }

    // Helper function to run all valid test cases
    void runAllValidTests() {
        for (const auto& testCase : validCases) {
            assertValidValueObject(testCase);
        }
    }

    // Helper function to run all invalid test cases
    void runAllInvalidTests() {
        for (const auto& testCase : invalidCases) {
            assertInvalidValueObject(testCase);
        }
    }

    // Helper function to run specific valid test cases
    void runSpecificValidTests(const std::vector<std::string>& testIds) {
        for (const auto& id : testIds) {
            if (validCases.find(id) != validCases.end()) {
                assertValidValueObject({id, validCases[id]});
            }
        }
    }

    // Helper function to run specific invalid test cases
    void runSpecificInvalidTests(const std::vector<std::string>& testIds) {
        for (const auto& id : testIds) {
            if (invalidCases.find(id) != invalidCases.end()) {
                assertInvalidValueObject({id, invalidCases[id]});
            }
        }
    }
};

#endif 