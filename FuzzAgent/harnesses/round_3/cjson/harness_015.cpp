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

namespace harness_015 {
/*
 * Harness 015: Deep Nested Structure Comparison Targeting cJSON_Compare Blocked Branches (73 blocked)
 * 
 * Target: Specifically focus on complex nested JSON structures (3+ levels deep) to exercise
 * recursive comparison logic and O(n^2) object comparison algorithm in cJSON_Compare.
 * 
 * This harness focuses on:
 * 1. Creating complex nested JSON structures (objects containing arrays containing objects...)
 * 2. Testing array comparisons with varying lengths and element orders
 * 3. Testing nested object comparisons with different property sets and orders
 * 4. Testing mixed type comparisons (objects containing arrays containing objects)
 * 5. Testing case-sensitive vs case-insensitive string comparisons in nested contexts
 * 6. Testing recursive comparison failures at different depths
 * 7. Exercising O(n^2) object comparison algorithm with varying object sizes
 * 8. Testing objects with same properties in different orders (JSON equality)
 * 9. Testing objects with subset/superset relationships
 * 
 * Key coverage targets from blocked branches:
 * - Array comparison paths (lines 3137-3159 with only 25 hits)
 * - Object comparison O(n^2) algorithm (lines 3158-3197)
 * - Deep recursive comparison failures
 * - Complex nested structure equality checks
 * - Edge cases with deeply nested empty objects/arrays
 */


// Helper to create deeply nested JSON structure (level specifies nesting depth)
static cJSON* create_deep_nested_object(FuzzedDataProvider& fdp, int level, const std::string& base_key) {
    if (level <= 0) {
        // Leaf level: create simple value
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (value_type) {
            case 0:
                return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1:
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
            case 2:
                return cJSON_CreateBool(fdp.ConsumeBool());
            case 3:
                return cJSON_CreateNull();
            case 4:
                return cJSON_CreateArray(); // Empty array at leaf
            default:
                return cJSON_CreateNull();
        }
    }
    
    // Create object with nested structures
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Determine number of properties at this level (1-4)
    int num_properties = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    for (int i = 0; i < num_properties; i++) {
        std::string key = base_key + "_" + std::to_string(i);
        
        // Determine what type of value to add (object, array, or primitive)
        uint8_t prop_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        cJSON* value = NULL;
        switch (prop_type) {
            case 0:
                // Nested object (go deeper)
                value = create_deep_nested_object(fdp, level - 1, key);
                break;
            case 1:
                // Array containing nested structures
                value = cJSON_CreateArray();
                if (value) {
                    int array_size = fdp.ConsumeIntegralInRange<int>(1, 3);
                    for (int j = 0; j < array_size; j++) {
                        cJSON* array_elem = create_deep_nested_object(fdp, level - 1, key + "_arr" + std::to_string(j));
                        if (array_elem) {
                            cJSON_AddItemToArray(value, array_elem);
                        }
                    }
                }
                break;
            case 2:
                // Primitive value (leaf)
                value = create_deep_nested_object(fdp, 0, key);
                break;
        }
        
        if (value) {
            cJSON_AddItemToObject(obj, key.c_str(), value);
        }
    }
    
    return obj;
}

// Helper to create object with properties in specific order
static cJSON* create_object_with_ordered_properties(FuzzedDataProvider& fdp, const std::vector<std::string>& keys, bool same_values) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    double base_value = fdp.ConsumeFloatingPoint<double>();
    
    for (size_t i = 0; i < keys.size(); i++) {
        double value = same_values ? base_value : base_value + i;
        cJSON_AddNumberToObject(obj, keys[i].c_str(), value);
    }
    
    return obj;
}

