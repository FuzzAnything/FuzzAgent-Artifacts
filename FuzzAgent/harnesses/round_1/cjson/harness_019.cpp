#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <climits>
#include <cfloat>
#include "cjson/cJSON.h"

namespace harness_019 {
/*
 * harness_019.cpp - cJSON_Compare invalid type and edge case testing harness
 * 
 * This harness targets the remaining 65 blocked branches in cJSON_Compare with
 * focus on areas not adequately covered by previous harnesses 010 and 011:
 * 1) Invalid type handling (cJSON_Invalid type, partially initialized items)
 * 2) Array length mismatches (arrays with different sizes, empty vs non-empty)
 * 3) Object comparison edge cases (different key sets, case-sensitive vs case-insensitive comparisons)
 * 4) Nested structure comparison edge cases (deep nesting with mixed types)
 * 5) Mixed type comparisons (string vs number, array vs object, boolean vs null, etc.)
 * 6) NULL pointer handling (various combinations of null pointers)
 * 7) Memory corruption scenarios (partially initialized cJSON items)
 * 
 * Strategy: Create cJSON items with manipulated types, test mixed type comparisons,
 * exercise object comparison edge cases, and test memory boundary conditions.
 */


extern "C" {
}

// Helper to create a cJSON item with potentially invalid type
static cJSON* create_item_with_type(FuzzedDataProvider& fdp, int forced_type) {
    // Create a normal item first
    uint8_t base_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    cJSON* item = nullptr;
    
    switch (base_type) {
        case 0:
            item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            break;
        case 1:
            item = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
            break;
        case 2:
            item = cJSON_CreateBool(fdp.ConsumeBool());
            break;
        case 3:
            item = cJSON_CreateNull();
            break;
        case 4:
            item = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(64).c_str());
            break;
        case 5:
            item = cJSON_CreateArray();
            break;
        case 6:
            item = cJSON_CreateObject();
            break;
        case 7:
            item = cJSON_CreateTrue();
            break;
    }
    
    if (item && forced_type >= 0) {
        // Manipulate the type field to create invalid or mixed types
        item->type = forced_type;
    }
    
    return item;
}

