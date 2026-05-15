#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_001 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Minimum size for meaningful array testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which array manipulation to perform
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    // Consume basic parameters for array operations
    int array_size = fdp.ConsumeIntegralInRange<int>(0, 100);
    int index = fdp.ConsumeIntegralInRange<int>(0, array_size);
    double number_value = fdp.ConsumeFloatingPoint<double>();
    int int_value = fdp.ConsumeIntegral<int>();
    bool bool_value = fdp.ConsumeBool();
    
    // Consume string for string array and string elements
    std::string string_value = fdp.ConsumeRandomLengthString(256);
    
    cJSON* array_obj = nullptr;
    cJSON* element = nullptr;
    cJSON* detached_item = nullptr;
    
    switch (operation) {
        case 0: {
            // Test cJSON_CreateArray and basic array operations
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                // Create and add various element types
                cJSON* num_elem = cJSON_CreateNumber(number_value);
                cJSON* str_elem = cJSON_CreateString(string_value.c_str());
                cJSON* bool_elem = cJSON_CreateBool(bool_value);
                cJSON* null_elem = cJSON_CreateNull();
                cJSON* true_elem = cJSON_CreateTrue();
                cJSON* false_elem = cJSON_CreateFalse();
                
                if (num_elem) cJSON_AddItemToArray(array_obj, num_elem);
                if (str_elem) cJSON_AddItemToArray(array_obj, str_elem);
                if (bool_elem) cJSON_AddItemToArray(array_obj, bool_elem);
                if (null_elem) cJSON_AddItemToArray(array_obj, null_elem);
                if (true_elem) cJSON_AddItemToArray(array_obj, true_elem);
                if (false_elem) cJSON_AddItemToArray(array_obj, false_elem);
                
                // Get array size
                (void)cJSON_GetArraySize(array_obj);
                
                // Clean up
                cJSON_Delete(array_obj);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_CreateIntArray
            if (array_size > 0) {
                std::vector<int> int_array(array_size);
                for (int i = 0; i < array_size; i++) {
                    int_array[i] = fdp.ConsumeIntegral<int>();
                }
                array_obj = cJSON_CreateIntArray(int_array.data(), array_size);
                if (array_obj != nullptr) {
                    // Test array access
                    for (int i = 0; i < array_size && i < 10; i++) {
                        element = cJSON_GetArrayItem(array_obj, i);
                        (void)element;
                    }
                    cJSON_Delete(array_obj);
                }
            }
            break;
        }
        
        case 2: {
            // Test cJSON_CreateStringArray
            if (array_size > 0 && array_size <= 20) {
                std::vector<const char*> string_array(array_size);
                std::vector<std::string> string_storage(array_size);
                for (int i = 0; i < array_size; i++) {
                    string_storage[i] = fdp.ConsumeRandomLengthString(100);
                    string_array[i] = string_storage[i].c_str();
                }
                array_obj = cJSON_CreateStringArray(string_array.data(), array_size);
                if (array_obj != nullptr) {
                    cJSON_Delete(array_obj);
                }
            }
            break;
        }
        
        case 3: {
            // Test cJSON_AddItemToArray with various element types
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                for (int i = 0; i < 5; i++) {
                    uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    cJSON* new_elem = nullptr;
                    
                    switch (elem_type) {
                        case 0:
                            new_elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 1:
                            new_elem = cJSON_CreateString(fdp.ConsumeRandomLengthString(100).c_str());
                            break;
                        case 2:
                            new_elem = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                        case 3:
                            new_elem = cJSON_CreateNull();
                            break;
                    }
                    
                    if (new_elem != nullptr) {
                        cJSON_AddItemToArray(array_obj, new_elem);
                    }
                }
                cJSON_Delete(array_obj);
            }
            break;
        }
        
        case 4: {
            // Test cJSON_InsertItemInArray
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                // Add some initial items
                for (int i = 0; i < 3; i++) {
                    cJSON* item = cJSON_CreateNumber(i);
                    if (item) cJSON_AddItemToArray(array_obj, item);
                }
                
                // Insert new item at random position
                int insert_pos = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj));
                cJSON* new_item = cJSON_CreateString(string_value.c_str());
                if (new_item != nullptr) {
                    cJSON_InsertItemInArray(array_obj, insert_pos, new_item);
                }
                
                cJSON_Delete(array_obj);
            }
            break;
        }
        
        case 5: {
            // Test cJSON_ReplaceItemInArray
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                // Add some initial items
                for (int i = 0; i < 5; i++) {
                    cJSON* item = cJSON_CreateNumber(i * 10);
                    if (item) cJSON_AddItemToArray(array_obj, item);
                }
                
                // Replace item at random position
                int replace_pos = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj) - 1);
                cJSON* new_item = cJSON_CreateString(string_value.c_str());
                if (new_item != nullptr && cJSON_GetArraySize(array_obj) > 0) {
                    cJSON_ReplaceItemInArray(array_obj, replace_pos, new_item);
                } else if (new_item != nullptr) {
                    cJSON_Delete(new_item);
                }
                
                cJSON_Delete(array_obj);
            }
            break;
        }
        
        case 6: {
            // Test cJSON_DeleteItemFromArray
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                // Add some initial items
                for (int i = 0; i < 5; i++) {
                    cJSON* item = cJSON_CreateNumber(i * 5);
                    if (item) cJSON_AddItemToArray(array_obj, item);
                }
                
                // Delete item at random position
                int delete_pos = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj) - 1);
                if (cJSON_GetArraySize(array_obj) > 0) {
                    cJSON_DeleteItemFromArray(array_obj, delete_pos);
                }
                
                cJSON_Delete(array_obj);
            }
            break;
        }
        
        case 7: {
            // Test cJSON_DetachItemFromArray
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                // Add some initial items
                for (int i = 0; i < 5; i++) {
                    cJSON* item = cJSON_CreateNumber(i * 7);
                    if (item) cJSON_AddItemToArray(array_obj, item);
                }
                
                // Detach item at random position
                int detach_pos = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj) - 1);
                if (cJSON_GetArraySize(array_obj) > 0) {
                    detached_item = cJSON_DetachItemFromArray(array_obj, detach_pos);
                    if (detached_item != nullptr) {
                        cJSON_Delete(detached_item);
                    }
                }
                
                cJSON_Delete(array_obj);
            }
            break;
        }
        
        case 8: {
            // Test cJSON_GetArrayItem and cJSON_GetArraySize
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                // Add mixed element types using fuzzed data
                cJSON* num_elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                std::string str_val = fdp.ConsumeRandomLengthString(100);
                cJSON* str_elem = cJSON_CreateString(str_val.c_str());
                cJSON* bool_elem = cJSON_CreateBool(fdp.ConsumeBool());
                cJSON* null_elem = cJSON_CreateNull();
                
                if (num_elem) cJSON_AddItemToArray(array_obj, num_elem);
                if (str_elem) cJSON_AddItemToArray(array_obj, str_elem);
                if (bool_elem) cJSON_AddItemToArray(array_obj, bool_elem);
                if (null_elem) cJSON_AddItemToArray(array_obj, null_elem);
                
                int arr_size = cJSON_GetArraySize(array_obj);
                (void)arr_size;
                
                // Access each element
                for (int i = 0; i < arr_size; i++) {
                    element = cJSON_GetArrayItem(array_obj, i);
                    if (element != nullptr) {
                        // Test type checking on array elements
                        (void)cJSON_IsNumber(element);
                        (void)cJSON_IsString(element);
                        (void)cJSON_IsBool(element);
                        (void)cJSON_IsNull(element);
                    }
                }
                
                cJSON_Delete(array_obj);
            }
            break;
        }
        
        case 9: {
            // Test complex array operations - combination of multiple operations
            array_obj = cJSON_CreateArray();
            if (array_obj != nullptr) {
                // Phase 1: Add initial elements using fuzzed data
                for (int i = 0; i < 3; i++) {
                    double num_val = fdp.ConsumeFloatingPoint<double>();
                    cJSON* item = cJSON_CreateNumber(num_val);
                    if (item) cJSON_AddItemToArray(array_obj, item);
                }
                
                // Phase 2: Insert new element with fuzzed string
                std::string insert_str = fdp.ConsumeRandomLengthString(100);
                cJSON* insert_item = cJSON_CreateString(insert_str.c_str());
                if (insert_item != nullptr) {
                    cJSON_InsertItemInArray(array_obj, 1, insert_item);
                }
                
                // Phase 3: Replace an element with fuzzed boolean
                bool replace_bool = fdp.ConsumeBool();
                cJSON* replace_item = cJSON_CreateBool(replace_bool);
                if (replace_item != nullptr && cJSON_GetArraySize(array_obj) > 2) {
                    cJSON_ReplaceItemInArray(array_obj, 2, replace_item);
                } else if (replace_item != nullptr) {
                    cJSON_Delete(replace_item);
                }
                
                // Phase 4: Detach an element
                if (cJSON_GetArraySize(array_obj) > 0) {
                    detached_item = cJSON_DetachItemFromArray(array_obj, 0);
                    if (detached_item != nullptr) {
                        cJSON_Delete(detached_item);
                    }
                }
                
                // Phase 5: Delete an element
                if (cJSON_GetArraySize(array_obj) > 0) {
                    cJSON_DeleteItemFromArray(array_obj, 0);
                }
                
                // Final: Verify array size and access remaining elements
                int final_size = cJSON_GetArraySize(array_obj);
                for (int i = 0; i < final_size; i++) {
                    element = cJSON_GetArrayItem(array_obj, i);
                    (void)element;
                }
                
                cJSON_Delete(array_obj);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_001
