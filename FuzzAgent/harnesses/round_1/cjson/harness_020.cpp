#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include "cjson/cJSON.h"

namespace harness_020 {
/*
 * harness_020.cpp - Comprehensive cJSON_SetValuestring error paths and cJSON_Add*ToObject helper functions testing
 * 
 * This harness specifically targets:
 * 1) cJSON_SetValuestring error conditions (15/20 branches currently blocked):
 *    - null object parameter
 *    - non-string types (number, object, array, boolean, null, raw)
 *    - cJSON_IsReference flag (reference items)
 *    - NULL valuestring in object
 *    - NULL new valuestring parameter
 *    - overlapping string memory scenarios
 *    - memory allocation failures
 * 
 * 2) cJSON_Add*ToObject helper functions (50% line coverage target):
 *    - cJSON_AddNullToObject, cJSON_AddTrueToObject, cJSON_AddFalseToObject
 *    - cJSON_AddBoolToObject, cJSON_AddNumberToObject, cJSON_AddStringToObject
 *    - cJSON_AddRawToObject, cJSON_AddObjectToObject, cJSON_AddArrayToObject
 *    - Test edge cases: null object parameters, duplicate keys, memory allocation failures
 * 
 * Strategy: Use FuzzedDataProvider to generate varied inputs that trigger all error paths,
 * test invalid parameters, overlapping memory, and simulate memory pressure scenarios.
 */


extern "C" {
}
// Helper to create string with potential overlapping memory
static char* create_overlapping_string(const char* base, size_t offset, size_t length) {
    if (base == nullptr || offset + length > strlen(base)) {
        return nullptr;
    }
    char* result = (char*)malloc(length + 1);
    if (result) {
        memcpy(result, base + offset, length);
        result[length] = '\0';
    }
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Test mode selector: 0 = SetValuestring error paths, 1 = Add*ToObject functions
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (test_mode == 0) {
        // ============================================
        // Test cJSON_SetValuestring error paths
        // ============================================
        
        // Create test objects of various types
        cJSON* string_item = nullptr;
        cJSON* number_item = nullptr;
        cJSON* object_item = nullptr;
        cJSON* array_item = nullptr;
        cJSON* bool_item = nullptr;
        cJSON* null_item = nullptr;
        cJSON* raw_item = nullptr;
        cJSON* string_ref_item = nullptr;
        
        // Create with fuzzed data
        std::string initial_string = fdp.ConsumeRandomLengthString(256);
        string_item = cJSON_CreateString(initial_string.c_str());
        
        number_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        object_item = cJSON_CreateObject();
        array_item = cJSON_CreateArray();
        bool_item = cJSON_CreateBool(fdp.ConsumeBool());
        null_item = cJSON_CreateNull();
        raw_item = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(128).c_str());
        
        // Create string reference
        std::string ref_string = fdp.ConsumeRandomLengthString(128);
        string_ref_item = cJSON_CreateStringReference(ref_string.c_str());
        
        // Test 1: NULL object parameter
        if (fdp.remaining_bytes() > 10) {
            std::string test_string = fdp.ConsumeRandomLengthString(64);
            char* result = cJSON_SetValuestring(nullptr, test_string.c_str());
            // Should return NULL
            (void)result;
        }
        
        // Test 2: NULL new valuestring parameter
        if (string_item) {
            char* result = cJSON_SetValuestring(string_item, nullptr);
            // Should return NULL
            (void)result;
        }
        
        // Test 3: Non-string types (should all fail)
        std::string test_valuestring = fdp.ConsumeRandomLengthString(128);
        
        if (number_item) {
            char* result = cJSON_SetValuestring(number_item, test_valuestring.c_str());
            // Should return NULL
            (void)result;
        }
        
        if (object_item) {
            char* result = cJSON_SetValuestring(object_item, test_valuestring.c_str());
            // Should return NULL
            (void)result;
        }
        
        if (array_item) {
            char* result = cJSON_SetValuestring(array_item, test_valuestring.c_str());
            // Should return NULL
            (void)result;
        }
        
        if (bool_item) {
            char* result = cJSON_SetValuestring(bool_item, test_valuestring.c_str());
            // Should return NULL
            (void)result;
        }
        
        if (null_item) {
            char* result = cJSON_SetValuestring(null_item, test_valuestring.c_str());
            // Should return NULL
            (void)result;
        }
        
        if (raw_item) {
            char* result = cJSON_SetValuestring(raw_item, test_valuestring.c_str());
            // Should return NULL
            (void)result;
        }
        
        // Test 4: cJSON_IsReference flag (reference items)
        if (string_ref_item) {
            char* result = cJSON_SetValuestring(string_ref_item, test_valuestring.c_str());
            // Should return NULL
            (void)result;
        }
        
        // Test 5: String with NULL valuestring (corrupted item)
        // Create a string item but manually corrupt it
        cJSON* corrupted_string = cJSON_CreateString("test");
        if (corrupted_string) {
            // Simulate corruption by freeing the string but not nullifying
            if (corrupted_string->valuestring) {
                free(corrupted_string->valuestring);  // Use proper deallocator in real scenario
                corrupted_string->valuestring = nullptr;
            }
            char* result = cJSON_SetValuestring(corrupted_string, test_valuestring.c_str());
            // Should return NULL because object->valuestring is NULL
            (void)result;
            cJSON_Delete(corrupted_string);
        }
        
        // Test 6: Overlapping string memory scenarios
        if (string_item && fdp.remaining_bytes() > 100) {
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(100);
            buffer.push_back(0); // Ensure null termination
            
            // Create a string with this buffer
            cJSON* temp_string = cJSON_CreateString((const char*)buffer.data());
            if (temp_string) {
                // Try to set value with overlapping data (same buffer)
                char* result = cJSON_SetValuestring(temp_string, (const char*)buffer.data());
                // May succeed or fail based on overlap detection
                (void)result;
                
                // Test with partial overlap
                if (buffer.size() > 50) {
                    result = cJSON_SetValuestring(temp_string, (const char*)buffer.data() + 25);
                    (void)result;
                }
                
                cJSON_Delete(temp_string);
            }
        }
        
        // Test 7: Memory allocation failure simulation
        // Create a normal string item
        cJSON* normal_string = cJSON_CreateString("short");
        if (normal_string && fdp.remaining_bytes() > 50) {
            // Try to set a very long string that might trigger reallocation
            std::string long_string = fdp.ConsumeRandomLengthString(4096);
            char* result = cJSON_SetValuestring(normal_string, long_string.c_str());
            // May succeed or fail based on memory availability
            (void)result;
            cJSON_Delete(normal_string);
        }
        
        // Test 8: Empty string handling
        if (string_item) {
            char* result = cJSON_SetValuestring(string_item, "");
            // Should succeed
            (void)result;
        }
        
        // Cleanup
        if (string_item) cJSON_Delete(string_item);
        if (number_item) cJSON_Delete(number_item);
        if (object_item) cJSON_Delete(object_item);
        if (array_item) cJSON_Delete(array_item);
        if (bool_item) cJSON_Delete(bool_item);
        if (null_item) cJSON_Delete(null_item);
        if (raw_item) cJSON_Delete(raw_item);
        if (string_ref_item) cJSON_Delete(string_ref_item);
        
    } else {
        // ============================================
        // Test cJSON_Add*ToObject helper functions
        // ============================================
        
        // Create parent object
        cJSON* parent_object = cJSON_CreateObject();
        if (!parent_object) {
            return 0;
        }
        
        // Test all Add*ToObject functions with various inputs
        
        // 1. cJSON_AddNullToObject
        std::string null_key = fdp.ConsumeRandomLengthString(32);
        cJSON* null_result = cJSON_AddNullToObject(parent_object, null_key.c_str());
        (void)null_result;
        
        // Test with NULL parameters
        cJSON* null_test = cJSON_AddNullToObject(nullptr, "test");
        (void)null_test;
        
        null_test = cJSON_AddNullToObject(parent_object, nullptr);
        (void)null_test;
        
        // 2. cJSON_AddTrueToObject
        std::string true_key = fdp.ConsumeRandomLengthString(32);
        cJSON* true_result = cJSON_AddTrueToObject(parent_object, true_key.c_str());
        (void)true_result;
        
        // Test with NULL parameters
        cJSON* true_test = cJSON_AddTrueToObject(nullptr, "test");
        (void)true_test;
        
        true_test = cJSON_AddTrueToObject(parent_object, nullptr);
        (void)true_test;
        
        // 3. cJSON_AddFalseToObject
        std::string false_key = fdp.ConsumeRandomLengthString(32);
        cJSON* false_result = cJSON_AddFalseToObject(parent_object, false_key.c_str());
        (void)false_result;
        
        // Test with NULL parameters
        cJSON* false_test = cJSON_AddFalseToObject(nullptr, "test");
        (void)false_test;
        
        false_test = cJSON_AddFalseToObject(parent_object, nullptr);
        (void)false_test;
        
        // 4. cJSON_AddBoolToObject
        std::string bool_key = fdp.ConsumeRandomLengthString(32);
        cJSON_bool bool_value = fdp.ConsumeBool();
        cJSON* bool_result = cJSON_AddBoolToObject(parent_object, bool_key.c_str(), bool_value);
        (void)bool_result;
        
        // Test with NULL parameters
        cJSON* bool_test = cJSON_AddBoolToObject(nullptr, "test", true);
        (void)bool_test;
        
        bool_test = cJSON_AddBoolToObject(parent_object, nullptr, false);
        (void)bool_test;
        
        // 5. cJSON_AddNumberToObject
        std::string number_key = fdp.ConsumeRandomLengthString(32);
        double number_value = fdp.ConsumeFloatingPoint<double>();
        cJSON* number_result = cJSON_AddNumberToObject(parent_object, number_key.c_str(), number_value);
        (void)number_result;
        
        // Test with special number values
        if (fdp.remaining_bytes() > 20) {
            // Test with NaN using 0.0/0.0 which should produce NaN
            double nan_value = 0.0 / 0.0;
            cJSON* nan_result = cJSON_AddNumberToObject(parent_object, "nan_key", nan_value);
            (void)nan_result;
            
            // Test with large values that approximate infinity
            double large_value = std::numeric_limits<double>::max();
            cJSON* inf_result = cJSON_AddNumberToObject(parent_object, "large_key", large_value);
            (void)inf_result;
            
            // Test with negative large values
            double neg_large_value = -std::numeric_limits<double>::max();
            cJSON* neg_inf_result = cJSON_AddNumberToObject(parent_object, "neg_large_key", neg_large_value);
            (void)neg_inf_result;
        }
        
        // Test with NULL parameters
        cJSON* number_test = cJSON_AddNumberToObject(nullptr, "test", 42.0);
        (void)number_test;
        
        number_test = cJSON_AddNumberToObject(parent_object, nullptr, 42.0);
        (void)number_test;
        
        // 6. cJSON_AddStringToObject
        std::string string_key = fdp.ConsumeRandomLengthString(32);
        std::string string_value = fdp.ConsumeRandomLengthString(256);
        cJSON* string_result = cJSON_AddStringToObject(parent_object, string_key.c_str(), string_value.c_str());
        (void)string_result;
        
        // Test with empty string
        cJSON* empty_string_result = cJSON_AddStringToObject(parent_object, "empty_key", "");
        (void)empty_string_result;
        
        // Test with NULL string value
        cJSON* null_string_result = cJSON_AddStringToObject(parent_object, "null_string_key", nullptr);
        (void)null_string_result;
        
        // Test with NULL parameters
        cJSON* string_test = cJSON_AddStringToObject(nullptr, "test", "value");
        (void)string_test;
        
        string_test = cJSON_AddStringToObject(parent_object, nullptr, "value");
        (void)string_test;
        
        // 7. cJSON_AddRawToObject
        std::string raw_key = fdp.ConsumeRandomLengthString(32);
        std::string raw_value = fdp.ConsumeRandomLengthString(256);
        cJSON* raw_result = cJSON_AddRawToObject(parent_object, raw_key.c_str(), raw_value.c_str());
        (void)raw_result;
        
        // Test with NULL parameters
        cJSON* raw_test = cJSON_AddRawToObject(nullptr, "test", "raw");
        (void)raw_test;
        
        raw_test = cJSON_AddRawToObject(parent_object, nullptr, "raw");
        (void)raw_test;
        
        raw_test = cJSON_AddRawToObject(parent_object, "null_raw", nullptr);
        (void)raw_test;
        
        // 8. cJSON_AddObjectToObject
        std::string object_key = fdp.ConsumeRandomLengthString(32);
        cJSON* object_result = cJSON_AddObjectToObject(parent_object, object_key.c_str());
        (void)object_result;
        
        // If object was created successfully, add some items to it
        if (object_result) {
            cJSON_AddNumberToObject(object_result, "nested_number", fdp.ConsumeFloatingPoint<double>());
            cJSON_AddStringToObject(object_result, "nested_string", fdp.ConsumeRandomLengthString(64).c_str());
        }
        
        // Test with NULL parameters
        cJSON* object_test = cJSON_AddObjectToObject(nullptr, "test");
        (void)object_test;
        
        object_test = cJSON_AddObjectToObject(parent_object, nullptr);
        (void)object_test;
        
        // 9. cJSON_AddArrayToObject
        std::string array_key = fdp.ConsumeRandomLengthString(32);
        cJSON* array_result = cJSON_AddArrayToObject(parent_object, array_key.c_str());
        (void)array_result;
        
        // If array was created successfully, add some items to it
        if (array_result) {
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
                if (i % 2 == 0) {
                    cJSON_AddItemToArray(array_result, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                } else {
                    cJSON_AddItemToArray(array_result, cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
                }
            }
        }
        
        // Test with NULL parameters
        cJSON* array_test = cJSON_AddArrayToObject(nullptr, "test");
        (void)array_test;
        
        array_test = cJSON_AddArrayToObject(parent_object, nullptr);
        (void)array_test;
        
        // Test duplicate keys - adding with same key should work but return NULL for subsequent adds
        std::string duplicate_key = fdp.ConsumeRandomLengthString(32);
        cJSON* first_dup = cJSON_AddNumberToObject(parent_object, duplicate_key.c_str(), 1.0);
        cJSON* second_dup = cJSON_AddStringToObject(parent_object, duplicate_key.c_str(), "duplicate");
        // second_dup should be NULL because key already exists
        (void)first_dup;
        (void)second_dup;
        
        // Test memory pressure by creating many items
        int num_extra_items = fdp.ConsumeIntegralInRange<int>(0, 20);
        for (int i = 0; i < num_extra_items && fdp.remaining_bytes() > 50; i++) {
            std::string extra_key = "extra_" + std::to_string(i);
            uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (item_type) {
                case 0:
                    cJSON_AddNumberToObject(parent_object, extra_key.c_str(), fdp.ConsumeFloatingPoint<double>());
                    break;
                case 1:
                    cJSON_AddStringToObject(parent_object, extra_key.c_str(), fdp.ConsumeRandomLengthString(64).c_str());
                    break;
                case 2:
                    cJSON_AddBoolToObject(parent_object, extra_key.c_str(), fdp.ConsumeBool());
                    break;
                case 3:
                    cJSON_AddNullToObject(parent_object, extra_key.c_str());
                    break;
            }
        }
        
        // Cleanup
        cJSON_Delete(parent_object);
    }
    
    return 0;
}

} // namespace harness_020
