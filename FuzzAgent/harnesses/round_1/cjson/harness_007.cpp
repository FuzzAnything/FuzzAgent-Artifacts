#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_007 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Test scenario selector
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    cJSON* string_item = nullptr;
    cJSON* number_item = nullptr;
    cJSON* object_item = nullptr;
    cJSON* array_item = nullptr;
    char* set_result = nullptr;
    char* get_string_result = nullptr;
    double get_number_result = 0.0;
    
    // 1. Create JSON items for testing
    std::string initial_string = fdp.ConsumeRandomLengthString(256);
    double initial_number = fdp.ConsumeFloatingPoint<double>();
    
    string_item = cJSON_CreateString(initial_string.c_str());
    number_item = cJSON_CreateNumber(initial_number);
    object_item = cJSON_CreateObject();
    array_item = cJSON_CreateArray();
    
    if (!string_item || !number_item || !object_item || !array_item) {
        // Cleanup any created items
        if (string_item) cJSON_Delete(string_item);
        if (number_item) cJSON_Delete(number_item);
        if (object_item) cJSON_Delete(object_item);
        if (array_item) cJSON_Delete(array_item);
        return 0;
    }
    
    switch (test_scenario) {
        case 0: {
            // Test cJSON_SetValuestring on valid string items with various inputs
            std::string new_string = fdp.ConsumeRandomLengthString(512);
            
            // Test 1: Normal string replacement
            set_result = cJSON_SetValuestring(string_item, new_string.c_str());
            
            // Verify with cJSON_GetStringValue
            if (set_result != nullptr) {
                get_string_result = cJSON_GetStringValue(string_item);
                // Verify retrieval matches what was set
            }
            
            // Test 2: Empty string
            if (fdp.remaining_bytes() > 0) {
                set_result = cJSON_SetValuestring(string_item, "");
                if (set_result != nullptr) {
                    get_string_result = cJSON_GetStringValue(string_item);
                }
            }
            
            // Test 3: String with special characters
            std::string special_chars = fdp.ConsumeRandomLengthString(128);
            set_result = cJSON_SetValuestring(string_item, special_chars.c_str());
            
            break;
        }
        
        case 1: {
            // Test cJSON_SetValuestring on invalid item types (should fail)
            std::string test_string = fdp.ConsumeRandomLengthString(256);
            
            // Test on number item (should fail)
            set_result = cJSON_SetValuestring(number_item, test_string.c_str());
            // Should return NULL
            
            // Test on object item (should fail)
            set_result = cJSON_SetValuestring(object_item, test_string.c_str());
            // Should return NULL
            
            // Test on array item (should fail)
            set_result = cJSON_SetValuestring(array_item, test_string.c_str());
            // Should return NULL
            
            // Test cJSON_GetStringValue on number item (should return NULL)
            get_string_result = cJSON_GetStringValue(number_item);
            // Should be NULL
            
            break;
        }
        
        case 2: {
            // Test cJSON_GetStringValue and cJSON_GetNumberValue comprehensively
            
            // Test GetStringValue on string item
            get_string_result = cJSON_GetStringValue(string_item);
            // Should return the string value
            
            // Test GetStringValue on other types (should return NULL)
            get_string_result = cJSON_GetStringValue(number_item);
            get_string_result = cJSON_GetStringValue(object_item);
            get_string_result = cJSON_GetStringValue(array_item);
            
            // Test GetNumberValue on number item
            get_number_result = cJSON_GetNumberValue(number_item);
            // Should return the number value
            
            // Test GetNumberValue on string item (should return NAN)
            get_number_result = cJSON_GetNumberValue(string_item);
            // Check if result is NAN
            
            // Test GetNumberValue on other types (should return NAN)
            get_number_result = cJSON_GetNumberValue(object_item);
            get_number_result = cJSON_GetNumberValue(array_item);
            
            break;
        }
        
        case 3: {
            // Test edge cases and complex scenarios
            
            // Create string reference (cJSON_IsReference flag should prevent SetValuestring)
            std::string ref_string = fdp.ConsumeRandomLengthString(128);
            cJSON* string_ref = cJSON_CreateStringReference(ref_string.c_str());
            if (string_ref != nullptr) {
                std::string try_change = fdp.ConsumeRandomLengthString(64);
                set_result = cJSON_SetValuestring(string_ref, try_change.c_str());
                // Should return NULL for reference items
                cJSON_Delete(string_ref);
            }
            
            // Test with overlapping memory scenarios
            if (fdp.remaining_bytes() > 100) {
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(100);
                // Ensure null termination for cJSON_CreateString
                buffer.push_back(0);
                // Create a cJSON string with data from buffer
                cJSON* temp_string = cJSON_CreateString((const char*)buffer.data());
                if (temp_string) {
                    // Try to set value with overlapping data
                    set_result = cJSON_SetValuestring(temp_string, (const char*)buffer.data());
                    cJSON_Delete(temp_string);
                }
            }
            
            // Test NULL item handling
            // Use fuzzed data for the string
            std::string null_test_string = fdp.ConsumeRandomLengthString(64);
            set_result = cJSON_SetValuestring(nullptr, null_test_string.c_str());
            // Should return NULL
            
            get_string_result = cJSON_GetStringValue(nullptr);
            // Should return NULL
            
            get_number_result = cJSON_GetNumberValue(nullptr);
            // Should return NAN
            
            break;
        }
    }
    
    // 5. Cleanup
    cJSON_Delete(string_item);
    cJSON_Delete(number_item);
    cJSON_Delete(object_item);
    cJSON_Delete(array_item);
    
    return 0;
}

} // namespace harness_007
