#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_022 {

extern "C" {
}

// Helper function to generate scientific notation strings
std::string generate_scientific_notation(FuzzedDataProvider& fdp) {
    uint8_t variant = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (variant) {
        case 0: // 1.23e+10 style
            return std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                   std::string(fdp.ConsumeBool() ? "e" : "E") + 
                   std::string(fdp.ConsumeBool() ? "+" : "") + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(-100, 100));
        
        case 1: // -4.56E-7 style
            return std::string("-") + std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                   std::string(fdp.ConsumeBool() ? "e" : "E") + std::string("-") + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(1, 100));
        
        case 2: // 0e0 style
            return std::string("0") + std::string(fdp.ConsumeBool() ? "e" : "E") + "0";
        
        case 3: // 1e999 style (large exponent)
            return std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                   std::string(fdp.ConsumeBool() ? "e" : "E") + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(300, 999));
        
        case 4: // Scientific with multiple signs
            {
                std::string result = std::string(fdp.ConsumeBool() ? "-" : "") + 
                                     std::to_string(fdp.ConsumeFloatingPoint<double>());
                result += std::string(fdp.ConsumeBool() ? "e" : "E");
                result += std::string(fdp.ConsumeBool() ? "+" : "-");
                result += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 100));
                return result;
            }
        
        case 5: // Scientific with no sign on exponent
            return std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                   std::string(fdp.ConsumeBool() ? "e" : "E") + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(-100, 100));
        
        case 6: // Very small scientific notation
            return std::to_string(fdp.ConsumeFloatingPointInRange<double>(1e-100, 1e-10)) + 
                   std::string(fdp.ConsumeBool() ? "e" : "E") + std::string("-") + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(100, 308));
        
        case 7: // Scientific with integer base
            return std::to_string(fdp.ConsumeIntegral<int64_t>()) + 
                   std::string(fdp.ConsumeBool() ? "e" : "E") + 
                   std::string(fdp.ConsumeBool() ? "+" : "-") + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 100));
    }
    return "0";
}

// Helper function to generate edge case numbers
std::string generate_edge_case_number(FuzzedDataProvider& fdp) {
    uint8_t edge_case = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    switch (edge_case) {
        case 0: // INT_MAX
            return std::to_string(std::numeric_limits<int>::max());
        
        case 1: // INT_MIN
            return std::to_string(std::numeric_limits<int>::min());
        
        case 2: // Overflow candidate (beyond int range)
            return std::to_string(static_cast<int64_t>(std::numeric_limits<int>::max()) + 
                                 fdp.ConsumeIntegralInRange<int64_t>(1, 1000000));
        
        case 3: // Underflow candidate
            return std::to_string(static_cast<int64_t>(std::numeric_limits<int>::min()) - 
                                 fdp.ConsumeIntegralInRange<int64_t>(1, 1000000));
        
        case 4: // Double limits
            return std::to_string(std::numeric_limits<double>::max());
        
        case 5: // Double min positive
            return std::to_string(std::numeric_limits<double>::min());
        
        case 6: // Infinity representation
            return fdp.ConsumeBool() ? "inf" : (fdp.ConsumeBool() ? "Infinity" : "INF");
        
        case 7: // NaN representation
            return fdp.ConsumeBool() ? "nan" : (fdp.ConsumeBool() ? "NaN" : "NAN");
        
        case 8: // Very large integer
            return std::to_string(fdp.ConsumeIntegral<int64_t>());
        
        case 9: // Very small number
            return std::to_string(fdp.ConsumeFloatingPointInRange<double>(-1e-100, 1e-100));
    }
    return "0";
}

// Helper function to generate malformed numbers
std::string generate_malformed_number(FuzzedDataProvider& fdp) {
    uint8_t malformed_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (malformed_type) {
        case 0: // 123. (trailing decimal)
            return std::to_string(fdp.ConsumeIntegral<int>()) + ".";
        
        case 1: // .456 (leading decimal)
            return "." + std::to_string(fdp.ConsumeIntegral<int>());
        
        case 2: // ++123
            return "++" + std::to_string(fdp.ConsumeIntegral<int>());
        
        case 3: // --456
            return "--" + std::to_string(fdp.ConsumeIntegral<int>());
        
        case 4: // 1.2.3 (multiple decimals)
            return std::to_string(fdp.ConsumeIntegral<int>()) + "." + 
                   std::to_string(fdp.ConsumeIntegral<int>()) + "." + 
                   std::to_string(fdp.ConsumeIntegral<int>());
        
        case 5: // e without number
            return std::string(fdp.ConsumeBool() ? "e" : "E") + 
                   std::to_string(fdp.ConsumeIntegral<int>());
        
        case 6: // number with e but no exponent
            return std::to_string(fdp.ConsumeIntegral<int>()) + 
                   std::string(fdp.ConsumeBool() ? "e" : "E");
        
        case 7: // Mixed signs
            return std::string(fdp.ConsumeBool() ? "+" : "-") + 
                   std::string(fdp.ConsumeBool() ? "+" : "-") + 
                   std::to_string(fdp.ConsumeIntegral<int>());
    }
    return "0";
}

