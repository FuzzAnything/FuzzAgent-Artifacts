#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "cjson/cJSON.h"

namespace harness_015 {
/*
 * harness_015.cpp - cJSON object and array manipulation functions comprehensive testing harness
 * 
 * This harness specifically targets cJSON object and array manipulation functions:
 * 1) cJSON_GetArraySize, cJSON_GetArrayItem
 * 2) cJSON_GetObjectItem, cJSON_GetObjectItemCaseSensitive, cJSON_HasObjectItem
 * 3) cJSON_AddItemToArray, cJSON_AddItemToObject, cJSON_AddItemToObjectCS
 * 4) cJSON_AddItemReferenceToArray, cJSON_AddItemReferenceToObject
 * 5) cJSON_DetachItemViaPointer, cJSON_DetachItemFromArray, cJSON_DeleteItemFromArray
 * 6) cJSON_DetachItemFromObject, cJSON_DeleteItemFromObject
 * 7) cJSON_InsertItemInArray, cJSON_ReplaceItemViaPointer, cJSON_ReplaceItemInArray, cJSON_ReplaceItemInObject
 * 
 * These functions have complex code paths involving memory management, pointer manipulation,
 * reference counting, and edge case handling. Focus on complex manipulation scenarios:
 * 1) Creating nested arrays/objects with various depth levels
 * 2) Adding items with regular addition vs reference addition
 * 3) Detaching vs deleting items (memory ownership differences)
 * 4) Mixing different item types (numbers, strings, booleans, nulls, arrays, objects)
 * 5) Edge cases: null/invalid pointers, out-of-bounds indices, duplicate keys
 * 6) Memory management during detach/delete operations
 * 7) Case-sensitive vs case-insensitive object operations
 * 
 * Strategy: Generate complex cJSON structures using FuzzedDataProvider and test
 * all manipulation functions with various scenarios to maximize coverage.
 */


extern "C" {
}

// Helper function to create a cJSON item based on type
// Uses stable storage for strings: pre-allocate vector capacity and store strings individually
// to ensure pointers remain valid
static cJSON* create_item_by_type(FuzzedDataProvider& fdp, uint8_t type, std::vector<std::unique_ptr<std::string>>& string_storage) {
    switch (type % 7) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateBool(fdp.ConsumeBool());
        case 2:
            return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        case 3: {
            std::string str = fdp.ConsumeRandomLengthString(100);
            auto str_ptr = std::make_unique<std::string>(str);
            const char* cstr = str_ptr->c_str();
            string_storage.push_back(std::move(str_ptr));
            return cJSON_CreateString(cstr);
        }
        case 4:
            return cJSON_CreateArray();
        case 5:
            return cJSON_CreateObject();
        case 6: {
            std::string raw_str = fdp.ConsumeRandomLengthString(50);
            auto str_ptr = std::make_unique<std::string>(raw_str);
            const char* cstr = str_ptr->c_str();
            string_storage.push_back(std::move(str_ptr));
            return cJSON_CreateRaw(cstr);
        }
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for meaningful manipulation scenarios
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    cJSON* root = nullptr;
    cJSON* array_obj = nullptr;
    cJSON* object_obj = nullptr;
    cJSON* item = nullptr;
    cJSON* detached_item = nullptr;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Comprehensive array manipulation
            array_obj = cJSON_CreateArray();
            if (!array_obj) break;
            // Add various items to array
            int num_items = fdp.ConsumeIntegralInRange<int>(1, 10);
            std::vector<cJSON*> reference_items; // Keep track of items added as references
            std::vector<std::unique_ptr<std::string>> string_storage; // Stable storage for strings
            
            // Pre-allocate storage to prevent reallocation
            string_storage.reserve(num_items * 2);
            
            for (int i = 0; i < num_items && fdp.remaining_bytes() > 8; i++) {
                item = create_item_by_type(fdp, fdp.ConsumeIntegral<uint8_t>(), string_storage);
                if (item) {
                    if (fdp.ConsumeBool()) {
                        cJSON_AddItemToArray(array_obj, item);
                        // Regular addition transfers ownership to array
                    } else {
                        cJSON_AddItemReferenceToArray(array_obj, item);
                        // Reference addition doesn't take ownership, store for later cleanup
                        reference_items.push_back(item);
                    }
                }
            }
            
            // Test array size and item retrieval
            int array_size = cJSON_GetArraySize(array_obj);
            for (int i = 0; i < array_size && i < 5; i++) {
                item = cJSON_GetArrayItem(array_obj, i);
                (void)item; // Use item
            }
            
            // Test edge cases: out of bounds indices
            if (array_size > 0) {
                (void)cJSON_GetArrayItem(array_obj, -1); // Invalid index
                (void)cJSON_GetArrayItem(array_obj, array_size); // Out of bounds
            }
            
            cJSON_Delete(array_obj);
            // Now safe to delete reference items since container is gone
            for (cJSON* ref_item : reference_items) {
                cJSON_Delete(ref_item);
            }
            break;

        }
        
