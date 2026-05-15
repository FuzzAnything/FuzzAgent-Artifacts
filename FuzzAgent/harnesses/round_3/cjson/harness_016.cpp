#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <memory>

namespace harness_016 {
/*
 * Harness 016: Deep Structural Comparison Paths Targeting Remaining 73 Blocked Branches in cJSON_Compare
 * 
 * Target: Specifically focus on complex structural comparison edge cases missed by previous harnesses:
 * 1. Invalid JSON types to trigger default switch case (line 3199 with 0 hits)
 * 2. Floating-point number comparisons near epsilon using compare_double internal function
 * 3. Mixed-type arrays (arrays containing different JSON types) to test array iteration logic
 * 4. Complex object key patterns with case variations and subset/superset relationships
 * 5. Circular-like structures (without actual cycles) to test deep recursion limits
 * 6. Both parsed JSON strings and programmatically created structures
 * 7. Edge cases with NULL valuestring pointers in String and Raw types
 * 8. O(n^2) object comparison algorithm with varying object sizes and key patterns
 * 
 * Key coverage targets:
 * - Default switch case for invalid types (lines 3096-3098, 3196-3198)
 * - compare_double function paths for floating-point epsilon comparisons
 * - Array iteration with mixed types and varying lengths
 * - Object comparison symmetry checking (subset/superset relationships)
 * - Case-sensitive vs case-insensitive key matching in nested contexts
 * - Deep structural equality with parsed vs created JSON
 */


// Helper to create invalid JSON types for testing default switch case
static cJSON* create_invalid_type_json(FuzzedDataProvider& fdp, int invalid_type_bit) {
    // Create a cJSON object with invalid type bits
    // Invalid types are any bits beyond the defined cJSON type range
    cJSON* item = cJSON_CreateNull(); // Start with a valid type
    if (!item) return NULL;
    
    // Set invalid type bits (beyond 0xFF mask or unknown combinations)
    // Use fuzzer input to generate various invalid type patterns
    uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>();
    
    // Create invalid type by OR-ing with bits outside the valid range
    // Valid types are: 0x01 (False), 0x02 (True), 0x04 (NULL), 0x08 (Number),
    // 0x10 (String), 0x20 (Array), 0x40 (Object), 0x80 (Raw)
    // Any other combination or bits above 0xFF should trigger default case
    
    // Set type to an invalid value
    item->type = invalid_type;
    
    // For some tests, also set valuestring to NULL to test that path
    if (fdp.ConsumeBool()) {
        if (item->valuestring) {
            cJSON_free(item->valuestring);
            item->valuestring = NULL;
        }
    }
    
    return item;
}

// Helper to create arrays with mixed types
static cJSON* create_mixed_type_array(FuzzedDataProvider& fdp, int max_elements) {
    cJSON* array = cJSON_CreateArray();
    if (!array) return NULL;
    
    int num_elements = fdp.ConsumeIntegralInRange<int>(0, max_elements);
    if (num_elements == 0) {
        // Test empty array comparison
        return array;
    }
    
    for (int i = 0; i < num_elements; i++) {
        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        cJSON* element = NULL;
        
        switch (elem_type) {
            case 0:
                element = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                element = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
                break;
            case 2:
                element = cJSON_CreateBool(fdp.ConsumeBool());
                break;
            case 3:
                element = cJSON_CreateNull();
                break;
            case 4:
                element = cJSON_CreateArray(); // Empty nested array
                break;
            case 5:
                element = cJSON_CreateObject(); // Empty nested object
                break;
            case 6:
                element = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(64).c_str());
                break;
            case 7:
                // Create array with one element to test nested structures
                element = cJSON_CreateArray();
                if (element) {
                    cJSON* nested = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                    if (nested) {
                        cJSON_AddItemToArray(element, nested);
                    }
                }
                break;
        }
        
        if (element) {
            cJSON_AddItemToArray(array, element);
        }
    }
    
    return array;
}

