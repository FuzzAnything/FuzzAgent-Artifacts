#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_025 {

extern "C" {
}

// Helper to precisely test overlapping string conditions
void test_overlapping_branches(FuzzedDataProvider& fdp) {
    // Create a buffer for overlapping tests
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(100, 500);
    std::vector<char> buffer(buffer_size);
    
    // Fill with random data
    std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
    if (random_data.size() < buffer_size) {
        buffer.assign(buffer_size, 'A');
    } else {
        std::copy(random_data.begin(), random_data.end(), buffer.begin());
    }
    
    // Test 1: v1_len <= v2_len AND strings DON'T overlap (should succeed with strcpy)
    {
        // Place two non-overlapping strings in buffer
        size_t v2_len = fdp.ConsumeIntegralInRange<size_t>(20, buffer_size/2 - 10);
        size_t v1_len = fdp.ConsumeIntegralInRange<size_t>(1, v2_len);  // v1_len <= v2_len
        
        char* target_str = buffer.data();
        char* source_str = buffer.data() + buffer_size/2;  // Non-overlapping
        
        // Ensure null termination
        if (v2_len > 0 && v2_len < buffer_size) target_str[v2_len - 1] = '\0';
        if (v1_len > 0 && v1_len < buffer_size/2) source_str[v1_len - 1] = '\0';
        
        cJSON* string_item = cJSON_CreateString(target_str);
        if (string_item != NULL && string_item->valuestring != NULL) {
            char* result = cJSON_SetValuestring(string_item, source_str);
            // Should succeed (non-overlapping, v1_len <= v2_len)
            cJSON_Delete(string_item);
        }
    }
    
    // Test 2: v1_len <= v2_len AND strings DO overlap (should return NULL)
    if (fdp.remaining_bytes() > 50) {
        size_t v2_len = fdp.ConsumeIntegralInRange<size_t>(30, buffer_size/2);
        size_t v1_len = fdp.ConsumeIntegralInRange<size_t>(1, v2_len);  // v1_len <= v2_len
        
        char* target_str = buffer.data();
        // Create overlapping source string that starts within target string
        char* source_str = target_str + v2_len/2;
        
        // Ensure null termination
        if (v2_len > 0 && v2_len < buffer_size) target_str[v2_len - 1] = '\0';
        if (v1_len > 0 && (v2_len/2 + v1_len) < buffer_size) source_str[v1_len - 1] = '\0';
        
        cJSON* string_item = cJSON_CreateString(target_str);
        if (string_item != NULL && string_item->valuestring != NULL) {
            char* result = cJSON_SetValuestring(string_item, source_str);
            // Should return NULL (overlapping, v1_len <= v2_len)
            cJSON_Delete(string_item);
        }
    }
    
    // Test 3: v1_len > v2_len (should use allocation path)
    if (fdp.remaining_bytes() > 50) {
        size_t v2_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
        size_t v1_len = fdp.ConsumeIntegralInRange<size_t>(v2_len + 1, 100);  // v1_len > v2_len
        
        char* target_str = buffer.data();
        char* source_str = buffer.data() + buffer_size/2;
        
        // Ensure null termination
        if (v2_len > 0 && v2_len < buffer_size) target_str[v2_len - 1] = '\0';
        if (v1_len > 0 && v1_len < buffer_size/2) source_str[v1_len - 1] = '\0';
        
        cJSON* string_item = cJSON_CreateString(target_str);
        if (string_item != NULL && string_item->valuestring != NULL) {
            char* result = cJSON_SetValuestring(string_item, source_str);
            // Should allocate new memory
            cJSON_Delete(string_item);
        }
    }
}

