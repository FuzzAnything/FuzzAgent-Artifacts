#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_002 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create two complex JSON objects for comparison testing
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    
    if (obj1 == NULL || obj2 == NULL) {
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
        return 0;
    }

    // Determine how many items to add to each object
    size_t item_count = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    // Add items to both objects (some identical, some different)
    for (size_t i = 0; i < item_count && fdp.remaining_bytes() > 0; i++) {
        std::string key = fdp.ConsumeRandomLengthString(16);
        std::string key2 = key; // Same key for obj2
        
        int item_type = fdp.ConsumeIntegralInRange<int>(0, 5);
        
        cJSON* item1 = NULL;
        cJSON* item2 = NULL;
        
        switch (item_type) {
            case 0: // String
                {
                    std::string value1 = fdp.ConsumeRandomLengthString(32);
                    std::string value2 = fdp.ConsumeRandomLengthString(32);
                    item1 = cJSON_CreateString(value1.c_str());
                    item2 = cJSON_CreateString(value2.c_str());
                }
                break;
            case 1: // Number
                {
                    double value1 = fdp.ConsumeFloatingPoint<double>();
                    double value2 = fdp.ConsumeFloatingPoint<double>();
                    item1 = cJSON_CreateNumber(value1);
                    item2 = cJSON_CreateNumber(value2);
                }
                break;
            case 2: // Boolean
                {
                    bool value1 = fdp.ConsumeBool();
                    bool value2 = fdp.ConsumeBool();
                    item1 = cJSON_CreateBool(value1);
                    item2 = cJSON_CreateBool(value2);
                }
                break;
            case 3: // Null
                item1 = cJSON_CreateNull();
                item2 = cJSON_CreateNull();
                break;
            case 4: // Array
                {
                    item1 = cJSON_CreateArray();
                    item2 = cJSON_CreateArray();
                    size_t array_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                    for (size_t j = 0; j < array_size && fdp.remaining_bytes() > 0; j++) {
                        double val = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddItemToArray(item1, cJSON_CreateNumber(val));
                        cJSON_AddItemToArray(item2, cJSON_CreateNumber(val));
                    }
                }
                break;
            case 5: // Nested object
                {
                    item1 = cJSON_CreateObject();
                    item2 = cJSON_CreateObject();
                    std::string nested_key = fdp.ConsumeRandomLengthString(8);
                    std::string nested_value = fdp.ConsumeRandomLengthString(16);
                    cJSON_AddStringToObject(item1, nested_key.c_str(), nested_value.c_str());
                    cJSON_AddStringToObject(item2, nested_key.c_str(), nested_value.c_str());
                }
                break;
        }
        
        if (item1 != NULL && item2 != NULL) {
            cJSON_AddItemToObject(obj1, key.c_str(), item1);
            cJSON_AddItemToObject(obj2, key2.c_str(), item2);
        }
    }

    // Now perform modification operations on the objects
    
    // 1. Modify string values using cJSON_SetValuestring
    // Find a string item in obj1 and modify it
    cJSON* current = obj1->child;
    while (current != NULL) {
        if (cJSON_IsString(current) && current->valuestring != NULL) {
            std::string new_value = fdp.ConsumeRandomLengthString(32);
            char* result = cJSON_SetValuestring(current, new_value.c_str());
            // result can be NULL if allocation fails, but we continue anyway
            break;
        }
        current = current->next;
    }
    
    // 2. Replace items in objects using both case-sensitive and insensitive versions
    if (obj1->child != NULL && fdp.remaining_bytes() > 0) {
        // Create new items for replacement
        cJSON* new_string_item = cJSON_CreateString(fdp.ConsumeRandomLengthString(24).c_str());
        cJSON* new_number_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        
        // Find a key to replace
        current = obj1->child;
        std::string replace_key;
        while (current != NULL && replace_key.empty()) {
            if (current->string != NULL) {
                replace_key = current->string;
            }
            current = current->next;
        }
        
        if (!replace_key.empty()) {
            // Test case-insensitive replacement
            cJSON_bool replaced = cJSON_ReplaceItemInObject(obj1, replace_key.c_str(), new_string_item);
            
            if (!replaced && new_string_item != NULL) {
                cJSON_Delete(new_string_item);
            }
            
            // Create another new item for case-sensitive replacement
            if (fdp.remaining_bytes() > 0) {
                std::string case_key = fdp.ConsumeRandomLengthString(12);
                // Add an item with this key first
                cJSON_AddItemToObject(obj1, case_key.c_str(), cJSON_CreateBool(true));
                
                // Now replace it with case-sensitive version
                cJSON_ReplaceItemInObjectCaseSensitive(obj1, case_key.c_str(), new_number_item);
            }
        }
    }
    
    // 3. Delete items from objects using both case-sensitive and insensitive versions
    if (obj1->child != NULL && fdp.remaining_bytes() > 0) {
        // Find a key to delete
        current = obj1->child;
        std::string delete_key;
        while (current != NULL && delete_key.empty()) {
            if (current->string != NULL) {
                delete_key = current->string;
            }
            current = current->next;
        }
        
        if (!delete_key.empty()) {
            // Test case-insensitive deletion
            cJSON_DeleteItemFromObject(obj1, delete_key.c_str());
            
            // Add another item for case-sensitive deletion test
            if (fdp.remaining_bytes() > 0) {
                std::string case_delete_key = fdp.ConsumeRandomLengthString(10);
                cJSON_AddItemToObject(obj1, case_delete_key.c_str(), cJSON_CreateNull());
                
                // Test case-sensitive deletion
                cJSON_DeleteItemFromObjectCaseSensitive(obj1, case_delete_key.c_str());
            }
        }
    }
    
    // 4. Compare the two objects using cJSON_Compare with both case_sensitive=true and false
    cJSON_bool compare_case_sensitive = cJSON_Compare(obj1, obj2, true);
    cJSON_bool compare_case_insensitive = cJSON_Compare(obj1, obj2, false);
    
    // Also compare obj1 with itself (should always be true)
    cJSON_bool self_compare = cJSON_Compare(obj1, obj1, true);
    
    // 5. Additional test: Compare after making modifications to obj2 to match obj1
    if (obj2->child != NULL && fdp.remaining_bytes() > 0) {
        current = obj2->child;
        while (current != NULL) {
            if (cJSON_IsString(current) && current->valuestring != NULL) {
                std::string match_value = fdp.ConsumeRandomLengthString(32);
                cJSON_SetValuestring(current, match_value.c_str());
                break;
            }
            current = current->next;
        }
        
        // Compare again after modification
        cJSON_bool modified_compare = cJSON_Compare(obj1, obj2, true);
    }
    
    // Cleanup
    cJSON_Delete(obj1);
    cJSON_Delete(obj2);
    
    return 0;
}

} // namespace harness_002
