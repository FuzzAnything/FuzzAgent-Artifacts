#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_012 {

extern "C" {
}

// Helper function to create JSON objects with NULL valuestring
cJSON* create_string_with_null_valuestring() {
    cJSON* item = (cJSON*)cJSON_malloc(sizeof(cJSON));
    if (item == NULL) return NULL;
    
    memset(item, 0, sizeof(cJSON));
    item->type = cJSON_String;
    item->valuestring = NULL;  // NULL valuestring - critical edge case
    item->string = NULL;
    item->next = item->prev = item->child = NULL;
    item->valueint = 0;
    item->valuedouble = 0.0;
    
    return item;
}

// Helper function to create JSON object with invalid type
cJSON* create_invalid_type_object(uint8_t invalid_type) {
    cJSON* item = (cJSON*)cJSON_malloc(sizeof(cJSON));
    if (item == NULL) return NULL;
    
    memset(item, 0, sizeof(cJSON));
    item->type = invalid_type;  // Invalid type value
    item->valuestring = NULL;
    item->string = NULL;
    item->next = item->prev = item->child = NULL;
    item->valueint = 0;
    item->valuedouble = 0.0;
    
    return item;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need sufficient input for comprehensive testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    // Test Scenario 0: NULL parameter handling (lines 3078-3081)
    if (test_scenario == 0) {
        // Create valid objects to test against NULL
        cJSON* valid_objects[3];
        
        // Create different types of valid objects
        valid_objects[0] = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
        valid_objects[1] = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        valid_objects[2] = cJSON_CreateObject();
        if (valid_objects[2] != NULL) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            cJSON_AddNumberToObject(valid_objects[2], key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        bool case_sensitive = fdp.ConsumeBool();
        
        // Test all combinations of NULL vs valid
        // NULL vs NULL
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        // NULL vs each valid object
        for (int i = 0; i < 3; i++) {
            if (valid_objects[i] != NULL) {
                cJSON_Compare(NULL, valid_objects[i], case_sensitive);
                cJSON_Compare(valid_objects[i], NULL, case_sensitive);
            }
        }
        
        // Test valid vs valid (baseline)
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (valid_objects[i] != NULL && valid_objects[j] != NULL) {
                    cJSON_Compare(valid_objects[i], valid_objects[j], case_sensitive);
                }
            }
        }
        
        // Cleanup
        for (int i = 0; i < 3; i++) {
            if (valid_objects[i] != NULL) cJSON_Delete(valid_objects[i]);
        }
        
        return 0;
    }
    
    // Test Scenario 1: Invalid type validation (default case at line 3199)
    if (test_scenario == 1) {
        // Create objects with invalid types
        uint8_t invalid_type1 = fdp.ConsumeIntegral<uint8_t>() | 0x80;  // Ensure high bit set
        uint8_t invalid_type2 = fdp.ConsumeIntegral<uint8_t>() | 0x40;  // Another invalid type
        
        cJSON* invalid_obj1 = create_invalid_type_object(invalid_type1);
        cJSON* invalid_obj2 = create_invalid_type_object(invalid_type2);
        
        // Create some valid objects for comparison
        cJSON* valid_obj1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        cJSON* valid_obj2 = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
        
        bool case_sensitive = fdp.ConsumeBool();
        
        // Test invalid type comparisons
        // Invalid vs invalid (same type)
        if (invalid_obj1 != NULL) {
            cJSON_Compare(invalid_obj1, invalid_obj1, case_sensitive);
        }
        
        // Invalid vs invalid (different types)
        if (invalid_obj1 != NULL && invalid_obj2 != NULL) {
            cJSON_Compare(invalid_obj1, invalid_obj2, case_sensitive);
        }
        
        // Invalid vs valid
        if (invalid_obj1 != NULL && valid_obj1 != NULL) {
            // First ensure types are different for the comparison to reach default case
            cJSON_Compare(invalid_obj1, valid_obj1, case_sensitive);
        }
        
        // Valid vs invalid  
        if (valid_obj2 != NULL && invalid_obj2 != NULL) {
            cJSON_Compare(valid_obj2, invalid_obj2, case_sensitive);
        }
        
        // Cleanup
        if (invalid_obj1 != NULL) cJSON_Delete(invalid_obj1);
        if (invalid_obj2 != NULL) cJSON_Delete(invalid_obj2);
        if (valid_obj1 != NULL) cJSON_Delete(valid_obj1);
        if (valid_obj2 != NULL) cJSON_Delete(valid_obj2);
        
        return 0;
    }
    
    // Test Scenario 2: String comparison edge cases (NULL valuestring checks lines 3123-3126)
    if (test_scenario == 2) {
        // Create strings with NULL valuestring
        cJSON* null_string1 = create_string_with_null_valuestring();
        cJSON* null_string2 = create_string_with_null_valuestring();
        
        // Create normal strings for comparison
        std::string str1 = fdp.ConsumeRandomLengthString(32);
        std::string str2 = fdp.ConsumeRandomLengthString(32);
        cJSON* normal_string1 = cJSON_CreateString(str1.c_str());
        cJSON* normal_string2 = cJSON_CreateString(str2.c_str());
        
        // Create empty string (valuestring points to empty string)
        cJSON* empty_string = cJSON_CreateString("");
        
        bool case_sensitive = fdp.ConsumeBool();
        
        // Test NULL valuestring cases
        // NULL valuestring vs NULL valuestring
        if (null_string1 != NULL && null_string2 != NULL) {
            cJSON_Compare(null_string1, null_string2, case_sensitive);
        }
        
        // NULL valuestring vs normal string
        if (null_string1 != NULL && normal_string1 != NULL) {
            cJSON_Compare(null_string1, normal_string1, case_sensitive);
            cJSON_Compare(normal_string1, null_string1, case_sensitive);
        }
        
        // NULL valuestring vs empty string
        if (null_string1 != NULL && empty_string != NULL) {
            cJSON_Compare(null_string1, empty_string, case_sensitive);
            cJSON_Compare(empty_string, null_string1, case_sensitive);
        }
        
        // Test strings that don't match (strcmp != 0 path)
        if (normal_string1 != NULL && normal_string2 != NULL && str1 != str2) {
            cJSON_Compare(normal_string1, normal_string2, case_sensitive);
        }
        
        // Test identical strings (strcmp == 0 path)
        std::string identical_str = fdp.ConsumeRandomLengthString(32);
        cJSON* identical_string1 = cJSON_CreateString(identical_str.c_str());
        cJSON* identical_string2 = cJSON_CreateString(identical_str.c_str());
        if (identical_string1 != NULL && identical_string2 != NULL) {
            cJSON_Compare(identical_string1, identical_string2, case_sensitive);
        }
        
        // Cleanup
        if (null_string1 != NULL) cJSON_Delete(null_string1);
        if (null_string2 != NULL) cJSON_Delete(null_string2);
        if (normal_string1 != NULL) cJSON_Delete(normal_string1);
        if (normal_string2 != NULL) cJSON_Delete(normal_string2);
        if (empty_string != NULL) cJSON_Delete(empty_string);
        if (identical_string1 != NULL) cJSON_Delete(identical_string1);
        if (identical_string2 != NULL) cJSON_Delete(identical_string2);
        
        return 0;
    }
    
    // Test Scenario 3: Object comparison edge cases (NULL check for a_element line 3183)
    if (test_scenario == 3) {
        // Create objects with different element sets
        cJSON* obj_base = cJSON_CreateObject();
        cJSON* obj_extra = cJSON_CreateObject();
        cJSON* obj_different = cJSON_CreateObject();
        
        if (obj_base != NULL && obj_extra != NULL && obj_different != NULL) {
            // Add common elements to base
            int base_count = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < base_count && fdp.remaining_bytes() > 0; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                cJSON_AddNumberToObject(obj_base, key.c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            // Add same elements to extra plus one more
            cJSON* copy = cJSON_Duplicate(obj_base, 1);
            if (copy != NULL) {
                // Add extra element
                std::string extra_key = fdp.ConsumeRandomLengthString(16);
                cJSON_AddNumberToObject(copy, extra_key.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_Delete(obj_extra);
                obj_extra = copy;
            }
            
            // Add completely different elements
            int diff_count = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < diff_count && fdp.remaining_bytes() > 0; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                cJSON_AddNumberToObject(obj_different, key.c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            bool case_sensitive = fdp.ConsumeBool();
            
            // Test object with extra element vs base (should trigger a_element NULL check)
            cJSON_Compare(obj_base, obj_extra, case_sensitive);
            cJSON_Compare(obj_extra, obj_base, case_sensitive);
            
            // Test completely different objects
            cJSON_Compare(obj_base, obj_different, case_sensitive);
            
            // Test empty object vs non-empty object
            cJSON* empty_obj = cJSON_CreateObject();
            if (empty_obj != NULL) {
                cJSON_Compare(empty_obj, obj_base, case_sensitive);
                cJSON_Compare(obj_base, empty_obj, case_sensitive);
                cJSON_Delete(empty_obj);
            }
        }
        
        // Cleanup
        if (obj_base != NULL) cJSON_Delete(obj_base);
        if (obj_extra != NULL) cJSON_Delete(obj_extra);
        if (obj_different != NULL) cJSON_Delete(obj_different);
        
        return 0;
    }
    
    // Test Scenario 4: Array length mismatch (lines 3151-3156)
    if (test_scenario == 4) {
        // Create arrays of different lengths
        int short_len = fdp.ConsumeIntegralInRange<int>(1, 5);
        int long_len = short_len + fdp.ConsumeIntegralInRange<int>(1, 5);
        
        cJSON* array_short = cJSON_CreateArray();
        cJSON* array_long = cJSON_CreateArray();
        
        if (array_short != NULL && array_long != NULL) {
            // Fill short array
            for (int i = 0; i < short_len && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(array_short, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            // Fill long array with more elements
            for (int i = 0; i < long_len && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(array_long, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            bool case_sensitive = fdp.ConsumeBool();
            
            // Test array length mismatch (short vs long)
            cJSON_Compare(array_short, array_long, case_sensitive);
            cJSON_Compare(array_long, array_short, case_sensitive);
            
            // Test same length arrays with different values
            cJSON* array_same_len1 = cJSON_CreateArray();
            cJSON* array_same_len2 = cJSON_CreateArray();
            
            if (array_same_len1 != NULL && array_same_len2 != NULL) {
                int same_len = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < same_len && fdp.remaining_bytes() > 0; i++) {
                    cJSON_AddItemToArray(array_same_len1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    cJSON_AddItemToArray(array_same_len2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>() + 1.0));
                }
                
                cJSON_Compare(array_same_len1, array_same_len2, case_sensitive);
                
                cJSON_Delete(array_same_len1);
                cJSON_Delete(array_same_len2);
            }
            
            // Test empty array vs non-empty array
            cJSON* empty_array = cJSON_CreateArray();
            if (empty_array != NULL) {
                cJSON_Compare(empty_array, array_short, case_sensitive);
                cJSON_Compare(array_short, empty_array, case_sensitive);
                cJSON_Delete(empty_array);
            }
        }
        
        // Cleanup
        if (array_short != NULL) cJSON_Delete(array_short);
        if (array_long != NULL) cJSON_Delete(array_long);
        
        return 0;
    }
    
    // Test Scenario 5: Combined edge cases from guidance
    if (test_scenario == 5) {
        // Test cJSON_Compare(obj_with_null_string, obj_with_valid_string, case_sensitive)
        cJSON* obj_with_null_string = cJSON_CreateObject();
        cJSON* obj_with_valid_string = cJSON_CreateObject();
        
        if (obj_with_null_string != NULL && obj_with_valid_string != NULL) {
            // Add string with NULL valuestring to first object
            cJSON* null_string_item = create_string_with_null_valuestring();
            if (null_string_item != NULL) {
                cJSON_AddItemToObject(obj_with_null_string, "str", null_string_item);
            }
            
            // Add normal string to second object
            std::string valid_str = fdp.ConsumeRandomLengthString(32);
            cJSON_AddStringToObject(obj_with_valid_string, "str", valid_str.c_str());
            
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(obj_with_null_string, obj_with_valid_string, case_sensitive);
        }
        
        if (obj_with_null_string != NULL) cJSON_Delete(obj_with_null_string);
        if (obj_with_valid_string != NULL) cJSON_Delete(obj_with_valid_string);
        
        return 0;
    }
    
    // Test Scenario 6: More combined cases
    if (test_scenario == 6) {
        // Test cJSON_Compare(obj_extra_element, obj_base, case_sensitive)
        cJSON* obj_base = cJSON_CreateObject();
        cJSON* obj_extra = cJSON_CreateObject();
        
        if (obj_base != NULL && obj_extra != NULL) {
            // Add common elements
            int count = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < count && fdp.remaining_bytes() > 0; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                double value = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(obj_base, key.c_str(), value);
                cJSON_AddNumberToObject(obj_extra, key.c_str(), value);
            }
            
            // Add extra element to obj_extra
            std::string extra_key = fdp.ConsumeRandomLengthString(16);
            cJSON_AddNumberToObject(obj_extra, extra_key.c_str(), fdp.ConsumeFloatingPoint<double>());
            
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(obj_extra, obj_base, case_sensitive);
        }
        
        if (obj_base != NULL) cJSON_Delete(obj_base);
        if (obj_extra != NULL) cJSON_Delete(obj_extra);
        
        return 0;
    }
    
    // Test Scenario 7: Array specific tests
    if (test_scenario == 7) {
        // Test cJSON_Compare(array_short, array_long, case_sensitive)
        cJSON* array_short = cJSON_CreateArray();
        cJSON* array_long = cJSON_CreateArray();
        
        if (array_short != NULL && array_long != NULL) {
            // Short array: 1-3 elements
            int short_len = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < short_len && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(array_short, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            // Long array: more elements
            int long_len = short_len + fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < long_len && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(array_long, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(array_short, array_long, case_sensitive);
        }
        
        if (array_short != NULL) cJSON_Delete(array_short);
        if (array_long != NULL) cJSON_Delete(array_long);
        
        return 0;
    }
    
    // Test Scenario 8: Invalid type object comparisons
    if (test_scenario == 8) {
        // Test cJSON_Compare(invalid_type_obj, valid_obj, case_sensitive)
        uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>() | 0xF0;  // Invalid type
        cJSON* invalid_type_obj = create_invalid_type_object(invalid_type);
        cJSON* valid_obj = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        
        if (invalid_type_obj != NULL && valid_obj != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(invalid_type_obj, valid_obj, case_sensitive);
        }
        
        if (invalid_type_obj != NULL) cJSON_Delete(invalid_type_obj);
        if (valid_obj != NULL) cJSON_Delete(valid_obj);
        
        return 0;
    }
    
    // Test Scenario 9: Comprehensive mixed testing
    if (test_scenario == 9) {
        // Test various combinations in one run
        bool case_sensitive = fdp.ConsumeBool();
        
        // NULL comparisons
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        // Create various objects
        cJSON* valid_string = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
        cJSON* null_string = create_string_with_null_valuestring();
        
        if (valid_string != NULL && null_string != NULL) {
            // NULL valuestring vs valid string
            cJSON_Compare(null_string, valid_string, case_sensitive);
            
            // Test with arrays
            cJSON* array1 = cJSON_CreateArray();
            cJSON* array2 = cJSON_CreateArray();
            
            if (array1 != NULL && array2 != NULL) {
                // Different length arrays
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                
                cJSON_Compare(array1, array2, case_sensitive);
                
                cJSON_Delete(array1);
                cJSON_Delete(array2);
            }
            
            // Test with objects having different elements
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            if (obj1 != NULL && obj2 != NULL) {
                std::string key1 = fdp.ConsumeRandomLengthString(16);
                std::string key2 = fdp.ConsumeRandomLengthString(16);
                cJSON_AddNumberToObject(obj1, key1.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_AddNumberToObject(obj2, key2.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_Compare(obj1, obj2, case_sensitive);
                
                cJSON_Delete(obj1);
                cJSON_Delete(obj2);
            }
            
            cJSON_Delete(valid_string);
            cJSON_Delete(null_string);
        }
        
        // Test invalid type
        uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>() | 0xC0;
        cJSON* invalid_obj = create_invalid_type_object(invalid_type);
        if (invalid_obj != NULL) {
            cJSON_Compare(invalid_obj, invalid_obj, case_sensitive);
            cJSON_Delete(invalid_obj);
        }
    }
    
    return 0;
}

} // namespace harness_012
