#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <limits>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_008 {
/*
 * Grammar-aware fuzzing harness for cJSON parse_number function
 * 
 * Targets specific edge cases identified in coverage analysis:
 * - NULL input buffer validation (0 hits)
 * - Memory allocation failure paths
 * - Complex scientific notation edge cases  
 * - Extreme number boundary conditions
 * - Locale-specific decimal point handling
 * 
 * parse_number has 50/54 branches blocked (91 hits) with core number
 * parsing logic unexplored. This harness uses structured grammar-based
 * input generation rather than random mutations.
 */



// Note: parse_number is a static function in cJSON.c, not directly callable
// We test it through cJSON_Parse and related functions
enum NumberGrammarType {
    GRAMMAR_INTEGER = 0,
    GRAMMAR_FRACTION = 1,
    GRAMMAR_EXPONENT = 2,
    GRAMMAR_NEGATIVE = 3,
    GRAMMAR_POSITIVE_EXPONENT = 4,
    GRAMMAR_NEGATIVE_EXPONENT = 5,
    GRAMMAR_DECIMAL_ONLY = 6,
    GRAMMAR_EXPONENT_ONLY = 7,
    GRAMMAR_INFINITY_LIKE = 8,
    GRAMMAR_NAN_LIKE = 9,
    GRAMMAR_LEADING_ZEROS = 10,
    GRAMMAR_TRAILING_DOT = 11,
    GRAMMAR_DOUBLE_DOT = 12,
    GRAMMAR_MULTIPLE_SIGNS = 13,
    GRAMMAR_MULTIPLE_EXPONENTS = 14,
    GRAMMAR_EXTREME_BOUNDARIES = 15,
    GRAMMAR_LOCALE_DECIMAL = 16,
    GRAMMAR_EMPTY = 17,
    GRAMMAR_NULL_TERMINATED = 18
};

