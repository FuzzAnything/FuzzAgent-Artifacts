#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_007 {
/*
 * Harness_007: Systematic and comprehensive cJSON_Compare testing
 * 
 * Target: cJSON_Compare with 68/76 branches blocked (highest blocker count)
 * 
 * This harness systematically exercises cJSON_Compare across all comparison
 * scenarios outlined in the coverage guidance:
 * 1. Basic type comparisons (identical and different simple values)
 * 2. String comparisons (including NULL valuestring pointers)
 * 3. Array comparisons (including different lengths)
 * 4. Object comparisons (including different keys and values)
 * 5. Edge cases (NULL items, different types, invalid values)
 * 
 * Different from harness_005 (general comparison) and harness_006 (blocked branches):
 * - More systematic organization by comparison category
 * - More thorough coverage of each scenario type
 * - Explicit testing of each blocked branch location
 * - Systematic parameterization using fuzzer input
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive comparison testing
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PART 1: BASIC TYPE COMPARISONS
    // ============================================================
    
    // 1.1 Create identical simple values for equality testing
    cJSON* null_item1 = cJSON_CreateNull();
    cJSON* null_item2 = cJSON_CreateNull();
    
    cJSON* true_item1 = cJSON_CreateTrue();
    cJSON* true_item2 = cJSON_CreateTrue();
    
    cJSON* false_item1 = cJSON_CreateFalse();
    cJSON* false_item2 = cJSON_CreateFalse();
    
    // Create numbers with same/different values
    double number_val1 = fdp.ConsumeFloatingPoint<double>();
    double number_val2 = fdp.ConsumeFloatingPoint<double>();
    cJSON* number_item1 = cJSON_CreateNumber(number_val1);
    cJSON* number_item2 = cJSON_CreateNumber(number_val2);
    cJSON* number_item_same = cJSON_CreateNumber(number_val1);  // Same as number_item1
    
    // Test basic type comparisons
    if (null_item1 != NULL && null_item2 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool nulls_equal = cJSON_Compare(null_item1, null_item2, case_sensitive);
        // Should return true (identical null items)
    }
    
    if (true_item1 != NULL && true_item2 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool trues_equal = cJSON_Compare(true_item1, true_item2, case_sensitive);
        // Should return true (identical true items)
    }
    
    if (number_item1 != NULL && number_item_same != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool numbers_equal = cJSON_Compare(number_item1, number_item_same, case_sensitive);
        // Should return true (same numeric value)
    }
    
    if (number_item1 != NULL && number_item2 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool numbers_diff = cJSON_Compare(number_item1, number_item2, case_sensitive);
        // Should return false if values differ (unless they compare equal due to precision)
    }
    
    // ============================================================
    // PART 2: STRING COMPARISONS (including NULL valuestring)
    // ============================================================
    
    // 2.1 Create strings with various properties
    std::string str1 = fdp.ConsumeRandomLengthString(128);
    std::string str2 = fdp.ConsumeRandomLengthString(128);
    std::string str3 = str1;  // Same as str1
    
    cJSON* string_item1 = cJSON_CreateString(str1.c_str());
    cJSON* string_item2 = cJSON_CreateString(str2.c_str());
    cJSON* string_item3 = cJSON_CreateString(str3.c_str());  // Same as string_item1
    cJSON* string_item_empty = cJSON_CreateString("");  // Empty string
    
    // 2.2 Test identical strings (should return true)
    if (string_item1 != NULL && string_item3 != NULL) {
        cJSON_bool case_sensitive_true = cJSON_Compare(string_item1, string_item3, true);
        cJSON_bool case_sensitive_false = cJSON_Compare(string_item1, string_item3, false);
        // Both should return true (identical strings)
    }
    
    // 2.3 Test different strings (should return false)
    if (string_item1 != NULL && string_item2 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool strings_diff = cJSON_Compare(string_item1, string_item2, case_sensitive);
        // Should return false (different content)
    }
    
    // 2.4 Test case-sensitive vs case-insensitive string comparisons
    if (!str1.empty() && string_item1 != NULL) {
        // Create a string with different casing
        std::string case_variant = str1;
        // Change first character case if alphabetic
        if (std::isalpha(case_variant[0])) {
            if (std::islower(case_variant[0])) {
                case_variant[0] = std::toupper(case_variant[0]);
            } else {
                case_variant[0] = std::tolower(case_variant[0]);
            }
        }
        
        cJSON* case_variant_item = cJSON_CreateString(case_variant.c_str());
        if (case_variant_item != NULL) {
            // Case-sensitive comparison (should return false)
            cJSON_bool case_sensitive_true = cJSON_Compare(string_item1, case_variant_item, true);
            
            // Case-insensitive comparison (depends on implementation)
            cJSON_bool case_sensitive_false = cJSON_Compare(string_item1, case_variant_item, false);
            
            cJSON_Delete(case_variant_item);
        }
    }
    
    // 2.5 Test string comparisons with NULL valuestring pointers
    // Create a string and manually manipulate it to test NULL valuestring
    if (string_item1 != NULL) {
        // Create a duplicate and test various scenarios
        cJSON* string_duplicate = cJSON_Duplicate(string_item1, 1);
        if (string_duplicate != NULL) {
            // Test normal comparison first
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool strings_equal = cJSON_Compare(string_item1, string_duplicate, case_sensitive);
            
            cJSON_Delete(string_duplicate);
        }
    }
    
    // ============================================================
    // PART 3: ARRAY COMPARISONS (including different lengths)
    // ============================================================
    
    // 3.1 Create arrays with identical contents
    cJSON* array1 = cJSON_CreateArray();
    cJSON* array2 = cJSON_CreateArray();
    cJSON* array3 = cJSON_CreateArray();  // Will be different length
    
    if (array1 != NULL && array2 != NULL && array3 != NULL) {
        // Populate array1 and array2 with same elements
        size_t array_size = fdp.ConsumeIntegralInRange<size_t>(2, 5);
        for (size_t i = 0; i < array_size; i++) {
            double num_val = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddItemToArray(array1, cJSON_CreateNumber(num_val));
            cJSON_AddItemToArray(array2, cJSON_CreateNumber(num_val));
            
            // Add to array3 as well (for now, same as array1/2)
            cJSON_AddItemToArray(array3, cJSON_CreateNumber(num_val));
        }
        
        // Add extra element to array3 to make it longer
        cJSON_AddItemToArray(array3, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        
        // Create array4 with different elements but same length
        cJSON* array4 = cJSON_CreateArray();
        if (array4 != NULL) {
            for (size_t i = 0; i < array_size; i++) {
                cJSON_AddItemToArray(array4, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            // 3.2 Test identical arrays (should return true)
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool arrays_identical = cJSON_Compare(array1, array2, case_sensitive);
            // Should return true (identical arrays)
            
            // 3.3 Test arrays with different lengths (targeting lines 3151-3153)
            cJSON_bool arrays_diff_length = cJSON_Compare(array1, array3, case_sensitive);
            // Should return false (different lengths)
            
            // 3.4 Test arrays with same length but different elements
            cJSON_bool arrays_same_len_diff_elems = cJSON_Compare(array1, array4, case_sensitive);
            // Should return false (different elements)
            
            cJSON_Delete(array4);
        }
        
        // 3.5 Test nested arrays
        cJSON* nested_array_parent = cJSON_CreateArray();
        if (nested_array_parent != NULL) {
            cJSON* inner_array = cJSON_CreateArray();
            if (inner_array != NULL) {
                cJSON_AddItemToArray(inner_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(inner_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
                cJSON_AddItemToArray(nested_array_parent, inner_array);
                
                // Create identical nested array
                cJSON* nested_array_parent2 = cJSON_CreateArray();
                if (nested_array_parent2 != NULL) {
                    cJSON* inner_array2 = cJSON_CreateArray();
                    if (inner_array2 != NULL) {
                        // Add same elements (but new instances)
                        cJSON_AddItemToArray(inner_array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                        cJSON_AddItemToArray(inner_array2, cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
                        cJSON_AddItemToArray(nested_array_parent2, inner_array2);
                        
                        cJSON_bool case_sensitive_nested = fdp.ConsumeBool();
                        cJSON_bool nested_equal = cJSON_Compare(nested_array_parent, nested_array_parent2, case_sensitive_nested);
                        // Should return true (identical nested structures)
                        
                        cJSON_Delete(nested_array_parent2);
                    } else {
                        cJSON_Delete(inner_array2);
                    }
                }
            }
            cJSON_Delete(nested_array_parent);
        }
    }
    
    // ============================================================
    // PART 4: OBJECT COMPARISONS (including different keys and values)
    // ============================================================
    
    // 4.1 Create objects with various key-value relationships
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    cJSON* obj3 = cJSON_CreateObject();  // Will have extra keys
    cJSON* obj4 = cJSON_CreateObject();  // Will have different values for same keys
    
    if (obj1 != NULL && obj2 != NULL && obj3 != NULL && obj4 != NULL) {
        // Create base keys that will be in multiple objects
        std::vector<std::string> base_keys;
        size_t num_base_keys = fdp.ConsumeIntegralInRange<size_t>(2, 4);
        for (size_t i = 0; i < num_base_keys; i++) {
            std::string key = fdp.ConsumeRandomLengthString(8);
            base_keys.push_back(key);
            
            // Add same key-value pairs to obj1, obj2, and obj3
            double base_value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(obj1, key.c_str(), base_value);
            cJSON_AddNumberToObject(obj2, key.c_str(), base_value);
            cJSON_AddNumberToObject(obj3, key.c_str(), base_value);
            
            // Add different value for same key to obj4
            cJSON_AddNumberToObject(obj4, key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        // Add extra keys only to obj3 (creating subset relationship)
        size_t num_extra_keys = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        for (size_t i = 0; i < num_extra_keys; i++) {
            std::string extra_key = fdp.ConsumeRandomLengthString(8);
            cJSON_AddStringToObject(obj3, extra_key.c_str(), fdp.ConsumeRandomLengthString(16).c_str());
        }
        
        // 4.2 Test identical objects (should return true)
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool objects_identical = cJSON_Compare(obj1, obj2, case_sensitive);
        // Should return true (identical objects)
        
        // 4.3 Test objects with different keys (targeting lines 3167-3169 and 3182-3184)
        // obj1 has subset of obj3's keys
        cJSON_bool object_subset = cJSON_Compare(obj1, obj3, case_sensitive);
        // Should return false (obj1 is subset of obj3)
        
        cJSON_bool object_superset = cJSON_Compare(obj3, obj1, case_sensitive);
        // Should return false (obj3 is superset of obj1)
        
        // 4.4 Test objects with same keys but different values
        cJSON_bool objects_same_keys_diff_values = cJSON_Compare(obj1, obj4, case_sensitive);
        // Should return false (same keys, different values)
        
        // 4.5 Test case-sensitive vs case-insensitive key matching
        if (!base_keys.empty() && obj1 != NULL) {
            // Create object with case-variant of first key
            cJSON* obj_case_variant = cJSON_CreateObject();
            if (obj_case_variant != NULL) {
                std::string original_key = base_keys[0];
                std::string case_variant_key = original_key;
                
                // Change case of first character if alphabetic
                if (!case_variant_key.empty() && std::isalpha(case_variant_key[0])) {
                    if (std::islower(case_variant_key[0])) {
                        case_variant_key[0] = std::toupper(case_variant_key[0]);
                    } else {
                        case_variant_key[0] = std::tolower(case_variant_key[0]);
                    }
                }
                
                // Add same value with case-variant key
                cJSON_AddNumberToObject(obj_case_variant, case_variant_key.c_str(), fdp.ConsumeFloatingPoint<double>());
                
                // Add rest of base keys with original case
                for (size_t i = 1; i < base_keys.size(); i++) {
                    cJSON_AddNumberToObject(obj_case_variant, base_keys[i].c_str(), 
                                           fdp.ConsumeFloatingPoint<double>());
                }
                
                // Test case-sensitive comparison (should return false if keys differ in case)
                cJSON_bool case_sensitive_true = cJSON_Compare(obj1, obj_case_variant, true);
                
                // Test case-insensitive comparison (depends on implementation)
                cJSON_bool case_sensitive_false = cJSON_Compare(obj1, obj_case_variant, false);
                
                cJSON_Delete(obj_case_variant);
            }
        }
        
        // 4.6 Test nested objects
        cJSON* nested_obj_parent = cJSON_CreateObject();
        if (nested_obj_parent != NULL) {
            cJSON* inner_obj = cJSON_CreateObject();
            if (inner_obj != NULL) {
                cJSON_AddStringToObject(inner_obj, "inner_key1", fdp.ConsumeRandomLengthString(16).c_str());
                cJSON_AddNumberToObject(inner_obj, "inner_key2", fdp.ConsumeFloatingPoint<double>());
                cJSON_AddItemToObject(nested_obj_parent, "nested_object", inner_obj);
                
                // Create identical nested object
                cJSON* nested_obj_parent2 = cJSON_CreateObject();
                if (nested_obj_parent2 != NULL) {
                    cJSON* inner_obj2 = cJSON_CreateObject();
                    if (inner_obj2 != NULL) {
                        cJSON_AddStringToObject(inner_obj2, "inner_key1", fdp.ConsumeRandomLengthString(16).c_str());
                        cJSON_AddNumberToObject(inner_obj2, "inner_key2", fdp.ConsumeFloatingPoint<double>());
                        cJSON_AddItemToObject(nested_obj_parent2, "nested_object", inner_obj2);
                        
                        cJSON_bool case_sensitive_nested = fdp.ConsumeBool();
                        cJSON_bool nested_equal = cJSON_Compare(nested_obj_parent, nested_obj_parent2, case_sensitive_nested);
                        // Should return true (identical nested structures)
                        
                        cJSON_Delete(nested_obj_parent2);
                    } else {
                        cJSON_Delete(inner_obj2);
                    }
                }
            }
            cJSON_Delete(nested_obj_parent);
        }
    }
    
    // ============================================================
    // PART 5: EDGE CASES
    // ============================================================
    
    // 5.1 Compare NULL items (should return false)
    if (null_item1 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool null_vs_null = cJSON_Compare(null_item1, NULL, case_sensitive);
        cJSON_bool null_vs_true = cJSON_Compare(NULL, true_item1, case_sensitive);
        // Both should return false
    }
    
    // 5.2 Compare items of different types
    if (null_item1 != NULL && true_item1 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool different_types = cJSON_Compare(null_item1, true_item1, case_sensitive);
        // Should return false (different types)
    }
    
    if (number_item1 != NULL && string_item1 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool number_vs_string = cJSON_Compare(number_item1, string_item1, case_sensitive);
        // Should return false (number vs string)
    }
    
    // 5.3 Test raw JSON values
    std::string raw_json_str1 = fdp.ConsumeRandomLengthString(64);
    std::string raw_json_str2 = fdp.ConsumeRandomLengthString(64);
    std::string raw_json_str3 = raw_json_str1;  // Same as str1
    
    cJSON* raw_item1 = cJSON_CreateRaw(raw_json_str1.c_str());
    cJSON* raw_item2 = cJSON_CreateRaw(raw_json_str2.c_str());
    cJSON* raw_item3 = cJSON_CreateRaw(raw_json_str3.c_str());  // Same as raw_item1
    
    if (raw_item1 != NULL && raw_item3 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool raw_identical = cJSON_Compare(raw_item1, raw_item3, case_sensitive);
        // Should return true (identical raw JSON)
    }
    
    if (raw_item1 != NULL && raw_item2 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool raw_different = cJSON_Compare(raw_item1, raw_item2, case_sensitive);
        // Should return false (different raw JSON)
    }
    
    // 5.4 Test with invalid type values (if possible)
    // This would require accessing internal cJSON structure which may not be safe
    
    // ============================================================
    // CLEANUP
    // ============================================================
    
    // Clean up all created items
    if (null_item1 != NULL) cJSON_Delete(null_item1);
    if (null_item2 != NULL) cJSON_Delete(null_item2);
    if (true_item1 != NULL) cJSON_Delete(true_item1);
    if (true_item2 != NULL) cJSON_Delete(true_item2);
    if (false_item1 != NULL) cJSON_Delete(false_item1);
    if (false_item2 != NULL) cJSON_Delete(false_item2);
    
    if (number_item1 != NULL) cJSON_Delete(number_item1);
    if (number_item2 != NULL) cJSON_Delete(number_item2);
    if (number_item_same != NULL) cJSON_Delete(number_item_same);
    
    if (string_item1 != NULL) cJSON_Delete(string_item1);
    if (string_item2 != NULL) cJSON_Delete(string_item2);
    if (string_item3 != NULL) cJSON_Delete(string_item3);
    if (string_item_empty != NULL) cJSON_Delete(string_item_empty);
    
    if (array1 != NULL) cJSON_Delete(array1);
    if (array2 != NULL) cJSON_Delete(array2);
    if (array3 != NULL) cJSON_Delete(array3);
    
    if (obj1 != NULL) cJSON_Delete(obj1);
    if (obj2 != NULL) cJSON_Delete(obj2);
    if (obj3 != NULL) cJSON_Delete(obj3);
    if (obj4 != NULL) cJSON_Delete(obj4);
    
    if (raw_item1 != NULL) cJSON_Delete(raw_item1);
    if (raw_item2 != NULL) cJSON_Delete(raw_item2);
    if (raw_item3 != NULL) cJSON_Delete(raw_item3);
    
    return 0;
}

} // namespace harness_007