// Test cJSON_IsReference objects (should always return NULL)
void test_reference_objects(FuzzedDataProvider& fdp) {
    // Test string reference
    {
        std::string ref_str = fdp.ConsumeRandomLengthString(32);
        const char* const_str = ref_str.c_str();
        cJSON* string_ref = cJSON_CreateStringReference(const_str);
        if (string_ref != NULL) {
            std::string new_str = fdp.ConsumeRandomLengthString(64);
            char* result = cJSON_SetValuestring(string_ref, new_str.c_str());
            // Should return NULL because it's a reference
            cJSON_Delete(string_ref);
        }
    }
    
    // Test object reference
    if (fdp.remaining_bytes() > 50) {
        cJSON* obj = cJSON_CreateObject();
        if (obj != NULL) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            std::string value = fdp.ConsumeRandomLengthString(32);
            cJSON_AddStringToObject(obj, key.c_str(), value.c_str());
            
            cJSON* obj_ref = cJSON_CreateObjectReference(obj);
            if (obj_ref != NULL) {
                std::string new_str = fdp.ConsumeRandomLengthString(64);
                char* result = cJSON_SetValuestring(obj_ref, new_str.c_str());
                // Should return NULL (not a string type AND is reference)
                cJSON_Delete(obj_ref);
            }
            cJSON_Delete(obj);
        }
    }
    
    // Test array reference
    if (fdp.remaining_bytes() > 50) {
        cJSON* arr = cJSON_CreateArray();
        if (arr != NULL) {
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                cJSON_AddItemToArray(arr, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            cJSON* arr_ref = cJSON_CreateArrayReference(arr);
            if (arr_ref != NULL) {
                std::string new_str = fdp.ConsumeRandomLengthString(64);
                char* result = cJSON_SetValuestring(arr_ref, new_str.c_str());
                // Should return NULL (not a string type AND is reference)
                cJSON_Delete(arr_ref);
            }
            cJSON_Delete(arr);
        }
    }
}

