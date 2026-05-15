#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_011 {
/*
 * Harness_011: Systematic and targeted cJSON_Compare testing for remaining blocked branches
 * 
 * Target: Specifically targets the 73 blocked branches in cJSON_Compare identified
 * as the single biggest coverage gap despite 2141 executions.
 * 
 * This harness is semantically distinct from previous harnesses (005-010) by:
 * 1. Focusing specifically on the 9 targeted scenarios outlined in coverage guidance
 * 2. Implementing systematic pairwise comparison testing for each scenario
 * 3. Creating controlled test cases for each blocked code path
 * 4. Testing edge cases not covered by previous harnesses
 * 
 * Targeted scenarios per coverage guidance:
 * 1) Identical objects (tests a == b path) - Line 3101-3104
 * 2) Different types (type mismatch) - Line 3078 type check
 * 3) Arrays with same elements but different lengths - Line 3151-3153
 * 4) Objects with same values but different keys - Line 3166-3169, 3182-3185
 * 5) Nested structures for deep comparison - Recursive comparison paths
 * 6) String comparisons with case_sensitive parameter - Line 3127-3132 with strcmp
 * 7) Number comparisons with different precision values - Line 3115-3119 compare_double
 * 8) Raw string comparisons - Line 3123-3132
 * 9) Invalid/null object comparisons - Line 3078 NULL checks
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for systematic testing of all 9 scenarios
    if (size < 384) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // SCENARIO 1: IDENTICAL OBJECTS (tests a == b path - Line 3101-3104)
    // ============================================================
    
    // Create identical objects by using same pointer
    cJSON* single_object = cJSON_CreateObject();
    if (single_object != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool identical_result = cJSON_Compare(single_object, single_object, case_sensitive);
        // Should return true (a == b)
    }
    
    // Create identical objects by duplication
    cJSON* original_object = cJSON_CreateObject();
    if (original_object != NULL) {
        // Add some properties
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(8);
            double value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(original_object, key.c_str(), value);
        }
        
        // Create identical duplicate
        cJSON* duplicate_object = cJSON_Duplicate(original_object, 1);
        if (duplicate_object != NULL) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool duplicate_result = cJSON_Compare(original_object, duplicate_object, case_sensitive);
            // Should return true (structurally identical)
            
            cJSON_Delete(duplicate_object);
        }
    }
    
    // ============================================================
    // SCENARIO 2: DIFFERENT TYPES (type mismatch - Line 3078)
    // ============================================================
    
    // Create objects of all different types for pairwise comparison
    std::vector<cJSON*> different_type_items;
    
    // Create one item of each type
    different_type_items.push_back(cJSON_CreateNull());
    different_type_items.push_back(cJSON_CreateTrue());
    different_type_items.push_back(cJSON_CreateFalse());
    different_type_items.push_back(cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    different_type_items.push_back(cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
    different_type_items.push_back(cJSON_CreateRaw(fdp.ConsumeRandomLengthString(24).c_str()));
    different_type_items.push_back(cJSON_CreateArray());
    different_type_items.push_back(cJSON_CreateObject());
    
    // Compare each type with every other type (should all return false)
    for (size_t i = 0; i < different_type_items.size() && i < 4; i++) {
        for (size_t j = i + 1; j < different_type_items.size() && j < 6; j++) {
            if (different_type_items[i] != NULL && different_type_items[j] != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool type_mismatch_result = cJSON_Compare(different_type_items[i], different_type_items[j], case_sensitive);
                // Should return false (different types)
            }
        }
    }
    
    // ============================================================
    // SCENARIO 3: ARRAYS WITH SAME ELEMENTS BUT DIFFERENT LENGTHS
    // ============================================================
    // Tests Line 3151-3153: "one of the arrays is longer than the other"
    
    // Create base array with some elements
    cJSON* base_array = cJSON_CreateArray();
    if (base_array != NULL) {
        // Add 2-4 elements to base array
        size_t base_size = fdp.ConsumeIntegralInRange<size_t>(2, 4);
        for (size_t i = 0; i < base_size && fdp.remaining_bytes() > 0; i++) {
            cJSON_AddItemToArray(base_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        
        // Create longer array (same elements plus extra)
        cJSON* longer_array = cJSON_Duplicate(base_array, 1);
        if (longer_array != NULL) {
            // Add extra elements to make it longer
            size_t extra_elements = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 0; i < extra_elements && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(longer_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
            }
            
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool length_mismatch_result = cJSON_Compare(base_array, longer_array, case_sensitive);
            // Should return false (different lengths)
            
            // Also test shorter array
            cJSON* shorter_array = cJSON_CreateArray();
            if (shorter_array != NULL && base_size > 1) {
                // Add fewer elements than base array
                for (size_t i = 0; i < base_size - 1; i++) {
                    cJSON* elem = cJSON_GetArrayItem(base_array, i);
                    if (elem != NULL) {
                        cJSON* dup_elem = cJSON_Duplicate(elem, 1);
                        if (dup_elem != NULL) {
                            cJSON_AddItemToArray(shorter_array, dup_elem);
                        }
                    }
                }
                
                cJSON_bool case_sensitive2 = fdp.ConsumeBool();
                cJSON_bool shorter_mismatch_result = cJSON_Compare(base_array, shorter_array, case_sensitive2);
                // Should return false (different lengths)
                
                cJSON_Delete(shorter_array);
            }
            
            cJSON_Delete(longer_array);
        }
    }
    
    // ============================================================
    // SCENARIO 4: OBJECTS WITH SAME VALUES BUT DIFFERENT KEYS
    // ============================================================
    // Tests Line 3166-3169 and 3182-3185 (key missing checks)
    
    // Create object with specific keys
    cJSON* obj_with_keys1 = cJSON_CreateObject();
    cJSON* obj_with_keys2 = cJSON_CreateObject();
    
    if (obj_with_keys1 != NULL && obj_with_keys2 != NULL) {
        // Create some shared keys
        std::vector<std::string> shared_keys;
        size_t num_shared = fdp.ConsumeIntegralInRange<size_t>(2, 4);
        for (size_t i = 0; i < num_shared; i++) {
            std::string key = fdp.ConsumeRandomLengthString(6);
            shared_keys.push_back(key);
            
            // Add same value for shared key to both objects
            double shared_value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(obj_with_keys1, key.c_str(), shared_value);
            cJSON_AddNumberToObject(obj_with_keys2, key.c_str(), shared_value);
        }
        
        // Add different keys to each object
        size_t num_unique1 = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        for (size_t i = 0; i < num_unique1; i++) {
            std::string unique_key = fdp.ConsumeRandomLengthString(6);
            cJSON_AddStringToObject(obj_with_keys1, unique_key.c_str(), 
                                   fdp.ConsumeRandomLengthString(12).c_str());
        }
        
        size_t num_unique2 = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        for (size_t i = 0; i < num_unique2; i++) {
            std::string unique_key = fdp.ConsumeRandomLengthString(6);
            cJSON_AddBoolToObject(obj_with_keys2, unique_key.c_str(), fdp.ConsumeBool());
        }
        
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool different_keys_result = cJSON_Compare(obj_with_keys1, obj_with_keys2, case_sensitive);
        // Should return false (different keys)
    }
    
    // ============================================================
    // SCENARIO 5: NESTED STRUCTURES FOR DEEP COMPARISON
    // ============================================================
    
    // Create nested object structure
    cJSON* nested1 = cJSON_CreateObject();
    cJSON* nested2 = cJSON_CreateObject();
    
    if (nested1 != NULL && nested2 != NULL) {
        // Add array as value in both objects
        cJSON* array1 = cJSON_CreateArray();
        cJSON* array2 = cJSON_CreateArray();
        
        if (array1 != NULL && array2 != NULL) {
            // Populate arrays with same elements
            size_t array_size = fdp.ConsumeIntegralInRange<size_t>(2, 4);
            for (size_t i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                double val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(val));
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(val));
            }
            
            cJSON_AddItemToObject(nested1, "nested_array", array1);
            cJSON_AddItemToObject(nested2, "nested_array", array2);
            
            // Add nested objects
            cJSON* inner1 = cJSON_CreateObject();
            cJSON* inner2 = cJSON_CreateObject();
            
            if (inner1 != NULL && inner2 != NULL) {
                cJSON_AddStringToObject(inner1, "inner_key", fdp.ConsumeRandomLengthString(8).c_str());
                cJSON_AddStringToObject(inner2, "inner_key", fdp.ConsumeRandomLengthString(8).c_str());
                
                cJSON_AddItemToObject(nested1, "inner_object", inner1);
                cJSON_AddItemToObject(nested2, "inner_object", inner2);
                
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool nested_result = cJSON_Compare(nested1, nested2, case_sensitive);
                // Result depends on whether string values match
            } else {
                if (inner1 != NULL) cJSON_Delete(inner1);
                if (inner2 != NULL) cJSON_Delete(inner2);
            }
        } else {
            if (array1 != NULL) cJSON_Delete(array1);
            if (array2 != NULL) cJSON_Delete(array2);
        }
    }
    
    // ============================================================
    // SCENARIO 6: STRING COMPARISONS WITH CASE_SENSITIVE PARAMETER
    // ============================================================
    // Tests Line 3127-3132 with strcmp
    
    // Create strings for case sensitivity testing
    std::string base_str = fdp.ConsumeRandomLengthString(16);
    if (!base_str.empty()) {
        // Create same case string
        cJSON* string_same_case = cJSON_CreateString(base_str.c_str());
        
        // Create different case string (change first character case)
        std::string diff_case_str = base_str;
        if (std::isalpha(diff_case_str[0])) {
            if (std::islower(diff_case_str[0])) {
                diff_case_str[0] = std::toupper(diff_case_str[0]);
            } else {
                diff_case_str[0] = std::tolower(diff_case_str[0]);
            }
        }
        cJSON* string_diff_case = cJSON_CreateString(diff_case_str.c_str());
        
        if (string_same_case != NULL && string_diff_case != NULL) {
            // Test case-sensitive (should return false if case differs)
            cJSON_bool case_sensitive_true = cJSON_Compare(string_same_case, string_diff_case, true);
            
            // Test case-insensitive (implementation may or may not handle this)
            cJSON_bool case_sensitive_false = cJSON_Compare(string_same_case, string_diff_case, false);
            
            // Test identical strings with both settings
            cJSON_bool identical_case_true = cJSON_Compare(string_same_case, string_same_case, true);
            cJSON_bool identical_case_false = cJSON_Compare(string_same_case, string_same_case, false);
        }
        
        if (string_same_case != NULL) cJSON_Delete(string_same_case);
        if (string_diff_case != NULL) cJSON_Delete(string_diff_case);
    }
    
    // ============================================================
    // SCENARIO 7: NUMBER COMPARISONS WITH DIFFERENT PRECISION VALUES
    // ============================================================
    // Tests Line 3115-3119 compare_double function
    
    // Test number comparisons with various precision scenarios
    std::vector<double> test_numbers;
    
    // Add some basic numbers
    test_numbers.push_back(0.0);
    test_numbers.push_back(1.0);
    test_numbers.push_back(-1.0);
    test_numbers.push_back(3.14159);
    test_numbers.push_back(fdp.ConsumeFloatingPoint<double>());
    test_numbers.push_back(fdp.ConsumeFloatingPoint<double>());
    
    // Add numbers that might have precision issues
    test_numbers.push_back(1.0 / 3.0);  // Repeating decimal
    test_numbers.push_back(0.1 + 0.2);  // Classic floating point issue
    
    // Create JSON numbers and compare them
    for (size_t i = 0; i < test_numbers.size() && i < 4; i++) {
        for (size_t j = i; j < test_numbers.size() && j < 6; j++) {
            cJSON* num1 = cJSON_CreateNumber(test_numbers[i]);
            cJSON* num2 = cJSON_CreateNumber(test_numbers[j]);
            
            if (num1 != NULL && num2 != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();  // Not used for numbers but required
                cJSON_bool number_result = cJSON_Compare(num1, num2, case_sensitive);
                // Result depends on compare_double precision handling
            }
            
            if (num1 != NULL) cJSON_Delete(num1);
            if (num2 != NULL) cJSON_Delete(num2);
        }
    }
    
    // ============================================================
    // SCENARIO 8: RAW STRING COMPARISONS
    // ============================================================
    // Tests Line 3123-3132 for cJSON_Raw type
    
    // Create raw JSON strings
    std::string raw1 = fdp.ConsumeRandomLengthString(32);
    std::string raw2 = fdp.ConsumeRandomLengthString(32);
    std::string raw3 = raw1;  // Same as raw1
    
    cJSON* raw_item1 = cJSON_CreateRaw(raw1.c_str());
    cJSON* raw_item2 = cJSON_CreateRaw(raw2.c_str());
    cJSON* raw_item3 = cJSON_CreateRaw(raw3.c_str());
    
    if (raw_item1 != NULL && raw_item2 != NULL && raw_item3 != NULL) {
        // Test different raw strings
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool raw_diff_result = cJSON_Compare(raw_item1, raw_item2, case_sensitive);
        // Should return false (different content)
        
        // Test identical raw strings
        cJSON_bool raw_same_result = cJSON_Compare(raw_item1, raw_item3, case_sensitive);
        // Should return true (identical content)
        
        // Test raw vs regular string (different types)
        cJSON* regular_string = cJSON_CreateString(raw1.c_str());
        if (regular_string != NULL) {
            cJSON_bool raw_vs_string_result = cJSON_Compare(raw_item1, regular_string, case_sensitive);
            // Should return false (different types: Raw vs String)
            
            cJSON_Delete(regular_string);
        }
    }
    
    // ============================================================
    // SCENARIO 9: INVALID/NULL OBJECT COMPARISONS
    // ============================================================
    // Tests Line 3078 NULL checks
    
    // Test NULL comparisons
    cJSON* valid_object = cJSON_CreateObject();
    if (valid_object != NULL) {
        // Compare NULL with valid object
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool null_vs_valid = cJSON_Compare(NULL, valid_object, case_sensitive);
        // Should return false (NULL check)
        
        cJSON_bool valid_vs_null = cJSON_Compare(valid_object, NULL, case_sensitive);
        // Should return false (NULL check)
        
        cJSON_bool null_vs_null = cJSON_Compare(NULL, NULL, case_sensitive);
        // Should return false (NULL check)
    }
    
    // Test with object that has NULL valuestring (for string/raw types)
    // This requires manual creation of cJSON struct
    cJSON* manually_created = (cJSON*)malloc(sizeof(cJSON));
    if (manually_created != NULL) {
        memset(manually_created, 0, sizeof(cJSON));
        manually_created->type = cJSON_String;
        manually_created->valuestring = NULL;  // Explicit NULL
        
        cJSON* normal_string = cJSON_CreateString("test");
        if (normal_string != NULL) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool null_string_result = cJSON_Compare(manually_created, normal_string, case_sensitive);
            // Should return false (NULL valuestring check - Line 3123-3126)
            
            cJSON_Delete(normal_string);
        }
        
        free(manually_created);
    }
    
    // ============================================================
    // CLEANUP ALL ALLOCATED OBJECTS
    // ============================================================
    
    // Cleanup scenario 1 objects
    if (single_object != NULL) cJSON_Delete(single_object);
    if (original_object != NULL) cJSON_Delete(original_object);
    
    // Cleanup scenario 2 objects
    for (cJSON* item : different_type_items) {
        if (item != NULL) cJSON_Delete(item);
    }
    
    // Cleanup scenario 3 objects
    if (base_array != NULL) cJSON_Delete(base_array);
    
    // Cleanup scenario 4 objects
    if (obj_with_keys1 != NULL) cJSON_Delete(obj_with_keys1);
    if (obj_with_keys2 != NULL) cJSON_Delete(obj_with_keys2);
    
    // Cleanup scenario 5 objects
    if (nested1 != NULL) cJSON_Delete(nested1);
    if (nested2 != NULL) cJSON_Delete(nested2);
    
    // Cleanup scenario 8 objects
    if (raw_item1 != NULL) cJSON_Delete(raw_item1);
    if (raw_item2 != NULL) cJSON_Delete(raw_item2);
    if (raw_item3 != NULL) cJSON_Delete(raw_item3);
    
    // Cleanup scenario 9 objects
    if (valid_object != NULL) cJSON_Delete(valid_object);
    
    return 0;
}

} // namespace harness_011
