#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cfloat>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_012 {
/*
 * Harness_012: Deterministic edge case testing for cJSON_Compare blocked branches
 * 
 * Target: Specifically targets the 73 blocked branches in cJSON_Compare identified
 * in coverage guidance harness_012_guidance.md with deterministic test cases.
 * 
 * This harness is semantically distinct from previous harnesses (005-011) by:
 * 1. Creating DETERMINISTIC test cases for each blocked condition
 * 2. Focusing on the 6 specific edge cases outlined in the guidance:
 *    - Array length mismatch: [1,2,3] vs [1,2]
 *    - Object key mismatch: {"a":1,"b":2} vs {"a":1}
 *    - Type validation edge cases (invalid type values)
 *    - String comparison variations: "Hello" vs "hello" with case_sensitive=false
 *    - Floating point precision: 1.0 vs 1.0000000000000002 near DBL_EPSILON
 *    - NULL/invalid input combinations
 * 3. Each test case is created deterministically based on fuzzer input selection
 * 4. Tests specific blocked code paths with controlled inputs
 * 
 * Blocked branches targeted:
 * - Line 3078: NULL check and type mismatch
 * - Line 3096: Default switch case for invalid types
 * - Line 3123: NULL valuestring check for strings/raw
 * - Line 3151: Array length mismatch (a_element != b_element)
 * - Lines 3166/3182: Object key mismatch (get_object_item returns NULL)
 * - Line 3115: compare_double with values near DBL_EPSILON threshold
 * - Line 3127: strcmp for string comparisons
 */



// Helper to create cJSON with invalid type (to hit default switch case at line 3096)
static cJSON* create_invalid_type_cjson(uint8_t invalid_type) {
    cJSON* item = (cJSON*)malloc(sizeof(cJSON));
    if (item == NULL) {
        return NULL;
    }
    memset(item, 0, sizeof(cJSON));
    item->type = invalid_type;  // Invalid type that doesn't match any cJSON type
    return item;
}

