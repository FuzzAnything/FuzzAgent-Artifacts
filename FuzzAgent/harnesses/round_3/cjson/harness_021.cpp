#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

namespace harness_021 {
/*
 * Harness 021: Comprehensive cJSON_Compare Scenarios to Unblock Branch Coverage
 * 
 * Target: Unblock 73/76 blocked branches in cJSON_Compare function
 * Based on coverage analysis: cJSON_Compare has only 3 branches covered out of 76
 * 
 * APIs to target:
 * - cJSON_Compare (primary) - main comparison function with case sensitivity
 * - cJSON_Parse (helper) - for creating JSON structures from strings
 * - cJSON_CreateArray, cJSON_CreateObject, cJSON_CreateString, cJSON_CreateNumber,
 *   cJSON_CreateBool, cJSON_CreateRaw, cJSON_CreateNull, cJSON_CreateTrue, cJSON_CreateFalse
 * - cJSON_AddItemToArray, cJSON_AddItemToObject (helper for building structures)
 * - cJSON_Delete (cleanup)
 * 
 * Invocation sequence:
 * 1. Create various JSON structures A and B (identical and different)
 * 2. Call cJSON_Compare(A, B, case_sensitive=true)
 * 3. Call cJSON_Compare(A, B, case_sensitive=false) for strings
 * 4. Test edge cases: NULL inputs, different types, nested structures, arrays of different lengths,
 *    objects with same/different keys, strings with case variations, numbers with floating-point precision differences
 * 
 * Specific test scenarios:
 * 1. Compare identical simple types (null, false, true, numbers)
 * 2. Compare different numbers (with compare_double precision logic)
 * 3. Compare strings with same/different content
 * 4. Compare strings with case variations (case_sensitive=true/false)
 * 5. Compare arrays of same/different lengths
 * 6. Compare arrays with same/different elements
 * 7. Compare objects with same/different keys
 * 8. Compare nested structures
 * 9. Test NULL input handling
 * 
 * Semantic diversity from existing harnesses:
 * - harness_000.cpp to harness_020.cpp focus on parsing, creation, and manipulation
 * - No existing harness specifically targets cJSON_Compare comprehensively
 * - This harness systematically tests comparison logic across all JSON types
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for various test scenarios
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration data
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 12;
    bool case_sensitive = fdp.ConsumeBool();
    bool test_null_inputs = fdp.ConsumeBool();
    
    // Consume various data for JSON creation
    double number1 = fdp.ConsumeFloatingPoint<double>();
    double number2 = fdp.ConsumeFloatingPoint<double>();
    bool bool1 = fdp.ConsumeBool();
    bool bool2 = fdp.ConsumeBool();
    
    std::string str1 = fdp.ConsumeRandomLengthString(256);
    std::string str2 = fdp.ConsumeRandomLengthString(256);
    std::string raw1 = fdp.ConsumeRandomLengthString(512);
    std::string raw2 = fdp.ConsumeRandomLengthString(512);
    
    std::string key1 = fdp.ConsumeRandomLengthString(64);
    std::string key2 = fdp.ConsumeRandomLengthString(64);
    std::string key3 = fdp.ConsumeRandomLengthString(64);
    std::string key4 = fdp.ConsumeRandomLengthString(64);
    
    std::string json_str1 = fdp.ConsumeRandomLengthString(1024);
    std::string json_str2 = fdp.ConsumeRandomLengthString(1024);
    
    // Create test results holder
    std::vector<cJSON_bool> comparison_results;
    
    // Test different scenarios based on test_scenario
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Compare identical simple types
            cJSON* null1 = cJSON_CreateNull();
            cJSON* null2 = cJSON_CreateNull();
            cJSON_bool result = cJSON_Compare(null1, null2, case_sensitive);
            comparison_results.push_back(result);
            cJSON_Delete(null1);
            cJSON_Delete(null2);
            
            cJSON* true1 = cJSON_CreateTrue();
            cJSON* true2 = cJSON_CreateTrue();
            result = cJSON_Compare(true1, true2, case_sensitive);
            comparison_results.push_back(result);
            cJSON_Delete(true1);
            cJSON_Delete(true2);
            
            cJSON* false1 = cJSON_CreateFalse();
            cJSON* false2 = cJSON_CreateFalse();
            result = cJSON_Compare(false1, false2, case_sensitive);
            comparison_results.push_back(result);
            cJSON_Delete(false1);
            cJSON_Delete(false2);
            
            cJSON* num1 = cJSON_CreateNumber(number1);
            cJSON* num2 = cJSON_CreateNumber(number1); // Same number
            result = cJSON_Compare(num1, num2, case_sensitive);
            comparison_results.push_back(result);
            cJSON_Delete(num1);
            cJSON_Delete(num2);
            break;
        }
        
        case 1: {
            // Scenario 2: Compare different numbers
            cJSON* num1 = cJSON_CreateNumber(number1);
            cJSON* num2 = cJSON_CreateNumber(number2);
            cJSON_bool result = cJSON_Compare(num1, num2, case_sensitive);
            comparison_results.push_back(result);
            cJSON_Delete(num1);
            cJSON_Delete(num2);
            break;
        }
        
        case 2: {
            // Scenario 3: Compare strings with same/different content
            cJSON* str_json1 = cJSON_CreateString(str1.c_str());
            cJSON* str_json2 = cJSON_CreateString(str1.c_str()); // Same string
            cJSON_bool result = cJSON_Compare(str_json1, str_json2, case_sensitive);
            comparison_results.push_back(result);
            cJSON_Delete(str_json1);
            cJSON_Delete(str_json2);
            
            // Different strings
            str_json1 = cJSON_CreateString(str1.c_str());
            str_json2 = cJSON_CreateString(str2.c_str());
            result = cJSON_Compare(str_json1, str_json2, case_sensitive);
            comparison_results.push_back(result);
            cJSON_Delete(str_json1);
            cJSON_Delete(str_json2);
            break;
        }
        case 3: {
            // Scenario 4: Compare strings with case variations
            std::string lower_str = fdp.ConsumeRandomLengthString(32);
            std::string upper_str = lower_str;
            std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
            std::string mixed_str = lower_str;
            if (mixed_str.length() > 0) {
                mixed_str[0] = toupper(mixed_str[0]);
            }
            
            cJSON* lower = cJSON_CreateString(lower_str.c_str());
            cJSON* upper = cJSON_CreateString(upper_str.c_str());
            cJSON* mixed = cJSON_CreateString(mixed_str.c_str());
            
            // Test case-sensitive comparison
            cJSON_bool result1 = cJSON_Compare(lower, upper, true); // case_sensitive=true
            cJSON_bool result2 = cJSON_Compare(lower, upper, false); // case_sensitive=false
            
            // Test mixed case
            cJSON_bool result3 = cJSON_Compare(lower, mixed, true);
            cJSON_bool result4 = cJSON_Compare(lower, mixed, false);
            
            comparison_results.push_back(result1);
            comparison_results.push_back(result2);
            comparison_results.push_back(result3);
            comparison_results.push_back(result4);
            
            cJSON_Delete(lower);
            cJSON_Delete(upper);
            cJSON_Delete(mixed);
            break;
        }
        
        case 4: {
            // Scenario 5: Compare arrays of same/different lengths
            cJSON* array1 = cJSON_CreateArray();
            cJSON* array2 = cJSON_CreateArray();
            
            // Add same number of elements
            int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < array_size; i++) {
                double val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(val));
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(val));
            }
            
            cJSON_bool same_length_result = cJSON_Compare(array1, array2, case_sensitive);
            comparison_results.push_back(same_length_result);
            
            // Add extra element to array2
            cJSON_AddItemToArray(array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            cJSON_bool diff_length_result = cJSON_Compare(array1, array2, case_sensitive);
            comparison_results.push_back(diff_length_result);
            
            cJSON_Delete(array1);
            cJSON_Delete(array2);
            break;
        }
        
        case 5: {
            // Scenario 6: Compare arrays with same/different elements
            cJSON* array1 = cJSON_CreateArray();
            cJSON* array2 = cJSON_CreateArray();
            
            // Add same elements
            for (int i = 0; i < 3; i++) {
                double val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(val));
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(val));
            }
            
            cJSON_bool same_elements_result = cJSON_Compare(array1, array2, case_sensitive);
            comparison_results.push_back(same_elements_result);
            
            // Modify one element in array2
            cJSON_Delete(array2->child->next); // Remove second element
            array2->child->next = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>() + 1.0); // Different value
            
            cJSON_bool diff_elements_result = cJSON_Compare(array1, array2, case_sensitive);
            comparison_results.push_back(diff_elements_result);
            
            cJSON_Delete(array1);
            cJSON_Delete(array2);
            break;
        }
        
        case 6: {
            // Scenario 7: Compare objects with same/different keys
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            // Add same keys with same values
            cJSON_AddItemToObject(obj1, key1.c_str(), cJSON_CreateNumber(number1));
            cJSON_AddItemToObject(obj2, key1.c_str(), cJSON_CreateNumber(number1));
            
            cJSON_AddItemToObject(obj1, key2.c_str(), cJSON_CreateString(str1.c_str()));
            cJSON_AddItemToObject(obj2, key2.c_str(), cJSON_CreateString(str1.c_str()));
            
            cJSON_bool same_keys_result = cJSON_Compare(obj1, obj2, case_sensitive);
            comparison_results.push_back(same_keys_result);
            
            // Add different key to obj2
            cJSON_AddItemToObject(obj2, key3.c_str(), cJSON_CreateNumber(number2));
            cJSON_bool diff_keys_result = cJSON_Compare(obj1, obj2, case_sensitive);
            comparison_results.push_back(diff_keys_result);
            
            cJSON_Delete(obj1);
            cJSON_Delete(obj2);
            break;
        }
        
        case 7: {
            // Scenario 8: Compare nested structures
            cJSON* nested1 = cJSON_CreateObject();
            cJSON* nested2 = cJSON_CreateObject();
            
            // Create nested array inside object
            cJSON* inner_array1 = cJSON_CreateArray();
            cJSON* inner_array2 = cJSON_CreateArray();
            
            for (int i = 0; i < 2; i++) {
                cJSON_AddItemToArray(inner_array1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(inner_array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            cJSON_AddItemToObject(nested1, "nested_array", inner_array1);
            cJSON_AddItemToObject(nested2, "nested_array", inner_array2);
            
            // Add object inside array
            cJSON* outer_array1 = cJSON_CreateArray();
            cJSON* outer_array2 = cJSON_CreateArray();
            
            cJSON* inner_obj1 = cJSON_CreateObject();
            cJSON* inner_obj2 = cJSON_CreateObject();
            
            cJSON_AddItemToObject(inner_obj1, "inner_key", cJSON_CreateString(str1.c_str()));
            cJSON_AddItemToObject(inner_obj2, "inner_key", cJSON_CreateString(str1.c_str()));
            
            cJSON_AddItemToArray(outer_array1, inner_obj1);
            cJSON_AddItemToArray(outer_array2, inner_obj2);
            
            cJSON_AddItemToObject(nested1, "outer_array", outer_array1);
            cJSON_AddItemToObject(nested2, "outer_array", outer_array2);
            
            cJSON_bool nested_result = cJSON_Compare(nested1, nested2, case_sensitive);
            comparison_results.push_back(nested_result);
            
            cJSON_Delete(nested1);
            cJSON_Delete(nested2);
            break;
        }
        
        case 8: {
            // Scenario 9: Test NULL input handling
            cJSON* valid_json = cJSON_CreateNumber(number1);
            
            // Test NULL as first parameter
            cJSON_bool result1 = cJSON_Compare(NULL, valid_json, case_sensitive);
            comparison_results.push_back(result1);
            
            // Test NULL as second parameter
            cJSON_bool result2 = cJSON_Compare(valid_json, NULL, case_sensitive);
            comparison_results.push_back(result2);
            
            // Test both NULL
            cJSON_bool result3 = cJSON_Compare(NULL, NULL, case_sensitive);
            comparison_results.push_back(result3);
            
            cJSON_Delete(valid_json);
            break;
        }
        
        case 9: {
            // Scenario 10: Compare different types (should return false)
            cJSON* null_json = cJSON_CreateNull();
            cJSON* bool_json = cJSON_CreateBool(bool1);
            cJSON* num_json = cJSON_CreateNumber(number1);
            cJSON* str_json = cJSON_CreateString(str1.c_str());
            cJSON* array_json = cJSON_CreateArray();
            cJSON* obj_json = cJSON_CreateObject();
            
            // Compare null with bool
            cJSON_bool result1 = cJSON_Compare(null_json, bool_json, case_sensitive);
            comparison_results.push_back(result1);
            
            // Compare number with string
            cJSON_bool result2 = cJSON_Compare(num_json, str_json, case_sensitive);
            comparison_results.push_back(result2);
            
            // Compare array with object
            cJSON_bool result3 = cJSON_Compare(array_json, obj_json, case_sensitive);
            comparison_results.push_back(result3);
            
            // Compare bool with number
            cJSON_bool result4 = cJSON_Compare(bool_json, num_json, case_sensitive);
            comparison_results.push_back(result4);
            
            cJSON_Delete(null_json);
            cJSON_Delete(bool_json);
            cJSON_Delete(num_json);
            cJSON_Delete(str_json);
            cJSON_Delete(array_json);
            cJSON_Delete(obj_json);
            break;
        }
        
        case 10: {
            // Scenario 11: Compare raw JSON strings
            cJSON* raw1_json = cJSON_CreateRaw(raw1.c_str());
            cJSON* raw2_json = cJSON_CreateRaw(raw1.c_str()); // Same raw
            cJSON* raw3_json = cJSON_CreateRaw(raw2.c_str()); // Different raw
            
            // Same raw strings
            cJSON_bool result1 = cJSON_Compare(raw1_json, raw2_json, case_sensitive);
            comparison_results.push_back(result1);
            
            // Different raw strings
            cJSON_bool result2 = cJSON_Compare(raw1_json, raw3_json, case_sensitive);
            comparison_results.push_back(result2);
            
            cJSON_Delete(raw1_json);
            cJSON_Delete(raw2_json);
            cJSON_Delete(raw3_json);
            break;
        }
        
        case 11: {
            // Scenario 12: Parse JSON strings and compare
            cJSON* parsed1 = cJSON_Parse(json_str1.c_str());
            cJSON* parsed2 = cJSON_Parse(json_str2.c_str());
            
            if (parsed1 != NULL && parsed2 != NULL) {
                cJSON_bool result = cJSON_Compare(parsed1, parsed2, case_sensitive);
                comparison_results.push_back(result);
            }
            
            // Also test comparing parsed JSON with created JSON of same content
            if (parsed1 != NULL) {
                // Re-create similar structure manually
                cJSON* manual = cJSON_CreateObject();
                if (manual != NULL) {
                    // Try to add some similar structure
                    cJSON_AddItemToObject(manual, "test_key", cJSON_CreateString("test_value"));
                    cJSON_bool result = cJSON_Compare(parsed1, manual, case_sensitive);
                    comparison_results.push_back(result);
                    cJSON_Delete(manual);
                }
            }
            
            if (parsed1 != NULL) cJSON_Delete(parsed1);
            if (parsed2 != NULL) cJSON_Delete(parsed2);
            break;
        }
    }
    
    // Ensure we actually use the comparison results to prevent optimization
    volatile cJSON_bool dummy = false;
    for (cJSON_bool result : comparison_results) {
        dummy = dummy || result;
    }
    
    return 0;
}

} // namespace harness_021
