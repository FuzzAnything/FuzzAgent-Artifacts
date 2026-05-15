#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_021 {
/*
 * Harness 021: Deep comparison testing for cJSON_Compare with semantic diversity
 * 
 * Focus: Targeting the remaining 73/76 blocked branches in cJSON_Compare by testing
 * comparison scenarios not covered by existing harnesses:
 * - harness_019: Edge cases (NULL, type mismatches, simple comparisons)
 * - harness_020: Structural comparisons with ordering variations
 * - This harness: Deep semantic comparisons (case-sensitive vs insensitive, 
 *   string vs raw type, floating point epsilon, complex nested structures)
 * 
 * Blocked branches to target:
 * 1. Object comparison paths (lines 3158-3198):
 *    - Objects with same keys but different values
 *    - Objects with different keys  
 *    - Nested object comparisons
 *    - Case-sensitive vs case-insensitive key matching
 * 2. Array comparison paths (lines 3137-3159):
 *    - Arrays of different lengths
 *    - Arrays with same elements in different order
 *    - Nested array comparisons
 * 3. Edge cases:
 *    - NULL pointer inputs
 *    - Invalid type comparisons
 *    - Identical object comparison
 *    - String vs raw type comparisons
 *    - Floating point number comparisons with epsilon
 */



// Helper to create JSON objects with case variation in keys
static cJSON* create_object_with_case_variation(FuzzedDataProvider& fdp, bool use_upper_case) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    int num_keys = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    for (int i = 0; i < num_keys; i++) {
        std::string key = fdp.ConsumeRandomLengthString(16);
        
        // Apply case variation if requested
        if (use_upper_case) {
            std::transform(key.begin(), key.end(), key.begin(), ::toupper);
        } else {
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        }
        
        // Create random value
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        cJSON* value = NULL;
        
        switch (value_type) {
            case 0:
                value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                value = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
                break;
            case 2:
                value = cJSON_CreateBool(fdp.ConsumeBool());
                break;
            case 3:
                value = cJSON_CreateArray();
                // Add some random elements to array
                if (value) {
                    int array_size = fdp.ConsumeIntegralInRange<int>(0, 4);
                    for (int j = 0; j < array_size; j++) {
                        cJSON* elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        if (elem) cJSON_AddItemToArray(value, elem);
                    }
                }
                break;
            default:
                value = cJSON_CreateNull();
                break;
        }
        
        if (value) {
            cJSON_AddItemToObject(obj, key.c_str(), value);
        }
    }
    
    return obj;
}

// Helper to create arrays with potentially different ordering
static cJSON* create_array_with_possible_duplicates(FuzzedDataProvider& fdp) {
    cJSON* arr = cJSON_CreateArray();
    if (!arr) return NULL;
    
    int array_size = fdp.ConsumeIntegralInRange<int>(1, 10);
    std::vector<double> numbers;
    std::vector<std::string> strings;
    
    // Generate some values that might be duplicates
    for (int i = 0; i < array_size; i++) {
        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (elem_type) {
            case 0: {
                double num = fdp.ConsumeFloatingPoint<double>();
                numbers.push_back(num);
                cJSON* elem = cJSON_CreateNumber(num);
                if (elem) cJSON_AddItemToArray(arr, elem);
                break;
            }
            case 1: {
                std::string str = fdp.ConsumeRandomLengthString(32);
                strings.push_back(str);
                cJSON* elem = cJSON_CreateString(str.c_str());
                if (elem) cJSON_AddItemToArray(arr, elem);
                break;
            }
            default: {
                cJSON* elem = cJSON_CreateBool(fdp.ConsumeBool());
                if (elem) cJSON_AddItemToArray(arr, elem);
                break;
            }
        }
    }
    
    return arr;
}

// Helper to test floating point comparisons with epsilon
static void test_floating_point_comparisons(FuzzedDataProvider& fdp, bool case_sensitive) {
    // Test numbers very close to each other (within epsilon)
    double base = fdp.ConsumeFloatingPoint<double>();
    double epsilon = 1e-12; // Small epsilon
    
    // Create numbers that are very close
    cJSON* num1 = cJSON_CreateNumber(base);
    cJSON* num2 = cJSON_CreateNumber(base + epsilon * 0.5); // Within epsilon
    cJSON* num3 = cJSON_CreateNumber(base + epsilon * 2.0); // Outside epsilon
    
    if (num1 && num2) {
        cJSON_Compare(num1, num2, case_sensitive); // Should be equal (within compare_double epsilon)
    }
    
    if (num1 && num3) {
        cJSON_Compare(num1, num3, case_sensitive); // Might not be equal
    }
    
    // Test with special floating point values
    cJSON* nan1 = cJSON_CreateNumber(NAN);
    cJSON* nan2 = cJSON_CreateNumber(NAN);
    cJSON* inf1 = cJSON_CreateNumber(INFINITY);
    cJSON* inf2 = cJSON_CreateNumber(-INFINITY);
    
    if (nan1 && nan2) cJSON_Compare(nan1, nan2, case_sensitive);
    if (inf1 && inf2) cJSON_Compare(inf1, inf2, case_sensitive);
    if (nan1 && inf1) cJSON_Compare(nan1, inf1, case_sensitive);
    
    if (num1) cJSON_Delete(num1);
    if (num2) cJSON_Delete(num2);
    if (num3) cJSON_Delete(num3);
    if (nan1) cJSON_Delete(nan1);
    if (nan2) cJSON_Delete(nan2);
    if (inf1) cJSON_Delete(inf1);
    if (inf2) cJSON_Delete(inf2);
}