// Helper to create string with NULL valuestring (tests line 3123)
static cJSON* create_string_with_null_valuestring() {
    cJSON* item = (cJSON*)malloc(sizeof(cJSON));
    if (item == NULL) {
        return NULL;
    }
    memset(item, 0, sizeof(cJSON));
    item->type = cJSON_String;
    item->valuestring = NULL;  // Explicitly NULL valuestring
    return item;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need enough data to select test case and provide parameters
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Select which deterministic test case to run
    uint8_t test_case = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // ============================================================
    // TEST CASE 0: ARRAY LENGTH MISMATCH 
    // ============================================================
    // Targets line 3151: if (a_element != b_element)
    if (test_case == 0) {
        // Determine array lengths from fuzzer input
        size_t long_len = fdp.ConsumeIntegralInRange<size_t>(3, 10);
        size_t short_len = long_len - 1;
        
        // Create longer array
        cJSON* array_long = cJSON_CreateArray();
        if (array_long != NULL) {
            for (size_t i = 0; i < long_len; i++) {
                double value = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddItemToArray(array_long, cJSON_CreateNumber(value));
            }
            
            // Create shorter array (subset of values)
            cJSON* array_short = cJSON_CreateArray();
            if (array_short != NULL) {
                for (size_t i = 0; i < short_len; i++) {
                    // Use same values for first short_len elements for fair comparison
                    if (i < short_len) {
                        // We'll create new number items with similar values
                        double value = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddItemToArray(array_short, cJSON_CreateNumber(value));
                    }
                }
                
                // Test comparison with both case_sensitive values
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(array_long, array_short, case_sensitive);
                // Should return false due to length mismatch
                
                // Also test reverse order
                cJSON_bool result_reverse = cJSON_Compare(array_short, array_long, case_sensitive);
                // Should also return false
                
                cJSON_Delete(array_short);
            }
            cJSON_Delete(array_long);
        }
    }
    
    // ============================================================
    // TEST CASE 1: OBJECT KEY MISMATCH
    // ============================================================
    // Targets lines 3166 and 3182: get_object_item returns NULL
    else if (test_case == 1) {
        // Create keys from fuzzer input
        std::string key1 = fdp.ConsumeRandomLengthString(8);
        std::string key2 = fdp.ConsumeRandomLengthString(8);
        
        // Ensure keys are different
        if (key1.empty()) key1 = "a";
        if (key2.empty()) key2 = "b";
        if (key1 == key2) key2 = "c";
        
        // Create object with both keys
        cJSON* object_full = cJSON_CreateObject();
        if (object_full != NULL) {
            double value1 = fdp.ConsumeFloatingPoint<double>();
            double value2 = fdp.ConsumeFloatingPoint<double>();
            
            cJSON_AddNumberToObject(object_full, key1.c_str(), value1);
            cJSON_AddNumberToObject(object_full, key2.c_str(), value2);
            
            // Create object with only first key
            cJSON* object_partial = cJSON_CreateObject();
            if (object_partial != NULL) {
                cJSON_AddNumberToObject(object_partial, key1.c_str(), value1);
                
                // Test comparison with both case_sensitive values
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(object_full, object_partial, case_sensitive);
                // Should return false due to missing key2
                
                // Also test reverse order
                cJSON_bool result_reverse = cJSON_Compare(object_partial, object_full, case_sensitive);
                // Should also return false (missing key2 in partial)
                
                cJSON_Delete(object_partial);
            }
            cJSON_Delete(object_full);
        }
    }
    
    // ============================================================
    // TEST CASE 2: TYPE VALIDATION EDGE CASES (invalid type values)
    // ============================================================
    // Targets line 3096: default switch case for invalid types
    else if (test_case == 2) {
        // Create cJSON items with invalid type values from fuzzer input
        uint8_t invalid_type1 = fdp.ConsumeIntegral<uint8_t>() | 0x80;  // Ensure high bit set
        uint8_t invalid_type2 = fdp.ConsumeIntegral<uint8_t>() | 0x40;  // Different invalid type
        
        cJSON* invalid_item1 = create_invalid_type_cjson(invalid_type1);
        cJSON* invalid_item2 = create_invalid_type_cjson(invalid_type2);
        
        // Also create a valid item for comparison with invalid
        cJSON* valid_item = cJSON_CreateNull();
        
        if (invalid_item1 != NULL) {
            // Compare invalid with invalid (same type)
            if (invalid_item2 != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(invalid_item1, invalid_item2, case_sensitive);
                // Should return false due to invalid type
                
                free(invalid_item2);
            }
            
            // Compare invalid with valid
            if (valid_item != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(invalid_item1, valid_item, case_sensitive);
                // Should return false due to type mismatch
                
                cJSON_Delete(valid_item);
            }
            
            free(invalid_item1);
        } else if (valid_item != NULL) {
            cJSON_Delete(valid_item);
        }
    }
    
    // ============================================================
    // TEST CASE 3: STRING COMPARISON VARIATIONS
    // ============================================================
    // Tests line 3127: strcmp comparison
    // Note: case_sensitive parameter only affects object key lookup, not string comparison
    else if (test_case == 3) {
        // Create base string from fuzzer input
        std::string base_str = fdp.ConsumeRandomLengthString(16);
        if (base_str.empty()) base_str = "test";
        
        // Create version with different case (if possible)
        std::string upper_str = base_str;
        std::string lower_str = base_str;
        
        // Simple case transformation for demonstration
        // In real fuzzing, the fuzzer will explore various string differences
        for (char& c : upper_str) {
            if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        }
        for (char& c : lower_str) {
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        }
        
        cJSON* string_upper = cJSON_CreateString(upper_str.c_str());
        cJSON* string_lower = cJSON_CreateString(lower_str.c_str());
        
        if (string_upper != NULL && string_lower != NULL) {
            // Test with case_sensitive parameter
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool result = cJSON_Compare(string_upper, string_lower, case_sensitive);
            // Should return false if strings differ (strcmp comparison)
            
            // Also test identical strings
            cJSON* string_upper2 = cJSON_CreateString(upper_str.c_str());
            if (string_upper2 != NULL) {
                cJSON_bool result_same = cJSON_Compare(string_upper, string_upper2, case_sensitive);
                // Should return true
                
                cJSON_Delete(string_upper2);
            }
            
            cJSON_Delete(string_lower);
            cJSON_Delete(string_upper);
        }
    }
    
    // ============================================================
    // TEST CASE 4: FLOATING POINT PRECISION NEAR DBL_EPSILON
    // ============================================================
    // Targets line 3115: compare_double with values near DBL_EPSILON threshold
    else if (test_case == 4) {
        // Base value from fuzzer input
        double base_value = fdp.ConsumeFloatingPoint<double>();
        if (std::isnan(base_value) || std::isinf(base_value)) {
            base_value = 1.0;  // Use default if invalid
        }
        
        double epsilon = DBL_EPSILON;
        
        // Determine factor from fuzzer input (0.5 for within, 2.0 for outside tolerance)
        double factor = fdp.ConsumeFloatingPointInRange<double>(0.1, 3.0);
        
        double test_value = base_value + epsilon * factor;
        
        cJSON* number_base = cJSON_CreateNumber(base_value);
        cJSON* number_test = cJSON_CreateNumber(test_value);
        
        if (number_base != NULL && number_test != NULL) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool result = cJSON_Compare(number_base, number_test, case_sensitive);
            // Result depends on whether test_value is within DBL_EPSILON tolerance
            
            cJSON_Delete(number_test);
            cJSON_Delete(number_base);
        }
    }
    
    // ============================================================
    // TEST CASE 5: NULL/INVALID INPUT COMBINATIONS
    // ============================================================
    // Targets line 3078: if ((a == NULL) || (b == NULL) || ...)
    // Targets line 3123: if ((a->valuestring == NULL) || (b->valuestring == NULL))
    else if (test_case == 5) {
        // Test various NULL combinations based on fuzzer input
        uint8_t null_test_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        if (null_test_type == 0) {
            // NULL vs valid item
            cJSON* valid_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            if (valid_item != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(NULL, valid_item, case_sensitive);
                // Should return false (a == NULL)
                
                cJSON_bool result2 = cJSON_Compare(valid_item, NULL, case_sensitive);
                // Should return false (b == NULL)
                
                cJSON_Delete(valid_item);
            }
        }
        else if (null_test_type == 1) {
            // String with NULL valuestring vs normal string
            cJSON* null_string = create_string_with_null_valuestring();
            
            // Create normal string from fuzzer input
            std::string normal_str = fdp.ConsumeRandomLengthString(16);
            cJSON* normal_string = cJSON_CreateString(normal_str.c_str());
            
            if (null_string != NULL && normal_string != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(null_string, normal_string, case_sensitive);
                // Should return false (NULL valuestring)
                
                cJSON_bool result2 = cJSON_Compare(normal_string, null_string, case_sensitive);
                // Should return false
                
                cJSON_Delete(normal_string);
                free(null_string);
            } else {
                if (null_string != NULL) free(null_string);
                if (normal_string != NULL) cJSON_Delete(normal_string);
            }
        }
        else if (null_test_type == 2) {
            // Two strings with NULL valuestring
            cJSON* null_string1 = create_string_with_null_valuestring();
            cJSON* null_string2 = create_string_with_null_valuestring();
            
            if (null_string1 != NULL && null_string2 != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(null_string1, null_string2, case_sensitive);
                // Should return false (both NULL valuestring)
                
                free(null_string1);
                free(null_string2);
            } else {
                if (null_string1 != NULL) free(null_string1);
                if (null_string2 != NULL) free(null_string2);
            }
        }
    }
    
    return 0;
}

} // namespace harness_012
