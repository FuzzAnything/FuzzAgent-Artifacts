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
#include <memory>

namespace harness_014 {
/*
 * Harness 014: Advanced cJSON_Compare Testing for Remaining Blocked Branches (69 blocked)
 * 
 * Target: Specifically focus on coverage gaps missed by harness_013:
 * 1. NULL string values: Test cJSON_Compare with NULL valuestring in cJSON_String and cJSON_Raw types (lines 3126-3129, 0 hits)
 * 2. Invalid type handling: Test default switch cases (lines 3099-3100, 0 hits) by creating objects with invalid type bits
 * 3. Object comparison failure paths: Test object comparison where a_element == NULL (lines 3183-3185, 0 hits)
 * 4. Complex nested object comparisons with structural differences
 * 5. Use both parsed JSON (cJSON_Parse) and manually created structures to reach different code paths
 * 6. Test case-sensitive vs case-insensitive string comparisons more thoroughly
 * 7. Include edge cases: empty objects vs objects with properties, arrays with different ordering
 * 8. Test with manipulated type bits to trigger invalid type handling
 * 
 * Key focus: Generate NULL valuestring scenarios by creating string objects with NULL pointers.
 * Test object comparisons where keys exist in one object but not the other.
 * Ensure semantic diversity from harness_013 by using different construction methods.
 */


// Helper function to manually create cJSON objects with custom type bits
static cJSON* create_custom_cjson_object(int custom_type, const char* valuestring, double valuedouble, const char* string_name) {
    // Allocate memory manually to create cJSON object with specific type bits
    cJSON* item = (cJSON*)malloc(sizeof(cJSON));
    if (item == NULL) {
        return NULL;
    }
    
    // Initialize the structure
    memset(item, 0, sizeof(cJSON));
    item->type = custom_type;
    
    if (valuestring != NULL) {
        item->valuestring = strdup(valuestring);
    } else {
        item->valuestring = NULL;
    }
    
    item->valuedouble = valuedouble;
    item->valueint = (int)valuedouble;
    
    if (string_name != NULL) {
        item->string = strdup(string_name);
    } else {
        item->string = NULL;
    }
    
    item->next = NULL;
    item->prev = NULL;
    item->child = NULL;
    
    return item;
}

// Helper to safely free manually created cJSON objects
static void free_manual_cjson(cJSON* item) {
    if (item == NULL) return;
    
    if (item->valuestring != NULL) {
        free(item->valuestring);
    }
    if (item->string != NULL) {
        free(item->string);
    }
    
    // Note: We don't recursively free children since we create simple objects
    free(item);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 48) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 10;
    bool case_sensitive = fdp.ConsumeBool();
    bool test_null_strings = fdp.ConsumeBool();
    bool test_invalid_types = fdp.ConsumeBool();
    bool use_parsed_json = fdp.ConsumeBool();
    
    // Consume numeric values for testing
    double number1 = fdp.ConsumeFloatingPoint<double>();
    double number2 = fdp.ConsumeFloatingPoint<double>();
    int array_size = fdp.ConsumeIntegralInRange<int>(0, 8);
    int object_items = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Consume strings for testing
    std::string string1 = fdp.ConsumeRandomLengthString(256);
    std::string string2 = fdp.ConsumeRandomLengthString(256);
    std::string json_input = fdp.ConsumeRandomLengthString(512);
    
    // Generate keys for objects
    std::vector<std::string> object_keys;
    for (int i = 0; i < object_items && fdp.remaining_bytes() > 0; i++) {
        object_keys.push_back(fdp.ConsumeRandomLengthString(64));
    }
    
