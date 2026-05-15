#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_007 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Minimum input size for comprehensive testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Test 1: Create various JSON items using cJSON_Create* functions
    cJSON* items[12];  // Store created items for type checking
    int item_count = 0;
    
    // Create different types of JSON items
    items[item_count++] = cJSON_CreateNull();
    items[item_count++] = cJSON_CreateTrue();
    items[item_count++] = cJSON_CreateFalse();
    items[item_count++] = cJSON_CreateBool(fdp.ConsumeBool());
    items[item_count++] = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
    
    std::string str_value = fdp.ConsumeRandomLengthString(128);
    items[item_count++] = cJSON_CreateString(str_value.c_str());
    
    std::string raw_value = fdp.ConsumeRandomLengthString(128);
    items[item_count++] = cJSON_CreateRaw(raw_value.c_str());
    
    items[item_count++] = cJSON_CreateArray();
    items[item_count++] = cJSON_CreateObject();
    
    // Add some items to array and object
    cJSON* array = items[item_count-2];
    cJSON* obj = items[item_count-1];
    
    if (array != NULL) {
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            double num = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddItemToArray(array, cJSON_CreateNumber(num));
        }
    }
    
    if (obj != NULL) {
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            double value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(obj, key.c_str(), value);
        }
    }
    
    // Test 2: Test all type checking functions on created items
    for (int i = 0; i < item_count; i++) {
        if (items[i] == NULL) continue;
        
        // Test all type checking functions
        // These should return appropriate boolean values based on item type
        cJSON_IsInvalid(items[i]);
        cJSON_IsFalse(items[i]);
        cJSON_IsTrue(items[i]);
        cJSON_IsBool(items[i]);
        cJSON_IsNull(items[i]);
        cJSON_IsNumber(items[i]);
        cJSON_IsString(items[i]);
        cJSON_IsArray(items[i]);
        cJSON_IsObject(items[i]);
        cJSON_IsRaw(items[i]);
    }
    
    // Test 3: Test print functions with preallocated buffers
    // Create a more complex JSON object for printing tests
    cJSON* print_test_obj = cJSON_CreateObject();
    if (print_test_obj != NULL) {
        // Add various items to test object
        std::string obj_key1 = fdp.ConsumeRandomLengthString(16);
        std::string obj_val1 = fdp.ConsumeRandomLengthString(32);
        cJSON_AddStringToObject(print_test_obj, obj_key1.c_str(), obj_val1.c_str());
        
        std::string obj_key2 = fdp.ConsumeRandomLengthString(16);
        double obj_val2 = fdp.ConsumeFloatingPoint<double>();
        cJSON_AddNumberToObject(print_test_obj, obj_key2.c_str(), obj_val2);
        
        // Test cJSON_PrintUnformatted
        char* unformatted = cJSON_PrintUnformatted(print_test_obj);
        if (unformatted != NULL) {
            // Test cJSON_PrintBuffered
            int prebuffer = fdp.ConsumeIntegralInRange<int>(1, 1024);
            bool fmt = fdp.ConsumeBool();
            char* buffered = cJSON_PrintBuffered(print_test_obj, prebuffer, fmt);
            if (buffered != NULL) {
                cJSON_free(buffered);
            }
            
            // Test cJSON_PrintPreallocated
            size_t buffer_size = strlen(unformatted) + 100;  // Extra space for safety
            char* prealloc_buffer = (char*)cJSON_malloc(buffer_size);
            if (prealloc_buffer != NULL) {
                bool format = fdp.ConsumeBool();
                cJSON_PrintPreallocated(print_test_obj, prealloc_buffer, buffer_size, format);
                cJSON_free(prealloc_buffer);
            }
            
            cJSON_free(unformatted);
        }
        
        cJSON_Delete(print_test_obj);
    }
    
    // Test 4: Test memory allocation APIs directly
    size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    void* allocated_mem = cJSON_malloc(alloc_size);
    if (allocated_mem != NULL) {
        // Use the memory (simulate writing some data)
        memset(allocated_mem, 0xAA, alloc_size);
        cJSON_free(allocated_mem);
    }
    
    // Test 5: Test case-sensitive object addition with cJSON_AddItemToObjectCS
    cJSON* cs_obj = cJSON_CreateObject();
    if (cs_obj != NULL) {
        std::string cs_key1 = fdp.ConsumeRandomLengthString(16);
        std::string cs_str_value1 = fdp.ConsumeRandomLengthString(32);
        cJSON* cs_item1 = cJSON_CreateString(cs_str_value1.c_str());
        if (cs_item1 != NULL) {
            cs_item1->type |= cJSON_StringIsConst;
            cJSON_AddItemToObjectCS(cs_obj, cs_key1.c_str(), cs_item1);
        }
        
        std::string cs_key2 = fdp.ConsumeRandomLengthString(16);
        double cs_num_value = fdp.ConsumeFloatingPoint<double>();
        cJSON* cs_item2 = cJSON_CreateNumber(cs_num_value);
        if (cs_item2 != NULL) {
            // For non-string items, we can still use CS version
            cJSON_AddItemToObjectCS(cs_obj, cs_key2.c_str(), cs_item2);
        }
        
        // Test getting items with case sensitivity
        if (!cs_key1.empty()) {
            cJSON* found1 = cJSON_GetObjectItemCaseSensitive(cs_obj, cs_key1.c_str());
            // Do nothing with found1, just testing the function
        }
        
        cJSON_Delete(cs_obj);
    }
    
    // Test 6: Test cJSON_SetNumberHelper and cJSON_SetValuestring
    cJSON* modifiable_obj = cJSON_CreateObject();
    if (modifiable_obj != NULL) {
        // Create a number item to modify
        double initial_number = fdp.ConsumeFloatingPoint<double>();
        cJSON* number_item = cJSON_CreateNumber(initial_number);
        if (number_item != NULL) {
            // Add to object first
            std::string num_key = fdp.ConsumeRandomLengthString(16);
            cJSON_AddItemToObject(modifiable_obj, num_key.c_str(), number_item);
            
            // Modify the number using cJSON_SetNumberHelper
            double new_number = fdp.ConsumeFloatingPoint<double>();
            cJSON_SetNumberHelper(number_item, new_number);
        }
        
        // Create a string item to modify
        std::string initial_string = fdp.ConsumeRandomLengthString(32);
        cJSON* string_item = cJSON_CreateString(initial_string.c_str());
        if (string_item != NULL) {
            // Add to object
            std::string str_key = fdp.ConsumeRandomLengthString(16);
            cJSON_AddItemToObject(modifiable_obj, str_key.c_str(), string_item);
            
            // Modify the string using cJSON_SetValuestring
            std::string new_string = fdp.ConsumeRandomLengthString(64);
            char* old_valuestring = cJSON_SetValuestring(string_item, new_string.c_str());
            // Note: old_valuestring pointer is owned by cJSON object, do not free
        }
        
        cJSON_Delete(modifiable_obj);
    }
    
    // Clean up all created items
    for (int i = 0; i < item_count; i++) {
        if (items[i] != NULL) {
            cJSON_Delete(items[i]);
        }
    }
    
    return 0;
}

} // namespace harness_007