// Test boundary conditions for the overlapping check
void test_overlapping_boundary_conditions(FuzzedDataProvider& fdp) {
    // Create test strings with exact boundary conditions
    size_t buffer_size = 256;
    std::vector<char> buffer(buffer_size, 'X');
    
    // Case 1: source ends exactly where target begins (valuestring + v1_len == object->valuestring)
    // According to condition: valuestring + v1_len < object->valuestring must be false
    // But object->valuestring + v2_len < valuestring also false
    // So !(false || false) = true, should return NULL
    {
        size_t v1_len = 50;
        size_t v2_len = 60;  // v1_len <= v2_len
        
        char* source_str = buffer.data();
        char* target_str = source_str + v1_len;  // Adjacent, not overlapping
        
        if (v1_len > 0) source_str[v1_len - 1] = '\0';
        if (v2_len > 0 && (v1_len + v2_len) < buffer_size) target_str[v2_len - 1] = '\0';
        
        cJSON* string_item = cJSON_CreateString(target_str);
        if (string_item != NULL && string_item->valuestring != NULL) {
            char* result = cJSON_SetValuestring(string_item, source_str);
            // Adjacent strings: source ends where target begins
            // Condition: !(valuestring + v1_len < object->valuestring || object->valuestring + v2_len < valuestring)
            // = !(source+50 < target || target+60 < source)
            // = !(false || false) = true, should return NULL
            cJSON_Delete(string_item);
        }
    }
    
    // Case 2: target ends exactly where source begins (object->valuestring + v2_len == valuestring)
    if (fdp.remaining_bytes() > 50) {
        size_t v2_len = 60;
        size_t v1_len = 50;  // v1_len <= v2_len
        
        char* target_str = buffer.data();
        char* source_str = target_str + v2_len;  // Adjacent, not overlapping
        
        if (v2_len > 0) target_str[v2_len - 1] = '\0';
        if (v1_len > 0 && (v2_len + v1_len) < buffer_size) source_str[v1_len - 1] = '\0';
        
        cJSON* string_item = cJSON_CreateString(target_str);
        if (string_item != NULL && string_item->valuestring != NULL) {
            char* result = cJSON_SetValuestring(string_item, source_str);
            // Adjacent strings: target ends where source begins
            cJSON_Delete(string_item);
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need enough data for comprehensive testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Test 1: Basic error conditions (already covered but ensure completeness)
    
    // NULL object
    if (fdp.remaining_bytes() > 10) {
        std::string test_str = fdp.ConsumeRandomLengthString(16);
        char* result = cJSON_SetValuestring(NULL, test_str.c_str());
    }
    
    // Non-string objects
    if (fdp.remaining_bytes() > 20) {
        // Number
        cJSON* num = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (num != NULL) {
            std::string test_str = fdp.ConsumeRandomLengthString(16);
            char* result = cJSON_SetValuestring(num, test_str.c_str());
            cJSON_Delete(num);
        }
        
        // Boolean
        cJSON* b = cJSON_CreateBool(fdp.ConsumeBool());
        if (b != NULL) {
            std::string test_str = fdp.ConsumeRandomLengthString(16);
            char* result = cJSON_SetValuestring(b, test_str.c_str());
            cJSON_Delete(b);
        }
        
        // Array
        cJSON* arr = cJSON_CreateArray();
        if (arr != NULL) {
            std::string test_str = fdp.ConsumeRandomLengthString(16);
            char* result = cJSON_SetValuestring(arr, test_str.c_str());
            cJSON_Delete(arr);
        }
        
        // Object
        cJSON* obj = cJSON_CreateObject();
        if (obj != NULL) {
            std::string test_str = fdp.ConsumeRandomLengthString(16);
            char* result = cJSON_SetValuestring(obj, test_str.c_str());
            cJSON_Delete(obj);
        }
    }
    
    // NULL valuestring
    if (fdp.remaining_bytes() > 10) {
        std::string initial = fdp.ConsumeRandomLengthString(32);
        cJSON* string_item = cJSON_CreateString(initial.c_str());
        if (string_item != NULL) {
            char* result = cJSON_SetValuestring(string_item, NULL);
            cJSON_Delete(string_item);
        }
    }
    
    // Test 2: Reference objects (critical for cJSON_IsReference branch)
    if (fdp.remaining_bytes() > 100) {
        test_reference_objects(fdp);
    }
    
    // Test 3: Overlapping string scenarios with precise conditions
    if (fdp.remaining_bytes() > 200) {
        test_overlapping_branches(fdp);
    }
    
    // Test 4: Boundary conditions for overlapping check
    if (fdp.remaining_bytes() > 150) {
        test_overlapping_boundary_conditions(fdp);
    }
    
    // Test 5: Normal operation with various string lengths
    if (fdp.remaining_bytes() > 100) {
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 30; i++) {
            std::string initial = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 100));
            std::string new_str = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 100));
            
            cJSON* string_item = cJSON_CreateString(initial.c_str());
            if (string_item != NULL) {
                char* result = cJSON_SetValuestring(string_item, new_str.c_str());
                cJSON_Delete(string_item);
            }
        }
    }
    
    // Test 6: Empty strings edge cases
    if (fdp.remaining_bytes() > 10) {
        // Empty to empty
        cJSON* empty = cJSON_CreateString("");
        if (empty != NULL) {
            char* result = cJSON_SetValuestring(empty, "");
            cJSON_Delete(empty);
        }
        
        // Non-empty to empty
        if (fdp.remaining_bytes() > 10) {
            cJSON* empty2 = cJSON_CreateString("");
            if (empty2 != NULL) {
                std::string new_str = fdp.ConsumeRandomLengthString(16);
                char* result = cJSON_SetValuestring(empty2, new_str.c_str());
                cJSON_Delete(empty2);
            }
        }
        
        // Empty to non-empty
        if (fdp.remaining_bytes() > 10) {
            std::string initial = fdp.ConsumeRandomLengthString(16);
            cJSON* str = cJSON_CreateString(initial.c_str());
            if (str != NULL) {
                char* result = cJSON_SetValuestring(str, "");
                cJSON_Delete(str);
            }
        }
    }
    
    // Test 7: Memory cleanup and repeated modifications
    if (fdp.remaining_bytes() > 200) {
        cJSON* string_item = cJSON_CreateString("initial");
        if (string_item != NULL) {
            // Modify multiple times
            int modifications = fdp.ConsumeIntegralInRange<int>(1, 10);
            for (int i = 0; i < modifications && fdp.remaining_bytes() > 20; i++) {
                std::string new_str = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 200));
                char* result = cJSON_SetValuestring(string_item, new_str.c_str());
            }
            cJSON_Delete(string_item);
        }
    }

    return 0;
}

} // namespace harness_025
