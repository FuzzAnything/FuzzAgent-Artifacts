#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_023 {
/*
 * Harness 023: Complex JSON structure comparisons for cJSON_Compare
 * 
 * Primary Target: cJSON_Compare with blocked branches in object and array comparison paths
 * Focus: Deep semantic comparisons of complex nested structures to exercise:
 * 1. Objects with same keys but different values (lines 3166-3177)
 * 2. Objects with different keys (lines 3166-3177, 3181-3193)
 * 3. Nested object comparisons (recursive calls)
 * 4. Arrays with same elements in different order (lines 3139-3156)
 * 5. Arrays of different lengths (lines 3151-3153)
 * 6. Case-sensitive vs case-insensitive key matching (through get_object_item)
 * 
 * Coverage analysis shows many uncovered paths in object/array comparison logic.
 * This harness complements existing ones by focusing on structural complexity
 * rather than type/edge case testing.
 * 
 * APIs to use:
 * - cJSON_Compare (primary)
 * - cJSON_CreateObject/Array
 * - cJSON_AddItemToObject/Array
 * - cJSON_CreateString/Number/Bool/Null
 * - cJSON_Delete
 * - cJSON_Parse (for complex nested structures)
 */


// Helper to create deeply nested objects
static cJSON* create_deep_nested_object(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0) {
        // Create leaf value
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (leaf_type) {
            case 0:
                return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1:
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 2:
                return cJSON_CreateBool(fdp.ConsumeBool());
            default:
                return cJSON_CreateNull();
        }
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    int num_keys = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_keys; i++) {
        std::string key = fdp.ConsumeRandomLengthString(16);
        cJSON* value = create_deep_nested_object(fdp, depth - 1);
        if (value) {
            cJSON_AddItemToObject(obj, key.c_str(), value);
        }
    }
    
    return obj;
}

// Helper to create objects with same keys but different values
static std::pair<cJSON*, cJSON*> create_objects_same_keys_different_values(FuzzedDataProvider& fdp) {
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    
    if (!obj1 || !obj2) {
        if (obj1) cJSON_Delete(obj1);
        if (obj2) cJSON_Delete(obj2);
        return {NULL, NULL};
    }
    
    int num_keys = fdp.ConsumeIntegralInRange<int>(2, 8);
    
    for (int i = 0; i < num_keys; i++) {
        std::string key = "key_" + std::to_string(i);
        
        // Create different values for same key
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        cJSON* value1 = NULL;
        cJSON* value2 = NULL;
        
        switch (value_type) {
            case 0: // Numbers (different values)
                value1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                value2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 1: // Strings (different strings)
                value1 = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                value2 = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                break;
            case 2: // Booleans (different bools)
                value1 = cJSON_CreateBool(fdp.ConsumeBool());
                value2 = cJSON_CreateBool(fdp.ConsumeBool());
                break;
            default: // Different types
                value1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                value2 = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                break;
        }
        
        if (value1) cJSON_AddItemToObject(obj1, key.c_str(), value1);
        if (value2) cJSON_AddItemToObject(obj2, key.c_str(), value2);
    }
    
    return {obj1, obj2};
}

// Helper to create objects with different keys
static std::pair<cJSON*, cJSON*> create_objects_different_keys(FuzzedDataProvider& fdp) {
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    
    if (!obj1 || !obj2) {
        if (obj1) cJSON_Delete(obj1);
        if (obj2) cJSON_Delete(obj2);
        return {NULL, NULL};
    }
    
    // Create different keys for each object
    int num_keys1 = fdp.ConsumeIntegralInRange<int>(2, 5);
    int num_keys2 = fdp.ConsumeIntegralInRange<int>(2, 5);
    
    for (int i = 0; i < num_keys1; i++) {
        std::string key = "obj1_key_" + std::to_string(i);
        cJSON* value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (value) cJSON_AddItemToObject(obj1, key.c_str(), value);
    }
    
    for (int i = 0; i < num_keys2; i++) {
        std::string key = "obj2_key_" + std::to_string(i);
        cJSON* value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (value) cJSON_AddItemToObject(obj2, key.c_str(), value);
    }
    
    return {obj1, obj2};
}

