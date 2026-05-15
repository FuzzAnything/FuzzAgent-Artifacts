#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <climits>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_018 {
/*
 * Harness_018: Targeted testing of parse_number function through cJSON_Parse
 * 
 * Target: parse_number function with 49/54 branches blocked (second highest blocker)
 * Focus: Exercising parse_number through cJSON_Parse with diverse numeric inputs:
 *   1) Decimal numbers with varying precision
 *   2) Scientific notation (e.g., 1.23e-4, -5.6E+7)
 *   3) Edge cases like very large/small numbers
 *   4) Integer overflow scenarios  
 *   5) Leading/trailing zeros
 *   6) Signs (+/-)
 *   7) Invalid number formats that should fail gracefully
 * 
 * SEMANTIC DIFFERENTIATION FROM PREVIOUS HARNESSES (000-017):
 * - harness_000: General bootstrap with basic number creation via cJSON_CreateNumber
 * - harness_013: Uses cJSON_Parse for comparison testing, includes some number edge cases
 * - harness_014-017: Focus on cJSON_Compare with programmatic object creation
 * - This harness_018: SPECIFICALLY targets parse_number through diverse JSON number strings
 * 
 * Implementation strategy:
 * 1. Generate diverse number strings from fuzzer input
 * 2. Create JSON objects containing these numbers in various formats
 * 3. Parse using cJSON_Parse to exercise parse_number
 * 4. Validate parsed values match expected behavior
 * 5. Test error handling for invalid number formats
 * 
 * Novel approach benefits:
 * 1. Directly targets parse_number's 49 blocked branches
 * 2. Tests strtod integration and locale handling (decimal point replacement)
 * 3. Exercises number boundary conditions (INT_MAX, INT_MIN, overflow saturation)
 * 4. Tests scientific notation parsing paths
 * 5. Tests sign handling (+/-) and leading/trailing zero handling
 * 6. Tests invalid number format graceful failure
 */



// Helper to generate number string with various formats
static std::string generate_number_string(FuzzedDataProvider& fdp) {
    uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    switch (format_type) {
        case 0:  // Simple integer
            return std::to_string(fdp.ConsumeIntegral<int>());
            
        case 1:  // Integer with leading zeros
            return std::string(fdp.ConsumeIntegralInRange<size_t>(0, 5), '0') + 
                   std::to_string(fdp.ConsumeIntegral<int>());
            
        case 2:  // Integer with trailing zeros  
            return std::to_string(fdp.ConsumeIntegral<int>()) + 
                   std::string(fdp.ConsumeIntegralInRange<size_t>(0, 5), '0');
            
        case 3:  // Simple decimal
            return std::to_string(fdp.ConsumeIntegral<int>()) + "." + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99999));
            
        case 4:  // Decimal with leading zeros
            return std::string(fdp.ConsumeIntegralInRange<size_t>(0, 3), '0') + "." + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99999));
            
        case 5:  // Scientific notation with positive exponent
            return std::to_string(fdp.ConsumeIntegral<int>()) + "e" + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99));
            
        case 6:  // Scientific notation with negative exponent
            return std::to_string(fdp.ConsumeIntegral<int>()) + "e-" + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99));
            
        case 7:  // Scientific notation with capital E
            return std::to_string(fdp.ConsumeIntegral<int>()) + "E" + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99));
            
        case 8:  // Decimal with scientific notation
            return std::to_string(fdp.ConsumeIntegral<int>()) + "." + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 999)) + "e" + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(-99, 99));
            
        case 9:  // Very large positive number (approaching overflow)
            return std::to_string(fdp.ConsumeIntegralInRange<long long>(INT_MAX, LLONG_MAX));
            
        case 10: // Very large negative number (approaching underflow)
            return "-" + std::to_string(fdp.ConsumeIntegralInRange<long long>(INT_MAX, LLONG_MAX));
            
        case 11: // Multiple signs (test error handling)
            return std::string(fdp.ConsumeBool() ? "+" : "-") + 
                   std::string(fdp.ConsumeBool() ? "+" : "-") + 
                   std::to_string(fdp.ConsumeIntegral<int>());
        default:
            return "0";
    }
}

// Helper to generate potentially invalid number string
static std::string generate_invalid_number_string(FuzzedDataProvider& fdp) {
    uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (invalid_type) {
        case 0:  // Just a decimal point
            return ".";
            
        case 1:  // Just a sign
            return std::string(fdp.ConsumeBool() ? "+" : "-");
        case 2:  // Multiple decimal points
            return "1.2.3";
            
        case 3:  // Letter in number
            return "123a456";
            
        case 4:  // Scientific notation without exponent
            return "1.2e";
            
        case 5:  // Empty exponent
            return "1.2e+";
            
        case 6:  // Invalid characters
            return "123$456";
            
        case 7:  // Hex format (not valid in JSON)
            return "0x" + std::to_string(fdp.ConsumeIntegral<int>());
            
        default:
            return "invalid";
    }
}

