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

namespace harness_025 {
/*
 * harness_025.cpp - Targeted cJSON_Compare branch coverage testing with focus on
 *                   complex nested object comparison and subset detection
 * 
 * This harness targets the 73/76 blocked branches in cJSON_Compare with a focus on:
 * 1) Complex nested objects with mixed types at multiple levels
 * 2) Variations with deliberate differences at specific nesting depths
 * 3) Exact duplicates for equality testing paths
 * 4) Objects with missing properties to test subset comparison logic
 * 5) Case-sensitive vs case-insensitive comparison paths
 * 
 * Semantic differentiation from harness_021/022:
 * - Focus on nested variation patterns (not just flat object relationships)
 * - Systematic difference placement at different recursion depths
 * - Testing the double-loop subset detection more thoroughly
 * - Mixed container types (objects containing arrays containing objects, etc.)
 * 
 * Coverage gaps specifically targeted:
 * 1) The O(n^2) object comparison algorithm (lines 3162-3191 in cJSON.c)
 * 2) Recursive comparison with mixed type nesting
 * 3) compare_double epsilon comparisons with nested numeric values
 * 4) String comparison with case sensitivity in nested contexts
 * 5) All type validation branches in the initial switch
 */


extern "C" {
}

// Helper to create a complex nested JSON structure
static cJSON* create_complex_nested_object(FuzzedDataProvider& fdp, int max_depth, int current_depth = 0) {
    if (current_depth >= max_depth) {
        // Create leaf node with random type
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (leaf_type) {
            case 0:
                return cJSON_CreateNull();
            case 1:
                return cJSON_CreateBool(fdp.ConsumeBool());
            case 2:
                return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 3:
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 4:
                return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
            case 5:
                return cJSON_CreateFalse();
            default:
                return cJSON_CreateNull();
        }
    }
    
    uint8_t container_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    if (container_type == 0) {
        // Create array with mixed elements
        cJSON* array = cJSON_CreateArray();
        if (array) {
            int num_elements = fdp.ConsumeIntegralInRange<int>(1, 4);
            for (int i = 0; i < num_elements; i++) {
                cJSON* element = create_complex_nested_object(fdp, max_depth, current_depth + 1);
                if (element) {
                    cJSON_AddItemToArray(array, element);
                }
            }
        }
        return array;
    } else {
        // Create object with mixed key-value pairs
        cJSON* obj = cJSON_CreateObject();
        if (obj) {
            int num_keys = fdp.ConsumeIntegralInRange<int>(2, 5);
            for (int i = 0; i < num_keys; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                cJSON* value = create_complex_nested_object(fdp, max_depth, current_depth + 1);
                if (value) {
                    cJSON_AddItemToObject(obj, key.c_str(), value);
                }
            }
        }
        return obj;
    }
}

// Create a variation of an object with specific differences
static cJSON* create_variation(FuzzedDataProvider& fdp, cJSON* original, int depth_to_modify, int current_depth = 0) {
    if (!original) return nullptr;
    
    // If we're at the target depth, modify this node
    if (current_depth == depth_to_modify) {
        // Create a completely different node at this level
        return create_complex_nested_object(fdp, 1); // Leaf node
    }
    
    // Duplicate the original
    cJSON* duplicate = cJSON_Duplicate(original, 1);
    if (!duplicate) return nullptr;
    
    // Recursively process children if this is a container
    if ((duplicate->type & 0xFF) == cJSON_Array || (duplicate->type & 0xFF) == cJSON_Object) {
        cJSON* child = duplicate->child;
        while (child) {
            cJSON* next = child->next;
            cJSON* modified_child = create_variation(fdp, child, depth_to_modify, current_depth + 1);
            if (modified_child && modified_child != child) {
                // Replace the child with modified version
                // For simplicity, we'll just delete and add (but this is complex)
                // Instead, we'll create a new variation from scratch
                cJSON_Delete(duplicate);
                return create_complex_nested_object(fdp, depth_to_modify + 1);
            }
            child = next;
        }
    }
    
    return duplicate;
}

// Create object with missing properties (subset relationship)
static void create_subset_objects(FuzzedDataProvider& fdp, cJSON** superset, cJSON** subset) {
    *superset = cJSON_CreateObject();
    *subset = cJSON_CreateObject();
    
    if (!*superset || !*subset) {
        if (*superset) cJSON_Delete(*superset);
        if (*subset) cJSON_Delete(*subset);
        *superset = *subset = nullptr;
        return;
    }
    
    // Create common properties
    int num_common = fdp.ConsumeIntegralInRange<int>(2, 4);
    for (int i = 0; i < num_common; i++) {
        std::string key = fdp.ConsumeRandomLengthString(12);
        cJSON* value = create_complex_nested_object(fdp, 2); // Depth 2
        
        if (value) {
            cJSON_AddItemToObject(*superset, key.c_str(), cJSON_Duplicate(value, 1));
            cJSON_AddItemToObject(*subset, key.c_str(), value);
        }
    }
    
    // Add extra properties only to superset
    int num_extra = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < num_extra; i++) {
        std::string key = fdp.ConsumeRandomLengthString(12);
        cJSON* value = create_complex_nested_object(fdp, 2);
        
        if (value) {
            cJSON_AddItemToObject(*superset, key.c_str(), value);
        }
    }
}