// Helper to create arrays with same elements in different order
static std::pair<cJSON*, cJSON*> create_arrays_different_order(FuzzedDataProvider& fdp) {
    cJSON* arr1 = cJSON_CreateArray();
    cJSON* arr2 = cJSON_CreateArray();
    
    if (!arr1 || !arr2) {
        if (arr1) cJSON_Delete(arr1);
        if (arr2) cJSON_Delete(arr2);
        return {NULL, NULL};
    }
    
    // Create elements
    int array_size = fdp.ConsumeIntegralInRange<int>(3, 8);
    std::vector<cJSON*> elements;
    
    for (int i = 0; i < array_size; i++) {
        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        cJSON* elem = NULL;
        
        switch (elem_type) {
            case 0:
                elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                elem = cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str());
                break;
            default:
                elem = cJSON_CreateBool(fdp.ConsumeBool());
                break;
        }
        
        if (elem) {
            elements.push_back(elem);
            // Add to both arrays in same order initially
            cJSON_AddItemToArray(arr1, cJSON_Duplicate(elem, 1));
            cJSON_AddItemToArray(arr2, cJSON_Duplicate(elem, 1));
        }
    }
    
    // Clean up original elements
    for (cJSON* elem : elements) {
        cJSON_Delete(elem);
    }
    
    return {arr1, arr2};
}

// Helper to create arrays of different lengths
static std::pair<cJSON*, cJSON*> create_arrays_different_lengths(FuzzedDataProvider& fdp) {
    cJSON* arr1 = cJSON_CreateArray();
    cJSON* arr2 = cJSON_CreateArray();
    
    if (!arr1 || !arr2) {
        if (arr1) cJSON_Delete(arr1);
        if (arr2) cJSON_Delete(arr2);
        return {NULL, NULL};
    }
    
    // Create longer array
    int len1 = fdp.ConsumeIntegralInRange<int>(5, 10);
    int len2 = fdp.ConsumeIntegralInRange<int>(1, 4); // Shorter array
    
    for (int i = 0; i < len1; i++) {
        cJSON* elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (elem) cJSON_AddItemToArray(arr1, elem);
    }
    
    for (int i = 0; i < len2; i++) {
        cJSON* elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (elem) cJSON_AddItemToArray(arr2, elem);
    }
    
    return {arr1, arr2};
}

// Helper to create objects with case variations in keys
static std::pair<cJSON*, cJSON*> create_objects_case_variations(FuzzedDataProvider& fdp, bool same_case) {
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    
    if (!obj1 || !obj2) {
        if (obj1) cJSON_Delete(obj1);
        if (obj2) cJSON_Delete(obj2);
        return {NULL, NULL};
    }
    
    int num_keys = fdp.ConsumeIntegralInRange<int>(2, 6);
    
    for (int i = 0; i < num_keys; i++) {
        std::string base_key = "key" + std::to_string(i);
        std::string key1, key2;
        
        if (same_case) {
            key1 = base_key;
            key2 = base_key;
        } else {
            // Different case variations
            key1 = base_key;
            std::transform(base_key.begin(), base_key.end(), base_key.begin(), ::toupper);
            key2 = base_key;
        }
        
        cJSON* value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (value) {
            cJSON* value_copy = cJSON_Duplicate(value, 1);
            cJSON_AddItemToObject(obj1, key1.c_str(), value);
            cJSON_AddItemToObject(obj2, key2.c_str(), value_copy);
        }
    }
    
    return {obj1, obj2};
}

