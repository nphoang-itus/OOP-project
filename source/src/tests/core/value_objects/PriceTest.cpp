#include <gtest/gtest.h>
#include "../../../core/value_objects/price/Price.h"
#include "BaseValueObjectTest.h"

class PriceTest : public BaseValueObjectTest<Price> {
protected:
    // Constants for error codes
    const std::string EMPTY_PRICE = "EMPTY_PRICE";
    const std::string INVALID_FORMAT = "INVALID_FORMAT";
    const std::string INVALID_CURRENCY = "INVALID_CURRENCY";
    const std::string INVALID_AMOUNT = "INVALID_AMOUNT";
    const std::string NEGATIVE_AMOUNT = "NEGATIVE_AMOUNT";
    
    // Constants for error messages
    const std::string EMPTY_MESSAGE = "price: Price cannot be empty";
    const std::string FORMAT_MESSAGE = "price: Price must be in format 'AMOUNT CURRENCY' (amount can use . or , as decimal separator)";
    const std::string CURRENCY_MESSAGE = "price: Currency must be one of: CAD, KRW, CNY, JPY, GBP, USD, AUD, SGD, EUR, VND";
    const std::string AMOUNT_MESSAGE = "price: Amount must be a valid number";
    const std::string NEGATIVE_MESSAGE = "price: Amount cannot be negative";

    // Implement virtual methods
    Result<Price> createValueObject(const std::string& value) override {
        return Price::create(value);
    }

    std::string getValue(const Price& obj) override {
        return obj.toString();
    }

    void setupTestCases() override {
        fieldName = "price";

        // Setup valid test cases
        validCases = {
            // Test cases without decimal
            {"V1", {"100 VND", "100.00 VND", "", "", "valid price without decimal"}},
            {"V2", {"1000 USD", "1000.00 USD", "", "", "valid price without decimal"}},
            
            // Test cases with dot decimal
            {"V3", {"100.00 VND", "100.00 VND", "", "", "valid price with dot decimal"}},
            {"V4", {"1000.50 USD", "1000.50 USD", "", "", "valid price with dot decimal"}},
            {"V5", {"0.00 EUR", "0.00 EUR", "", "", "valid price with zero amount"}},
            {"V6", {"999999.99 GBP", "999999.99 GBP", "", "", "valid price with large amount"}},
            {"V7", {"0.01 JPY", "0.01 JPY", "", "", "valid price with small amount"}},
            
            // Test cases with comma decimal
            {"V8", {"100,00 VND", "100.00 VND", "", "", "valid price with comma decimal"}},
            {"V9", {"1000,50 USD", "1000.50 USD", "", "", "valid price with comma decimal"}},
            {"V10", {"0,00 EUR", "0.00 EUR", "", "", "valid price with zero amount and comma"}},
            {"V11", {"999999,99 GBP", "999999.99 GBP", "", "", "valid price with large amount and comma"}},
            {"V12", {"0,01 JPY", "0.01 JPY", "", "", "valid price with small amount and comma"}},
            
            // Test cases with different currencies
            {"V13", {"100.00 vnd", "100.00 VND", "", "", "valid price with lowercase currency"}},
            {"V14", {"100,00 vnd", "100.00 VND", "", "", "valid price with lowercase currency and comma"}}
        };

        // Setup invalid test cases
        invalidCases = {
            // Empty input
            {"I1", {"", "", EMPTY_PRICE, EMPTY_MESSAGE, "empty string"}},
            
            // Invalid format
            {"I2", {"100VND", "", INVALID_FORMAT, FORMAT_MESSAGE, "missing space"}},
            {"I3", {"100 VND USD", "", INVALID_FORMAT, FORMAT_MESSAGE, "multiple currencies"}},
            {"I4", {"100.00 VND ", "", INVALID_FORMAT, FORMAT_MESSAGE, "trailing space"}},
            {"I5", {" 100.00 VND", "", INVALID_FORMAT, FORMAT_MESSAGE, "leading space"}},
            {"I6", {"100.00.00 VND", "", INVALID_FORMAT, FORMAT_MESSAGE, "multiple dots"}},
            {"I7", {"100,00,00 VND", "", INVALID_FORMAT, FORMAT_MESSAGE, "multiple commas"}},
            {"I8", {"100.00,00 VND", "", INVALID_FORMAT, FORMAT_MESSAGE, "mixed separators"}},
            {"I12", {"abc VND", "", INVALID_FORMAT, FORMAT_MESSAGE, "non-numeric amount"}},
            {"I13", {"1e2 VND", "", INVALID_FORMAT, FORMAT_MESSAGE, "scientific notation"}},
            
            // Invalid currency
            {"I9", {"100.00 XXX", "", INVALID_CURRENCY, CURRENCY_MESSAGE, "invalid currency code"}},
            {"I10", {"100.00 VN", "", INVALID_CURRENCY, CURRENCY_MESSAGE, "invalid currency length"}},
            {"I11", {"100.00 VNDD", "", INVALID_CURRENCY, CURRENCY_MESSAGE, "invalid currency length"}},
            
            // Invalid amount
            
            // Negative amount
            {"I14", {"-100.00 VND", "", NEGATIVE_AMOUNT, NEGATIVE_MESSAGE, "negative amount with dot"}},
            {"I15", {"-100,00 VND", "", NEGATIVE_AMOUNT, NEGATIVE_MESSAGE, "negative amount with comma"}},
            {"I16", {"-0.01 VND", "", NEGATIVE_AMOUNT, NEGATIVE_MESSAGE, "small negative amount with dot"}},
            {"I17", {"-0,01 VND", "", NEGATIVE_AMOUNT, NEGATIVE_MESSAGE, "small negative amount with comma"}}
        };
    }
};

