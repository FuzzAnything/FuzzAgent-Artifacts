#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <cfloat>
#include "cjson/cJSON.h"

namespace harness_028 {
/*
 * harness_028.cpp - Advanced number parsing logic coverage testing
 *                   targeting parse_number function with 48 blocked branches
 * 
 * This harness targets the 48/54 blocked branches in parse_number with focus on:
 * 1) Scientific notation: 1e10, -2.5e-3, 0e0, +1.23E+45, -9.87e-67
 * 2) Decimal precision edge cases: .0, 0., 0.0, 123.4567890123456789
 * 3) Infinity and NaN representations: Infinity, -Infinity, NaN
 * 4) Number format boundaries: INT_MAX, INT_MIN, overflow/underflow values
 * 5) Whitespace handling around numbers: leading/trailing spaces, tabs, newlines
 * 
 * Public APIs targeted:
 * - cJSON_ParseWithLengthOpts (Primary)
 * - cJSON_Parse
 * - cJSON_ParseWithOpts
 * 
 * Semantic differentiation from existing harnesses:
 * - Systematic generation of scientific notation formats
 * - Explicit testing of decimal point edge cases
 * - Infinity and NaN string representations as JSON numbers
 * - Boundary value testing for integer overflow/underflow
 * - Comprehensive whitespace variations around numbers
 * 
 * Coverage gaps specifically targeted in parse_number:
 * 1) switch cases for +, -, e, E, . characters (lines 329-353)
 * 2) has_decimal_point logic and locale replacement (lines 366-376)
 * 3) strtod conversion failure paths (lines 379-384)
 * 4) Integer overflow/underflow saturation (lines 389-400)
 * 5) Allocation failure paths in number_c_string allocation (lines 357-361)
 * 6) Input buffer validation (lines 317-320)
 */


extern "C" {
}

// Helper to generate scientific notation numbers
static std::string generate_scientific_notation(FuzzedDataProvider& fdp) {
    std::string result;
    
    // Consume pattern type
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (pattern_type) {
        case 0:
            // Positive number with positive exponent: 1e10
            result = std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1000)) + 
                     "e" + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 100));
            break;
        case 1:
            // Negative number with negative exponent: -2.5e-3
            result = "-" + std::to_string(fdp.ConsumeFloatingPointInRange<double>(0.1, 999.9)) + 
                     "e-" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 100));
            break;
        case 2:
            // Zero exponent: 0e0
            result = "0e0";
            break;
        case 3:
            // Capital E: 1.23E+45
            result = std::to_string(fdp.ConsumeFloatingPointInRange<double>(0.1, 999.9)) + 
                     "E+" + std::to_string(fdp.ConsumeIntegralInRange<int>(10, 100));
            break;
        case 4:
            // Multiple decimal places with exponent: 123.456e-78
            result = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999)) + "." +
                     std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999)) + 
                     "e-" + std::to_string(fdp.ConsumeIntegralInRange<int>(10, 99));
            break;
        case 5:
            // Just exponent without mantissa decimal: 123e456
            result = std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999)) + 
                     "e" + std::to_string(fdp.ConsumeIntegralInRange<int>(100, 1000));
            break;
        case 6:
            // Decimal with no integer part: .123e45
            result = "." + std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999)) + 
                     "e" + std::to_string(fdp.ConsumeIntegralInRange<int>(10, 99));
            break;
        case 7:
            // Integer with no decimal: 123e-45
            result = std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999)) + 
                     "e-" + std::to_string(fdp.ConsumeIntegralInRange<int>(10, 99));
            break;
    }
    
    return result;
}

// Helper to generate decimal precision edge cases
static std::string generate_decimal_edge_case(FuzzedDataProvider& fdp) {
    std::string result;
    
    uint8_t edge_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (edge_type) {
        case 0:
            // .0 (decimal point only with zero)
            result = ".0";
            break;
        case 1:
            // 0. (zero with decimal point only)
            result = "0.";
            break;
        case 2:
            // 0.0 (zero with decimal point and zero)
            result = "0.0";
            break;
        case 3:
            // Many decimal places
            result = "123." + std::string(fdp.ConsumeIntegralInRange<size_t>(5, 20), '9');
            break;
        case 4:
            // Very small decimal
            result = "0." + std::to_string(fdp.ConsumeIntegralInRange<int>(100000, 999999));
            break;
        case 5:
            // Mixed decimal
            result = std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99)) + "." +
                     std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99));
            break;
    }
    
    return result;
}

// Helper to generate special number representations
static std::string generate_special_number(FuzzedDataProvider& fdp) {
    std::string result;
    
    uint8_t special_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (special_type) {
        case 0:
            result = "Infinity";
            break;
        case 1:
            result = "-Infinity";
            break;
        case 2:
            result = "NaN";
            break;
        case 3:
            result = "-NaN";
            break;
    }
    
    return result;
}