    // Use test_scenario to systematically test different uncovered comparison scenarios
    switch (test_scenario) {
        case 0: {
            // Test 1: NULL valuestring in cJSON_String and cJSON_Raw types
            // Create strings with NULL valuestring manually
            cJSON* str_with_null1 = create_custom_cjson_object(cJSON_String, NULL, 0.0, NULL);
            cJSON* str_with_null2 = create_custom_cjson_object(cJSON_String, NULL, 0.0, NULL);
            
            if (str_with_null1 != NULL && str_with_null2 != NULL) {
                // Both have NULL valuestring - should return false (line 3126-3129)
                cJSON_bool null_str_compare = cJSON_Compare(str_with_null1, str_with_null2, case_sensitive);
                (void)null_str_compare;
                
                // Compare NULL string with normal string
                cJSON* normal_str = cJSON_CreateString(string1.c_str());
                if (normal_str != NULL) {
                    cJSON_bool null_vs_normal = cJSON_Compare(str_with_null1, normal_str, case_sensitive);
                    cJSON_bool normal_vs_null = cJSON_Compare(normal_str, str_with_null1, case_sensitive);
                    (void)null_vs_normal;
                    (void)normal_vs_null;
                    cJSON_Delete(normal_str);
                }
                
                // Test with Raw type as well
                cJSON* raw_with_null1 = create_custom_cjson_object(cJSON_Raw, NULL, 0.0, NULL);
                cJSON* raw_with_null2 = create_custom_cjson_object(cJSON_Raw, NULL, 0.0, NULL);
                
                if (raw_with_null1 != NULL && raw_with_null2 != NULL) {
                    cJSON_bool null_raw_compare = cJSON_Compare(raw_with_null1, raw_with_null2, case_sensitive);
                    (void)null_raw_compare;
                    free_manual_cjson(raw_with_null1);
                    free_manual_cjson(raw_with_null2);
                }
            }
            
            if (str_with_null1 != NULL) free_manual_cjson(str_with_null1);
            if (str_with_null2 != NULL) free_manual_cjson(str_with_null2);
            break;
        }
        
        case 1: {
            // Test 2: Invalid type handling - default switch cases
            if (test_invalid_types) {
                // Create objects with invalid type bits
                // Test various invalid type combinations
                int invalid_types[] = {
                    0,  // cJSON_Invalid (0) - though this might be valid
                    255, // All bits set but not a valid type
                    512, // cJSON_StringIsConst flag alone
                    256, // cJSON_IsReference flag alone
                    768, // Both StringIsConst and IsReference
                };
                
                for (int i = 0; i < 5; i++) {
                    cJSON* invalid1 = create_custom_cjson_object(invalid_types[i], "test", 0.0, NULL);
                    cJSON* invalid2 = create_custom_cjson_object(invalid_types[i], "test", 0.0, NULL);
                    
                    if (invalid1 != NULL && invalid2 != NULL) {
                        // This should trigger the default switch case (lines 3099-3100)
                        cJSON_bool invalid_compare = cJSON_Compare(invalid1, invalid2, case_sensitive);
                        (void)invalid_compare;
                        
                        // Also compare invalid with valid type
                        cJSON* valid_str = cJSON_CreateString("valid");
                        if (valid_str != NULL) {
                            cJSON_bool invalid_vs_valid = cJSON_Compare(invalid1, valid_str, case_sensitive);
                            cJSON_bool valid_vs_invalid = cJSON_Compare(valid_str, invalid1, case_sensitive);
                            (void)invalid_vs_valid;
                            (void)valid_vs_invalid;
                            cJSON_Delete(valid_str);
                        }
                    }
                    
                    if (invalid1 != NULL) free_manual_cjson(invalid1);
                    if (invalid2 != NULL) free_manual_cjson(invalid2);
                }
            }
            break;
        }
        
        case 2: {
            // Test 3: Object comparison where a_element == NULL (lines 3183-3185)
            // Create objects with mismatched keys
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            if (obj1 != NULL && obj2 != NULL) {
                // Add different keys to each object
                for (size_t i = 0; i < object_keys.size() && i < 3; i++) {
                    std::string key1 = "key1_" + object_keys[i];
                    std::string key2 = "key2_" + object_keys[i];
                    
                    cJSON_AddNumberToObject(obj1, key1.c_str(), number1 + i);
                    cJSON_AddNumberToObject(obj2, key2.c_str(), number2 + i);
                }
                
                // Also add one common key with different values
                if (object_keys.size() > 0) {
                    std::string common_key = "common_key";
                    cJSON_AddNumberToObject(obj1, common_key.c_str(), 100.0);
                    cJSON_AddNumberToObject(obj2, common_key.c_str(), 200.0);
                }
                
                // Test comparison - should trigger various failure paths
                cJSON_bool obj_compare = cJSON_Compare(obj1, obj2, case_sensitive);
                (void)obj_compare;
                
                // Test with empty object vs non-empty object
                cJSON* empty_obj = cJSON_CreateObject();
                if (empty_obj != NULL) {
                    cJSON_bool empty_vs_full = cJSON_Compare(empty_obj, obj1, case_sensitive);
                    cJSON_bool full_vs_empty = cJSON_Compare(obj1, empty_obj, case_sensitive);
                    (void)empty_vs_full;
                    (void)full_vs_empty;
                    cJSON_Delete(empty_obj);
                }
            }
            
            if (obj1 != NULL) cJSON_Delete(obj1);
            if (obj2 != NULL) cJSON_Delete(obj2);
            break;
        }
        
        case 3: {
            // Test 4: Complex nested object comparisons with structural differences
            // Create deeply nested structures with differences
            cJSON* nested1 = cJSON_CreateObject();
            cJSON* nested2 = cJSON_CreateObject();
            
            if (nested1 != NULL && nested2 != NULL) {
                // Build similar but different nested structures
                cJSON* arr1 = cJSON_CreateArray();
                cJSON* arr2 = cJSON_CreateArray();
                
                if (arr1 != NULL && arr2 != NULL) {
                    // Fill arrays with different sizes/values
                    for (int i = 0; i < array_size && i < 4; i++) {
                        cJSON_AddItemToArray(arr1, cJSON_CreateNumber(number1 + i));
                        if (i < array_size - 1) {  // Make arr2 one element shorter
                            cJSON_AddItemToArray(arr2, cJSON_CreateNumber(number2 + i));
                        }
                    }
                    
                    cJSON_AddItemToObject(nested1, "array", arr1);
                    cJSON_AddItemToObject(nested2, "array", arr2);
                } else {
                    if (arr1 != NULL) cJSON_Delete(arr1);
                    if (arr2 != NULL) cJSON_Delete(arr2);
                }
                
                // Add nested objects with different structures
                cJSON* inner1 = cJSON_CreateObject();
                cJSON* inner2 = cJSON_CreateObject();
                
                if (inner1 != NULL && inner2 != NULL) {
                    cJSON_AddStringToObject(inner1, "inner_key1", string1.c_str());
                    cJSON_AddStringToObject(inner2, "inner_key2", string2.c_str());
                    
                    cJSON_AddItemToObject(nested1, "inner", inner1);
                    cJSON_AddItemToObject(nested2, "inner", inner2);
                } else {
                    if (inner1 != NULL) cJSON_Delete(inner1);
                    if (inner2 != NULL) cJSON_Delete(inner2);
                }
                
                // Test comparison of complex nested structures
                cJSON_bool nested_compare = cJSON_Compare(nested1, nested2, case_sensitive);
                (void)nested_compare;
            }
            
            if (nested1 != NULL) cJSON_Delete(nested1);
            if (nested2 != NULL) cJSON_Delete(nested2);
            break;
        }
        
        case 4: {
            // Test 5: Use parsed JSON vs manually created structures
            if (use_parsed_json && !json_input.empty()) {
                cJSON* parsed = cJSON_Parse(json_input.c_str());
                
                if (parsed != NULL) {
                    // Create a similar structure manually
                    cJSON* manual = cJSON_CreateObject();
                    if (manual != NULL) {
                        // Add some properties similar to what might be in parsed JSON
                        cJSON_AddStringToObject(manual, "test_string", string1.c_str());
                        cJSON_AddNumberToObject(manual, "test_number", number1);
                        
                        // Compare parsed vs manual (likely different)
                        cJSON_bool parsed_vs_manual = cJSON_Compare(parsed, manual, case_sensitive);
                        (void)parsed_vs_manual;
                        
                        // Compare parsed with itself
                        cJSON_bool parsed_self = cJSON_Compare(parsed, parsed, case_sensitive);
                        (void)parsed_self;
                        
                        cJSON_Delete(manual);
                    }
                    
                    cJSON_Delete(parsed);
                }
            }
            break;
        }
        
        case 5: {
            // Test 6: Case-sensitive vs case-insensitive string comparisons
            // Create strings with case differences
            cJSON* upper_str = cJSON_CreateString("TESTSTRING");
            cJSON* lower_str = cJSON_CreateString("teststring");
            cJSON* mixed_str = cJSON_CreateString("TestString");
            
            if (upper_str != NULL && lower_str != NULL && mixed_str != NULL) {
                // Test case-sensitive comparisons
                cJSON_bool upper_lower_cs = cJSON_Compare(upper_str, lower_str, true); // case-sensitive
                cJSON_bool upper_mixed_cs = cJSON_Compare(upper_str, mixed_str, true);
                
                // Test case-insensitive comparisons
                // Note: cJSON_Compare doesn't actually do case-insensitive string comparison
                // for cJSON_String type, only for object key lookups
                cJSON_bool upper_lower_ci = cJSON_Compare(upper_str, lower_str, false);
                cJSON_bool upper_mixed_ci = cJSON_Compare(upper_str, mixed_str, false);
                
                (void)upper_lower_cs;
                (void)upper_mixed_cs;
                (void)upper_lower_ci;
                (void)upper_mixed_ci;
                
                // Test with object keys (where case sensitivity matters)
                cJSON* obj_case1 = cJSON_CreateObject();
                cJSON* obj_case2 = cJSON_CreateObject();
                
                if (obj_case1 != NULL && obj_case2 != NULL) {
                    cJSON_AddNumberToObject(obj_case1, "KeyName", 1.0);
                    cJSON_AddNumberToObject(obj_case2, "keyname", 1.0);  // Different case
                    cJSON_AddNumberToObject(obj_case2, "KEYNAME", 2.0);  // Another different case
                    
                    cJSON_bool obj_case_cs = cJSON_Compare(obj_case1, obj_case2, true);
                    cJSON_bool obj_case_ci = cJSON_Compare(obj_case1, obj_case2, false);
                    (void)obj_case_cs;
                    (void)obj_case_ci;
                    
                    cJSON_Delete(obj_case1);
                    cJSON_Delete(obj_case2);
                }
            }
            
            if (upper_str != NULL) cJSON_Delete(upper_str);
            if (lower_str != NULL) cJSON_Delete(lower_str);
            if (mixed_str != NULL) cJSON_Delete(mixed_str);
            break;
        }
        
        case 6: {
            // Test 7: Arrays with different ordering
            cJSON* array_ordered = cJSON_CreateArray();
            cJSON* array_reversed = cJSON_CreateArray();
            
            if (array_ordered != NULL && array_reversed != NULL) {
                // Create values for arrays
                double values[4];
                for (int i = 0; i < 4 && i < array_size; i++) {
                    values[i] = fdp.ConsumeFloatingPoint<double>();
                }
                
                // Add in order
                for (int i = 0; i < 4 && i < array_size; i++) {
                    cJSON_AddItemToArray(array_ordered, cJSON_CreateNumber(values[i]));
                }
                
                // Add in reverse order
                for (int i = std::min(3, array_size - 1); i >= 0; i--) {
                    cJSON_AddItemToArray(array_reversed, cJSON_CreateNumber(values[i]));
                }
                
                // Arrays with same values but different order should compare as different
                cJSON_bool ordered_vs_reversed = cJSON_Compare(array_ordered, array_reversed, case_sensitive);
                (void)ordered_vs_reversed;
            }
            
            if (array_ordered != NULL) cJSON_Delete(array_ordered);
            if (array_reversed != NULL) cJSON_Delete(array_reversed);
            break;
        }
        
        case 7: {
            // Test 8: Mixed type comparisons that should fail early
            // Compare objects of completely different types
            cJSON* bool_obj = cJSON_CreateBool(true);
            cJSON* num_obj = cJSON_CreateNumber(42.0);
            cJSON* str_obj = cJSON_CreateString("test");
            cJSON* null_obj = cJSON_CreateNull();
            cJSON* arr_obj = cJSON_CreateArray();
            cJSON* obj_obj = cJSON_CreateObject();
            
            // Cross-type comparisons (should return false due to type mismatch)
            if (bool_obj != NULL && num_obj != NULL) {
                cJSON_bool bool_vs_num = cJSON_Compare(bool_obj, num_obj, case_sensitive);
                (void)bool_vs_num;
            }
            
            if (str_obj != NULL && arr_obj != NULL) {
                cJSON_bool str_vs_arr = cJSON_Compare(str_obj, arr_obj, case_sensitive);
                (void)str_vs_arr;
            }
            
            if (null_obj != NULL && obj_obj != NULL) {
                cJSON_bool null_vs_obj = cJSON_Compare(null_obj, obj_obj, case_sensitive);
                (void)null_vs_obj;
            }
            
            // Cleanup
            if (bool_obj != NULL) cJSON_Delete(bool_obj);
            if (num_obj != NULL) cJSON_Delete(num_obj);
            if (str_obj != NULL) cJSON_Delete(str_obj);
            if (null_obj != NULL) cJSON_Delete(null_obj);
            if (arr_obj != NULL) cJSON_Delete(arr_obj);
            if (obj_obj != NULL) cJSON_Delete(obj_obj);
            break;
        }
        
        case 8: {
            // Test 9: Special floating point values
            cJSON* nan1 = cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN());
            cJSON* nan2 = cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN());
            cJSON* inf1 = cJSON_CreateNumber(std::numeric_limits<double>::infinity());
            cJSON* inf2 = cJSON_CreateNumber(std::numeric_limits<double>::infinity());
            cJSON* neg_inf = cJSON_CreateNumber(-std::numeric_limits<double>::infinity());
            cJSON* normal = cJSON_CreateNumber(42.0);
            