// Helper function to generate locale-sensitive decimal representations
std::string generate_locale_sensitive_number(FuzzedDataProvider& fdp) {
    uint8_t locale_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (locale_type) {
        case 0: // Standard decimal point
            return std::to_string(fdp.ConsumeIntegral<int>()) + "." + 
                   std::to_string(fdp.ConsumeIntegral<int>());
        
        case 1: // Comma as decimal separator (should fail or be locale-dependent)
            return std::to_string(fdp.ConsumeIntegral<int>()) + "," + 
                   std::to_string(fdp.ConsumeIntegral<int>());
        
        case 2: // Multiple decimal points with commas
            return std::to_string(fdp.ConsumeIntegral<int>()) + ".123,456";
        
        case 3: // Thousand separators (commas)
            return "1,234.56";
        
        case 4: // Thousand separators (periods)
            return "1.234,56";
        
        case 5: // Mixed separators
            return "1,234.567,89";
    }
    return "0";
}

// Helper function to generate boundary value numbers
std::string generate_boundary_number(FuzzedDataProvider& fdp) {
    uint8_t boundary_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (boundary_type) {
        case 0: // Very large positive number
            return std::to_string(fdp.ConsumeFloatingPointInRange<double>(1e100, 1e308));
        
        case 1: // Very small positive number
            return std::to_string(fdp.ConsumeFloatingPointInRange<double>(1e-308, 1e-100));
        
        case 2: // Very large negative number
            return std::string("-") + std::to_string(fdp.ConsumeFloatingPointInRange<double>(1e100, 1e308));
        
        case 3: // Very small negative number
            return std::string("-") + std::to_string(fdp.ConsumeFloatingPointInRange<double>(1e-308, 1e-100));
        
        case 4: // Number with many decimal places
            {
                std::string int_part = std::to_string(fdp.ConsumeIntegral<int>());
                std::string frac_part;
                int decimal_digits = fdp.ConsumeIntegralInRange<int>(10, 50);
                for (int i = 0; i < decimal_digits; i++) {
                    frac_part += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                }
                return int_part + "." + frac_part;
            }
        
        case 5: // Number with leading zeros
            return std::string(fdp.ConsumeBool() ? "-" : "") + 
                   "0" + std::to_string(fdp.ConsumeIntegral<int>()) + 
                   "." + std::to_string(fdp.ConsumeIntegral<int>());
        
        case 6: // Number with trailing zeros
            return std::to_string(fdp.ConsumeIntegral<int>()) + 
                   "." + std::to_string(fdp.ConsumeIntegral<int>()) + 
                   "0000";
        
        case 7: // Binary/octal/hex representation (invalid in JSON)
            {
                uint8_t base = fdp.ConsumeIntegral<uint8_t>() % 3;
                switch (base) {
                    case 0: return "0b" + std::to_string(fdp.ConsumeIntegral<int>());
                    case 1: return "0" + std::to_string(fdp.ConsumeIntegral<int>()); // octal
                    case 2: return "0x" + std::to_string(fdp.ConsumeIntegral<int>());
                }
                return "0";
            }
    }
    return "0";
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need sufficient input for comprehensive number testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate a collection of test numbers from different categories
    std::vector<std::string> test_numbers;
    
    // Determine how many numbers to generate from each category
    int scientific_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    int edge_case_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    int malformed_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    int locale_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    int boundary_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    // Generate scientific notation variants
    for (int i = 0; i < scientific_count && fdp.remaining_bytes() > 0; i++) {
        test_numbers.push_back(generate_scientific_notation(fdp));
    }
    
    // Generate edge case numbers
    for (int i = 0; i < edge_case_count && fdp.remaining_bytes() > 0; i++) {
        test_numbers.push_back(generate_edge_case_number(fdp));
    }
    
    // Generate malformed numbers
    for (int i = 0; i < malformed_count && fdp.remaining_bytes() > 0; i++) {
        test_numbers.push_back(generate_malformed_number(fdp));
    }
    
    // Generate locale-sensitive numbers
    for (int i = 0; i < locale_count && fdp.remaining_bytes() > 0; i++) {
        test_numbers.push_back(generate_locale_sensitive_number(fdp));
    }
    
    // Generate boundary value numbers
    for (int i = 0; i < boundary_count && fdp.remaining_bytes() > 0; i++) {
        test_numbers.push_back(generate_boundary_number(fdp));
    }
    
    // Also generate some random numbers directly from fuzzer input
    int random_count = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < random_count && fdp.remaining_bytes() > 0; i++) {
        uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (num_type) {
            case 0:
                test_numbers.push_back(std::to_string(fdp.ConsumeIntegral<int64_t>()));
                break;
            case 1:
                test_numbers.push_back(std::to_string(fdp.ConsumeFloatingPoint<double>()));
                break;
            case 2:
                test_numbers.push_back(fdp.ConsumeRandomLengthString(50));
                break;
            case 3:
                test_numbers.push_back(std::string(fdp.ConsumeBool() ? "-" : "") + 
                                       std::to_string(fdp.ConsumeIntegral<int>()) + 
                                       std::string(fdp.ConsumeBool() ? "." : "") + 
                                       std::string(fdp.ConsumeBool() ? std::to_string(fdp.ConsumeIntegral<int>()) : ""));
                break;
        }
    }
    
    // Test 1: Parse individual numbers as standalone JSON values
    for (const auto& num_str : test_numbers) {
        // Create a simple JSON string with the number
        std::string json_str = num_str;
        
        // Use cJSON_Parse to trigger parse_number
        cJSON* parsed = cJSON_Parse(json_str.c_str());
        if (parsed != NULL) {
            // If parsing succeeded, check the type and do some basic validation
            if (cJSON_IsNumber(parsed)) {
                // Access the number values to exercise the data access paths
                double dval = parsed->valuedouble;
                int ival = parsed->valueint;
                (void)dval; // Mark as used to avoid warnings
                (void)ival; // Mark as used to avoid warnings
            }
            cJSON_Delete(parsed);
        }
    }
    
    // Test 2: Parse numbers in array context
    if (!test_numbers.empty()) {
        std::string array_json = "[";
        for (size_t i = 0; i < test_numbers.size(); i++) {
            if (i > 0) array_json += ", ";
            array_json += test_numbers[i];
        }
        array_json += "]";
        
        cJSON* array_parsed = cJSON_Parse(array_json.c_str());
        if (array_parsed != NULL) {
            cJSON_Delete(array_parsed);
        }
    }
    
    // Test 3: Parse numbers in object context
    if (!test_numbers.empty()) {
        std::string object_json = "{";
        for (size_t i = 0; i < test_numbers.size() && i < 10; i++) {
            if (i > 0) object_json += ", ";
            object_json += "\"key" + std::to_string(i) + "\": " + test_numbers[i];
        }
        object_json += "}";
        
        cJSON* object_parsed = cJSON_Parse(object_json.c_str());
        if (object_parsed != NULL) {
            cJSON_Delete(object_parsed);
        }
    }
    
    // Test 4: Use cJSON_ParseWithOpts for more control
    for (const auto& num_str : test_numbers) {
        const char* endptr = nullptr;
        cJSON* parsed_with_opts = cJSON_ParseWithOpts(num_str.c_str(), &endptr, 1);
        if (parsed_with_opts != NULL) {
            cJSON_Delete(parsed_with_opts);
        }
    }
    
    // Test 5: Create numbers using cJSON_CreateNumber and related functions
    // to compare with parsed numbers
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
        double test_val = fdp.ConsumeFloatingPoint<double>();
        cJSON* created_num = cJSON_CreateNumber(test_val);
        if (created_num != NULL) {
            // Test cJSON_SetNumberHelper
            double new_val = fdp.ConsumeFloatingPoint<double>();
            cJSON_SetNumberHelper(created_num, new_val);
            
            cJSON_Delete(created_num);
        }
    }
    
    return 0;
}

} // namespace harness_022
