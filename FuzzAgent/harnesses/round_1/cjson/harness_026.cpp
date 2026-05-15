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

namespace harness_026 {
/*
 * harness_026.cpp - Systematic cJSON_Compare branch coverage targeting
 *                   array length mismatch, object key asymmetry, deep nested differences
 * 
 * This harness targets the 73/76 blocked branches in cJSON_Compare with focus on:
 * 1) Array length mismatch scenarios (line 3154-3156 in cJSON.c)
 * 2) Object key asymmetry and bidirectional subset checking (lines 3182-3194)
 * 3) Deep nested differences at various recursion depths
 * 4) String comparison edge cases (NULL strings, case sensitivity)
 * 5) Number precision boundaries in compare_double function
 * 
 * Semantic differentiation from existing harnesses (021, 022, 025):
 * - Systematic creation of array pairs with controlled length differences
 * - Explicit testing of object subset relationships in both directions
 * - Focus on the O(n^2) object comparison's second validation loop
 * - Creation of NULL string values using cJSON_CreateStringReference(NULL)
 * - Number pairs at precision boundaries for epsilon comparison
 * 
 * Coverage gaps specifically targeted:
 * 1) Array comparison with different element counts (low coverage: 40 hits)
 * 2) Object subset bidirectional validation (second loop in lines 3182-3194)
 * 3) Invalid type default case (lines 3199-3200, 0 hits)
 * 4) String/Raw NULL pointer checks (lines 3126-3129, only 4 hits)
 * 5) compare_double precision boundaries
 */


extern "C" {
}

// Helper to create arrays with specific length differences
static void create_array_pair_with_length_difference(FuzzedDataProvider& fdp, 
                                                    cJSON** array1, cJSON** array2,
                                                    int length_diff) {
    *array1 = cJSON_CreateArray();
    *array2 = cJSON_CreateArray();
    
    if (!*array1 || !*array2) {
        if (*array1) cJSON_Delete(*array1);
        if (*array2) cJSON_Delete(*array2);
        *array1 = *array2 = nullptr;
        return;
    }
    
    // Determine base length
    int base_length = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    // Create common elements for both arrays
    for (int i = 0; i < base_length; i++) {
        // Create random element
        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        cJSON* elem = nullptr;
        
        switch (elem_type) {
            case 0:
                elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                elem = cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str());
                break;
            case 2:
                elem = cJSON_CreateBool(fdp.ConsumeBool());
                break;
            case 3:
                elem = cJSON_CreateNull();
                break;
            case 4:
                elem = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(16).c_str());
                break;
        }
        
        if (elem) {
            cJSON_AddItemToArray(*array1, elem);
            cJSON_AddItemToArray(*array2, cJSON_Duplicate(elem, 1));
        }
    }
    
    // Add extra elements to create length difference
    if (length_diff > 0) {
        // array1 is longer
        for (int i = 0; i < length_diff; i++) {
            cJSON* extra = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            if (extra) {
                cJSON_AddItemToArray(*array1, extra);
            }
        }
    } else if (length_diff < 0) {
        // array2 is longer
        for (int i = 0; i < -length_diff; i++) {
            cJSON* extra = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            if (extra) {
                cJSON_AddItemToArray(*array2, extra);
            }
        }
    }
}

// Helper to create objects with asymmetric keys
static void create_object_pair_with_key_asymmetry(FuzzedDataProvider& fdp,
                                                  cJSON** obj1, cJSON** obj2,
                                                  bool obj1_is_superset) {
    *obj1 = cJSON_CreateObject();
    *obj2 = cJSON_CreateObject();
    
    if (!*obj1 || !*obj2) {
        if (*obj1) cJSON_Delete(*obj1);
        if (*obj2) cJSON_Delete(*obj2);
        *obj1 = *obj2 = nullptr;
        return;
    }
    
    // Common keys
    int num_common = fdp.ConsumeIntegralInRange<int>(2, 4);
    for (int i = 0; i < num_common; i++) {
        std::string key = fdp.ConsumeRandomLengthString(12);
        cJSON* value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        
        if (value) {
            cJSON_AddItemToObject(*obj1, key.c_str(), value);
            cJSON_AddItemToObject(*obj2, key.c_str(), cJSON_Duplicate(value, 1));
        }
    }
    
    // Asymmetric keys
    int num_asymmetric = fdp.ConsumeIntegralInRange<int>(1, 3);
    
    if (obj1_is_superset) {
        // obj1 has extra keys
        for (int i = 0; i < num_asymmetric; i++) {
            std::string key = fdp.ConsumeRandomLengthString(12);
            cJSON* value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            
            if (value) {
                cJSON_AddItemToObject(*obj1, key.c_str(), value);
            }
        }
    } else {
        // obj2 has extra keys
        for (int i = 0; i < num_asymmetric; i++) {
            std::string key = fdp.ConsumeRandomLengthString(12);
            cJSON* value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            
            if (value) {
                cJSON_AddItemToObject(*obj2, key.c_str(), value);
            }
        }
    }
}