        case 1: {
            // Scenario 1: Comprehensive object manipulation
            object_obj = cJSON_CreateObject();
            if (!object_obj) break;
            
            // Add various items to object with different keys
            int num_items = fdp.ConsumeIntegralInRange<int>(1, 8);
            std::vector<cJSON*> reference_items; // Track items added as references
            std::vector<std::unique_ptr<std::string>> key_storage; // Stable storage for keys
            std::vector<std::unique_ptr<std::string>> string_storage; // Stable storage for strings
            
            // Pre-allocate to prevent reallocation
            key_storage.reserve(num_items + 1);
            string_storage.reserve(num_items * 2);
            
            for (int i = 0; i < num_items && fdp.remaining_bytes() > 16; i++) {
                std::string key = fdp.ConsumeRandomLengthString(20);
                auto key_ptr = std::make_unique<std::string>(key);
                const char* key_cstr = key_ptr->c_str();
                key_storage.push_back(std::move(key_ptr));
                
                item = create_item_by_type(fdp, fdp.ConsumeIntegral<uint8_t>(), string_storage);
                if (item && !key.empty()) {
                    // Use different addition methods
                    uint8_t add_method = fdp.ConsumeIntegral<uint8_t>() % 3;
                    if (add_method == 0) {
                        cJSON_AddItemToObject(object_obj, key_cstr, item);
                        // Regular addition transfers ownership to object
                    } else if (add_method == 1) {
                        cJSON_AddItemToObjectCS(object_obj, key_cstr, item);
                        // Regular addition transfers ownership to object
                    } else {
                        cJSON_AddItemReferenceToObject(object_obj, key_cstr, item);
                        // Reference addition doesn't take ownership, store for later cleanup
                        reference_items.push_back(item);
                    }
                } else if (item) {
                    cJSON_Delete(item);
                }
            }
            
            // Test object item retrieval
            if (fdp.remaining_bytes() > 8) {
                std::string search_key = fdp.ConsumeRandomLengthString(20);
                auto search_key_ptr = std::make_unique<std::string>(search_key);
                const char* search_key_cstr = search_key_ptr->c_str();
                key_storage.push_back(std::move(search_key_ptr));
                
                item = cJSON_GetObjectItem(object_obj, search_key_cstr);
                item = cJSON_GetObjectItemCaseSensitive(object_obj, search_key_cstr);
                (void)cJSON_HasObjectItem(object_obj, search_key_cstr);
            }
            
            cJSON_Delete(object_obj);
            // Now safe to delete reference items since container is gone
            for (cJSON* ref_item : reference_items) {
                cJSON_Delete(ref_item);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Array item detachment and deletion
            array_obj = cJSON_CreateArray();
            if (!array_obj) break;
            
            // Populate array with items (using numeric values to avoid string storage issues)
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 8; i++) {
                item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (item) cJSON_AddItemToArray(array_obj, item);
            }
            
            if (cJSON_GetArraySize(array_obj) > 0) {
                // Test DetachItemFromArray
                int detach_idx = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj) - 1);
                detached_item = cJSON_DetachItemFromArray(array_obj, detach_idx);
                if (detached_item) {
                    cJSON_Delete(detached_item); // We own the detached item
                }
                
