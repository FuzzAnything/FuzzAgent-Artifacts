#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_001 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Determine whether to parse JSON or create arrays from scratch
    bool parse_json = fdp.ConsumeBool();
    
    cJSON* root_array = NULL;
    
    if (parse_json) {
        // Parse JSON array from input
        std::string json_input = fdp.ConsumeRandomLengthString(512);
        cJSON* parsed = cJSON_Parse(json_input.c_str());
        
        if (parsed != NULL && cJSON_IsArray(parsed)) {
            root_array = parsed;
        } else {
            // If parsing fails or result is not an array, create a new array
            if (parsed != NULL) {
                cJSON_Delete(parsed);
            }
            root_array = cJSON_CreateArray();
        }
    } else {
        // Create a new array directly
        root_array = cJSON_CreateArray();
    }
    
    if (root_array == NULL) {
        return 0;
    }

    // Create typed arrays using fuzzer data
    // 1. Create double array
    size_t double_count = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    std::vector<double> double_numbers;
    for (size_t i = 0; i < double_count && fdp.remaining_bytes() >= sizeof(double); i++) {
        double_numbers.push_back(fdp.ConsumeFloatingPoint<double>());
    }
    if (!double_numbers.empty()) {
        cJSON* double_array = cJSON_CreateDoubleArray(double_numbers.data(), double_numbers.size());
        if (double_array != NULL) {
            cJSON_AddItemToArray(root_array, double_array);
        }
    }

    // 2. Create float array
    size_t float_count = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    std::vector<float> float_numbers;
    for (size_t i = 0; i < float_count && fdp.remaining_bytes() >= sizeof(float); i++) {
        float_numbers.push_back(fdp.ConsumeFloatingPoint<float>());
    }
    if (!float_numbers.empty()) {
        cJSON* float_array = cJSON_CreateFloatArray(float_numbers.data(), float_numbers.size());
        if (float_array != NULL) {
            cJSON_AddItemToArray(root_array, float_array);
        }
    }

    // 3. Create int array
    size_t int_count = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    std::vector<int> int_numbers;
    for (size_t i = 0; i < int_count && fdp.remaining_bytes() >= sizeof(int); i++) {
        int_numbers.push_back(fdp.ConsumeIntegral<int>());
    }
    if (!int_numbers.empty()) {
        cJSON* int_array = cJSON_CreateIntArray(int_numbers.data(), int_numbers.size());
        if (int_array != NULL) {
            cJSON_AddItemToArray(root_array, int_array);
        }
    }

    // 4. Create string array
    size_t string_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    std::vector<const char*> string_ptrs;
    std::vector<std::string> strings_storage;
    
    // Reserve capacity first to prevent reallocation
    strings_storage.reserve(string_count);
    for (size_t i = 0; i < string_count && fdp.remaining_bytes() > 0; i++) {
        strings_storage.push_back(fdp.ConsumeRandomLengthString(50));
    }
    // Store pointers after vector has finished growing
    for (const auto& str : strings_storage) {
        string_ptrs.push_back(str.c_str());
    }
    
    if (!string_ptrs.empty()) {
        cJSON* string_array = cJSON_CreateStringArray(string_ptrs.data(), string_ptrs.size());
        if (string_array != NULL) {
            cJSON_AddItemToArray(root_array, string_array);
        }
    }

    // Add some simple items to the root array for manipulation
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 0; i++) {
        int item_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        switch (item_type) {
            case 0:
                cJSON_AddItemToArray(root_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                break;
            case 1:
                cJSON_AddItemToArray(root_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(20).c_str()));
                break;
            case 2:
                cJSON_AddItemToArray(root_array, cJSON_CreateBool(fdp.ConsumeBool()));
                break;
            case 3:
                cJSON_AddItemToArray(root_array, cJSON_CreateNull());
                break;
        }
    }

    // Get array size for manipulation operations
    int array_size = cJSON_GetArraySize(root_array);
    
    if (array_size > 0) {
        // Test insertion at random position
        int insert_pos = fdp.ConsumeIntegralInRange<int>(0, array_size);
        cJSON* insert_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (insert_item != NULL) {
            cJSON_InsertItemInArray(root_array, insert_pos, insert_item);
        }
        
        // Update array size after insertion
        array_size = cJSON_GetArraySize(root_array);
        
        if (array_size > 0) {
            // Test replacement at random position
            int replace_pos = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON* replace_item = cJSON_CreateString(fdp.ConsumeRandomLengthString(30).c_str());
            if (replace_item != NULL) {
                cJSON_ReplaceItemInArray(root_array, replace_pos, replace_item);
            }
            
            // Test detaching item
            int detach_pos = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON* detached = cJSON_DetachItemFromArray(root_array, detach_pos);
            if (detached != NULL) {
                cJSON_Delete(detached);
            }
            
            // Update array size after detach
            array_size = cJSON_GetArraySize(root_array);
            
            if (array_size > 0) {
                // Test deleting item
                int delete_pos = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
                cJSON_DeleteItemFromArray(root_array, delete_pos);
            }
        }
    }

    // Cleanup
    cJSON_Delete(root_array);
    
    return 0;
}

} // namespace harness_001