// Helper to create objects with complex key patterns
static cJSON* create_complex_key_object(FuzzedDataProvider& fdp, int max_properties) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    int num_props = fdp.ConsumeIntegralInRange<int>(0, max_properties);
    
    for (int i = 0; i < num_props; i++) {
        // Generate keys with case variations
        std::string base_key = fdp.ConsumeRandomLengthString(16);
        
        // Create case variations: lowercase, uppercase, mixed case
        std::string key_lower = base_key;
        std::transform(key_lower.begin(), key_lower.end(), key_lower.begin(), ::tolower);
        
        std::string key_upper = base_key;
        std::transform(key_upper.begin(), key_upper.end(), key_upper.begin(), ::toupper);
        
        // Choose which key variation to use
        uint8_t key_variant = fdp.ConsumeIntegral<uint8_t>() % 3;
        std::string final_key;
        
        switch (key_variant) {
            case 0: final_key = key_lower; break;
            case 1: final_key = key_upper; break;
            case 2: final_key = base_key; break; // Mixed case
        }
        
        // Create different types of values
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        cJSON* value = NULL;
        
        switch (value_type) {
            case 0:
                value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                value = cJSON_CreateString(fdp.ConsumeRandomLengthString(128).c_str());
                break;
            case 2:
                value = cJSON_CreateBool(fdp.ConsumeBool());
                break;
            case 3:
                value = cJSON_CreateArray();
                // Add 0-2 elements to array
                if (value) {
                    int array_elems = fdp.ConsumeIntegralInRange<int>(0, 2);
                    for (int j = 0; j < array_elems; j++) {
                        cJSON* arr_elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        if (arr_elem) {
                            cJSON_AddItemToArray(value, arr_elem);
                        }
                    }
                }
                break;
            case 4:
                value = cJSON_CreateObject();
                // Add 0-1 nested properties
                if (value && fdp.ConsumeBool()) {
                    cJSON_AddItemToObject(value, "nested", 
                        cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                }
                break;
            case 5:
                value = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(64).c_str());
                break;
        }
        
        if (value) {
            cJSON_AddItemToObject(obj, final_key.c_str(), value);
        }
    }
    
    return obj;
}

// Helper to create floating-point numbers near epsilon for compare_double testing
static void create_epsilon_numbers(FuzzedDataProvider& fdp, double& num1, double& num2) {
    // Generate numbers very close to each other to test compare_double epsilon logic
    double base = fdp.ConsumeFloatingPoint<double>();
    
    // Small epsilon values
    const double epsilon = std::numeric_limits<double>::epsilon();
    const double small_epsilon = epsilon * 0.1;
    const double larger_epsilon = epsilon * 10.0;
    
    uint8_t epsilon_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (epsilon_type) {
        case 0:
            // Numbers within epsilon (should compare equal)
            num1 = base;
            num2 = base + (small_epsilon * 0.5);
            break;
        case 1:
            // Numbers just outside epsilon (should compare different)
            num1 = base;
            num2 = base + (larger_epsilon * 2.0);
            break;
        case 2:
            // NaN testing
            num1 = std::numeric_limits<double>::quiet_NaN();
            num2 = base;
            break;
        case 3:
            // Infinity testing
            num1 = std::numeric_limits<double>::infinity();
            num2 = -std::numeric_limits<double>::infinity();
            break;
    }
}