// Test all valid cases
TEST_F(PriceTest, ValidPrices) {
    runAllValidTests();
}

// Test all invalid cases
TEST_F(PriceTest, InvalidPrices) {
    runAllInvalidTests();
}

// Test value object operations
TEST_F(PriceTest, ValueObjectOperations) {
    // Test equality with different decimal separators
    auto result1 = Price::create("100.00 VND");
    auto result2 = Price::create("100,00 VND");
    auto result3 = Price::create("200.00 VND");

    ASSERT_TRUE(result1.has_value() && result2.has_value() && result3.has_value());

    // Test equality
    EXPECT_TRUE(*result1 == *result2);  // Should be equal despite different decimal separators
    EXPECT_FALSE(*result1 == *result3);

    // Test inequality
    EXPECT_FALSE(*result1 != *result2);
    EXPECT_TRUE(*result1 != *result3);

    // Test with different currencies
    auto result4 = Price::create("100.00 vnd");
    auto result5 = Price::create("100,00 vnd");
    ASSERT_TRUE(result4.has_value() && result5.has_value());
    EXPECT_TRUE(*result4 == *result5);  // Should be equal despite case and decimal separator differences
}

// Test getters
TEST_F(PriceTest, Getters) {
    // Test with dot decimal
    auto result1 = Price::create("100.50 VND");
    ASSERT_TRUE(result1.has_value());
    EXPECT_DOUBLE_EQ(result1->getAmount(), 100.50);
    EXPECT_EQ(result1->getCurrency(), "VND");
    EXPECT_EQ(result1->getCurrencyName().value(), "Vietnamese Dong");

    // Test with comma decimal
    auto result2 = Price::create("100,50 VND");
    ASSERT_TRUE(result2.has_value());
    EXPECT_DOUBLE_EQ(result2->getAmount(), 100.50);
    EXPECT_EQ(result2->getCurrency(), "VND");
    EXPECT_EQ(result2->getCurrencyName().value(), "Vietnamese Dong");

    // Test without decimal
    auto result3 = Price::create("100 VND");
    ASSERT_TRUE(result3.has_value());
    EXPECT_DOUBLE_EQ(result3->getAmount(), 100.00);
    EXPECT_EQ(result3->getCurrency(), "VND");
    EXPECT_EQ(result3->getCurrencyName().value(), "Vietnamese Dong");
}

// Test create with separate parameters
TEST_F(PriceTest, CreateWithSeparateParams) {
    // Test with different amounts
    auto result1 = Price::create(100.50, "VND");
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->toString(), "100.50 VND");

    auto result2 = Price::create(0.00, "USD");
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->toString(), "0.00 USD");

    auto result3 = Price::create(999999.99, "EUR");
    ASSERT_TRUE(result3.has_value());
    EXPECT_EQ(result3->toString(), "999999.99 EUR");
}

// Test formatting
TEST_F(PriceTest, Formatting) {
    // Test with dot decimal
    auto result1 = Price::create("100.50 VND");
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->toString(), "100.50 VND");

    // Test with comma decimal
    auto result2 = Price::create("100,50 VND");
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->toString(), "100.50 VND");

    // Test without decimal
    auto result3 = Price::create("100 VND");
    ASSERT_TRUE(result3.has_value());
    EXPECT_EQ(result3->toString(), "100.00 VND");

    // Test with lowercase currency
    auto result4 = Price::create("100.50 vnd");
    ASSERT_TRUE(result4.has_value());
    EXPECT_EQ(result4->toString(), "100.50 VND");
} 