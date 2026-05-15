#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 10;
    uint8_t array_size = fdp.ConsumeIntegralInRange<uint8_t>(1, 20);
    bool use_reference = fdp.ConsumeBool();
    int insert_position = fdp.ConsumeIntegralInRange<int>(0, array_size);
    int replace_position = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
    int detach_position = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
    int delete_position = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
    
    // Consume numeric arrays for specialized array creation
    std::vector<int> int_array;
    std::vector<float> float_array;
    std::vector<double> double_array;
    std::vector<std::string> string_array;
    
    for (int i = 0; i < array_size; i++) {
        if (fdp.remaining_bytes() >= sizeof(int)) {
            int_array.push_back(fdp.ConsumeIntegral<int>());
        }
        if (fdp.remaining_bytes() >= sizeof(float)) {
            float_array.push_back(fdp.ConsumeFloatingPoint<float>());
        }
        if (fdp.remaining_bytes() >= sizeof(double)) {
            double_array.push_back(fdp.ConsumeFloatingPoint<double>());
        }
        if (fdp.remaining_bytes() > 0) {
            string_array.push_back(fdp.ConsumeRandomLengthString(64));
        }
    }
    
    // Consume remaining strings for various operations
    std::string json_input = fdp.ConsumeRandomLengthString(1024);
    std::string new_string_value = fdp.ConsumeRandomLengthString(256);

    // Use operation to determine API flow
    switch (operation) {
        case 0: {
            // Test specialized array creation functions
            if (!int_array.empty()) {
                cJSON* int_json_array = cJSON_CreateIntArray(int_array.data(), int_array.size());
                if (int_json_array != NULL) {
                    char* printed = cJSON_Print(int_json_array);
                    if (printed != NULL) {
                        free(printed);
                    }
                    cJSON_Delete(int_json_array);
                }
            }
            break;
        }
        
        case 1: {
            // Test float array creation
            if (!float_array.empty()) {
                cJSON* float_json_array = cJSON_CreateFloatArray(float_array.data(), float_array.size());
                if (float_json_array != NULL) {
                    char* printed = cJSON_Print(float_json_array);
                    if (printed != NULL) {
                        free(printed);
                    }
                    cJSON_Delete(float_json_array);
                }
            }
            break;
        }
        
        case 2: {
            // Test double array creation
            if (!double_array.empty()) {
                cJSON* double_json_array = cJSON_CreateDoubleArray(double_array.data(), double_array.size());
                if (double_json_array != NULL) {
                    char* printed = cJSON_Print(double_json_array);
                    if (printed != NULL) {
                        free(printed);
                    }
                    cJSON_Delete(double_json_array);
                }
            }
            break;
        }
        
        case 3: {
            // Test string array creation
            if (!string_array.empty()) {
                std::vector<const char*> c_strings;
                for (const auto& str : string_array) {
                    c_strings.push_back(str.c_str());
                }
                cJSON* string_json_array = cJSON_CreateStringArray(c_strings.data(), c_strings.size());
                if (string_json_array != NULL) {
                    char* printed = cJSON_Print(string_json_array);
                    if (printed != NULL) {
                        free(printed);
                    }
                    cJSON_Delete(string_json_array);
                }
            }
            break;
        }
        
        case 4: {
            // Test array reference creation and manipulation
            cJSON* base_array = cJSON_CreateArray();
            if (base_array != NULL) {
                // Add some items to base array
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON* item = cJSON_CreateNumber(val);
                    cJSON_AddItemToArray(base_array, item);
                }
                
                // Create array reference
                cJSON* array_ref = cJSON_CreateArrayReference(base_array);
                if (array_ref != NULL) {
                    // Test that reference shares same data
                    int ref_size = cJSON_GetArraySize(array_ref);
                    int base_size = cJSON_GetArraySize(base_array);
                    
                    // Add item reference to another array
                    cJSON* another_array = cJSON_CreateArray();
                    if (another_array != NULL) {
                        cJSON_AddItemReferenceToArray(another_array, array_ref);
                        cJSON_Delete(another_array);
                    }
                    
                    cJSON_Delete(array_ref);
                }
                cJSON_Delete(base_array);
            }
            break;
        }
        
        case 5: {
            // Test array insertion operations
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                // Add initial items
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON* item = cJSON_CreateNumber(val);
                    cJSON_AddItemToArray(array, item);
                }
                
                // Insert new item at specified position
                cJSON* new_item = cJSON_CreateString(new_string_value.c_str());
                if (new_item != NULL) {
                    cJSON_InsertItemInArray(array, insert_position, new_item);
                }
                
                // Verify insertion worked
                int final_size = cJSON_GetArraySize(array);
                
                char* printed = cJSON_Print(array);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 6: {
            // Test array item replacement
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                // Add initial items
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON* item = cJSON_CreateNumber(val);
                    cJSON_AddItemToArray(array, item);
                }
                
                // Replace item at specified position
                if (array_size > 0) {
                    cJSON* replacement_item = cJSON_CreateString(new_string_value.c_str());
                    if (replacement_item != NULL) {
                        cJSON_ReplaceItemInArray(array, replace_position, replacement_item);
                    }
                }
                
                char* printed = cJSON_Print(array);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 7: {
            // Test array item detachment
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                // Add initial items
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON* item = cJSON_CreateNumber(val);
                    cJSON_AddItemToArray(array, item);
                }
                
                // Detach item
                if (array_size > 0) {
                    cJSON* detached = cJSON_DetachItemFromArray(array, detach_position);
                    if (detached != NULL) {
                        cJSON_Delete(detached);
                    }
                }
                
                char* printed = cJSON_Print(array);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 8: {
            // Test array item deletion
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                // Add initial items
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON* item = cJSON_CreateNumber(val);
                    cJSON_AddItemToArray(array, item);
                }
                
                // Delete item
                if (array_size > 0) {
                    cJSON_DeleteItemFromArray(array, delete_position);
                }
                
                char* printed = cJSON_Print(array);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 9: {
            // Comprehensive array lifecycle test
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                // Phase 1: Create array with various item types
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    switch (item_type) {
                        case 0:
                            cJSON_AddItemToArray(array, cJSON_CreateNull());
                            break;
                        case 1:
                            cJSON_AddItemToArray(array, cJSON_CreateBool(fdp.ConsumeBool()));
                            break;
                        case 2: {
                            double val = fdp.ConsumeFloatingPoint<double>();
                            cJSON_AddItemToArray(array, cJSON_CreateNumber(val));
                            break;
                        }
                        case 3: {
                            std::string str = fdp.ConsumeRandomLengthString(128);
                            cJSON_AddItemToArray(array, cJSON_CreateString(str.c_str()));
                            break;
                        }
                    }
                }
                
                // Phase 2: Query array properties
                int actual_size = cJSON_GetArraySize(array);
                if (actual_size > 0) {
                    // Get random item
                    int query_index = fdp.ConsumeIntegralInRange<int>(0, actual_size - 1);
                    cJSON* queried_item = cJSON_GetArrayItem(array, query_index);
                    
                    // Check item type
                    if (queried_item != NULL) {
                        if (cJSON_IsNull(queried_item)) {
                            // Null item
                        } else if (cJSON_IsBool(queried_item)) {
                            bool is_true = cJSON_IsTrue(queried_item);
                        } else if (cJSON_IsNumber(queried_item)) {
                            double num = cJSON_GetNumberValue(queried_item);
                        } else if (cJSON_IsString(queried_item)) {
                            char* str_val = cJSON_GetStringValue(queried_item);
                        }
                    }
                }
                
                // Phase 3: Array iteration
                cJSON* element;
                cJSON_ArrayForEach(element, array) {
                    // Just iterate through all elements
                }
                
                // Phase 4: Cleanup
                char* printed = cJSON_Print(array);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(array);
            }
            break;
        }
    }

    return 0;
}

} // namespace harness_001