// Test case 1: Complex nested objects with variations at different depths
static void test_nested_variations(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 100) return;
    
    // Create base complex object
    int max_depth = fdp.ConsumeIntegralInRange<int>(2, 4);
    cJSON* base = create_complex_nested_object(fdp, max_depth);
    
    if (!base) return;
    
    // Test 1a: Compare with exact duplicate (should be equal)
    cJSON* duplicate = cJSON_Duplicate(base, 1);
    if (duplicate) {
        cJSON_Compare(base, duplicate, case_sensitive);
        cJSON_Compare(base, duplicate, !case_sensitive); // Test opposite case sensitivity
        cJSON_Delete(duplicate);
    }
    
    // Test 1b: Create variations with differences at different depths
    for (int depth = 0; depth < max_depth; depth++) {
        cJSON* variation = create_variation(fdp, base, depth);
        if (variation) {
            cJSON_Compare(base, variation, case_sensitive);
            cJSON_Compare(variation, base, case_sensitive); // Test symmetry
            
            // Also test with swapped case sensitivity
            if (fdp.ConsumeBool()) {
                cJSON_Compare(base, variation, !case_sensitive);
            }
            
            cJSON_Delete(variation);
        }
    }
    
    // Test 1c: Create completely different object for comparison
    cJSON* different = create_complex_nested_object(fdp, max_depth);
    if (different) {
        cJSON_Compare(base, different, case_sensitive);
        cJSON_Delete(different);
    }
    
    cJSON_Delete(base);
}

// Test case 2: Subset/superset relationships
static void test_subset_relationships(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 50) return;
    
    cJSON* superset = nullptr;
    cJSON* subset = nullptr;
    
    create_subset_objects(fdp, &superset, &subset);
    
    if (superset && subset) {
        // subset should be compared as not equal to superset (missing properties)
        cJSON_Compare(superset, subset, case_sensitive);
        cJSON_Compare(subset, superset, case_sensitive);
        
        // Test case-insensitive version
        cJSON_Compare(superset, subset, !case_sensitive);
        
        // Create another subset with different extra properties
        cJSON* subset2 = nullptr;
        cJSON* superset2 = nullptr;
        create_subset_objects(fdp, &superset2, &subset2);
        
        if (superset2 && subset2) {
            // These should not be equal either
            cJSON_Compare(superset, superset2, case_sensitive);
            cJSON_Compare(subset, subset2, case_sensitive);
            
            cJSON_Delete(superset2);
            cJSON_Delete(subset2);
        }
    }
    
    if (superset) cJSON_Delete(superset);
    if (subset) cJSON_Delete(subset);
}