// Generate number string based on grammar type
std::string generate_number_string(FuzzedDataProvider &fdp, NumberGrammarType type) {
    switch (type) {
        case GRAMMAR_INTEGER: {
            int64_t val = fdp.ConsumeIntegral<int64_t>();
            return std::to_string(val);
        }
        
        case GRAMMAR_FRACTION: {
            double val = fdp.ConsumeFloatingPoint<double>();
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%.15f", val);
            return buffer;
        }
        
        case GRAMMAR_EXPONENT: {
            double base = fdp.ConsumeFloatingPointInRange<double>(-1e6, 1e6);
            int exponent = fdp.ConsumeIntegralInRange<int>(-308, 308);
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "%.15e%d", base, exponent);
            return buffer;
        }
        
        case GRAMMAR_NEGATIVE: {
            double val = fdp.ConsumeFloatingPoint<double>();
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "-%.15f", val < 0 ? -val : val);
            return buffer;
        }
        
        case GRAMMAR_POSITIVE_EXPONENT: {
            double base = fdp.ConsumeFloatingPointInRange<double>(0.0, 1e6);
            int exponent = fdp.ConsumeIntegralInRange<int>(0, 308);
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "%.15e+%d", base, exponent);
            return buffer;
        }
        
        case GRAMMAR_NEGATIVE_EXPONENT: {
            double base = fdp.ConsumeFloatingPointInRange<double>(0.0, 1e6);
            int exponent = fdp.ConsumeIntegralInRange<int>(-308, 0);
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "%.15e%d", base, exponent);
            return buffer;
        }
        
        case GRAMMAR_DECIMAL_ONLY: {
            // Just ".123" or similar
            if (fdp.ConsumeBool()) {
                return "." + std::to_string(fdp.ConsumeIntegral<int>());
            } else {
                return "-." + std::to_string(fdp.ConsumeIntegral<int>());
            }
        }
        
        case GRAMMAR_EXPONENT_ONLY: {
            // "e10" or similar
            int exponent = fdp.ConsumeIntegralInRange<int>(-308, 308);
            char buffer[64];
            if (fdp.ConsumeBool()) {
                snprintf(buffer, sizeof(buffer), "e%d", exponent);
            } else {
                snprintf(buffer, sizeof(buffer), "E%d", exponent);
            }
            return buffer;
        }
        
        case GRAMMAR_INFINITY_LIKE: {
            // Strings that look like infinity representations
            const char* patterns[] = {
                "Infinity", "INF", "inf", "INFINITY", "1.#INF", "-1.#INF",
                "1e999", "-1e999", "1e9999", "-1e9999"
            };
            return patterns[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(patterns)/sizeof(patterns[0]) - 1)];
        }
        
        case GRAMMAR_NAN_LIKE: {
            // Strings that look like NaN representations
            const char* patterns[] = {
                "NaN", "nan", "NAN", "1.#QNAN", "-1.#IND", "1.#SNAN",
                "0.0/0.0", "-0.0/0.0", "inf/inf"
            };
            return patterns[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(patterns)/sizeof(patterns[0]) - 1)];
        }
        
        case GRAMMAR_LEADING_ZEROS: {
            int val = fdp.ConsumeIntegral<int>();
            int zeros = fdp.ConsumeIntegralInRange<int>(1, 10);
            std::string result(zeros, '0');
            result += std::to_string(val);
            if (fdp.ConsumeBool()) {
                result += "." + std::to_string(fdp.ConsumeIntegral<int>());
            }
            return result;
        }
        
        case GRAMMAR_TRAILING_DOT: {
            int val = fdp.ConsumeIntegral<int>();
            return std::to_string(val) + ".";
        }
        
        case GRAMMAR_DOUBLE_DOT: {
            int val1 = fdp.ConsumeIntegral<int>();
            int val2 = fdp.ConsumeIntegral<int>();
            return std::to_string(val1) + ".." + std::to_string(val2);
        }
        
        case GRAMMAR_MULTIPLE_SIGNS: {
            int signs = fdp.ConsumeIntegralInRange<int>(2, 5);
            std::string result;
            for (int i = 0; i < signs; i++) {
                result += fdp.ConsumeBool() ? "+" : "-";
            }
            result += std::to_string(fdp.ConsumeIntegral<int>());
            return result;
        }
        
        case GRAMMAR_MULTIPLE_EXPONENTS: {
            double val = fdp.ConsumeFloatingPoint<double>();
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "%.15fe10e5", val);
            return buffer;
        }
        
        case GRAMMAR_EXTREME_BOUNDARIES: {
            const char* extremes[] = {
                "1.7976931348623157e+308",  // DBL_MAX
                "-1.7976931348623157e+308", // -DBL_MAX
                "2.2250738585072014e-308",  // DBL_MIN
                "-2.2250738585072014e-308", // -DBL_MIN
                "4.9406564584124654e-324",  // DBL_TRUE_MIN
                "1e-323",                   // Near underflow
                "9.999999999999999e307",    // Near overflow
                "2147483647",               // INT_MAX
                "-2147483648",              // INT_MIN
                "9223372036854775807",      // LLONG_MAX
                "-9223372036854775808",     // LLONG_MIN
            };
            return extremes[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(extremes)/sizeof(extremes[0]) - 1)];
        }
        
        case GRAMMAR_LOCALE_DECIMAL: {
            // Test with comma as decimal separator (some locales)
            int whole = fdp.ConsumeIntegral<int>();
            int frac = fdp.ConsumeIntegral<int>();
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%d,%03d", whole, frac);
            return buffer;
        }
        
        case GRAMMAR_EMPTY:
            return "";
            
        case GRAMMAR_NULL_TERMINATED: {
            // Valid number but test with extra null bytes
            int val = fdp.ConsumeIntegral<int>();
            std::string result = std::to_string(val);
            // Insert null bytes
            size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, result.length());
            result.insert(pos, 1, '\0');
            return result;
        }
        
        default:
            return "0";
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Choose grammar type based on input
    NumberGrammarType grammar_type = static_cast<NumberGrammarType>(
        fdp.ConsumeIntegralInRange<int>(0, GRAMMAR_NULL_TERMINATED));
    // Generate number string using grammar rules
    std::string number_str = generate_number_string(fdp, grammar_type);
    // Test 1: Parse number through cJSON_Parse with different formats
    if (!number_str.empty() && number_str.length() < 1024) {
        // Test as standalone number
        cJSON* num = cJSON_Parse(number_str.c_str());
        if (num) {
            cJSON_Delete(num);
        }
        
        // Test number in JSON object
        std::string json_obj = "{\"value\": " + number_str + "}";
        cJSON* obj = cJSON_Parse(json_obj.c_str());
        if (obj) {
            cJSON_Delete(obj);
        }
        
        // Test number in JSON array
        std::string json_array = "[" + number_str + "]";
        cJSON* arr = cJSON_Parse(json_array.c_str());
        if (arr) {
            cJSON_Delete(arr);
        }
        
        // Test with cJSON_ParseWithLength
        cJSON* num_len = cJSON_ParseWithLength(number_str.c_str(), number_str.length());
        if (num_len) {
            cJSON_Delete(num_len);
        }
        
        // Test with cJSON_ParseWithOpts
        const char* parse_end = nullptr;
        cJSON* num_opts = cJSON_ParseWithOpts(number_str.c_str(), &parse_end, 0);
        if (num_opts) {
            cJSON_Delete(num_opts);
        }
    }
    
    // Test 2: NULL input buffer validation (0 hits in coverage)
    // Test cJSON_Parse with NULL input
    cJSON_Parse(nullptr);
    
    // Test cJSON_ParseWithLength with NULL input
    cJSON_ParseWithLength(nullptr, 0);
    cJSON_ParseWithLength("test", 0);  // Zero length
    
    // Test cJSON_ParseWithOpts with NULL input
    const char* parse_end = nullptr;
    cJSON_ParseWithOpts(nullptr, &parse_end, 0);
    
    // Test 3: Memory allocation failure paths
    // We can't directly control malloc failures, but we can test with
    // very large numbers that might trigger allocation issues
    if (fdp.ConsumeBool() && !number_str.empty() && number_str.length() < 4096) {
        // Create a very large number string to stress allocation
        std::string large_number = "0.";
        for (int i = 0; i < 1000; i++) {
            large_number += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
        }
        large_number += "e" + std::to_string(fdp.ConsumeIntegralInRange<int>(-308, 308));
        
        cJSON* large_num = cJSON_Parse(large_number.c_str());
        if (large_num) {
            cJSON_Delete(large_num);
        }
    }
    
    // Test 4: Extreme boundary cases
    // Test with extreme values from grammar
    if (grammar_type == GRAMMAR_EXTREME_BOUNDARIES) {
        // The extreme values are already in number_str
        cJSON* extreme = cJSON_Parse(number_str.c_str());
        if (extreme) {
            cJSON_Delete(extreme);
        }
        
        // Also test cJSON_SetNumberHelper with extreme values
        cJSON object;
        memset(&object, 0, sizeof(object));
        object.type = cJSON_Number;
        
        double test_value = fdp.ConsumeFloatingPoint<double>();
        cJSON_SetNumberHelper(&object, test_value);
        
        // Test with NULL object (should return NAN according to code)
        cJSON_SetNumberHelper(nullptr, test_value);
    }
    
    // Test 5: Locale-specific decimal testing
    // parse_number uses get_decimal_point() internally
    {
        // Test with different decimal representations
        std::string decimal_test;
        if (fdp.ConsumeBool()) {
            decimal_test = "3.14159";  // Standard decimal point
        } else {
            decimal_test = "3,14159";  // European style (might fail but tests locale logic)
        }
        
        cJSON* decimal_num = cJSON_Parse(decimal_test.c_str());
        if (decimal_num) {
            cJSON_Delete(decimal_num);
        }
        
        // Test cJSON_GetNumberValue on parsed numbers
        if (!number_str.empty()) {
            cJSON* test_num = cJSON_Parse(number_str.c_str());
            if (test_num) {
                double val = cJSON_GetNumberValue(test_num);
                (void)val; // Use value to avoid unused variable warning
                cJSON_Delete(test_num);
            }
        }
    }
    
    // Test 6: Additional number-related APIs
    {
        // Test cJSON_CreateNumber
        double create_val = fdp.ConsumeFloatingPoint<double>();
        cJSON* created = cJSON_CreateNumber(create_val);
        if (created) {
            // Test cJSON_GetNumberValue on created number
            double retrieved = cJSON_GetNumberValue(created);
            (void)retrieved;
            
            // Test cJSON_IsNumber
            if (cJSON_IsNumber(created)) {
                // Valid number
            }
            
            cJSON_Delete(created);
        }
        
        // Test cJSON_AddNumberToObject
        cJSON* root = cJSON_CreateObject();
        if (root) {
            double add_val = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(root, "test_key", add_val);
            cJSON_Delete(root);
        }
    }
    
    // Test 7: Test malformed number parsing
    {
        // Generate intentionally malformed numbers
        std::string malformed;
        uint8_t malform_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (malform_type) {
            case 0:
                malformed = "123abc";  // Trailing non-numeric
                break;
            case 1:
                malformed = "--123";   // Double negative
                break;
            case 2:
                malformed = "123..456"; // Double decimal
                break;
            case 3:
                malformed = "123e";    // Exponent without value
                break;
            case 4:
                malformed = "e123";    // No base before exponent
                break;
            case 5:
                malformed = "";        // Empty string
                break;
        }
        
        cJSON* malformed_num = cJSON_Parse(malformed.c_str());
        if (malformed_num) {
            cJSON_Delete(malformed_num);
        }
    }
    
    return 0;
}

} // namespace harness_008
