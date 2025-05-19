#include <gtest/gtest.h>
#include "../../../core/value_objects/contact_info/ContactInfo.h"
#include "../../../core/value_objects/contact_info/ContactInfoFormatter.h"
#include "BaseValueObjectTest.h"

class ContactInfoTest : public BaseValueObjectTest<ContactInfo> {
protected:
    // Constants for error codes
    const std::string EMPTY_EMAIL = "EMPTY_EMAIL";
    const std::string INVALID_EMAIL_FORMAT = "INVALID_EMAIL_FORMAT";
    const std::string EMAIL_TOO_LONG = "EMAIL_TOO_LONG";
    const std::string EMPTY_PHONE = "EMPTY_PHONE";
    const std::string INVALID_PHONE_FORMAT = "INVALID_PHONE_FORMAT";
    const std::string PHONE_TOO_LONG = "PHONE_TOO_LONG";
    const std::string INVALID_ADDRESS_FORMAT = "INVALID_ADDRESS_FORMAT";
    const std::string ADDRESS_TOO_LONG = "ADDRESS_TOO_LONG";
    
    // Constants for error messages
    const std::string EMPTY_EMAIL_MESSAGE = "contactInfo: Email cannot be empty";
    const std::string INVALID_EMAIL_MESSAGE = "contactInfo: Invalid email format";
    const std::string EMAIL_TOO_LONG_MESSAGE = "contactInfo: Email must not exceed 254 characters";
    const std::string EMPTY_PHONE_MESSAGE = "contactInfo: Phone number cannot be empty";
    const std::string INVALID_PHONE_MESSAGE = "contactInfo: Invalid phone number format";
    const std::string PHONE_TOO_LONG_MESSAGE = "contactInfo: Phone number must not exceed 15 digits";
    const std::string INVALID_ADDRESS_MESSAGE = "contactInfo: Invalid address format";
    const std::string ADDRESS_TOO_LONG_MESSAGE = "contactInfo: Address must not exceed 100 characters";

    // Implement virtual methods
    Result<ContactInfo> createValueObject(const std::string& value) override {
        return ContactInfo::create(value);
    }