// Test case 3: Mixed type comparisons at different levels
static void test_mixed_type_comparisons(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 80) return;
    
    // Create objects with specifically mixed types at different levels
    cJSON* mixed1 = cJSON_CreateObject();
    cJSON* mixed2 = cJSON_CreateObject();
    
    if (!mixed1 || !mixed2) {
        if (mixed1) cJSON_Delete(mixed1);
        if (mixed2) cJSON_Delete(mixed2);
        return;
    }
    
    // Add array containing objects containing arrays
    cJSON* nested_array1 = cJSON_CreateArray();
    if (nested_array1) {
        cJSON* obj_in_array = cJSON_CreateObject();
        if (obj_in_array) {
            cJSON* inner_array = cJSON_CreateArray();
            if (inner_array) {
                cJSON_AddItemToArray(inner_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(inner_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(20).c_str()));
                cJSON_AddItemToObject(obj_in_array, "inner", inner_array);
            }
            cJSON_AddItemToObject(obj_in_array, "flag", cJSON_CreateBool(fdp.ConsumeBool()));
            cJSON_AddItemToArray(nested_array1, obj_in_array);
        }
        cJSON_AddItemToObject(mixed1, "complex_array", nested_array1);
    }
    
    // Create similar but slightly different structure for mixed2
    cJSON* nested_array2 = cJSON_CreateArray();
    if (nested_array2) {
        cJSON* obj_in_array = cJSON_CreateObject();
        if (obj_in_array) {
            cJSON* inner_array = cJSON_CreateArray();
            if (inner_array) {
                cJSON_AddItemToArray(inner_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                // Different string content
                cJSON_AddItemToArray(inner_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(20).c_str()));
                cJSON_AddItemToObject(obj_in_array, "inner", inner_array);
            }
            cJSON_AddItemToObject(obj_in_array, "flag", cJSON_CreateBool(fdp.ConsumeBool()));
            cJSON_AddItemToArray(nested_array2, obj_in_array);
        }
        cJSON_AddItemToObject(mixed2, "complex_array", nested_array2);
    }
    
    // Add some additional properties with case variations
    std::string key1 = "Key";
    std::string key2 = "key"; // Different case
    
    cJSON_AddItemToObject(mixed1, key1.c_str(), cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    cJSON_AddItemToObject(mixed2, key2.c_str(), cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    
    // Compare with both case sensitivities
    cJSON_Compare(mixed1, mixed2, true);  // case-sensitive
    cJSON_Compare(mixed1, mixed2, false); // case-insensitive
    
    cJSON_Delete(mixed1);
    cJSON_Delete(mixed2);
}

// Test case 4: Edge cases for string comparison with NULL values
static void test_string_edge_cases(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Create strings that might trigger edge cases
    cJSON* str1 = cJSON_CreateString(fdp.ConsumeRandomLengthString(50).c_str());
    cJSON* str2 = cJSON_CreateString(fdp.ConsumeRandomLengthString(50).c_str());
    
    if (str1 && str2) {
        cJSON_Compare(str1, str2, case_sensitive);
        
        // Test with exact same strings
        std::string same_str = fdp.ConsumeRandomLengthString(30);
        cJSON* same1 = cJSON_CreateString(same_str.c_str());
        cJSON* same2 = cJSON_CreateString(same_str.c_str());
        
        if (same1 && same2) {
            cJSON_Compare(same1, same2, case_sensitive);
            cJSON_Delete(same1);
            cJSON_Delete(same2);
        }
        
        // Test case variations
        std::string lower = "teststring";
        std::string upper = "TESTSTRING";
        cJSON* lower_str = cJSON_CreateString(lower.c_str());
        cJSON* upper_str = cJSON_CreateString(upper.c_str());
        
        if (lower_str && upper_str) {
            cJSON_Compare(lower_str, upper_str, true);  // case-sensitive: false
            cJSON_Compare(lower_str, upper_str, false); // case-insensitive: true (depends on implementation)
            cJSON_Delete(lower_str);
            cJSON_Delete(upper_str);
        }
    }
    
    if (str1) cJSON_Delete(str1);
    if (str2) cJSON_Delete(str2);
}

// Test case 5: Numeric comparison edge cases
static void test_numeric_edge_cases(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Test special floating point values
    double special_values[] = {
        0.0, -0.0, 
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::quiet_NaN(),
        DBL_MIN, DBL_MAX,
        1.0, -1.0, 1e-10, -1e-10
    };
    
    for (size_t i = 0; i < sizeof(special_values)/sizeof(special_values[0]) && fdp.remaining_bytes() > 10; i++) {
        for (size_t j = 0; j < sizeof(special_values)/sizeof(special_values[0]); j++) {
            cJSON* num1 = cJSON_CreateNumber(special_values[i]);
            cJSON* num2 = cJSON_CreateNumber(special_values[j]);
            
            if (num1 && num2) {
                cJSON_Compare(num1, num2, case_sensitive);
                cJSON_Delete(num1);
                cJSON_Delete(num2);
            }
        }
    }
    
    // Test numbers very close together (epsilon comparison)
    double base = fdp.ConsumeFloatingPoint<double>();
    cJSON* num1 = cJSON_CreateNumber(base);
    cJSON* num2 = cJSON_CreateNumber(base + DBL_EPSILON * 0.5); // Within epsilon
    cJSON* num3 = cJSON_CreateNumber(base + DBL_EPSILON * 2.0); // Outside epsilon
    
    if (num1 && num2) {
        cJSON_Compare(num1, num2, case_sensitive);
        cJSON_Delete(num1);
        num1 = NULL;  // Set to NULL after deletion to prevent double-free
        cJSON_Delete(num2);
    }
    
    if (num3) {
        // Don't check num1 here since it may have been deleted
        // Create a new num1 for comparison with num3
        cJSON* new_num1 = cJSON_CreateNumber(base);
        if (new_num1) {
            cJSON_Compare(new_num1, num3, case_sensitive);
            cJSON_Delete(new_num1);
        }
        cJSON_Delete(num3);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine case sensitivity from input
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    // Run all test cases
    test_nested_variations(fdp, case_sensitive);
    test_subset_relationships(fdp, case_sensitive);
    test_mixed_type_comparisons(fdp, case_sensitive);
    test_string_edge_cases(fdp, case_sensitive);
    test_numeric_edge_cases(fdp, case_sensitive);
    
    // Additional random comparisons to fill gaps
    int num_random = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_random && fdp.remaining_bytes() > 20; i++) {
        cJSON* item1 = create_complex_nested_object(fdp, fdp.ConsumeIntegralInRange<int>(1, 3));
        cJSON* item2 = create_complex_nested_object(fdp, fdp.ConsumeIntegralInRange<int>(1, 3));
        
        if (item1 && item2) {
            cJSON_Compare(item1, item2, case_sensitive);
            if (fdp.ConsumeBool()) {
                cJSON_Compare(item1, item2, !case_sensitive);
            }
            
            cJSON_Delete(item1);
            cJSON_Delete(item2);
        }
    }
    
    return 0;
}

} // namespace harness_025