// Test mixed type comparisons
static void test_mixed_type_comparisons(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Create items of different types and compare them
    std::vector<cJSON*> items;
    
    // Create various type items
    items.push_back(cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    items.push_back(cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
    items.push_back(cJSON_CreateBool(fdp.ConsumeBool()));
    items.push_back(cJSON_CreateNull());
    items.push_back(cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str()));
    items.push_back(cJSON_CreateArray());
    items.push_back(cJSON_CreateObject());
    items.push_back(cJSON_CreateTrue());
    items.push_back(cJSON_CreateFalse());
    
    // Compare all pairs of different types
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
}

// Test array length mismatch scenarios
static void test_array_length_mismatches(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Test empty vs non-empty arrays
    cJSON* empty_array = cJSON_CreateArray();
    cJSON* non_empty_array = cJSON_CreateArray();
    
    if (non_empty_array && fdp.remaining_bytes() > 8) {
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 10);
        for (int i = 0; i < num_items && fdp.remaining_bytes() > 8; i++) {
            if (i % 2 == 0) {
                cJSON_AddItemToArray(non_empty_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            } else {
                cJSON_AddItemToArray(non_empty_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(20).c_str()));
            }
        }
    }
    
    // Compare empty vs non-empty
    if (empty_array && non_empty_array) {
        cJSON_Compare(empty_array, non_empty_array, case_sensitive);
        cJSON_Compare(non_empty_array, empty_array, case_sensitive);
    }
    
    // Test arrays with different lengths but same prefix
    cJSON* array1 = cJSON_CreateArray();
    cJSON* array2 = cJSON_CreateArray();
    
    if (array1 && array2 && fdp.remaining_bytes() > 16) {
        int len1 = fdp.ConsumeIntegralInRange<int>(2, 8);
        int len2 = fdp.ConsumeIntegralInRange<int>(len1 + 1, len1 + 4);
        
        // Add same items to both arrays for first len1 items
        for (int i = 0; i < len1 && fdp.remaining_bytes() > 8; i++) {
            double num = fdp.ConsumeFloatingPoint<double>();
            cJSON* num_item = cJSON_CreateNumber(num);
            if (num_item) {
                cJSON_AddItemToArray(array1, num_item);
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(num));
            }
        }
        
        // Add extra items to array2
        for (int i = len1; i < len2 && fdp.remaining_bytes() > 8; i++) {
            cJSON_AddItemToArray(array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        
        cJSON_Compare(array1, array2, case_sensitive);
        cJSON_Compare(array2, array1, case_sensitive);
    }
    
    if (empty_array) cJSON_Delete(empty_array);
    if (non_empty_array) cJSON_Delete(non_empty_array);
    if (array1) cJSON_Delete(array1);
    if (array2) cJSON_Delete(array2);
}

// Test object comparison edge cases
static void test_object_edge_cases(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Test objects with same keys but different values
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    
    if (obj1 && obj2 && fdp.remaining_bytes() > 32) {
        int num_keys = fdp.ConsumeIntegralInRange<int>(2, 8);
        
        for (int i = 0; i < num_keys && fdp.remaining_bytes() > 16; i++) {
            std::string key = "key_" + std::to_string(i);
            
            // Add same key with different values
            cJSON_AddNumberToObject(obj1, key.c_str(), fdp.ConsumeFloatingPoint<double>());
            cJSON_AddNumberToObject(obj2, key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        cJSON_Compare(obj1, obj2, case_sensitive);
    }
    
    // Test objects with different key sets (subset/superset relationship)
    cJSON* parent_obj = cJSON_CreateObject();
    cJSON* child_obj = cJSON_CreateObject();
    
    if (parent_obj && child_obj && fdp.remaining_bytes() > 32) {
        int parent_keys = fdp.ConsumeIntegralInRange<int>(3, 6);
        int child_keys = fdp.ConsumeIntegralInRange<int>(1, parent_keys - 1);
        
        // Add keys to parent
        for (int i = 0; i < parent_keys && fdp.remaining_bytes() > 16; i++) {
            std::string key = "parent_key_" + std::to_string(i);
            cJSON_AddNumberToObject(parent_obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        // Add subset of keys to child
        for (int i = 0; i < child_keys && fdp.remaining_bytes() > 16; i++) {
            std::string key = "parent_key_" + std::to_string(i);
            // Try to get the same value from parent (simulate copying)
            cJSON* parent_item = cJSON_GetObjectItem(parent_obj, key.c_str());
            if (parent_item) {
                cJSON_AddNumberToObject(child_obj, key.c_str(), parent_item->valuedouble);
            }
        }
        
        cJSON_Compare(parent_obj, child_obj, case_sensitive);
        cJSON_Compare(child_obj, parent_obj, case_sensitive);
    }
    
    // Test case-sensitive vs case-insensitive comparisons
    if (fdp.remaining_bytes() > 16) {
        cJSON* case_obj1 = cJSON_CreateObject();
        cJSON* case_obj2 = cJSON_CreateObject();
        
        if (case_obj1 && case_obj2) {
            // Add keys that differ only in case
            cJSON_AddNumberToObject(case_obj1, "KeyName", 42.0);
            cJSON_AddNumberToObject(case_obj2, "keyname", 42.0);
            cJSON_AddNumberToObject(case_obj1, "ANOTHER", 100.0);
            cJSON_AddNumberToObject(case_obj2, "another", 100.0);
            
            // Test both case-sensitive and case-insensitive
            cJSON_Compare(case_obj1, case_obj2, 1);  // case-sensitive
            cJSON_Compare(case_obj1, case_obj2, 0);  // case-insensitive
        }
        
        if (case_obj1) cJSON_Delete(case_obj1);
        if (case_obj2) cJSON_Delete(case_obj2);
    }
    
    if (obj1) cJSON_Delete(obj1);
    if (obj2) cJSON_Delete(obj2);
    if (parent_obj) cJSON_Delete(parent_obj);
    if (child_obj) cJSON_Delete(child_obj);
}

// Test NULL pointer handling
static void test_null_pointer_handling(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Various NULL pointer combinations
    cJSON* valid_item = cJSON_CreateNumber(42.0);
    
    // Test all NULL pointer combinations
    cJSON_Compare(nullptr, nullptr, case_sensitive);
    if (valid_item) {
        cJSON_Compare(valid_item, nullptr, case_sensitive);
        cJSON_Compare(nullptr, valid_item, case_sensitive);
        
        // Test with partially NULL strings in string items
        cJSON* string_with_null = cJSON_CreateString("test");
        if (string_with_null) {
            // Can't easily set valuestring to NULL without causing issues,
            // but we can test the API with valid strings
            cJSON_Compare(string_with_null, valid_item, case_sensitive);
            cJSON_Compare(valid_item, string_with_null, case_sensitive);
            cJSON_Delete(string_with_null);
        }
    }
    
    if (valid_item) cJSON_Delete(valid_item);
}

// Test nested structure edge cases
static void test_nested_edge_cases(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Create deeply nested structures with mixed types
    cJSON* root1 = cJSON_CreateObject();
    cJSON* root2 = cJSON_CreateObject();
    
    if (root1 && root2 && fdp.remaining_bytes() > 64) {
        // Add nested array to object
        cJSON* nested_array1 = cJSON_CreateArray();
        cJSON* nested_array2 = cJSON_CreateArray();
        
        if (nested_array1 && nested_array2) {
            // Fill arrays with mixed types at different nesting levels
            int depth = fdp.ConsumeIntegralInRange<int>(1, 4);
            
            cJSON* current1 = nested_array1;
            cJSON* current2 = nested_array2;
            
            for (int level = 0; level < depth && fdp.remaining_bytes() > 16; level++) {
                // Add a simple value
                cJSON_AddItemToArray(current1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(current2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                
                // Create nested array for next level
                if (level < depth - 1) {
                    cJSON* next_array1 = cJSON_CreateArray();
                    cJSON* next_array2 = cJSON_CreateArray();
                    
                    if (next_array1 && next_array2) {
                        cJSON_AddItemToArray(current1, next_array1);
                        cJSON_AddItemToArray(current2, next_array2);
                        current1 = next_array1;
                        current2 = next_array2;
                    }
                }
            }
            
            cJSON_AddItemToObject(root1, "nested", nested_array1);
            cJSON_AddItemToObject(root2, "nested", nested_array2);
        }
        
        // Test comparison of nested structures
        cJSON_Compare(root1, root2, case_sensitive);
        
        // Modify one structure slightly and test again
        if (fdp.ConsumeBool() && nested_array1 && nested_array1->child) {
            // Change a value in the nested structure
            cJSON* first_elem = nested_array1->child;
            if (first_elem && (first_elem->type & 0xFF) == cJSON_Number) {
                first_elem->valuedouble += 0.001;
                cJSON_Compare(root1, root2, case_sensitive);
            }
        }
    }
    
    if (root1) cJSON_Delete(root1);
    if (root2) cJSON_Delete(root2);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume case sensitivity flag
    cJSON_bool case_sensitive = fdp.ConsumeBool() ? 1 : 0;
    
    // Test scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (scenario) {
        case 0:
            // Test mixed type comparisons
            test_mixed_type_comparisons(fdp, case_sensitive);
            break;
            
        case 1:
            // Test array length mismatches
            test_array_length_mismatches(fdp, case_sensitive);
            break;
            
        case 2:
            // Test object edge cases
            test_object_edge_cases(fdp, case_sensitive);
            break;
            
        case 3:
            // Test NULL pointer handling
            test_null_pointer_handling(fdp, case_sensitive);
            break;
            
        case 4:
            // Test nested edge cases
            test_nested_edge_cases(fdp, case_sensitive);
            break;
            
        case 5: {
            // Test invalid type handling and memory corruption scenarios
            // Create items with manipulated types
            std::vector<int> invalid_types = {
                cJSON_Invalid,  // 0
                255,            // Invalid type value
                128,            // Another invalid value
                cJSON_Number | cJSON_String,  // Combined type (invalid)
                cJSON_Array | cJSON_Object,   // Another combined type
            };
            
            for (int invalid_type : invalid_types) {
                cJSON* item1 = create_item_with_type(fdp, invalid_type);
                cJSON* item2 = create_item_with_type(fdp, -1);  // Normal item
                
                if (item1 && item2) {
                    cJSON_Compare(item1, item2, case_sensitive);
                    cJSON_Compare(item2, item1, case_sensitive);
                    cJSON_Compare(item1, item1, case_sensitive);
                }
                
                if (item1) cJSON_Delete(item1);
                if (item2) cJSON_Delete(item2);
                
                if (fdp.remaining_bytes() < 16) break;
            }
            
            // Test identical pointer comparison (a == b)
            if (fdp.remaining_bytes() > 8) {
                cJSON* item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (item) {
                    cJSON_Compare(item, item, case_sensitive);
                    cJSON_Delete(item);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_019