// Helper to test string vs raw type comparisons
static void test_string_vs_raw_comparisons(FuzzedDataProvider& fdp, bool case_sensitive) {
    std::string str_content = fdp.ConsumeRandomLengthString(128);
    
    // Create string and raw JSON with same content
    cJSON* str_json = cJSON_CreateString(str_content.c_str());
    
    // Create raw JSON (JSON string literal)
    std::string raw_content = "\"" + str_content + "\"";
    cJSON* raw_json = cJSON_CreateRaw(raw_content.c_str());
    
    // Create another string with different content
    std::string str2_content = fdp.ConsumeRandomLengthString(128);
    cJSON* str2_json = cJSON_CreateString(str2_content.c_str());
    
    // String vs Raw comparison (should be false - different types)
    if (str_json && raw_json) {
        cJSON_Compare(str_json, raw_json, case_sensitive);
    }
    
    // String vs String (same content)
    if (str_json && str2_json) {
        cJSON_Compare(str_json, str2_json, case_sensitive);
    }
    
    // Test NULL string values
    cJSON* null_str = cJSON_CreateString(NULL);
    if (null_str && str_json) {
        cJSON_Compare(null_str, str_json, case_sensitive);
    }
    
    if (str_json) cJSON_Delete(str_json);
    if (raw_json) cJSON_Delete(raw_json);
    if (str2_json) cJSON_Delete(str2_json);
    if (null_str) cJSON_Delete(null_str);
}