// Helper to create complex mixed structures (objects containing arrays containing objects...)
static cJSON* create_mixed_structure(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0) {
        return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
    }
    
    uint8_t struct_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (struct_type == 0) {
        // Create object
        cJSON* obj = cJSON_CreateObject();
        if (!obj) return NULL;
        
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_items; i++) {
            std::string key = fdp.ConsumeRandomLengthString(12);
            cJSON* value = create_mixed_structure(fdp, depth - 1);
            if (value) {
                cJSON_AddItemToObject(obj, key.c_str(), value);
            }
        }
        return obj;
    } else {
        // Create array
        cJSON* arr = cJSON_CreateArray();
        if (!arr) return NULL;
        
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_items; i++) {
            cJSON* value = create_mixed_structure(fdp, depth - 1);
            if (value) {
                cJSON_AddItemToArray(arr, value);
            }
        }
        return arr;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;  // Need minimum data for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Test different comparison scenarios
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    switch (test_scenario) {
        case 0: {
            // Deep nested objects comparison
            int depth = fdp.ConsumeIntegralInRange<int>(1, 4);
            cJSON* nested1 = create_deep_nested_object(fdp, depth);
            cJSON* nested2 = create_deep_nested_object(fdp, depth);
            
            if (nested1 && nested2) {
                cJSON_Compare(nested1, nested2, case_sensitive);
                // Also compare with itself
                cJSON_Compare(nested1, nested1, case_sensitive);
            }
            
            if (nested1) cJSON_Delete(nested1);
            if (nested2) cJSON_Delete(nested2);
            break;
        }
        
        case 1: {
            // Objects with same keys but different values
            auto [obj1, obj2] = create_objects_same_keys_different_values(fdp);
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, case_sensitive);
                // Compare each with itself
                cJSON_Compare(obj1, obj1, case_sensitive);
                cJSON_Compare(obj2, obj2, case_sensitive);
            }
            
            if (obj1) cJSON_Delete(obj1);
            if (obj2) cJSON_Delete(obj2);
            break;
        }
        
        case 2: {
            // Objects with different keys
            auto [obj1, obj2] = create_objects_different_keys(fdp);
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, case_sensitive);
                // Compare each with itself
                cJSON_Compare(obj1, obj1, case_sensitive);
                cJSON_Compare(obj2, obj2, case_sensitive);
            }
            
            if (obj1) cJSON_Delete(obj1);
            if (obj2) cJSON_Delete(obj2);
            break;
        }
        
        case 3: {
            // Arrays with same elements (should be equal)
            auto [arr1, arr2] = create_arrays_different_order(fdp);
            if (arr1 && arr2) {
                cJSON_Compare(arr1, arr2, case_sensitive);
                // Compare each with itself
                cJSON_Compare(arr1, arr1, case_sensitive);
                cJSON_Compare(arr2, arr2, case_sensitive);
            }
            
            if (arr1) cJSON_Delete(arr1);
            if (arr2) cJSON_Delete(arr2);
            break;
        }
        
        case 4: {
            // Arrays of different lengths
            auto [arr1, arr2] = create_arrays_different_lengths(fdp);
            if (arr1 && arr2) {
                cJSON_Compare(arr1, arr2, case_sensitive);
                // Compare each with itself
                cJSON_Compare(arr1, arr1, case_sensitive);
                cJSON_Compare(arr2, arr2, case_sensitive);
            }
            
            if (arr1) cJSON_Delete(arr1);
            if (arr2) cJSON_Delete(arr2);
            break;
        }
        
        case 5: {
            // Objects with case variations in keys
            bool same_case = fdp.ConsumeBool();
            auto [obj1, obj2] = create_objects_case_variations(fdp, same_case);
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, case_sensitive);
                // Compare each with itself
                cJSON_Compare(obj1, obj1, case_sensitive);
                cJSON_Compare(obj2, obj2, case_sensitive);
            }
            
            if (obj1) cJSON_Delete(obj1);
            if (obj2) cJSON_Delete(obj2);
            break;
        }
        
        case 6: {
            // Mixed structure comparison
            int depth = fdp.ConsumeIntegralInRange<int>(2, 4);
            cJSON* mixed1 = create_mixed_structure(fdp, depth);
            cJSON* mixed2 = create_mixed_structure(fdp, depth);
            
            if (mixed1 && mixed2) {
                cJSON_Compare(mixed1, mixed2, case_sensitive);
                // Compare with itself
                cJSON_Compare(mixed1, mixed1, case_sensitive);
                cJSON_Compare(mixed2, mixed2, case_sensitive);
            }
            
            if (mixed1) cJSON_Delete(mixed1);
            if (mixed2) cJSON_Delete(mixed2);
            break;
        }
        
        case 7: {
            // Parse complex JSON strings and compare
            if (fdp.remaining_bytes() > 100) {
                std::string json_str1 = fdp.ConsumeRandomLengthString(512);
                std::string json_str2 = fdp.ConsumeRandomLengthString(512);
                
                cJSON* parsed1 = cJSON_Parse(json_str1.c_str());
                cJSON* parsed2 = cJSON_Parse(json_str2.c_str());
                
                if (parsed1 && parsed2) {
                    cJSON_Compare(parsed1, parsed2, case_sensitive);
                    cJSON_Compare(parsed1, parsed1, case_sensitive);
                    cJSON_Compare(parsed2, parsed2, case_sensitive);
                }
                
                if (parsed1) cJSON_Delete(parsed1);
                if (parsed2) cJSON_Delete(parsed2);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_023