    std::string getValue(const ContactInfo& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "contact info";

        // Setup valid test cases
        validCases = {
            // Basic valid cases
            {"V1", {"user@example.com|+84123456789|123 Street", 
                    "user@example.com|+84123456789|123 Street", "", "", "basic valid case"}},
            {"V2", {"user@example.com|+84123456789|", 
                    "user@example.com|+84123456789|", "", "", "no address"}},
            
            // Email variations
            {"V3", {"user.name@example.com|+84123456789|123 Street", 
                    "user.name@example.com|+84123456789|123 Street", "", "", "email with dot"}},
            {"V4", {"user+tag@example.com|+84123456789|123 Street", 
                    "user+tag@example.com|+84123456789|123 Street", "", "", "email with plus"}},
            {"V5", {"a@b.co|+84123456789|123 Street", 
                    "a@b.co|+84123456789|123 Street", "", "", "minimal valid email"}},
            
            // Phone variations
            {"V6", {"user@example.com|84123456789|123 Street", 
                    "user@example.com|84123456789|123 Street", "", "", "phone without plus"}},
            {"V7", {"user@example.com|+841234567890123|123 Street", 
                    "user@example.com|+841234567890123|123 Street", "", "", "15-digit phone"}},
            
            // Address variations
            {"V8", {"user@example.com|+84123456789|123 Street, City", 
                    "user@example.com|+84123456789|123 Street, City", "", "", "address with comma"}},
            {"V9", {"user@example.com|+84123456789|123 Street - City", 
                    "user@example.com|+84123456789|123 Street - City", "", "", "address with hyphen"}},
            {"V10", {"user@example.com|+84123456789|123 Street, City, Country", 
                    "user@example.com|+84123456789|123 Street, City, Country", "", "", "address with multiple parts"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Invalid email format
            {"I3", {"invalid-email|+84123456789|123 Street", "", INVALID_EMAIL_FORMAT, INVALID_EMAIL_MESSAGE, "invalid email format"}},
            {"I4", {"user@|+84123456789|123 Street", "", INVALID_EMAIL_FORMAT, INVALID_EMAIL_MESSAGE, "incomplete email"}},
            {"I5", {"@example.com|+84123456789|123 Street", "", INVALID_EMAIL_FORMAT, INVALID_EMAIL_MESSAGE, "missing local part"}},
            {"I6", {"user@|+84123456789|123 Street", "", INVALID_EMAIL_FORMAT, INVALID_EMAIL_MESSAGE, "missing domain"}},
            {"I7", {"user@example.|+84123456789|123 Street", "", INVALID_EMAIL_FORMAT, INVALID_EMAIL_MESSAGE, "incomplete domain"}},
            
            // Email length validation
            {"I8", {std::string(255, 'a') + "@example.com|+84123456789|123 Street", 
                    "", EMAIL_TOO_LONG, EMAIL_TOO_LONG_MESSAGE, "email too long"}},
            
            // Invalid phone format
            {"I9", {"user@example.com|abc123|123 Street", "", INVALID_PHONE_FORMAT, INVALID_PHONE_MESSAGE, "non-numeric phone"}},
            {"I11", {"user@example.com|1234567890123456|123 Street", "", PHONE_TOO_LONG, PHONE_TOO_LONG_MESSAGE , "too long phone"}},
            {"I10", {"user@example.com|123|123 Street", "", INVALID_PHONE_FORMAT, INVALID_PHONE_MESSAGE, "too short phone"}},
            {"I12", {"user@example.com|+1234567890123456|123 Street", "", PHONE_TOO_LONG, PHONE_TOO_LONG_MESSAGE, "phone with plus too long"}},
            
            // Invalid address format
            {"I13", {"user@example.com|+84123456789|123 Street!", "", INVALID_ADDRESS_FORMAT, INVALID_ADDRESS_MESSAGE, "invalid address characters"}},
            {"I14", {"user@example.com|+84123456789|123 Street with very long address that exceeds the maximum length allowed for addresses and contains some special characters that are not allowed in the address format", 
                    "", ADDRESS_TOO_LONG, ADDRESS_TOO_LONG_MESSAGE, "too long address"}},
            
            // Invalid format combinations
            {"I15", {"invalid-email|abc123|123 Street!", "", INVALID_EMAIL_FORMAT + ";" + INVALID_PHONE_FORMAT  + ";" + INVALID_ADDRESS_FORMAT, INVALID_EMAIL_MESSAGE + "; " + INVALID_PHONE_MESSAGE + "; " + INVALID_ADDRESS_MESSAGE, "multiple invalid fields"}}
        };
    }

    // Additional test methods for create with individual parameters
    void testCreateWithIndividualParams(
        const std::string& email, 
        const std::string& phone, 
        const std::string& address,
        bool shouldSucceed,
        const std::string& expectedErrorCode = "",
        const std::string& expectedErrorMessage = "") {
        auto result = ContactInfo::create(email, phone, address);
        if (shouldSucceed) {
            ASSERT_TRUE(result.has_value()) 
                << "Failed to create ContactInfo with email: " << email 
                << ", phone: " << phone 
                << ", address: " << address;
            EXPECT_EQ(result->getEmail(), email);
            EXPECT_EQ(result->getPhone(), phone);
            EXPECT_EQ(result->getAddress(), address);
        } else {
            ASSERT_FALSE(result.has_value());
            EXPECT_EQ(result.error().code, expectedErrorCode);
            EXPECT_EQ(result.error().message, expectedErrorMessage);
        }
    }
};

// Test all valid cases
TEST_F(ContactInfoTest, ValidContactInfo) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(ContactInfoTest, InvalidContactInfo) {
    runAllInvalidTests();
}

// Test create with individual parameters
TEST_F(ContactInfoTest, CreateWithIndividualParams) {
    // Valid cases
    testCreateWithIndividualParams(
        "user@example.com",
        "+84123456789",
        "123 Street",
        true
    );

    testCreateWithIndividualParams(
        "user.name@example.com",
        "84123456789",
        "",
        true
    );

    // Invalid cases - Empty fields
    testCreateWithIndividualParams(
        "",
        "+84123456789",
        "123 Street",
        false,
        EMPTY_EMAIL,
        EMPTY_EMAIL_MESSAGE
    );

    testCreateWithIndividualParams(
        "user@example.com",
        "",
        "123 Street",
        false,
        EMPTY_PHONE,
        EMPTY_PHONE_MESSAGE
    );

    // Invalid cases - Email validation
    testCreateWithIndividualParams(
        "invalid-email",
        "+84123456789",
        "123 Street",
        false,
        INVALID_EMAIL_FORMAT,
        INVALID_EMAIL_MESSAGE
    );

    testCreateWithIndividualParams(
        std::string(255, 'a') + "@example.com",
        "+84123456789",
        "123 Street",
        false,
        EMAIL_TOO_LONG,
        EMAIL_TOO_LONG_MESSAGE
    );

    // Invalid cases - Phone validation
    testCreateWithIndividualParams(
        "user@example.com",
        "abc123",
        "123 Street",
        false,
        INVALID_PHONE_FORMAT,
        INVALID_PHONE_MESSAGE
    );

    testCreateWithIndividualParams(
        "user@example.com",
        "+1234567890123456",
        "123 Street",
        false,
        PHONE_TOO_LONG,
        PHONE_TOO_LONG_MESSAGE
    );

    // Invalid cases - Address validation
    testCreateWithIndividualParams(
        "user@example.com",
        "+84123456789",
        "123 Street!",
        false,
        INVALID_ADDRESS_FORMAT,
        INVALID_ADDRESS_MESSAGE
    );

    testCreateWithIndividualParams(
        "user@example.com",
        "+84123456789",
        std::string(101, 'a'),
        false,
        ADDRESS_TOO_LONG,
        ADDRESS_TOO_LONG_MESSAGE
    );
}

// Test formatting variations
TEST_F(ContactInfoTest, FormattingVariations) {
    auto result = ContactInfo::create("user@example.com", "+84123456789", "123 Street");
    ASSERT_TRUE(result.has_value());
    const ContactInfo& contact = *result;

    // Test default format
    EXPECT_EQ(contact.toString(), "user@example.com|+84123456789|123 Street");

    // Test custom separator
    EXPECT_EQ(ContactInfoFormatter::toString(contact.getEmail(), contact.getPhone(), contact.getAddress(), ", "), 
              "user@example.com, +84123456789, 123 Street");

    // Test custom prefix and suffix
    EXPECT_EQ(contact.toString(), 
              "user@example.com|+84123456789|123 Street");
}

// Test value object operations
TEST_F(ContactInfoTest, ValueObjectOperations) {
    auto result1 = ContactInfo::create("user@example.com", "+84123456789", "123 Street");
    auto result2 = ContactInfo::create("user@example.com", "+84123456789", "123 Street");
    auto result3 = ContactInfo::create("other@example.com", "+84987654321", "456 Street");

    ASSERT_TRUE(result1.has_value() && result2.has_value() && result3.has_value());

    // Test equality
    EXPECT_TRUE(*result1 == *result2);
    EXPECT_FALSE(*result1 == *result3);

    // Test inequality
    EXPECT_FALSE(*result1 != *result2);
    EXPECT_TRUE(*result1 != *result3);
} 