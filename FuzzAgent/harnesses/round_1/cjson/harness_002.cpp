#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include "cjson/cJSON.h"

namespace harness_002 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Minimum size for meaningful object manipulation testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode to test different object manipulation scenarios
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Create root object
    cJSON* root_object = cJSON_CreateObject();
    if (root_object == nullptr) {
        return 0;
    }
    
    // Store key strings to keep them alive for the duration of the test
    // Use std::deque instead of std::vector to avoid reallocation and keep c_str() pointers valid
    std::deque<std::string> key_strings;
    
    // Consume number of items to add to object
    size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    // Create and add items to object
    for (size_t i = 0; i < num_items && fdp.remaining_bytes() > 10; i++) {
        // Consume key name and store it
        key_strings.push_back(fdp.ConsumeRandomLengthString(50));
        const char* key_name = key_strings.back().c_str();
        
        // Consume value type (0: string, 1: number, 2: boolean, 3: null)
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (value_type) {
            case 0: {  // String
                std::string str_value = fdp.ConsumeRandomLengthString(100);
                if (operation_mode == 0) {
                    cJSON* item = cJSON_CreateString(str_value.c_str());
                    if (item) {
                        cJSON_AddItemToObject(root_object, key_name, item);
                    }
                } else if (operation_mode == 1) {
                    cJSON* item = cJSON_CreateString(str_value.c_str());
                    if (item) {
                        cJSON_AddItemToObjectCS(root_object, key_name, item);
                    }
                } else {
                    cJSON_AddStringToObject(root_object, key_name, str_value.c_str());
                }
                break;
            }
            case 1: {  // Number
                double num_value = fdp.ConsumeFloatingPoint<double>();
                if (operation_mode == 0) {
                    cJSON* item = cJSON_CreateNumber(num_value);
                    if (item) {
                        cJSON_AddItemToObject(root_object, key_name, item);
                    }
                } else if (operation_mode == 1) {
                    cJSON* item = cJSON_CreateNumber(num_value);
                    if (item) {
                        cJSON_AddItemToObjectCS(root_object, key_name, item);
                    }
                } else {
                    cJSON_AddNumberToObject(root_object, key_name, num_value);
                }
                break;
            }
            case 2: {  // Boolean
                bool bool_value = fdp.ConsumeBool();
                if (operation_mode == 0) {
                    cJSON* item = cJSON_CreateBool(bool_value ? 1 : 0);
                    if (item) {
                        cJSON_AddItemToObject(root_object, key_name, item);
                    }
                } else if (operation_mode == 1) {
                    cJSON* item = cJSON_CreateBool(bool_value ? 1 : 0);
                    if (item) {
                        cJSON_AddItemToObjectCS(root_object, key_name, item);
                    }
                } else {
                    cJSON_AddBoolToObject(root_object, key_name, bool_value ? 1 : 0);
                }
                break;
            }
            case 3: {  // Null
                if (operation_mode == 0) {
                    cJSON* item = cJSON_CreateNull();
                    if (item) {
                        cJSON_AddItemToObject(root_object, key_name, item);
                    }
                } else if (operation_mode == 1) {
                    cJSON* item = cJSON_CreateNull();
                    if (item) {
                        cJSON_AddItemToObjectCS(root_object, key_name, item);
                    }
                } else {
                    cJSON_AddNullToObject(root_object, key_name);
                }
                break;
            }
        }
    }
    
    // Now test retrieval and manipulation operations
    if (fdp.remaining_bytes() > 10) {
        // Consume a key to test retrieval operations and store it
        key_strings.push_back(fdp.ConsumeRandomLengthString(50));
        const char* test_key = key_strings.back().c_str();
        
        // Test cJSON_GetObjectItem and cJSON_GetObjectItemCaseSensitive
        cJSON* retrieved_item = nullptr;
        if (operation_mode == 0) {
            retrieved_item = cJSON_GetObjectItem(root_object, test_key);
        } else {
            retrieved_item = cJSON_GetObjectItemCaseSensitive(root_object, test_key);
        }
        
        // Test cJSON_HasObjectItem
        (void)cJSON_HasObjectItem(root_object, test_key);
        
        // If we found an item and have enough input, test replacement
        if (retrieved_item != nullptr && fdp.remaining_bytes() > 20) {
            // Create a replacement item
            cJSON* replacement_item = nullptr;
            uint8_t replacement_type = fdp.ConsumeIntegral<uint8_t>() % 2;
            
            if (replacement_type == 0) {
                std::string replacement_str = fdp.ConsumeRandomLengthString(50);
                replacement_item = cJSON_CreateString(replacement_str.c_str());
            } else {
                double replacement_num = fdp.ConsumeFloatingPoint<double>();
                replacement_item = cJSON_CreateNumber(replacement_num);
            }
            
            if (replacement_item != nullptr) {
                // Test replacement operations
                if (operation_mode == 0) {
                    cJSON_ReplaceItemInObject(root_object, test_key, replacement_item);
                } else {
                    cJSON_ReplaceItemInObjectCaseSensitive(root_object, test_key, replacement_item);
                }
            }
        }
        
        // Test detachment operations if we have enough input
        if (fdp.remaining_bytes() > 10) {
            key_strings.push_back(fdp.ConsumeRandomLengthString(50));
            const char* detach_key = key_strings.back().c_str();
            cJSON* detached_item = nullptr;
            
            if (operation_mode == 0) {
                detached_item = cJSON_DetachItemFromObject(root_object, detach_key);
            } else {
                detached_item = cJSON_DetachItemFromObjectCaseSensitive(root_object, detach_key);
            }
            
            // Clean up detached item if it exists
            if (detached_item != nullptr) {
                cJSON_Delete(detached_item);
            }
        }
        
        // Test deletion operations if we have enough input
        if (fdp.remaining_bytes() > 10) {
            key_strings.push_back(fdp.ConsumeRandomLengthString(50));
            const char* delete_key = key_strings.back().c_str();
            
            if (operation_mode == 0) {
                cJSON_DeleteItemFromObject(root_object, delete_key);
            } else {
                cJSON_DeleteItemFromObjectCaseSensitive(root_object, delete_key);
            }
        }
    }
    
    // Test object printing and validation
    if (fdp.remaining_bytes() > 5) {
        // Test printing the object
        char* printed = cJSON_Print(root_object);
        if (printed != nullptr) {
            free(printed);
        }
        
        // Test unformatted printing
        char* printed_unformatted = cJSON_PrintUnformatted(root_object);
        if (printed_unformatted != nullptr) {
            free(printed_unformatted);
        }
        
        // Test buffered printing with random buffer size
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        bool formatted = fdp.ConsumeBool();
        char* printed_buffered = cJSON_PrintBuffered(root_object, buffer_size, formatted);
        if (printed_buffered != nullptr) {
            free(printed_buffered);
        }
    }
    
    // Cleanup - delete the root object
    cJSON_Delete(root_object);
    
    return 0;
}

} // namespace harness_002