                // Test DeleteItemFromArray
                if (cJSON_GetArraySize(array_obj) > 0) {
                    int delete_idx = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj) - 1);
                    cJSON_DeleteItemFromArray(array_obj, delete_idx);
                }
            }
            
            cJSON_Delete(array_obj);
            break;
        }
        
        case 3: {
            // Scenario 3: Object item detachment and deletion
            object_obj = cJSON_CreateObject();
            if (!object_obj) break;
            
            // Use simple numeric values with string literal keys
            cJSON* num1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            cJSON* num2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            
            if (num1) cJSON_AddItemToObject(object_obj, "key1", num1);
            if (num2) cJSON_AddItemToObject(object_obj, "key2", num2);
            
            // Test DetachItemFromObject
            if (fdp.ConsumeBool()) {
                detached_item = cJSON_DetachItemFromObject(object_obj, "key1");
                if (detached_item) {
                    cJSON_Delete(detached_item);
                }
            }
            
            // Test DeleteItemFromObject
            if (fdp.ConsumeBool()) {
                cJSON_DeleteItemFromObject(object_obj, "key2");
            }
            
            cJSON_Delete(object_obj);
            break;
        }
        
        case 4: {
            // Scenario 4: Array insertion and replacement
            array_obj = cJSON_CreateArray();
            if (!array_obj) break;
            
            // Add some initial items
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 8; i++) {
                item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (item) cJSON_AddItemToArray(array_obj, item);
            }
            
            if (cJSON_GetArraySize(array_obj) > 0) {
                // Test InsertItemInArray
                int insert_idx = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj));
                std::string insert_str = fdp.ConsumeRandomLengthString(20);
                auto insert_str_ptr = std::make_unique<std::string>(insert_str);
                const char* insert_cstr = insert_str_ptr->c_str();
                std::vector<std::unique_ptr<std::string>> temp_storage;
                temp_storage.push_back(std::move(insert_str_ptr));
                
                item = cJSON_CreateString(insert_cstr);
                if (item) cJSON_InsertItemInArray(array_obj, insert_idx, item);
                
                // Test ReplaceItemInArray
                if (cJSON_GetArraySize(array_obj) > 0) {
                    int replace_idx = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array_obj) - 1);
                    cJSON* new_item = cJSON_CreateBool(fdp.ConsumeBool());
                    if (new_item) cJSON_ReplaceItemInArray(array_obj, replace_idx, new_item);
                }
            }
            
            cJSON_Delete(array_obj);
            break;
        }
        
        case 5: {
            // Scenario 5: Object replacement operations
            object_obj = cJSON_CreateObject();
            if (!object_obj) break;
            
            // Add initial items
            cJSON* str_item = cJSON_CreateString("original");
            if (str_item) cJSON_AddItemToObject(object_obj, "string_key", str_item);
            
            cJSON* num_item = cJSON_CreateNumber(42.0);
            if (num_item) cJSON_AddItemToObject(object_obj, "number_key", num_item);
            
            // Test ReplaceItemViaPointer
            if (str_item) {
                std::string new_str = fdp.ConsumeRandomLengthString(30);
                auto new_str_ptr = std::make_unique<std::string>(new_str);
                const char* new_cstr = new_str_ptr->c_str();
                std::vector<std::unique_ptr<std::string>> temp_storage;
                temp_storage.push_back(std::move(new_str_ptr));
                
                cJSON* new_str_item = cJSON_CreateString(new_cstr);
                if (new_str_item) cJSON_ReplaceItemViaPointer(object_obj, str_item, new_str_item);
            }
            
            // Test ReplaceItemInObject
            cJSON* new_num = cJSON_CreateNumber(99.0);
            if (new_num) cJSON_ReplaceItemInObject(object_obj, "number_key", new_num);
            
            cJSON_Delete(object_obj);
            break;
        }
        
        case 6: {
            // Scenario 6: DetachItemViaPointer
            array_obj = cJSON_CreateArray();
            if (!array_obj) break;
            
            // Add items
            cJSON* target_item = nullptr;
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 8; i++) {
                item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (item) {
                    cJSON_AddItemToArray(array_obj, item);
                    if (i == 2) target_item = item; // Mark middle item
                }
            }
            
            if (target_item) {
                detached_item = cJSON_DetachItemViaPointer(array_obj, target_item);
                if (detached_item) {
                    cJSON_Delete(detached_item);
                }
            }
            
            cJSON_Delete(array_obj);
            break;
        }
        
        case 7: {
            // Scenario 7: Mixed array and object operations
            root = cJSON_CreateObject();
            if (!root) break;
            
            // Create an array inside object
            array_obj = cJSON_CreateArray();
            if (array_obj) {
                cJSON_AddItemToObject(root, "nested_array", array_obj);
                
                // Add items to nested array (using numeric values)
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 8; i++) {
                    item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                    if (item) cJSON_AddItemToArray(array_obj, item);
                }
                
                // Manipulate nested array
                int nested_size = cJSON_GetArraySize(array_obj);
                if (nested_size > 0) {
                    // Get and use an item from nested array
                    item = cJSON_GetArrayItem(array_obj, 0);
                    if (item && cJSON_IsObject(item)) {
                        // Add something to nested object
                        cJSON* nested_obj_item = cJSON_CreateNumber(3.14);
                        if (nested_obj_item) {
                            cJSON_AddItemToObject(item, "pi", nested_obj_item);
                        }
                    }
                }
            }
            
            cJSON_Delete(root);
            break;
        }
        case 8: {
            // Scenario 8: Complex reference manipulation
            // Create items that will be shared via references
            cJSON* shared_num = cJSON_CreateNumber(100.0);
            cJSON* shared_str = cJSON_CreateString("shared_string");
            
            if (shared_num && shared_str) {
                // Create array and add references
                array_obj = cJSON_CreateArray();
                if (array_obj) {
                    cJSON_AddItemReferenceToArray(array_obj, shared_num);
                    cJSON_AddItemReferenceToArray(array_obj, shared_str);
                    cJSON_AddItemReferenceToArray(array_obj, shared_num); // Duplicate reference
                }
                
                // Create object and add references
                object_obj = cJSON_CreateObject();
                if (object_obj) {
                    cJSON_AddItemReferenceToObject(object_obj, "ref_num", shared_num);
                    cJSON_AddItemReferenceToObject(object_obj, "ref_str", shared_str);
                }
                
                // Delete containers first (they contain references to shared items)
                // References don't own the shared items, so shared items remain alive
                if (array_obj) cJSON_Delete(array_obj);
                if (object_obj) cJSON_Delete(object_obj);
                
                // Now safe to delete the shared items since no references remain
                cJSON_Delete(shared_num);
                cJSON_Delete(shared_str);
            } else {
                if (shared_num) cJSON_Delete(shared_num);
                if (shared_str) cJSON_Delete(shared_str);
            }
            break;
        }
        
        case 9: {
            // Scenario 9: Edge cases with null/invalid pointers
            // Test functions with null parameters
            array_obj = cJSON_CreateArray();
            
            // These should handle null pointers gracefully
            (void)cJSON_GetArraySize(nullptr);
            (void)cJSON_GetArrayItem(nullptr, 0);
            (void)cJSON_GetArrayItem(array_obj, -1); // Invalid index
            (void)cJSON_GetArrayItem(array_obj, 999); // Out of bounds
            
            // Object functions with null
            (void)cJSON_GetObjectItem(nullptr, "key");
            (void)cJSON_GetObjectItemCaseSensitive(nullptr, "key");
            (void)cJSON_HasObjectItem(nullptr, "key");
            
            // Addition functions with null
            cJSON_AddItemToArray(nullptr, nullptr);
            cJSON_AddItemToObject(nullptr, "key", nullptr);
            cJSON_AddItemToObjectCS(nullptr, "key", nullptr);
            cJSON_AddItemReferenceToArray(nullptr, nullptr);
            cJSON_AddItemReferenceToObject(nullptr, "key", nullptr);
            
            // Detach/delete functions with null
            (void)cJSON_DetachItemFromArray(nullptr, 0);
            cJSON_DeleteItemFromArray(nullptr, 0);
            (void)cJSON_DetachItemFromObject(nullptr, "key");
            cJSON_DeleteItemFromObject(nullptr, "key");
            
            if (array_obj) cJSON_Delete(array_obj);
            break;
        }
        
        case 10: {
            // Scenario 10: Complex nested manipulation with error recovery
            root = cJSON_CreateObject();
            if (!root) break;
            
            // Create complex structure
            array_obj = cJSON_CreateArray();
            if (array_obj) {
                cJSON_AddItemToObject(root, "data", array_obj);
                
                // Fill array with mixed types (avoid string storage issues)
                for (int i = 0; i < 5 && fdp.remaining_bytes() > 8; i++) {
                    uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    switch (item_type) {
                        case 0:
                            item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 1:
                            // Use numeric values instead of strings to avoid storage issues
                            item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 2:
                            item = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                        case 3:
                            item = cJSON_CreateObject();
                            if (item && fdp.remaining_bytes() > 8) {
                                cJSON* nested = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                if (nested) cJSON_AddItemToObject(item, "nested_val", nested);
                            }
                            break;
                        default:
                            item = cJSON_CreateNull();
                    }
                    
                    if (item) {
                        // Sometimes add as reference
                        if (fdp.ConsumeBool() && i > 0) {
                            cJSON_AddItemReferenceToArray(array_obj, item);
                            cJSON_Delete(item);
                        } else {
                            cJSON_AddItemToArray(array_obj, item);
                        }
                    }
                }
                
                // Perform various operations
                int arr_size = cJSON_GetArraySize(array_obj);
                if (arr_size > 2) {
                    // Insert at middle (use numeric value)
                    item = cJSON_CreateNumber(123);
                    if (item) cJSON_InsertItemInArray(array_obj, arr_size / 2, item);
                    
                    // Replace first item
                    item = cJSON_CreateNumber(999);
                    if (item) cJSON_ReplaceItemInArray(array_obj, 0, item);
                    
                    // Detach last item
                    if (arr_size > 0) {
                        detached_item = cJSON_DetachItemFromArray(array_obj, arr_size - 1);
                        if (detached_item) cJSON_Delete(detached_item);
                    }
                }
                
                // Get object item from root and manipulate
                cJSON* data_item = cJSON_GetObjectItem(root, "data");
                if (data_item && cJSON_IsArray(data_item)) {
                    // Access nested array through pointer
                    (void)cJSON_GetArraySize(data_item);
                }
            }
            
            cJSON_Delete(root);
            break;
        }
        case 11: {
            // Scenario 11: Case sensitivity testing for object operations
            object_obj = cJSON_CreateObject();
            if (!object_obj) break;
            
            // Add items with case-variant keys using string literals
            cJSON* item1 = cJSON_CreateNumber(1);
            cJSON* item2 = cJSON_CreateNumber(2);
            cJSON* item3 = cJSON_CreateNumber(3);
            
            if (item1) cJSON_AddItemToObject(object_obj, "KEY", item1); // Case-insensitive
            if (item2) cJSON_AddItemToObjectCS(object_obj, "key", item2); // Case-sensitive (different)
            if (item3) cJSON_AddItemToObject(object_obj, "Key", item3); // Case-insensitive (different case)
            
            // Test retrieval with different case
            cJSON* found1 = cJSON_GetObjectItem(object_obj, "KEY");
            cJSON* found2 = cJSON_GetObjectItem(object_obj, "key");
            cJSON* found3 = cJSON_GetObjectItemCaseSensitive(object_obj, "key");
            cJSON* found4 = cJSON_GetObjectItemCaseSensitive(object_obj, "KEY");
            
            // Test HasObjectItem with different case
            (void)cJSON_HasObjectItem(object_obj, "key");
            (void)cJSON_HasObjectItem(object_obj, "KEY");
            (void)cJSON_HasObjectItem(object_obj, "Key");
            (void)cJSON_HasObjectItem(object_obj, "kEy");
            
            cJSON_Delete(object_obj);
            break;
        }
        
        default:
            break;
    }
    
    return 0;
}

} // namespace harness_015