// Helper to create circular-like structures (without actual cycles)
static cJSON* create_circular_like_structure(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0) {
        return cJSON_CreateNull();
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Generate property names from fuzzer input
    std::string self_ref_key = fdp.ConsumeRandomLengthString(16);
    if (self_ref_key.empty()) {
        self_ref_key = "ref";
    }
    
    std::string value_key = fdp.ConsumeRandomLengthString(16);
    if (value_key.empty()) {
        value_key = "value";
    }
    
    std::string text_key = fdp.ConsumeRandomLengthString(16);
    if (text_key.empty()) {
        text_key = "text";
    }
    
    cJSON* nested = create_circular_like_structure(fdp, depth - 1);
    
    if (nested) {
        cJSON_AddItemToObject(obj, self_ref_key.c_str(), nested);
        
        // Add other properties using fuzzer input
        if (fdp.ConsumeBool()) {
            cJSON_AddItemToObject(obj, value_key.c_str(), 
                cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        
        if (fdp.ConsumeBool()) {
            cJSON_AddItemToObject(obj, text_key.c_str(),
                cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
        }
    }
    
    return obj;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration data
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool case_sensitive = fdp.ConsumeBool();
    bool use_parsed_json = fdp.ConsumeBool();
    bool test_invalid_types = fdp.ConsumeBool();
    bool test_epsilon_numbers = fdp.ConsumeBool();
    
    // Consume sizes and counts
    int array_size = fdp.ConsumeIntegralInRange<int>(0, 10);
    int object_props = fdp.ConsumeIntegralInRange<int>(0, 8);
    int nesting_depth = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    // Consume strings for parsed JSON testing
    std::string json_str1 = fdp.ConsumeRandomLengthString(512);
    std::string json_str2 = fdp.ConsumeRandomLengthString(512);
    
    // Consume numeric values
    double num1 = 0.0, num2 = 0.0;
    if (test_epsilon_numbers) {
        create_epsilon_numbers(fdp, num1, num2);
    } else {
        num1 = fdp.ConsumeFloatingPoint<double>();
        num2 = fdp.ConsumeFloatingPoint<double>();
    }
    
    cJSON* a = NULL;
    cJSON* b = NULL;
    cJSON_bool comparison_result = false;
    
    // Select test scenario based on fuzzer input
    switch (test_scenario) {
        case 0: {
            // Test 1: Invalid JSON types to trigger default switch case
            if (test_invalid_types) {
                a = create_invalid_type_json(fdp, 0);
                b = create_invalid_type_json(fdp, 1);
                
                if (a && b) {
                    comparison_result = cJSON_Compare(a, b, case_sensitive);
                }
            }
            break;
        }
        
        case 1: {
            // Test 2: Floating-point epsilon number comparisons
            a = cJSON_CreateNumber(num1);
            b = cJSON_CreateNumber(num2);
            
            if (a && b) {
                comparison_result = cJSON_Compare(a, b, case_sensitive);
            }
            break;
        }
        
        case 2: {
            // Test 3: Mixed-type array comparisons
            a = create_mixed_type_array(fdp, array_size);
            
            // Create b as either identical, different, or parsed version
            if (use_parsed_json && !json_str1.empty()) {
                // Try to parse JSON string for comparison
                b = cJSON_Parse(json_str1.c_str());
                if (!b) {
                    // If parsing fails, create similar structure programmatically
                    b = create_mixed_type_array(fdp, array_size);
                }
            } else {
                b = create_mixed_type_array(fdp, array_size);
            }
            
            if (a && b) {
                comparison_result = cJSON_Compare(a, b, case_sensitive);
            }
            break;
        }
        
        case 3: {
            // Test 4: Complex object key patterns with case variations
            a = create_complex_key_object(fdp, object_props);
            
            if (use_parsed_json && !json_str1.empty()) {
                // Create JSON string from a, then parse it back for b
                char* a_json = cJSON_Print(a);
                if (a_json) {
                    b = cJSON_Parse(a_json);
                    cJSON_free(a_json);
                }
                if (!b) {
                    // If parsing fails, create similar object
                    b = create_complex_key_object(fdp, object_props);
                }
            } else {
                // Create similar or different object
                bool create_similar = fdp.ConsumeBool();
                if (create_similar) {
                    // Reset fdp consumption to create similar structure
                    // (simplified - in real fuzzing this would need proper state handling)
                    b = create_complex_key_object(fdp, object_props);
                } else {
                    // Create different object
                    b = create_complex_key_object(fdp, object_props / 2 + 1);
                }
            }
            
            if (a && b) {
                comparison_result = cJSON_Compare(a, b, case_sensitive);
            }
            break;
        }
        
        case 4: {
            // Test 5: Circular-like structures (without actual cycles)
            a = create_circular_like_structure(fdp, nesting_depth);
            b = create_circular_like_structure(fdp, nesting_depth);
            
            if (a && b) {
                comparison_result = cJSON_Compare(a, b, case_sensitive);
            }
            break;
        }
        
        case 5: {
            // Test 6: NULL valuestring pointers in String/Raw types
            a = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
            b = cJSON_CreateString(NULL); // Should have NULL valuestring
            
            if (a && b) {
                // b should have NULL valuestring if CreateString(NULL) handles it
                // If not, manually set valuestring to NULL
                if (b->valuestring) {
                    cJSON_free(b->valuestring);
                    b->valuestring = NULL;
                }
                
                comparison_result = cJSON_Compare(a, b, case_sensitive);
            }
            break;
        }
        
        case 6: {
            // Test 7: Identical object pointer comparison (a == b)
            a = cJSON_CreateObject();
            if (a) {
                // Consume values from fuzzer input for properties
                double test_num = fdp.ConsumeFloatingPoint<double>();
                std::string test_str = fdp.ConsumeRandomLengthString(64);
                std::string prop_name1 = fdp.ConsumeRandomLengthString(16);
                std::string prop_name2 = fdp.ConsumeRandomLengthString(16);
                
                // Add some properties using fuzzer-provided values
                cJSON_AddItemToObject(a, prop_name1.c_str(), cJSON_CreateNumber(test_num));
                cJSON_AddItemToObject(a, prop_name2.c_str(), cJSON_CreateString(test_str.c_str()));
                
                // Compare a with itself
                comparison_result = cJSON_Compare(a, a, case_sensitive);
                
                // Also create b as duplicate for normal comparison
                b = cJSON_Duplicate(a, true);
                if (b) {
                    cJSON_bool duplicate_comparison = cJSON_Compare(a, b, case_sensitive);
                    (void)duplicate_comparison; // Use result
                }
            }
            break;
        }
        
        case 7: {
            // Test 8: Parsed JSON vs programmatically created structures
            if (!json_str1.empty() && !json_str2.empty()) {
                a = cJSON_Parse(json_str1.c_str());
                b = cJSON_Parse(json_str2.c_str());
                
                if (a && b) {
                    comparison_result = cJSON_Compare(a, b, case_sensitive);
                } else {
                    // If parsing fails, create fallback structures
                    if (!a) a = cJSON_CreateObject();
                    if (!b) b = cJSON_CreateObject();
                    
                    if (a && b) {
                        comparison_result = cJSON_Compare(a, b, case_sensitive);
                    }
                }
            }
            break;
        }
    }
    
    // Cleanup
    if (a) cJSON_Delete(a);
    if (b) cJSON_Delete(b);
    
    // Use comparison_result to prevent optimization
    if (comparison_result == (cJSON_bool)0xDEADBEEF) {
        return 1; // Never happens, just to use the result
    }
    
    return 0;
}

} // namespace harness_016
