#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_008 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Need sufficient input for comprehensive comparison testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first for configuration
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool case_sensitive = fdp.ConsumeBool();
    
    // Test Scenario 1: Type mismatch comparisons (targeting lines 3079-3081)
    if (test_scenario == 0) {
        // Create JSON items of different types for comparison
        cJSON* item1 = NULL;
        cJSON* item2 = NULL;
        
        // Randomly select two different types
        uint8_t type1 = fdp.ConsumeIntegral<uint8_t>() % 8;
        uint8_t type2 = (type1 + 1 + (fdp.ConsumeIntegral<uint8_t>() % 7)) % 8;
        
        // Create first item
        switch (type1) {
            case 0: // cJSON_Invalid (0) - actually not valid, but test edge case
                // Use NULL for invalid
                break;
            case 1: // cJSON_False
                item1 = cJSON_CreateFalse();
                break;
            case 2: // cJSON_True
                item1 = cJSON_CreateTrue();
                break;
            case 3: // cJSON_NULL
                item1 = cJSON_CreateNull();
                break;
            case 4: // cJSON_Number
                item1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 5: // cJSON_String
                item1 = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
                break;
            case 6: // cJSON_Array
                item1 = cJSON_CreateArray();
                // Add some items to array
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                    cJSON_AddItemToArray(item1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                }
                break;
            case 7: // cJSON_Object
                item1 = cJSON_CreateObject();
                // Add some items to object
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                    std::string key = fdp.ConsumeRandomLengthString(16);
                    cJSON_AddNumberToObject(item1, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                }
                break;
        }
        
        // Create second item of different type
        switch (type2) {
            case 0: // cJSON_Invalid (0)
                break;
            case 1: // cJSON_False
                item2 = cJSON_CreateFalse();
                break;
            case 2: // cJSON_True
                item2 = cJSON_CreateTrue();
                break;
            case 3: // cJSON_NULL
                item2 = cJSON_CreateNull();
                break;
            case 4: // cJSON_Number
                item2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 5: // cJSON_String
                item2 = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
                break;
            case 6: // cJSON_Array
                item2 = cJSON_CreateArray();
                // Add some items to array
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                    cJSON_AddItemToArray(item2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                }
                break;
            case 7: // cJSON_Object
                item2 = cJSON_CreateObject();
                // Add some items to object
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                    std::string key = fdp.ConsumeRandomLengthString(16);
                    cJSON_AddNumberToObject(item2, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                }
                break;
        }
        
        // Test comparison with type mismatch (should return false)
        if (item1 != NULL && item2 != NULL) {
            cJSON_Compare(item1, item2, case_sensitive);
        }
        
        // Cleanup
        if (item1 != NULL) cJSON_Delete(item1);
        if (item2 != NULL) cJSON_Delete(item2);
    }
    
    // Test Scenario 2: String/Raw comparison failures (targeting lines 3123-3135)
    else if (test_scenario == 1) {
        // Create strings with different content
        std::string str1 = fdp.ConsumeRandomLengthString(64);
        std::string str2 = fdp.ConsumeRandomLengthString(64);
        
        // Ensure they're different (with high probability)
        if (str1 != str2) {
            cJSON* string1 = cJSON_CreateString(str1.c_str());
            cJSON* string2 = cJSON_CreateString(str2.c_str());
            
            // Test string comparison (should return false for different strings)
            cJSON_Compare(string1, string2, case_sensitive);
            
            // Test with NULL valuestring
            if (string1 != NULL && string2 != NULL) {
                // Create raw JSON with different content
                std::string raw1 = fdp.ConsumeRandomLengthString(64);
                std::string raw2 = fdp.ConsumeRandomLengthString(64);
                
                cJSON* raw_json1 = cJSON_CreateRaw(raw1.c_str());
                cJSON* raw_json2 = cJSON_CreateRaw(raw2.c_str());
                
                if (raw_json1 != NULL && raw_json2 != NULL) {
                    cJSON_Compare(raw_json1, raw_json2, case_sensitive);
                }
                
                if (raw_json1 != NULL) cJSON_Delete(raw_json1);
                if (raw_json2 != NULL) cJSON_Delete(raw_json2);
            }
            
            if (string1 != NULL) cJSON_Delete(string1);
            if (string2 != NULL) cJSON_Delete(string2);
        }
        
        // Test case-sensitive vs case-insensitive comparisons
        std::string lower_str = "teststring";
        std::string upper_str = "TESTSTRING";
        
        cJSON* lower = cJSON_CreateString(lower_str.c_str());
        cJSON* upper = cJSON_CreateString(upper_str.c_str());
        
        if (lower != NULL && upper != NULL) {
            // Test with case_sensitive = true (should return false)
            cJSON_Compare(lower, upper, true);
            // Test with case_sensitive = false (should return true for strcmp with case-insensitive)
            // Actually strcmp is case-sensitive, so this will test that path
            cJSON_Compare(lower, upper, false);
        }
        
        if (lower != NULL) cJSON_Delete(lower);
        if (upper != NULL) cJSON_Delete(upper);
    }
    
    // Test Scenario 3: Array comparison failures (targeting lines 3142-3156)
    else if (test_scenario == 2) {
        // Create arrays with different elements
        cJSON* array1 = cJSON_CreateArray();
        cJSON* array2 = cJSON_CreateArray();
        
        if (array1 != NULL && array2 != NULL) {
            // Add different number of elements
            int count1 = fdp.ConsumeIntegralInRange<int>(1, 10);
            int count2 = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < count1; i++) {
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            for (int i = 0; i < count2; i++) {
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            // Test array comparison (should return false for different arrays)
            cJSON_Compare(array1, array2, case_sensitive);
            
            // Test with same number of elements but different values
            cJSON* array3 = cJSON_CreateArray();
            cJSON* array4 = cJSON_CreateArray();
            
            if (array3 != NULL && array4 != NULL) {
                int count = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < count; i++) {
                    cJSON_AddItemToArray(array3, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    cJSON_AddItemToArray(array4, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>() + 1.0)); // Different value
                }
                
                cJSON_Compare(array3, array4, case_sensitive);
            }
            
            if (array3 != NULL) cJSON_Delete(array3);
            if (array4 != NULL) cJSON_Delete(array4);
        }
        
        if (array1 != NULL) cJSON_Delete(array1);
        if (array2 != NULL) cJSON_Delete(array2);
        
        // Test empty vs non-empty arrays
        cJSON* empty_array = cJSON_CreateArray();
        cJSON* nonempty_array = cJSON_CreateArray();
        
        if (empty_array != NULL && nonempty_array != NULL) {
            double array_value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddItemToArray(nonempty_array, cJSON_CreateNumber(array_value));
            cJSON_Compare(empty_array, nonempty_array, case_sensitive);
        }
        
        if (empty_array != NULL) cJSON_Delete(empty_array);
        if (nonempty_array != NULL) cJSON_Delete(nonempty_array);
    }
    
    // Test Scenario 4: Object comparison failures (targeting lines 3158 onward)
    else if (test_scenario == 3) {
        // Create objects with different keys/values
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 != NULL && obj2 != NULL) {
            // Add different keys
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                std::string key1 = fdp.ConsumeRandomLengthString(16);
                std::string key2 = fdp.ConsumeRandomLengthString(16);
                
                cJSON_AddNumberToObject(obj1, key1.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_AddNumberToObject(obj2, key2.c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            cJSON_Compare(obj1, obj2, case_sensitive);
            
            // Test objects with same keys but different values
            cJSON* obj3 = cJSON_CreateObject();
            cJSON* obj4 = cJSON_CreateObject();
            
            if (obj3 != NULL && obj4 != NULL) {
                std::string common_key = fdp.ConsumeRandomLengthString(16);
                cJSON_AddNumberToObject(obj3, common_key.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_AddNumberToObject(obj4, common_key.c_str(), fdp.ConsumeFloatingPoint<double>() + 1.0);
                
                cJSON_Compare(obj3, obj4, case_sensitive);
            }
            
            if (obj3 != NULL) cJSON_Delete(obj3);
            if (obj4 != NULL) cJSON_Delete(obj4);
        }
        
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
        
        // Test empty vs non-empty objects
        cJSON* empty_obj = cJSON_CreateObject();
        cJSON* nonempty_obj = cJSON_CreateObject();
        
        if (empty_obj != NULL && nonempty_obj != NULL) {
            std::string test_key = fdp.ConsumeRandomLengthString(16);
            double test_value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(nonempty_obj, test_key.c_str(), test_value);
            cJSON_Compare(empty_obj, nonempty_obj, case_sensitive);
        }
        
        if (empty_obj != NULL) cJSON_Delete(empty_obj);
        if (nonempty_obj != NULL) cJSON_Delete(nonempty_obj);
    }
    
    else if (test_scenario == 4) {
        // Create numbers with different values
        double num1 = fdp.ConsumeFloatingPoint<double>();
        double num2 = fdp.ConsumeFloatingPoint<double>();
        
        // Ensure they're different
        if (num1 != num2) {
            cJSON* number1 = cJSON_CreateNumber(num1);
            cJSON* number2 = cJSON_CreateNumber(num2);
            
            if (number1 != NULL && number2 != NULL) {
                cJSON_Compare(number1, number2, case_sensitive);
            }
            
            if (number1 != NULL) cJSON_Delete(number1);
            if (number2 != NULL) cJSON_Delete(number2);
        }
        
        // Test edge cases: NaN, infinity, very small/large numbers
        cJSON* nan_json = cJSON_CreateNumber(0.0/0.0); // NaN
        cJSON* inf_json = cJSON_CreateNumber(1.0/0.0); // Infinity
        
        if (nan_json != NULL && inf_json != NULL) {
            cJSON_Compare(nan_json, inf_json, case_sensitive);
            // Compare NaN with itself
            cJSON_Compare(nan_json, nan_json, case_sensitive);
            // Compare Infinity with itself
            cJSON_Compare(inf_json, inf_json, case_sensitive);
        }
        
        if (nan_json != NULL) cJSON_Delete(nan_json);
        if (inf_json != NULL) cJSON_Delete(inf_json);
    }
    
    // Test Scenario 6: Identical structures (should return true)
    else if (test_scenario == 5) {
        // Create identical JSON structures
        std::string json_str = fdp.ConsumeRandomLengthString(128);
        
        cJSON* parsed1 = cJSON_Parse(json_str.c_str());
        cJSON* parsed2 = cJSON_Parse(json_str.c_str());
        
        if (parsed1 != NULL && parsed2 != NULL) {
            // These should be equal (identical JSON)
            cJSON_Compare(parsed1, parsed2, case_sensitive);
        }
        
        if (parsed1 != NULL) cJSON_Delete(parsed1);
        if (parsed2 != NULL) cJSON_Delete(parsed2);
        
        // Create identical objects manually
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 != NULL && obj2 != NULL) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            double value = fdp.ConsumeFloatingPoint<double>();
            
            cJSON_AddNumberToObject(obj1, key.c_str(), value);
            cJSON_AddNumberToObject(obj2, key.c_str(), value);
            
            cJSON_Compare(obj1, obj2, case_sensitive);
        }
        
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
    }
    
    // Test Scenario 7: Nested structures with variations
    else if (test_scenario == 6) {
        // Create deeply nested structures
        cJSON* nested1 = cJSON_CreateObject();
        cJSON* nested2 = cJSON_CreateObject();
        
        if (nested1 != NULL && nested2 != NULL) {
            // Add nested array
            cJSON* array1 = cJSON_CreateArray();
            cJSON* array2 = cJSON_CreateArray();
            
            if (array1 != NULL && array2 != NULL) {
                // Add objects to arrays
                cJSON* obj_in_array1 = cJSON_CreateObject();
                cJSON* obj_in_array2 = cJSON_CreateObject();
                
                if (obj_in_array1 != NULL && obj_in_array2 != NULL) {
                    std::string key = fdp.ConsumeRandomLengthString(16);
                    std::string value1 = fdp.ConsumeRandomLengthString(32);
                    std::string value2 = fdp.ConsumeRandomLengthString(32);
                    
                    cJSON_AddStringToObject(obj_in_array1, key.c_str(), value1.c_str());
                    cJSON_AddStringToObject(obj_in_array2, key.c_str(), value2.c_str());
                    
                    cJSON_AddItemToArray(array1, obj_in_array1);
                    cJSON_AddItemToArray(array2, obj_in_array2);
                }
                
                cJSON_AddItemToObject(nested1, "nested_array", array1);
                cJSON_AddItemToObject(nested2, "nested_array", array2);
            }
            
            // Test comparison of complex nested structures
            cJSON_Compare(nested1, nested2, case_sensitive);
        }
        
        if (nested1 != NULL) cJSON_Delete(nested1);
        if (nested2 != NULL) cJSON_Delete(nested2);
    }
    
    // Test Scenario 8: Mixed type testing with NULL pointers
    else if (test_scenario == 7) {
        // Test with NULL pointers (edge cases)
        double test_number = fdp.ConsumeFloatingPoint<double>();
        cJSON* valid_item = cJSON_CreateNumber(test_number);
        
        if (valid_item != NULL) {
            // Test comparison with NULL (should return false)
            cJSON_Compare(valid_item, NULL, case_sensitive);
            cJSON_Compare(NULL, valid_item, case_sensitive);
            cJSON_Compare(NULL, NULL, case_sensitive);
            
            cJSON_Delete(valid_item);
        }
        
        // Test with invalid type (type > cJSON_Object)
        double test_number2 = fdp.ConsumeFloatingPoint<double>();
        cJSON* item_with_invalid_type = cJSON_CreateNumber(test_number2);
        if (item_with_invalid_type != NULL) {
            // Manually set an invalid type
            item_with_invalid_type->type = 0xFF; // Invalid type
            cJSON_Compare(item_with_invalid_type, item_with_invalid_type, case_sensitive);
            cJSON_Delete(item_with_invalid_type);
        }
    }
    
    return 0;
}

} // namespace harness_008
