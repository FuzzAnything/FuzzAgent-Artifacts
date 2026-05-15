#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <cfloat>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_023 {

extern "C" {
}

// Helper to create JSON number with specific floating point values
cJSON* create_number_with_value(FuzzedDataProvider& fdp, double value) {
    return cJSON_CreateNumber(value);
}

// Helper to create JSON with specific numeric edge cases
cJSON* create_number_edge_case(FuzzedDataProvider& fdp) {
    uint8_t edge_case = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (edge_case) {
        case 0: return cJSON_CreateNumber(0.0);
        case 1: return cJSON_CreateNumber(-0.0);
        case 2: return cJSON_CreateNumber(DBL_MAX);
        case 3: return cJSON_CreateNumber(DBL_MIN);
        case 4: return cJSON_CreateNumber(INFINITY);
        case 5: return cJSON_CreateNumber(-INFINITY);
        case 6: return cJSON_CreateNumber(NAN);
        case 7: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        default: return cJSON_CreateNumber(0.0);
    }
}

// Helper to create Raw JSON type
cJSON* create_raw_json(FuzzedDataProvider& fdp) {
    std::string raw_data = fdp.ConsumeRandomLengthString(256);
    return cJSON_CreateRaw(raw_data.c_str());
}

// Helper to create arrays with specific length mismatches
void create_arrays_with_length_mismatch(FuzzedDataProvider& fdp, cJSON** arr1, cJSON** arr2) {
    *arr1 = cJSON_CreateArray();
    *arr2 = cJSON_CreateArray();
    
    if (*arr1 == NULL || *arr2 == NULL) {
        if (*arr1) cJSON_Delete(*arr1);
        if (*arr2) cJSON_Delete(*arr2);
        *arr1 = *arr2 = NULL;
        return;
    }
    
    // Create arrays with different lengths
    int len1 = fdp.ConsumeIntegralInRange<int>(0, 5);
    int len2 = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Make sure lengths are different
    if (len1 == len2) {
        len2 = len1 + 1;
    }
    
    for (int i = 0; i < len1 && fdp.remaining_bytes() > 0; i++) {
        cJSON_AddItemToArray(*arr1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    }
    
    for (int i = 0; i < len2 && fdp.remaining_bytes() > 0; i++) {
        cJSON_AddItemToArray(*arr2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    }
}

// Helper to create objects with case-sensitive key differences
void create_objects_with_case_keys(FuzzedDataProvider& fdp, cJSON** obj1, cJSON** obj2) {
    *obj1 = cJSON_CreateObject();
    *obj2 = cJSON_CreateObject();
    
    if (*obj1 == NULL || *obj2 == NULL) {
        if (*obj1) cJSON_Delete(*obj1);
        if (*obj2) cJSON_Delete(*obj2);
        *obj1 = *obj2 = NULL;
        return;
    }
    
    // Create keys with case differences
    std::string base_key = fdp.ConsumeRandomLengthString(8);
    if (base_key.empty()) base_key = "key";
    
    std::string lower_key = base_key;
    std::string upper_key = base_key;
    
    // Convert to lower and upper case
    std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(), ::tolower);
    std::transform(upper_key.begin(), upper_key.end(), upper_key.begin(), ::toupper);
    
    // Add different values with case-different keys
    double value1 = fdp.ConsumeFloatingPoint<double>();
    double value2 = fdp.ConsumeFloatingPoint<double>();
    
    cJSON_AddNumberToObject(*obj1, lower_key.c_str(), value1);
    cJSON_AddNumberToObject(*obj2, upper_key.c_str(), value2);
    
    // Add some additional common properties
    int common_props = fdp.ConsumeIntegralInRange<int>(0, 3);
    for (int i = 0; i < common_props && fdp.remaining_bytes() > 0; i++) {
        std::string common_key = fdp.ConsumeRandomLengthString(6);
        double common_value = fdp.ConsumeFloatingPoint<double>();
        cJSON_AddNumberToObject(*obj1, common_key.c_str(), common_value);
        cJSON_AddNumberToObject(*obj2, common_key.c_str(), common_value);
    }
}

// Helper to create reference strings (cJSON_StringIsConst)
// Note: cJSON_CreateStringReference requires the string to have longer lifetime than the cJSON object
// The caller must ensure the string data remains valid

// Helper to test invalid/corrupted JSON types
cJSON* create_potentially_invalid_type(FuzzedDataProvider& fdp) {
    // Try to create an object and manually manipulate the type field
    // to simulate invalid types that might occur in corrupted memory
    cJSON* obj = cJSON_CreateObject();
    if (obj == NULL) return NULL;
    
    uint8_t invalid_type_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    if (invalid_type_choice == 0) {
        // Set to an invalid type value
        obj->type = 0xFF; // Invalid type
    } else if (invalid_type_choice == 1) {
        // Set to a type outside valid range
        obj->type = 0x100;
    } else if (invalid_type_choice == 2) {
        // Combine with reference flag
        obj->type = cJSON_String | cJSON_IsReference;
    }
    // else keep as valid object
    
    return obj;
}

// Test number comparison with floating point edge cases
void test_number_edge_cases(FuzzedDataProvider& fdp, bool case_sensitive) {
    cJSON* num1 = create_number_edge_case(fdp);
    cJSON* num2 = create_number_edge_case(fdp);
    
    if (num1 != NULL && num2 != NULL) {
        // Test comparison
        cJSON_Compare(num1, num2, case_sensitive);
        
        // Test self-comparison
        cJSON_Compare(num1, num1, case_sensitive);
        cJSON_Compare(num2, num2, case_sensitive);
        
        // Test with specific known equal values
        cJSON* zero1 = cJSON_CreateNumber(0.0);
        cJSON* zero2 = cJSON_CreateNumber(0.0);
        cJSON* neg_zero = cJSON_CreateNumber(-0.0);
        
        if (zero1 != NULL && zero2 != NULL && neg_zero != NULL) {
            cJSON_Compare(zero1, zero2, case_sensitive); // Should be true
            cJSON_Compare(zero1, neg_zero, case_sensitive); // Should be true (0.0 == -0.0)
        }
        
        if (zero1) cJSON_Delete(zero1);
        if (zero2) cJSON_Delete(zero2);
        if (neg_zero) cJSON_Delete(neg_zero);
    }
    
    if (num1) cJSON_Delete(num1);
    if (num2) cJSON_Delete(num2);
}

// Test Raw JSON type comparisons
void test_raw_json_comparisons(FuzzedDataProvider& fdp, bool case_sensitive) {
    cJSON* raw1 = create_raw_json(fdp);
    cJSON* raw2 = create_raw_json(fdp);
    
    if (raw1 != NULL && raw2 != NULL) {
        // Compare raw JSON objects
        cJSON_Compare(raw1, raw2, case_sensitive);
        
        // Compare raw with string (different types)
        cJSON* str = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
        if (str != NULL) {
            cJSON_Compare(raw1, str, case_sensitive); // Should be false (different types)
            cJSON_Delete(str);
        }
        
        // Self comparison
        cJSON_Compare(raw1, raw1, case_sensitive);
        cJSON_Compare(raw2, raw2, case_sensitive);
    }
    
    if (raw1) cJSON_Delete(raw1);
    if (raw2) cJSON_Delete(raw2);
}

// Test array length mismatch scenarios
void test_array_length_mismatches(FuzzedDataProvider& fdp, bool case_sensitive) {
    cJSON* arr1 = NULL;
    cJSON* arr2 = NULL;
    
    create_arrays_with_length_mismatch(fdp, &arr1, &arr2);
    
    if (arr1 != NULL && arr2 != NULL) {
        // Compare arrays with different lengths (should return false)
        cJSON_Compare(arr1, arr2, case_sensitive);
        
        // Compare arrays with same content but potentially different lengths
        cJSON_Compare(arr1, arr1, case_sensitive);
        cJSON_Compare(arr2, arr2, case_sensitive);
        
        // Create empty array for comparison
        cJSON* empty_arr = cJSON_CreateArray();
        if (empty_arr != NULL) {
            cJSON_Compare(arr1, empty_arr, case_sensitive);
            cJSON_Compare(empty_arr, arr2, case_sensitive);
            cJSON_Delete(empty_arr);
        }
    }
    
    if (arr1) cJSON_Delete(arr1);
    if (arr2) cJSON_Delete(arr2);
}

// Test object case sensitivity thoroughly
void test_object_case_sensitivity(FuzzedDataProvider& fdp) {
    cJSON* obj1 = NULL;
    cJSON* obj2 = NULL;
    
    create_objects_with_case_keys(fdp, &obj1, &obj2);
    
    if (obj1 != NULL && obj2 != NULL) {
        // Test with case_sensitive = true
        cJSON_Compare(obj1, obj2, true);
        
        // Test with case_sensitive = false
        cJSON_Compare(obj1, obj2, false);
        
        // Test self comparisons
        cJSON_Compare(obj1, obj1, true);
        cJSON_Compare(obj1, obj1, false);
        cJSON_Compare(obj2, obj2, true);
        cJSON_Compare(obj2, obj2, false);
        
        // Create object with same keys but different values
        cJSON* obj3 = cJSON_CreateObject();
        if (obj3 != NULL) {
            std::string key = fdp.ConsumeRandomLengthString(6);
            cJSON_AddNumberToObject(obj3, key.c_str(), fdp.ConsumeFloatingPoint<double>());
            cJSON_AddNumberToObject(obj3, key.c_str(), fdp.ConsumeFloatingPoint<double>()); // Same key twice
            
            cJSON_Compare(obj1, obj3, true);
            cJSON_Compare(obj1, obj3, false);
            
            cJSON_Delete(obj3);
        }
    }
    
    if (obj1) cJSON_Delete(obj1);
    if (obj2) cJSON_Delete(obj2);
}

// Test reference strings and string comparisons
void test_string_references(FuzzedDataProvider& fdp, bool case_sensitive) {
    // Test regular strings
    std::string str1 = fdp.ConsumeRandomLengthString(32);
    std::string str2 = fdp.ConsumeRandomLengthString(32);
    
    cJSON* json_str1 = cJSON_CreateString(str1.c_str());
    cJSON* json_str2 = cJSON_CreateString(str2.c_str());
    
    if (json_str1 != NULL && json_str2 != NULL) {
        cJSON_Compare(json_str1, json_str2, case_sensitive);
        cJSON_Compare(json_str1, json_str1, case_sensitive);
        cJSON_Compare(json_str2, json_str2, case_sensitive);
        
        // Test with NULL valuestring (should return false)
        cJSON* str_with_null = cJSON_CreateString("");
        if (str_with_null != NULL && str_with_null->valuestring != NULL) {
            // Manually set valuestring to NULL to test that branch
            char* old_valuestring = str_with_null->valuestring;
            str_with_null->valuestring = NULL;
            cJSON_Compare(json_str1, str_with_null, case_sensitive);
            cJSON_Compare(str_with_null, json_str2, case_sensitive);
            // Restore to avoid double free
            str_with_null->valuestring = old_valuestring;
            cJSON_Delete(str_with_null);
        }
    }
    
    // Test string references - use cJSON_CreateString instead of cJSON_CreateStringReference
    // to avoid use-after-free issues with temporary strings
    std::string ref_str1_data = fdp.ConsumeRandomLengthString(32);
    std::string ref_str2_data = fdp.ConsumeRandomLengthString(32);
    
    cJSON* ref_str1 = cJSON_CreateString(ref_str1_data.c_str());
    cJSON* ref_str2 = cJSON_CreateString(ref_str2_data.c_str());
    
    if (ref_str1 != NULL && ref_str2 != NULL) {
        cJSON_Compare(ref_str1, ref_str2, case_sensitive);
        cJSON_Compare(ref_str1, ref_str1, case_sensitive);
        cJSON_Compare(ref_str2, ref_str2, case_sensitive);
        
        // Compare reference string with regular string
        if (json_str1 != NULL) {
            cJSON_Compare(ref_str1, json_str1, case_sensitive);
            cJSON_Compare(json_str1, ref_str1, case_sensitive);
        }
    }
    
    if (json_str1) cJSON_Delete(json_str1);
    if (json_str2) cJSON_Delete(json_str2);
    if (ref_str1) cJSON_Delete(ref_str1);
    if (ref_str2) cJSON_Delete(ref_str2);
}
// Test invalid/corrupted types
void test_invalid_types(FuzzedDataProvider& fdp, bool case_sensitive) {
    cJSON* potentially_invalid = create_potentially_invalid_type(fdp);
    cJSON* valid_obj = cJSON_CreateObject();
    
    if (potentially_invalid != NULL && valid_obj != NULL) {
        // Compare potentially invalid with valid object
        cJSON_Compare(potentially_invalid, valid_obj, case_sensitive);
        cJSON_Compare(valid_obj, potentially_invalid, case_sensitive);
        
        // Compare potentially invalid with itself
        cJSON_Compare(potentially_invalid, potentially_invalid, case_sensitive);
        
        // Test NULL comparisons
        cJSON_Compare(NULL, potentially_invalid, case_sensitive);
        cJSON_Compare(potentially_invalid, NULL, case_sensitive);
    }
    
    // Test with different actual types
    cJSON* types[7];
    types[0] = cJSON_CreateFalse();
    types[1] = cJSON_CreateTrue();
    types[2] = cJSON_CreateNull();
    types[3] = cJSON_CreateNumber(42.0);
    types[4] = cJSON_CreateString("test");
    types[5] = cJSON_CreateArray();
    types[6] = cJSON_CreateObject();
    
    // Compare all types with each other (should mostly return false for different types)
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (types[i] != NULL && types[j] != NULL) {
                cJSON_Compare(types[i], types[j], case_sensitive);
            }
        }
    }
    
    // Cleanup
    if (potentially_invalid) cJSON_Delete(potentially_invalid);
    if (valid_obj) cJSON_Delete(valid_obj);
    for (int i = 0; i < 7; i++) {
        if (types[i]) cJSON_Delete(types[i]);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0; // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector focusing on uncovered branches
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool case_sensitive = fdp.ConsumeBool();
    
    switch (test_scenario) {
        case 0:
            // Test 1: Number comparisons with floating point edge cases
            // Targets compare_double() and number comparison branches
            test_number_edge_cases(fdp, case_sensitive);
            break;
            
        case 1:
            // Test 2: Raw JSON type comparisons
            // Targets cJSON_Raw type handling (often overlooked)
            test_raw_json_comparisons(fdp, case_sensitive);
            break;
            
        case 2:
            // Test 3: Array length mismatch scenarios
            // Targets array length comparison branch (a_element != b_element)
            test_array_length_mismatches(fdp, case_sensitive);
            break;
            
        case 3:
            // Test 4: Object case sensitivity and key lookup
            // Targets get_object_item() with case_sensitive parameter
            test_object_case_sensitivity(fdp);
            break;
            
        case 4:
            // Test 5: String references and edge cases
            // Targets string comparison with NULL valuestring and reference strings
            test_string_references(fdp, case_sensitive);
            break;
            
        case 5:
            // Test 6: Invalid/corrupted types and type validation
            // Targets invalid type checking in switch statement
            test_invalid_types(fdp, case_sensitive);
            break;
            
        case 6:
            // Test 7: Mixed nested structures with various types
            // Comprehensive test hitting multiple code paths
            {
                // Create complex nested structure
                cJSON* root1 = cJSON_CreateObject();
                cJSON* root2 = cJSON_CreateObject();
                
                if (root1 != NULL && root2 != NULL) {
                    // Add mixed types to both objects
                    cJSON_AddNumberToObject(root1, "num", fdp.ConsumeFloatingPoint<double>());
                    cJSON_AddNumberToObject(root2, "num", fdp.ConsumeFloatingPoint<double>());
                    
                    cJSON_AddStringToObject(root1, "str", fdp.ConsumeRandomLengthString(16).c_str());
                    cJSON_AddStringToObject(root2, "str", fdp.ConsumeRandomLengthString(16).c_str());
                    
                    // Add nested array
                    cJSON* arr1 = cJSON_CreateArray();
                    cJSON* arr2 = cJSON_CreateArray();
                    if (arr1 != NULL && arr2 != NULL) {
                        int arr_len = fdp.ConsumeIntegralInRange<int>(0, 3);
                        for (int i = 0; i < arr_len && fdp.remaining_bytes() > 0; i++) {
                            cJSON_AddItemToArray(arr1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                            cJSON_AddItemToArray(arr2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                        }
                        cJSON_AddItemToObject(root1, "arr", arr1);
                        cJSON_AddItemToObject(root2, "arr", arr2);
                    }
                    
                    // Compare the complex structures
                    cJSON_Compare(root1, root2, case_sensitive);
                    cJSON_Compare(root1, root1, case_sensitive);
                    cJSON_Compare(root2, root2, case_sensitive);
                    
                    // Test with opposite case sensitivity
                    cJSON_Compare(root1, root2, !case_sensitive);
                }
                
                if (root1) cJSON_Delete(root1);
                if (root2) cJSON_Delete(root2);
            }
            break;
            
        case 7:
            // Test 8: Deep recursion stress test
            // Tests recursive comparison with deep nesting
            {
                int depth = fdp.ConsumeIntegralInRange<int>(1, 4); // Limit depth for safety
                cJSON* deep1 = NULL;
                cJSON* deep2 = NULL;
                
                // Create deeply nested object
                deep1 = cJSON_CreateObject();
                deep2 = cJSON_CreateObject();
                
                cJSON* current1 = deep1;
                cJSON* current2 = deep2;
                
                for (int i = 0; i < depth && current1 != NULL && current2 != NULL && fdp.remaining_bytes() > 0; i++) {
                    std::string key = "level" + std::to_string(i);
                    cJSON* nested1 = cJSON_CreateObject();
                    cJSON* nested2 = cJSON_CreateObject();
                    
                    if (nested1 != NULL && nested2 != NULL) {
                        cJSON_AddItemToObject(current1, key.c_str(), nested1);
                        cJSON_AddItemToObject(current2, key.c_str(), nested2);
                        current1 = nested1;
                        current2 = nested2;
                    }
                }
                
                // Add some leaf values
                if (current1 != NULL && current2 != NULL) {
                    cJSON_AddNumberToObject(current1, "value", fdp.ConsumeFloatingPoint<double>());
                    cJSON_AddNumberToObject(current2, "value", fdp.ConsumeFloatingPoint<double>());
                }
                
                if (deep1 != NULL && deep2 != NULL) {
                    cJSON_Compare(deep1, deep2, case_sensitive);
                    cJSON_Compare(deep1, deep1, case_sensitive);
                    cJSON_Compare(deep2, deep2, case_sensitive);
                }
                
                if (deep1) cJSON_Delete(deep1);
                if (deep2) cJSON_Delete(deep2);
            }
            break;
    }
    
    return 0;
}

} // namespace harness_023