// Helper to generate boundary values
static std::string generate_boundary_value(FuzzedDataProvider& fdp) {
    std::string result;
    
    uint8_t boundary_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (boundary_type) {
        case 0:
            // INT_MAX boundary
            result = std::to_string(INT_MAX);
            break;
        case 1:
            // INT_MIN boundary
            result = std::to_string(INT_MIN);
            break;
        case 2:
            // Just above INT_MAX
            result = std::to_string(static_cast<long long>(INT_MAX) + 1);
            break;
        case 3:
            // Just below INT_MIN
            result = std::to_string(static_cast<long long>(INT_MIN) - 1);
            break;
        case 4:
            // Very large positive
            result = "1" + std::string(fdp.ConsumeIntegralInRange<size_t>(10, 20), '0');
            break;
    }
    
    return result;
}

// Helper to add whitespace around number
static std::string add_whitespace(const std::string& number, FuzzedDataProvider& fdp) {
    std::string result;
    
    // Add leading whitespace
    uint8_t leading_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (leading_type) {
        case 0:
            result += " ";  // space
            break;
        case 1:
            result += "\t"; // tab
            break;
        case 2:
            result += "\n"; // newline
            break;
        case 3:
            result += "  \t\n "; // mixed
            break;
    }
    
    // Add the number
    result += number;
    
    // Add trailing whitespace
    uint8_t trailing_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (trailing_type) {
        case 0:
            result += " ";
            break;
        case 1:
            result += "\t";
            break;
        case 2:
            result += "\n";
            break;
        case 3:
            result += " \t\n ";
            break;
    }
    
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for number generation and parsing options
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parsing method selection
    uint8_t parse_method = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume require_null_terminated flag
    bool require_null_terminated = fdp.ConsumeBool();
    
    // Consume number generation strategy
    uint8_t number_strategy = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    std::string number_string;
    
    // Generate number based on strategy
    switch (number_strategy) {
        case 0:
            number_string = generate_scientific_notation(fdp);
            break;
        case 1:
            number_string = generate_decimal_edge_case(fdp);
            break;
        case 2:
            number_string = generate_special_number(fdp);
            break;
        case 3:
            number_string = generate_boundary_value(fdp);
            break;
        case 4:
            // Simple random number
            if (fdp.ConsumeBool()) {
                number_string = std::to_string(fdp.ConsumeIntegral<int>());
            } else {
                number_string = std::to_string(fdp.ConsumeFloatingPoint<double>());
            }
            break;
    }
    
    // Add optional whitespace
    if (fdp.ConsumeBool()) {
        number_string = add_whitespace(number_string, fdp);
    }
    
    // Create JSON string with the number
    std::string json_string = "{\"number\": " + number_string + "}";
    
    cJSON* json_obj = nullptr;
    const char* parse_end = nullptr;
    
    // Parse using selected method
    switch (parse_method) {
        case 0:
            // cJSON_Parse - basic parsing
            json_obj = cJSON_Parse(json_string.c_str());
            break;
            
        case 1:
            // cJSON_ParseWithOpts - parsing with options
            json_obj = cJSON_ParseWithOpts(json_string.c_str(), &parse_end, require_null_terminated);
            break;
            
        case 2:
            // cJSON_ParseWithLengthOpts - parsing with length and options (primary target)
            json_obj = cJSON_ParseWithLengthOpts(json_string.c_str(), json_string.size(), 
                                                 &parse_end, require_null_terminated);
            break;
    }
    
    // If parsing succeeded, extract and validate the number
    if (json_obj != nullptr) {
        // Get the number item
        cJSON* number_item = cJSON_GetObjectItem(json_obj, "number");
        
        if (number_item != nullptr && cJSON_IsNumber(number_item)) {
            // Test number properties
            double value = number_item->valuedouble;
            int int_value = number_item->valueint;
            
            // Test various number operations
            (void)value;
            (void)int_value;
            
            // Test cJSON_SetNumberHelper
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                double new_value = fdp.ConsumeFloatingPoint<double>();
                cJSON_SetNumberHelper(number_item, new_value);
            }
            
            // Test cJSON_IsNumber validation
            (void)cJSON_IsNumber(number_item);
            
            // Test printing the number
            char* printed = cJSON_Print(number_item);
            if (printed != nullptr) {
                free(printed);
            }
            
            // Test printing the whole object
            char* full_printed = cJSON_Print(json_obj);
            if (full_printed != nullptr) {
                free(full_printed);
            }
        }
        
        // Clean up
        cJSON_Delete(json_obj);
    }
    
    // Also test parsing standalone numbers (not in object)
    if (fdp.remaining_bytes() > 4) {
        // Create standalone number test
        uint8_t standalone_test = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        std::string standalone_number;
        switch (standalone_test) {
            case 0:
                standalone_number = generate_scientific_notation(fdp);
                break;
            case 1:
                standalone_number = number_string; // reuse previously generated
                break;
            case 2:
                standalone_number = std::to_string(fdp.ConsumeIntegral<int>());
                break;
            case 3:
                standalone_number = std::to_string(fdp.ConsumeFloatingPoint<double>());
                break;
        }
        
        // Parse standalone number
        cJSON* standalone_obj = cJSON_Parse(standalone_number.c_str());
        if (standalone_obj != nullptr) {
            if (cJSON_IsNumber(standalone_obj)) {
                // Test number extraction
                double val = standalone_obj->valuedouble;
                int ival = standalone_obj->valueint;
                (void)val;
                (void)ival;
            }
            cJSON_Delete(standalone_obj);
        }
    }
    
    return 0;
}

} // namespace harness_028