// Helper to create JSON string with numbers in various contexts
static std::string create_json_with_numbers(FuzzedDataProvider& fdp, bool include_invalid = false) {
    std::ostringstream oss;
    
    uint8_t context_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (context_type) {
        case 0:  // Single number as root
            oss << generate_number_string(fdp);
            break;
            
        case 1:  // Array of numbers
            oss << "[";
            {
                size_t num_count = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                for (size_t i = 0; i < num_count; i++) {
                    if (i > 0) oss << ",";
                    oss << generate_number_string(fdp);
                    
                    // Occasionally include invalid numbers
                    if (include_invalid && fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                        oss << "," << generate_invalid_number_string(fdp);
                    }
                }
            }
            oss << "]";
            break;
            
        case 2:  // Object with number values
            oss << "{";
            {
                size_t num_count = fdp.ConsumeIntegralInRange<size_t>(1, 6);
                for (size_t i = 0; i < num_count; i++) {
                    if (i > 0) oss << ",";
                    oss << "\"key" << i << "\":" << generate_number_string(fdp);
                }
            }
            oss << "}";
            break;
            
        case 3:  // Nested structure with numbers
            oss << "{\"numbers\":[";
            {
                size_t num_count = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t i = 0; i < num_count; i++) {
                    if (i > 0) oss << ",";
                    
                    // Mix regular and scientific notation
                    if (fdp.ConsumeBool()) {
                        oss << generate_number_string(fdp);
                    } else {
                        // Force scientific notation
                        int base = fdp.ConsumeIntegralInRange<int>(1, 9);
                        int exp = fdp.ConsumeIntegralInRange<int>(-99, 99);
                        oss << base << "e" << exp;
                    }
                }
            }
            oss << "],\"metadata\":{\"count\":" << fdp.ConsumeIntegral<int>() << "}}";
            break;
            
        case 4:  // Numbers mixed with other types
            oss << "[";
            oss << "\"string\"," << fdp.ConsumeBool() << ",";
            oss << generate_number_string(fdp) << ",null,";
            oss << "{\"nested\":" << generate_number_string(fdp) << "}";
            oss << "]";
            break;
            
        case 5:  // Edge case testing structure
            oss << "{";
            oss << "\"int_max\":" << INT_MAX << ",";
            oss << "\"int_min\":" << INT_MIN << ",";
            oss << "\"large_positive\":" << fdp.ConsumeIntegralInRange<long long>(INT_MAX + 1LL, LLONG_MAX) << ",";
            oss << "\"large_negative\":" << -static_cast<long long>(fdp.ConsumeIntegralInRange<long long>(INT_MAX + 1LL, LLONG_MAX)) << ",";
            oss << "\"decimal_precision\":" << fdp.ConsumeIntegral<int>() << "." << fdp.ConsumeIntegralInRange<int>(0, 999999) << ",";
            oss << "\"scientific\":" << fdp.ConsumeIntegral<int>() << "e" << fdp.ConsumeIntegralInRange<int>(-10, 10);
            oss << "}";
            break;
    }
    
    return oss.str();
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for generating diverse number formats
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Track number of test iterations based on available input
    size_t max_iterations = std::min<size_t>(fdp.ConsumeIntegralInRange<size_t>(1, 10), 
                                            fdp.remaining_bytes() / 32);
    
    for (size_t iter = 0; iter < max_iterations && fdp.remaining_bytes() > 32; iter++) {
        // ============================================================
        // PART 1: TEST VALID NUMBER FORMATS
        // ============================================================
        
        // Generate JSON string with diverse number formats
        std::string json_str = create_json_with_numbers(fdp, false);
        
        // Parse the JSON string - this will exercise parse_number
        cJSON* parsed = cJSON_Parse(json_str.c_str());
        
        if (parsed != NULL) {
            // If parsing succeeded, validate the structure
            // This helps ensure parse_number worked correctly
            
            // For arrays, check each element
            if (cJSON_IsArray(parsed)) {
                cJSON* item = parsed->child;
                while (item != NULL) {
                    // Check if array element is a number (should be based on our generation)
                    if (cJSON_IsNumber(item)) {
                        // Access number values to ensure they were parsed correctly
                        double val_double = item->valuedouble;
                        int val_int = item->valueint;
                        
                        // Basic validation: if value is within int range, valueint should match
                        if (val_double >= INT_MIN && val_double <= INT_MAX) {
                            // Check overflow saturation logic (lines 389-400 in cJSON.c)
                            // parse_number should have set valueint correctly
                            // No assertion, just accessing to ensure no crashes
                        }
                    }
                    item = item->next;
                }
            }
            
            // For objects, check number values
            if (cJSON_IsObject(parsed)) {
                cJSON* item = parsed->child;
                while (item != NULL) {
                    if (cJSON_IsNumber(item)) {
                        // Access number values
                        double val_double = item->valuedouble;
                        int val_int = item->valueint;
                        
                        // Test cJSON_GetNumberValue helper
                        double get_val = cJSON_GetNumberValue(item);
                    }
                    item = item->next;
                }
            }
            
            // Test printing the parsed JSON (exercises print_number indirectly)
            char* printed = cJSON_Print(parsed);
            if (printed != NULL) {
                free(printed);
            }
            
            cJSON_Delete(parsed);
        }
        
        // ============================================================
        // PART 2: TEST INVALID NUMBER FORMATS (SHOULD FAIL GRACEFULLY)
        // ============================================================
        
        if (fdp.remaining_bytes() > 32) {
            // Create JSON with intentionally invalid numbers
            std::ostringstream invalid_oss;
            invalid_oss << "[";
            
            // Mix valid and invalid numbers
            size_t mix_count = fdp.ConsumeIntegralInRange<size_t>(1, 4);
            for (size_t i = 0; i < mix_count; i++) {
                if (i > 0) invalid_oss << ",";
                
                if (fdp.ConsumeBool()) {
                    invalid_oss << generate_number_string(fdp);  // Valid
                } else {
                    invalid_oss << generate_invalid_number_string(fdp);  // Invalid
                }
            }
            invalid_oss << "]";
            
            std::string invalid_json = invalid_oss.str();
            cJSON* invalid_parsed = cJSON_Parse(invalid_json.c_str());
            
            // Invalid parsing should return NULL or partial parse
            // Either outcome is acceptable for testing error paths
            if (invalid_parsed != NULL) {
                cJSON_Delete(invalid_parsed);
            }
        }
        
        // ============================================================
        // PART 3: TEST cJSON_ParseWithOpts WITH NUMBERS
        // ============================================================
        
        if (fdp.remaining_bytes() > 32) {
            // Test with parse options
            std::string opt_json = "{\"num\":" + generate_number_string(fdp) + "}";
            const char* parse_end = NULL;
            cJSON_bool require_null_terminated = fdp.ConsumeBool();
            
            cJSON* opt_parsed = cJSON_ParseWithOpts(opt_json.c_str(), &parse_end, require_null_terminated);
            
            if (opt_parsed != NULL) {
                cJSON_Delete(opt_parsed);
            }
        }
        
        // ============================================================
        // PART 4: TEST SCIENTIFIC NOTATION SPECIFICALLY
        // ============================================================
        
        if (fdp.remaining_bytes() > 32) {
            // Generate various scientific notation formats
            std::vector<std::string> sci_notations;
            
            // Positive and negative exponents
            sci_notations.push_back(std::to_string(fdp.ConsumeIntegral<int>()) + "e+" + 
                                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99)));
            sci_notations.push_back(std::to_string(fdp.ConsumeIntegral<int>()) + "e-" + 
                                   std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99)));
            sci_notations.push_back(std::to_string(fdp.ConsumeIntegral<int>()) + "E" + 
                                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99)));
            
            // Decimal with scientific notation
            sci_notations.push_back(std::to_string(fdp.ConsumeIntegral<int>()) + "." + 
                                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 999)) + "e" + 
                                   std::to_string(fdp.ConsumeIntegralInRange<int>(-99, 99)));
            
            // Very small/large exponents
            sci_notations.push_back("1e" + std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999)));
            sci_notations.push_back("1e-" + std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999)));
            
            // Test each scientific notation
            for (const auto& sci_num : sci_notations) {
                std::string sci_json = "{\"sci_num\":" + sci_num + "}";
                cJSON* sci_parsed = cJSON_Parse(sci_json.c_str());
                
                if (sci_parsed != NULL) {
                    cJSON* num_item = cJSON_GetObjectItem(sci_parsed, "sci_num");
                    if (num_item != NULL && cJSON_IsNumber(num_item)) {
                        // Verify scientific notation was parsed
                        double sci_val = num_item->valuedouble;
                    }
                    cJSON_Delete(sci_parsed);
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_018