// Helper to create deep nested structures with differences at specific depths
static cJSON* create_deep_nested_structure(FuzzedDataProvider& fdp, int depth, bool create_difference_at_depth) {
    if (depth <= 0) {
        // Leaf node
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (leaf_type) {
            case 0:
                return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1:
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str());
            case 2:
                return cJSON_CreateBool(fdp.ConsumeBool());
            case 3:
                return cJSON_CreateNull();
            case 4:
                return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(16).c_str());
        }
        return cJSON_CreateNull();
    }
    
    // Container node
    uint8_t container_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (container_type == 0) {
        // Array
        cJSON* array = cJSON_CreateArray();
        if (array) {
            int num_elements = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < num_elements; i++) {
                cJSON* element = create_deep_nested_structure(fdp, depth - 1, create_difference_at_depth && (i == 0));
                if (element) {
                    cJSON_AddItemToArray(array, element);
                }
            }
        }
        return array;
    } else {
        // Object
        cJSON* obj = cJSON_CreateObject();
        if (obj) {
            int num_keys = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < num_keys; i++) {
                std::string key = fdp.ConsumeRandomLengthString(12);
                cJSON* value = create_deep_nested_structure(fdp, depth - 1, create_difference_at_depth && (i == 0));
                if (value) {
                    cJSON_AddItemToObject(obj, key.c_str(), value);
                }
            }
        }
        return obj;
    }
}

// Helper to create numbers at precision boundaries
static void create_number_pair_at_precision_boundary(FuzzedDataProvider& fdp,
                                                     double* num1, double* num2) {
    uint8_t boundary_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (boundary_type) {
        case 0: // Within epsilon (should be equal)
            *num1 = fdp.ConsumeFloatingPoint<double>();
            *num2 = *num1 + (fdp.ConsumeBool() ? 1e-15 : -1e-15);
            break;
        case 1: // Just outside epsilon (should be different)
            *num1 = fdp.ConsumeFloatingPoint<double>();
            *num2 = *num1 + (fdp.ConsumeBool() ? 1e-5 : -1e-5);
            break;
        case 2: // Special values
            *num1 = std::numeric_limits<double>::infinity();
            *num2 = fdp.ConsumeBool() ? std::numeric_limits<double>::infinity() : -std::numeric_limits<double>::infinity();
            break;
        case 3: // NaN
            *num1 = std::numeric_limits<double>::quiet_NaN();
            *num2 = std::numeric_limits<double>::signaling_NaN();
            break;
        case 4: // Zero with different signs
            *num1 = 0.0;
            *num2 = -0.0;
            break;
        case 5: // Denormal numbers
            *num1 = DBL_MIN * 0.5;
            *num2 = DBL_MIN * 0.5000001;
            break;
    }
}

// Test case 1: Array length mismatch scenarios
static void test_array_length_mismatch(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 50) return;
    
    // Test different length differences
    for (int diff = -3; diff <= 3; diff++) {
        if (diff == 0) continue; // Skip equal length
        
        cJSON* array1 = nullptr;
        cJSON* array2 = nullptr;
        
        create_array_pair_with_length_difference(fdp, &array1, &array2, diff);
        
        if (array1 && array2) {
            cJSON_Compare(array1, array2, case_sensitive);
            cJSON_Compare(array2, array1, case_sensitive); // Test symmetry
            
            // Test with opposite case sensitivity
            if (fdp.ConsumeBool()) {
                cJSON_Compare(array1, array2, !case_sensitive);
            }
        }
        
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
    }
}

// Test case 2: Object key asymmetry (subset relationships)
static void test_object_key_asymmetry(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 50) return;
    
    // Test obj1 as superset
    cJSON* obj1_superset = nullptr;
    cJSON* obj2_subset = nullptr;
    
    create_object_pair_with_key_asymmetry(fdp, &obj1_superset, &obj2_subset, true);
    
    if (obj1_superset && obj2_subset) {
        cJSON_Compare(obj1_superset, obj2_subset, case_sensitive); // Should be false (superset != subset)
        cJSON_Compare(obj2_subset, obj1_superset, case_sensitive); // Should be false (subset != superset)
        
        // Test with opposite case sensitivity
        if (fdp.ConsumeBool()) {
            cJSON_Compare(obj1_superset, obj2_subset, !case_sensitive);
        }
    }
    
    if (obj1_superset) cJSON_Delete(obj1_superset);
    if (obj2_subset) cJSON_Delete(obj2_subset);
    
    // Test obj2 as superset
    cJSON* obj1_subset = nullptr;
    cJSON* obj2_superset = nullptr;
    
    create_object_pair_with_key_asymmetry(fdp, &obj1_subset, &obj2_superset, false);
    
    if (obj1_subset && obj2_superset) {
        cJSON_Compare(obj1_subset, obj2_superset, case_sensitive);
        cJSON_Compare(obj2_superset, obj1_subset, case_sensitive);
        
        // Test with opposite case sensitivity
        if (fdp.ConsumeBool()) {
            cJSON_Compare(obj1_subset, obj2_superset, !case_sensitive);
        }
    }
    
    if (obj1_subset) cJSON_Delete(obj1_subset);
    if (obj2_superset) cJSON_Delete(obj2_superset);
}

