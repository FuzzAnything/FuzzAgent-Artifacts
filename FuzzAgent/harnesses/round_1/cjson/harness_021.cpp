#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include "cjson/cJSON.h"

namespace harness_021 {
/*
 * harness_021.cpp - Comprehensive cJSON_Compare branch coverage testing
 * 
 * This harness targets the 71/76 blocked branches in cJSON_Compare (despite 822 hits).
 * Focus on exercising all comparison paths through systematic generation of
 * JSON object pairs with controlled differences to trigger specific branches.
 * 
 * Coverage gaps to address:
 * 1) Type-specific comparison branches for all JSON types
 * 2) String comparison paths (NULL strings, strcmp results, case sensitivity)
 * 3) Number comparison via compare_double function
 * 4) Array comparison edge cases (different lengths, element mismatches)
 * 5) Object comparison complexity (key mismatches, recursive comparisons)
 * 6) Mixed nesting scenarios with deep recursion
 * 7) Edge cases: identical objects, partial matches, complete mismatches
 * 
 * Strategy: Use FuzzedDataProvider to generate structured JSON pairs that
 * systematically test each comparison branch through controlled variations.
 */


extern "C" {
}

// Helper to create JSON objects with specific characteristics
static cJSON* create_json_with_type(FuzzedDataProvider& fdp, int json_type, bool create_valid) {
    if (!create_valid) {
        // Create potentially invalid or manipulated items
        uint8_t forced_type = fdp.ConsumeIntegral<uint8_t>() % 256;
        cJSON* item = (cJSON*)malloc(sizeof(cJSON));
        if (item) {
            memset(item, 0, sizeof(cJSON));
            item->type = forced_type;
            // Don't set child/next pointers to avoid crashes
        }
        return item;
    }
    
    switch (json_type) {
        case cJSON_False:
            return cJSON_CreateFalse();
            
        case cJSON_True:
            return cJSON_CreateTrue();
            
        case cJSON_NULL:
            return cJSON_CreateNull();
            
        case cJSON_Number: {
            // Generate numbers that exercise compare_double paths
            uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (num_type) {
                case 0: // Regular floating point
                    return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                case 1: // Special values (inf, nan)
                    return cJSON_CreateNumber(fdp.ConsumeBool() ? 
                        std::numeric_limits<double>::infinity() : 
                        -std::numeric_limits<double>::infinity());
                case 2: { // Very close numbers (to test epsilon comparison)
                    double base = fdp.ConsumeFloatingPoint<double>();
                    return cJSON_CreateNumber(base);
                }
                case 3: // Zero with different signs
                    return cJSON_CreateNumber(fdp.ConsumeBool() ? 0.0 : -0.0);
            }
            break;
        }
            
        case cJSON_String: {
            std::string str = fdp.ConsumeRandomLengthString(128);
            // Create strings that might be NULL or empty
            if (fdp.ConsumeBool() && !str.empty()) {
                // Sometimes create with NULL valuestring
                cJSON* item = cJSON_CreateString(nullptr);
                return item;
            }
            return cJSON_CreateString(str.c_str());
        }
            
        case cJSON_Raw: {
            std::string raw = fdp.ConsumeRandomLengthString(128);
            return cJSON_CreateRaw(raw.c_str());
        }
            
        case cJSON_Array: {
            cJSON* array = cJSON_CreateArray();
            if (array && fdp.ConsumeBool()) {
                // Add some elements
                int element_count = fdp.ConsumeIntegralInRange<int>(0, 5);
                for (int i = 0; i < element_count; i++) {
                    int elem_type = fdp.ConsumeIntegralInRange<int>(cJSON_False, cJSON_Object);
                    cJSON* elem = create_json_with_type(fdp, elem_type, true);
                    if (elem) {
                        cJSON_AddItemToArray(array, elem);
                    }
                }
            }
            return array;
        }
            
        case cJSON_Object: {
            cJSON* obj = cJSON_CreateObject();
            if (obj && fdp.ConsumeBool()) {
                // Add some key-value pairs
                int pair_count = fdp.ConsumeIntegralInRange<int>(0, 5);
                for (int i = 0; i < pair_count; i++) {
                    std::string key = fdp.ConsumeRandomLengthString(32);
                    int value_type = fdp.ConsumeIntegralInRange<int>(cJSON_False, cJSON_Object);
                    cJSON* value = create_json_with_type(fdp, value_type, true);
                    if (value) {
                        cJSON_AddItemToObject(obj, key.c_str(), value);
                    }
                }
            }
            return obj;
        }
    }
    
    return nullptr;
}

// Test specific comparison scenarios
static void test_comparison_scenarios(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Scenario 1: NULL pointer combinations
    cJSON_Compare(nullptr, nullptr, case_sensitive);
    
    cJSON* valid_item = create_json_with_type(fdp, cJSON_Number, true);
    if (valid_item) {
        cJSON_Compare(nullptr, valid_item, case_sensitive);
        cJSON_Compare(valid_item, nullptr, case_sensitive);
        cJSON_Delete(valid_item);
    }
    
    // Scenario 2: Type mismatches
    std::vector<cJSON*> items;
    for (int type = cJSON_False; type <= cJSON_Object; type++) {
        cJSON* item = create_json_with_type(fdp, type, true);
        if (item) items.push_back(item);
    }
    
    // Compare all pairs (including same type and different types)
    for (size_t i = 0; i < items.size(); i++) {
        for (size_t j = 0; j < items.size(); j++) {
            if (items[i] && items[j]) {
                cJSON_Compare(items[i], items[j], case_sensitive);
            }
        }
    }
    
    // Cleanup
    for (cJSON* item : items) {
        if (item) cJSON_Delete(item);
    }
    items.clear();
    
    // Scenario 3: String comparison edge cases
    cJSON* str1 = cJSON_CreateString("");
    cJSON* str2 = cJSON_CreateString("test");
    cJSON* str3 = cJSON_CreateString("TEST");
    cJSON* str4 = cJSON_CreateString("test");
    
    if (str1 && str2 && str3 && str4) {
        // Same strings
        cJSON_Compare(str2, str4, case_sensitive);
        // Different strings
        cJSON_Compare(str1, str2, case_sensitive);
        cJSON_Compare(str2, str3, case_sensitive);
        // Case sensitivity test
        cJSON_Compare(str2, str3, case_sensitive);
        cJSON_Compare(str2, str3, !case_sensitive);
    }
    
    if (str1) cJSON_Delete(str1);
    if (str2) cJSON_Delete(str2);
    if (str3) cJSON_Delete(str3);
    if (str4) cJSON_Delete(str4);
    
    // Scenario 4: Array comparison edge cases
    cJSON* empty_array = cJSON_CreateArray();
    cJSON* array1 = cJSON_CreateArray();
    cJSON* array2 = cJSON_CreateArray();
    
    if (array1 && array2) {
        // Add different elements to create mismatches
        cJSON_AddItemToArray(array1, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(array1, cJSON_CreateString("test"));
        
        cJSON_AddItemToArray(array2, cJSON_CreateNumber(2.0));
        cJSON_AddItemToArray(array2, cJSON_CreateString("test"));
        
        // Different length arrays
        cJSON_Compare(empty_array, array1, case_sensitive);
        cJSON_Compare(array1, empty_array, case_sensitive);
        
        // Same length, different elements
        cJSON_Compare(array1, array2, case_sensitive);
        
        // Arrays with nested structures
        cJSON* nested_array = cJSON_CreateArray();
        if (nested_array) {
            cJSON* inner_obj = cJSON_CreateObject();
            if (inner_obj) {
                cJSON_AddStringToObject(inner_obj, "key", "value");
                cJSON_AddItemToArray(nested_array, inner_obj);
            }
            cJSON_Compare(array1, nested_array, case_sensitive);
            cJSON_Delete(nested_array);
        }
    }
    
    if (empty_array) cJSON_Delete(empty_array);
    if (array1) cJSON_Delete(array1);
    if (array2) cJSON_Delete(array2);
    
    // Scenario 5: Object comparison complexity
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    cJSON* obj3 = cJSON_CreateObject();
    
    if (obj1 && obj2 && obj3) {
        // Same key-value pairs
        cJSON_AddStringToObject(obj1, "name", "value");
        cJSON_AddStringToObject(obj2, "name", "value");
        
        // Different values for same key
        cJSON_AddStringToObject(obj3, "name", "different");
        
        // Different keys
        cJSON_AddStringToObject(obj1, "extra", "data");
        
        // Test comparisons
        cJSON_Compare(obj1, obj2, case_sensitive); // Should be false (different keys)
        cJSON_Compare(obj2, obj3, case_sensitive); // Should be false (different values)
        
        // Case sensitivity in object keys
        cJSON* obj_case1 = cJSON_CreateObject();
        cJSON* obj_case2 = cJSON_CreateObject();
        if (obj_case1 && obj_case2) {
            cJSON_AddStringToObject(obj_case1, "Key", "value");
            cJSON_AddStringToObject(obj_case2, "key", "value");
            cJSON_Compare(obj_case1, obj_case2, case_sensitive);
            cJSON_Compare(obj_case1, obj_case2, !case_sensitive);
            cJSON_Delete(obj_case1);
            cJSON_Delete(obj_case2);
        }
    }
    
    if (obj1) cJSON_Delete(obj1);
    if (obj2) cJSON_Delete(obj2);
    if (obj3) cJSON_Delete(obj3);
    
    // Scenario 6: Deep nesting
    if (fdp.remaining_bytes() > 100) {
        cJSON* deep1 = cJSON_CreateObject();
        cJSON* deep2 = cJSON_CreateObject();
        
        if (deep1 && deep2) {
            // Create nested structure
            cJSON* inner_array1 = cJSON_CreateArray();
            cJSON* inner_array2 = cJSON_CreateArray();
            
            if (inner_array1 && inner_array2) {
                for (int i = 0; i < 3; i++) {
                    cJSON* nested_obj = cJSON_CreateObject();
                    if (nested_obj) {
                        cJSON_AddNumberToObject(nested_obj, "idx", i);
                        cJSON_AddItemToArray(inner_array1, nested_obj);
                        
                        // Create similar but slightly different object for deep2
                        cJSON* nested_obj2 = cJSON_CreateObject();
                        if (nested_obj2) {
                            cJSON_AddNumberToObject(nested_obj2, "idx", 
                                fdp.ConsumeBool() ? i : i + 1);
                            cJSON_AddItemToArray(inner_array2, nested_obj2);
                        }
                    }
                }
                
                cJSON_AddItemToObject(deep1, "data", inner_array1);
                cJSON_AddItemToObject(deep2, "data", inner_array2);
                
                // Compare deeply nested structures
                cJSON_Compare(deep1, deep2, case_sensitive);
            } else {
                if (inner_array1) cJSON_Delete(inner_array1);
                if (inner_array2) cJSON_Delete(inner_array2);
            }
        }
        
        if (deep1) cJSON_Delete(deep1);
        if (deep2) cJSON_Delete(deep2);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test both case-sensitive and case-insensitive modes
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    // Run comprehensive comparison scenarios
    test_comparison_scenarios(fdp, case_sensitive);
    
    // Additional random comparisons with generated JSON
    int num_comparisons = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_comparisons && fdp.remaining_bytes() > 16; i++) {
        int type1 = fdp.ConsumeIntegralInRange<int>(cJSON_False, cJSON_Object);
        int type2 = fdp.ConsumeIntegralInRange<int>(cJSON_False, cJSON_Object);
        
        bool valid1 = fdp.ConsumeBool();
        bool valid2 = fdp.ConsumeBool();
        
        cJSON* item1 = create_json_with_type(fdp, type1, valid1);
        cJSON* item2 = create_json_with_type(fdp, type2, valid2);
        
        if (item1 && item2) {
            cJSON_Compare(item1, item2, case_sensitive);
            
            // Also test with swapped case sensitivity
            if (fdp.ConsumeBool()) {
                cJSON_Compare(item1, item2, !case_sensitive);
            }
        }
        
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
    }
    
    return 0;
}

} // namespace harness_021