            if (nan1 != NULL && nan2 != NULL && inf1 != NULL && inf2 != NULL && 
                neg_inf != NULL && normal != NULL) {
                // NaN comparisons (should return false as NaN != NaN)
                cJSON_bool nan_nan = cJSON_Compare(nan1, nan2, case_sensitive);
                cJSON_bool nan_normal = cJSON_Compare(nan1, normal, case_sensitive);
                
                // Infinity comparisons
                cJSON_bool inf_inf = cJSON_Compare(inf1, inf2, case_sensitive);
                cJSON_bool inf_neg_inf = cJSON_Compare(inf1, neg_inf, case_sensitive);
                
                (void)nan_nan;
                (void)nan_normal;
                (void)inf_inf;
                (void)inf_neg_inf;
            }
            
            if (nan1 != NULL) cJSON_Delete(nan1);
            if (nan2 != NULL) cJSON_Delete(nan2);
            if (inf1 != NULL) cJSON_Delete(inf1);
            if (inf2 != NULL) cJSON_Delete(inf2);
            if (neg_inf != NULL) cJSON_Delete(neg_inf);
            if (normal != NULL) cJSON_Delete(normal);
            break;
        }
        
        case 9: {
            // Test 10: Combined edge cases
            // Create objects with various edge cases
            cJSON* edge1 = cJSON_CreateObject();
            cJSON* edge2 = cJSON_CreateObject();
            
            if (edge1 != NULL && edge2 != NULL) {
                // Add empty string
                cJSON_AddStringToObject(edge1, "empty", "");
                
                // Add very long string if we have enough data
                if (fdp.remaining_bytes() > 100) {
                    std::string long_str = fdp.ConsumeRandomLengthString(100);
                    cJSON_AddStringToObject(edge2, "long", long_str.c_str());
                }
                
                // Add special number values
                cJSON_AddNumberToObject(edge1, "zero", 0.0);
                cJSON_AddNumberToObject(edge2, "negative", -0.0);
                
                // Add boolean with opposite values
                cJSON_AddBoolToObject(edge1, "flag", true);
                cJSON_AddBoolToObject(edge2, "flag", false);
                
                // Add nested empty array
                cJSON* empty_arr = cJSON_CreateArray();
                if (empty_arr != NULL) {
                    cJSON_AddItemToObject(edge1, "empty_array", empty_arr);
                }
                
                // Add nested array with content
                cJSON* filled_arr = cJSON_CreateArray();
                if (filled_arr != NULL) {
                    cJSON_AddItemToArray(filled_arr, cJSON_CreateNumber(1.0));
                    cJSON_AddItemToObject(edge2, "filled_array", filled_arr);
                }
                
                // Compare the edge case objects
                cJSON_bool edge_compare = cJSON_Compare(edge1, edge2, case_sensitive);
                (void)edge_compare;
            }
            
            if (edge1 != NULL) cJSON_Delete(edge1);
            if (edge2 != NULL) cJSON_Delete(edge2);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_014