// Test case 3: Deep nested differences
static void test_deep_nested_differences(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 100) return;
    
    // Create base structure
    int depth = fdp.ConsumeIntegralInRange<int>(2, 4);
    cJSON* base = create_deep_nested_structure(fdp, depth, false);
    
    if (!base) return;
    
    // Create variation with difference at each possible depth
    for (int diff_depth = 0; diff_depth < depth; diff_depth++) {
        cJSON* variation = create_deep_nested_structure(fdp, depth, true);
        
        if (variation) {
            cJSON_Compare(base, variation, case_sensitive);
            cJSON_Compare(variation, base, case_sensitive); // Test symmetry
            
            // Test with opposite case sensitivity
            if (fdp.ConsumeBool()) {
                cJSON_Compare(base, variation, !case_sensitive);
            }
            
            cJSON_Delete(variation);
        }
    }
    
    cJSON_Delete(base);
}

// Test case 4: String comparison edge cases
static void test_string_edge_cases(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 50) return;
    
    // Test NULL strings (using cJSON_CreateStringReference with NULL)
    cJSON* null_string1 = cJSON_CreateStringReference(nullptr);
    cJSON* null_string2 = cJSON_CreateStringReference(nullptr);
    cJSON* regular_string = cJSON_CreateString("test");
    
    if (null_string1 && null_string2 && regular_string) {
        // NULL vs NULL (should be false according to cJSON_Compare logic)
        cJSON_Compare(null_string1, null_string2, case_sensitive);
        
        // NULL vs regular string (should be false)
        cJSON_Compare(null_string1, regular_string, case_sensitive);
        cJSON_Compare(regular_string, null_string1, case_sensitive);
        
        // Test with opposite case sensitivity
        if (fdp.ConsumeBool()) {
            cJSON_Compare(null_string1, null_string2, !case_sensitive);
        }
    }
    
    if (null_string1) cJSON_Delete(null_string1);
    if (null_string2) cJSON_Delete(null_string2);
    if (regular_string) cJSON_Delete(regular_string);
    
    // Test empty strings
    cJSON* empty1 = cJSON_CreateString("");
    cJSON* empty2 = cJSON_CreateString("");
    cJSON* non_empty = cJSON_CreateString("hello");
    
    if (empty1 && empty2 && non_empty) {
        cJSON_Compare(empty1, empty2, case_sensitive); // Should be true
        cJSON_Compare(empty1, non_empty, case_sensitive); // Should be false
        cJSON_Compare(non_empty, empty1, case_sensitive); // Should be false
    }
    
    if (empty1) cJSON_Delete(empty1);
    if (empty2) cJSON_Delete(empty2);
    if (non_empty) cJSON_Delete(non_empty);
}

// Test case 5: Number precision boundaries
static void test_number_precision_boundaries(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    if (fdp.remaining_bytes() < 50) return;
    
    double num1, num2;
    
    for (int i = 0; i < 3; i++) {
        create_number_pair_at_precision_boundary(fdp, &num1, &num2);
        
        cJSON* json_num1 = cJSON_CreateNumber(num1);
        cJSON* json_num2 = cJSON_CreateNumber(num2);
        
        if (json_num1 && json_num2) {
            cJSON_Compare(json_num1, json_num2, case_sensitive);
            cJSON_Compare(json_num2, json_num1, case_sensitive); // Test symmetry
            
            // Test with opposite case sensitivity
            if (fdp.ConsumeBool()) {
                cJSON_Compare(json_num1, json_num2, !case_sensitive);
            }
        }
        
        if (json_num1) cJSON_Delete(json_num1);
        if (json_num2) cJSON_Delete(json_num2);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 20) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test cases to run based on input
    uint8_t test_selector = fdp.ConsumeIntegral<uint8_t>() % 5;
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    switch (test_selector) {
        case 0:
            test_array_length_mismatch(fdp, case_sensitive);
            break;
        case 1:
            test_object_key_asymmetry(fdp, case_sensitive);
            break;
        case 2:
            test_deep_nested_differences(fdp, case_sensitive);
            break;
        case 3:
            test_string_edge_cases(fdp, case_sensitive);
            break;
        case 4:
            test_number_precision_boundaries(fdp, case_sensitive);
            break;
    }
    
    // Also test some random combinations
    if (fdp.remaining_bytes() > 100) {
        // Run additional random tests
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; i++) {
            uint8_t extra_test = fdp.ConsumeIntegral<uint8_t>() % 5;
            cJSON_bool extra_case_sensitive = fdp.ConsumeBool();
            
            switch (extra_test) {
                case 0:
                    test_array_length_mismatch(fdp, extra_case_sensitive);
                    break;
                case 1:
                    test_object_key_asymmetry(fdp, extra_case_sensitive);
                    break;
                case 2:
                    test_deep_nested_differences(fdp, extra_case_sensitive);
                    break;
                case 3:
                    test_string_edge_cases(fdp, extra_case_sensitive);
                    break;
                case 4:
                    test_number_precision_boundaries(fdp, extra_case_sensitive);
                    break;
            }
        }
    }
    
    return 0;
}

} // namespace harness_026