// Helper to create deeply nested structures for comparison
static cJSON* create_deeply_nested_structure(FuzzedDataProvider& fdp, int depth, bool is_object) {
    if (depth <= 0) {
        // Leaf node
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (leaf_type) {
            case 0: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1: return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 2: return cJSON_CreateBool(fdp.ConsumeBool());
            default: return cJSON_CreateNull();
        }
    }
    
    if (is_object) {
        cJSON* obj = cJSON_CreateObject();
        if (!obj) return NULL;
        
        int num_props = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_props; i++) {
            std::string key = "key_" + std::to_string(i) + "_" + fdp.ConsumeRandomLengthString(8);
            cJSON* value = create_deeply_nested_structure(fdp, depth - 1, !is_object);
            if (value) {
                cJSON_AddItemToObject(obj, key.c_str(), value);
            }
        }
        return obj;
    } else {
        cJSON* arr = cJSON_CreateArray();
        if (!arr) return NULL;
        
        int num_elements = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_elements; i++) {
            cJSON* element = create_deeply_nested_structure(fdp, depth - 1, !is_object);
            if (element) {
                cJSON_AddItemToArray(arr, element);
            }
        }
        return arr;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0; // Need enough data for meaningful comparisons
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool case_sensitive = fdp.ConsumeBool();
    bool use_parsed_json = fdp.ConsumeBool();
    
    // Consume numeric values
    double num_val1 = fdp.ConsumeFloatingPoint<double>();
    double num_val2 = fdp.ConsumeFloatingPoint<double>();
    
    // Consume string data
    std::string str1 = fdp.ConsumeRandomLengthString(128);
    std::string str2 = fdp.ConsumeRandomLengthString(128);
    std::string json_input = fdp.ConsumeRemainingBytesAsString();
    
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *parsed1 = NULL;
    cJSON *parsed2 = NULL;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Case-sensitive vs case-insensitive object key matching
            item1 = create_object_with_case_variation(fdp, false); // lowercase keys
            item2 = create_object_with_case_variation(fdp, true);  // UPPERCASE keys
            
            // Test both case-sensitive and case-insensitive
            if (item1 && item2) {
                cJSON_Compare(item1, item2, true);   // case-sensitive (should fail for different case)
                cJSON_Compare(item1, item2, false);  // case-insensitive (might succeed)
            }
            
            // Also test with same case
            cJSON* item3 = create_object_with_case_variation(fdp, false);
            if (item1 && item3) {
                cJSON_Compare(item1, item3, case_sensitive);
            }
            
            if (item1) { cJSON_Delete(item1); item1 = NULL; }
            if (item2) { cJSON_Delete(item2); item2 = NULL; }
            if (item3) cJSON_Delete(item3);
            break;
        }
        
        case 1: {
            // Scenario 2: String vs Raw type comparisons
            test_string_vs_raw_comparisons(fdp, case_sensitive);
            break;
        }
        
        case 2: {
            // Scenario 3: Floating point comparisons with epsilon
            test_floating_point_comparisons(fdp, case_sensitive);
            break;
        }
        
        case 3: {
            // Scenario 4: Objects with same keys but different values
            item1 = cJSON_CreateObject();
            item2 = cJSON_CreateObject();
            
            if (item1 && item2) {
                // Add same keys with different values
                for (int i = 0; i < 3; i++) {
                    std::string key = "key" + std::to_string(i);
                    cJSON_AddItemToObject(item1, key.c_str(), cJSON_CreateNumber(num_val1));
                    cJSON_AddItemToObject(item2, key.c_str(), cJSON_CreateNumber(num_val2));
                }
                
                cJSON_Compare(item1, item2, case_sensitive);
            }
            
            if (item1) { cJSON_Delete(item1); item1 = NULL; }
            if (item2) { cJSON_Delete(item2); item2 = NULL; }
            break;
        }
        
        case 4: {
            // Scenario 5: Objects with different keys
            item1 = cJSON_CreateObject();
            item2 = cJSON_CreateObject();
            
            if (item1 && item2) {
                // Add different keys
                cJSON_AddItemToObject(item1, "key1", cJSON_CreateString(str1.c_str()));
                cJSON_AddItemToObject(item1, "key2", cJSON_CreateNumber(num_val1));
                
                cJSON_AddItemToObject(item2, "key3", cJSON_CreateString(str2.c_str()));
                cJSON_AddItemToObject(item2, "key4", cJSON_CreateNumber(num_val2));
                
                cJSON_Compare(item1, item2, case_sensitive);
            }
            
            if (item1) { cJSON_Delete(item1); item1 = NULL; }
            if (item2) { cJSON_Delete(item2); item2 = NULL; }
            break;
        }
        
        case 5: {
            // Scenario 6: Arrays with same elements in different order
            item1 = create_array_with_possible_duplicates(fdp);
            
            // Create a copy with potentially different order
            if (item1) {
                // Parse the printed JSON to get similar structure
                char* printed = cJSON_Print(item1);
                if (printed) {
                    parsed1 = cJSON_Parse(printed);
                    free(printed);
                    
                    if (parsed1) {
                        // Arrays should be equal even if printed/parsed
                        cJSON_Compare(item1, parsed1, case_sensitive);
                    }
                }
            }
            
            if (item1) { cJSON_Delete(item1); item1 = NULL; }
            if (parsed1) { cJSON_Delete(parsed1); parsed1 = NULL; }
            break;
        }
        
        case 6: {
            // Scenario 7: Deeply nested structure comparisons
            int depth = fdp.ConsumeIntegralInRange<int>(1, 4);
            item1 = create_deeply_nested_structure(fdp, depth, true);
            item2 = create_deeply_nested_structure(fdp, depth, true);
            
            if (item1 && item2) {
                cJSON_Compare(item1, item2, case_sensitive);
                
                // Also compare with itself
                cJSON_Compare(item1, item1, case_sensitive); // Should be true (identical)
            }
            
            if (item1) { cJSON_Delete(item1); item1 = NULL; }
            if (item2) { cJSON_Delete(item2); item2 = NULL; }
            break;
        }
        
        case 7: {
            // Scenario 8: Mixed parsing and creation comparisons
            if (use_parsed_json && !json_input.empty()) {
                parsed1 = cJSON_Parse(json_input.c_str());
                parsed2 = cJSON_ParseWithOpts(json_input.c_str(), NULL, 1); // require null-terminated
                
                if (parsed1 && parsed2) {
                    cJSON_Compare(parsed1, parsed2, case_sensitive);
                }
                
                // Compare parsed with created
                item1 = cJSON_CreateObject();
                if (item1 && parsed1) {
                    cJSON_AddItemToObject(item1, "parsed_copy", cJSON_Duplicate(parsed1, 1));
                    cJSON_Compare(parsed1, item1, case_sensitive);
                }
                
                if (parsed1) { cJSON_Delete(parsed1); parsed1 = NULL; }
                if (parsed2) { cJSON_Delete(parsed2); parsed2 = NULL; }
                if (item1) { cJSON_Delete(item1); item1 = NULL; }
            }
        }
    }
    
    // Cleanup any remaining items (should be NULL if already cleaned up in cases)
    if (item1) { cJSON_Delete(item1); item1 = NULL; }
    if (item2) { cJSON_Delete(item2); item2 = NULL; }
    if (parsed1) { cJSON_Delete(parsed1); parsed1 = NULL; }
    if (parsed2) { cJSON_Delete(parsed2); parsed2 = NULL; }
    
    return 0;
}

} // namespace harness_021