// Helper to create array with mixed types at different depths
static cJSON* create_mixed_type_array(FuzzedDataProvider& fdp, int depth) {
    cJSON* arr = cJSON_CreateArray();
    if (!arr) return NULL;
    
    int size = fdp.ConsumeIntegralInRange<int>(2, 5);
    
    for (int i = 0; i < size; i++) {
        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        cJSON* elem = NULL;
        
        switch (elem_type) {
            case 0:
                elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                elem = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                break;
            case 2:
                if (depth > 0) {
                    elem = create_deep_nested_object(fdp, depth - 1, "arr_obj_" + std::to_string(i));
                } else {
                    elem = cJSON_CreateObject();
                }
                break;
            case 3:
                if (depth > 0) {
                    elem = create_mixed_type_array(fdp, depth - 1);
                } else {
                    elem = cJSON_CreateArray();
                }
                break;
        }
        
        if (elem) {
            cJSON_AddItemToArray(arr, elem);
        }
    }
    
    return arr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for complex nested structure testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool case_sensitive = fdp.ConsumeBool();
    int nesting_depth = fdp.ConsumeIntegralInRange<int>(2, 4); // 2-4 levels deep
    bool use_parsed_json = fdp.ConsumeBool();
    
    // Consume values for property names and strings
    std::vector<std::string> property_keys;
    int num_keys = fdp.ConsumeIntegralInRange<int>(3, 6);
    for (int i = 0; i < num_keys && fdp.remaining_bytes() > 0; i++) {
        property_keys.push_back(fdp.ConsumeRandomLengthString(16));
    }
    
    // Ensure we have at least some keys
    while (property_keys.size() < 3) {
        property_keys.push_back("key" + std::to_string(property_keys.size()));
    }
    
    // Different order of same keys
    std::vector<std::string> reordered_keys = property_keys;
    std::reverse(reordered_keys.begin(), reordered_keys.end());
    
    // Subset of keys (for subset/superset testing)
    std::vector<std::string> subset_keys;
    if (property_keys.size() > 2) {
        subset_keys.push_back(property_keys[0]);
        subset_keys.push_back(property_keys[1]);
    } else {
        subset_keys = property_keys;
    }
    
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Deeply nested identical objects (should return true)
            cJSON* deep_obj1 = create_deep_nested_object(fdp, nesting_depth, "root");
            cJSON* deep_obj2 = create_deep_nested_object(fdp, nesting_depth, "root");
            
            if (deep_obj1 && deep_obj2) {
                // Compare identical nesting structures (may have different random values)
                cJSON_bool deep_compare = cJSON_Compare(deep_obj1, deep_obj2, case_sensitive);
                
                // Self comparison (a == b)
                cJSON_bool self_compare = cJSON_Compare(deep_obj1, deep_obj1, case_sensitive);
            }
            
            if (deep_obj1) cJSON_Delete(deep_obj1);
            if (deep_obj2) cJSON_Delete(deep_obj2);
            break;
        }
        
        case 1: {
            // Scenario 2: Objects with same properties in different orders (JSON equality)
            cJSON* ordered_obj1 = create_object_with_ordered_properties(fdp, property_keys, true);
            cJSON* ordered_obj2 = create_object_with_ordered_properties(fdp, reordered_keys, true);
            
            if (ordered_obj1 && ordered_obj2) {
                // These should compare equal regardless of order (JSON objects are unordered)
                cJSON_bool order_compare = cJSON_Compare(ordered_obj1, ordered_obj2, case_sensitive);
                
                // Create a copy with same order for self-comparison
                cJSON* ordered_obj1_copy = create_object_with_ordered_properties(fdp, property_keys, true);
                if (ordered_obj1_copy) {
                    cJSON_bool same_order_compare = cJSON_Compare(ordered_obj1, ordered_obj1_copy, case_sensitive);
                    cJSON_Delete(ordered_obj1_copy);
                }
            }
            
            if (ordered_obj1) cJSON_Delete(ordered_obj1);
            if (ordered_obj2) cJSON_Delete(ordered_obj2);
            break;
        }
        
        case 2: {
            // Scenario 3: Objects with different values (should return false)
            cJSON* obj_same_keys1 = create_object_with_ordered_properties(fdp, property_keys, false);
            cJSON* obj_same_keys2 = create_object_with_ordered_properties(fdp, property_keys, false);
            
            if (obj_same_keys1 && obj_same_keys2) {
                // Same keys, different values (should be false unless by chance same)
                cJSON_bool diff_values_compare = cJSON_Compare(obj_same_keys1, obj_same_keys2, case_sensitive);
            }
            
            if (obj_same_keys1) cJSON_Delete(obj_same_keys1);
            if (obj_same_keys2) cJSON_Delete(obj_same_keys2);
            break;
        }
        
        case 3: {
            // Scenario 4: Subset/superset object relationships
            cJSON* full_obj = create_object_with_ordered_properties(fdp, property_keys, true);
            cJSON* subset_obj = create_object_with_ordered_properties(fdp, subset_keys, true);
            
            if (full_obj && subset_obj) {
                // Subset should not equal superset (should return false)
                cJSON_bool subset_compare = cJSON_Compare(full_obj, subset_obj, case_sensitive);
                cJSON_bool superset_compare = cJSON_Compare(subset_obj, full_obj, case_sensitive);
                
                // Same subset objects should compare equal
                cJSON* subset_obj2 = create_object_with_ordered_properties(fdp, subset_keys, true);
                if (subset_obj2) {
                    cJSON_bool same_subset_compare = cJSON_Compare(subset_obj, subset_obj2, case_sensitive);
                    cJSON_Delete(subset_obj2);
                }
            }
            
            if (full_obj) cJSON_Delete(full_obj);
            if (subset_obj) cJSON_Delete(subset_obj);
            break;
        }
        
        case 4: {
            // Scenario 5: Mixed type arrays with nested structures
            cJSON* mixed_array1 = create_mixed_type_array(fdp, nesting_depth);
            cJSON* mixed_array2 = create_mixed_type_array(fdp, nesting_depth);
            
            if (mixed_array1 && mixed_array2) {
                // Compare arrays with potentially different mixed contents
                cJSON_bool array_compare = cJSON_Compare(mixed_array1, mixed_array2, case_sensitive);
                
                // Self comparison
                cJSON_bool array_self = cJSON_Compare(mixed_array1, mixed_array1, case_sensitive);
                
                // Test with empty arrays
                cJSON* empty_array1 = cJSON_CreateArray();
                cJSON* empty_array2 = cJSON_CreateArray();
                if (empty_array1 && empty_array2) {
                    cJSON_bool empty_compare = cJSON_Compare(empty_array1, empty_array2, case_sensitive);
                    cJSON_Delete(empty_array1);
                    cJSON_Delete(empty_array2);
                }
            }
            
            if (mixed_array1) cJSON_Delete(mixed_array1);
            if (mixed_array2) cJSON_Delete(mixed_array2);
            break;
        }
        
        case 5: {
            // Scenario 6: Case-sensitive vs case-insensitive in nested objects
            // Create object with case-sensitive keys
            cJSON* case_obj = cJSON_CreateObject();
            if (case_obj) {
                std::string lower_key = "key";
                std::string upper_key = "KEY";
                std::string mixed_key = "Key";
                
                double value = fdp.ConsumeFloatingPoint<double>();
                
                cJSON_AddNumberToObject(case_obj, lower_key.c_str(), value);
                cJSON* case_obj_copy = cJSON_CreateObject();
                if (case_obj_copy) {
                    cJSON_AddNumberToObject(case_obj_copy, upper_key.c_str(), value);
                    
                    // Case-sensitive comparison (should be false for different case keys)
                    cJSON_bool case_sensitive_compare = cJSON_Compare(case_obj, case_obj_copy, true);
                    
                    // Case-insensitive comparison (should be true for same value different case keys)
                    cJSON_bool case_insensitive_compare = cJSON_Compare(case_obj, case_obj_copy, false);
                    
                    cJSON_Delete(case_obj_copy);
                }
                
                // Create another with same case for control
                cJSON* same_case_obj = cJSON_CreateObject();
                if (same_case_obj) {
                    cJSON_AddNumberToObject(same_case_obj, lower_key.c_str(), value);
                    cJSON_bool same_case_compare = cJSON_Compare(case_obj, same_case_obj, case_sensitive);
                    cJSON_Delete(same_case_obj);
                }
                
                cJSON_Delete(case_obj);
            }
            break;
        }
        
        case 6: {
            // Scenario 7: Complex nested object containing arrays containing objects
            cJSON* complex_root = cJSON_CreateObject();
            if (complex_root) {
                // Add a nested array
                cJSON* nested_array = cJSON_CreateArray();
                if (nested_array) {
                    // Add objects to array
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                        cJSON* obj_in_array = cJSON_CreateObject();
                        if (obj_in_array) {
                            int id_value = fdp.ConsumeIntegral<int>();
                            cJSON_AddNumberToObject(obj_in_array, "id", id_value);
                            cJSON_AddStringToObject(obj_in_array, "name", 
                                fdp.ConsumeRandomLengthString(16).c_str());
                            cJSON_AddItemToArray(nested_array, obj_in_array);
                        }
                    }
                    cJSON_AddItemToObject(complex_root, "items", nested_array);
                }
                
                // Add another nested object
                cJSON* nested_obj = cJSON_CreateObject();
                if (nested_obj) {
                    cJSON_AddNumberToObject(nested_obj, "count", fdp.ConsumeIntegral<int>());
                    
                    // Add array inside nested object
                    cJSON* inner_array = cJSON_CreateArray();
                    if (inner_array) {
                        for (int i = 0; i < 2 && fdp.remaining_bytes() > 0; i++) {
                            cJSON_AddItemToArray(inner_array, 
                                cJSON_CreateString(fdp.ConsumeRandomLengthString(8).c_str()));
                        }
                        cJSON_AddItemToObject(nested_obj, "tags", inner_array);
                    }
                    
                    cJSON_AddItemToObject(complex_root, "metadata", nested_obj);
                }
                
                // Create similar but not identical structure
                cJSON* complex_root2 = cJSON_CreateObject();
                if (complex_root2) {
                    // Try to build similar structure but with different values
                    cJSON* nested_array2 = cJSON_CreateArray();
                    if (nested_array2) {
                        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                            cJSON* obj_in_array2 = cJSON_CreateObject();
                            if (obj_in_array2) {
                                int id_value2 = fdp.ConsumeIntegral<int>();
                                cJSON_AddNumberToObject(obj_in_array2, "id", id_value2); // Different id from fuzzer input
                                cJSON_AddStringToObject(obj_in_array2, "name",
                                    fdp.ConsumeRandomLengthString(16).c_str());
                                cJSON_AddItemToArray(nested_array2, obj_in_array2);
                            }
                        }
                        cJSON_AddItemToObject(complex_root2, "items", nested_array2);
                    }
                    
                    // Compare the complex structures
                    cJSON_bool complex_compare = cJSON_Compare(complex_root, complex_root2, case_sensitive);
                    cJSON_bool self_compare = cJSON_Compare(complex_root, complex_root, case_sensitive);
                    
                    cJSON_Delete(complex_root2);
                }
                
                cJSON_Delete(complex_root);
            }
            break;
        }
        
        case 7: {
            // Scenario 8: Parsed JSON vs programmatically created structures
            // Create JSON string from fuzzer input
            std::string json_str = fdp.ConsumeRandomLengthString(512);
            
            // Parse JSON
            cJSON* parsed_json = cJSON_Parse(json_str.c_str());
            
            // Create programmatic equivalent if parsing succeeded
            if (parsed_json) {
                // Create a simple programmatic structure to compare with
                cJSON* programmatic = cJSON_CreateObject();
                if (programmatic) {
                    // Consume values from fuzzer input instead of hardcoding
                    double test_value = fdp.ConsumeFloatingPoint<double>();
                    std::string message_str = fdp.ConsumeRandomLengthString(64);
                    
                    cJSON_AddNumberToObject(programmatic, "test", test_value);
                    cJSON_AddStringToObject(programmatic, "message", message_str.c_str());
                    
                    // Compare parsed vs programmatic (likely different)
                    cJSON_bool parsed_vs_programmatic = cJSON_Compare(parsed_json, programmatic, case_sensitive);
                    
                    // Parse the same JSON string again for identical comparison
                    cJSON* parsed_json2 = cJSON_Parse(json_str.c_str());
                    if (parsed_json2) {
                        // Two parses of same string should compare equal
                        cJSON_bool parsed_identical = cJSON_Compare(parsed_json, parsed_json2, case_sensitive);
                        cJSON_Delete(parsed_json2);
                    }
                    
                    cJSON_Delete(programmatic);
                }
                
                cJSON_Delete(parsed_json);
            }
            break;
        }

    }
    
    return 0;
}

} // namespace harness_015
