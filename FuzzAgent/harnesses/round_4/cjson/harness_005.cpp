#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_005 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing of type checking and detachment operations
    // Need enough data for: operation type, JSON data, various parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Determine the main operation to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Create a JSON structure to work with
    cJSON* root = NULL;
    
    // Determine the type of JSON structure to create based on fuzzer input
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (structure_type) {
        case 0: {
            // Create a null item
            root = cJSON_CreateNull();
            break;
        }
        case 1: {
            // Create a boolean item
            bool bool_val = fdp.ConsumeBool();
            root = cJSON_CreateBool(bool_val ? 1 : 0);
            break;
        }
        case 2: {
            // Create a number item
            double num_val = fdp.ConsumeFloatingPoint<double>();
            root = cJSON_CreateNumber(num_val);
            break;
        }
        case 3: {
            // Create a string item
            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() < str_len) {
                return 0;
            }
            std::string str_val = fdp.ConsumeBytesAsString(str_len);
            root = cJSON_CreateString(str_val.c_str());
            break;
        }
        case 4: {
            // Create a raw JSON item
            size_t raw_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() < raw_len) {
                return 0;
            }
            std::string raw_val = fdp.ConsumeBytesAsString(raw_len);
            root = cJSON_CreateRaw(raw_val.c_str());
            break;
        }
        case 5: {
            // Create a complex structure (array or object)
            bool create_array = fdp.ConsumeBool();
            if (create_array) {
                root = cJSON_CreateArray();
                if (root != NULL && fdp.remaining_bytes() > 0) {
                    // Add some items to the array
                    int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
                    for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                        uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        cJSON* item = NULL;
                        
                        switch (item_type) {
                            case 0:
                                item = cJSON_CreateNull();
                                break;
                            case 1:
                                item = cJSON_CreateBool(fdp.ConsumeBool());
                                break;
                            case 2:
                                item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                break;
                            case 3: {
                                size_t item_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                if (fdp.remaining_bytes() < item_str_len) break;
                                std::string item_str = fdp.ConsumeBytesAsString(item_str_len);
                                item = cJSON_CreateString(item_str.c_str());
                                break;
                            }
                        }
                        
                        if (item != NULL) {
                            cJSON_AddItemToArray(root, item);
                        }
                    }
                }
            } else {
                root = cJSON_CreateObject();
                if (root != NULL && fdp.remaining_bytes() > 0) {
                    // Add some items to the object
                    int obj_size = fdp.ConsumeIntegralInRange<int>(1, 5);
                    for (int i = 0; i < obj_size && fdp.remaining_bytes() > 0; i++) {
                        // Generate key
                        size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                        if (fdp.remaining_bytes() < key_len) break;
                        std::string key = fdp.ConsumeBytesAsString(key_len);
                        
                        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        
                        switch (value_type) {
                            case 0:
                                cJSON_AddNullToObject(root, key.c_str());
                                break;
                            case 1:
                                cJSON_AddBoolToObject(root, key.c_str(), fdp.ConsumeBool());
                                break;
                            case 2:
                                cJSON_AddNumberToObject(root, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                                break;
                            case 3: {
                                size_t val_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                if (fdp.remaining_bytes() < val_str_len) break;
                                std::string val_str = fdp.ConsumeBytesAsString(val_str_len);
                                cJSON_AddStringToObject(root, key.c_str(), val_str.c_str());
                                break;
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    
    if (root == NULL) {
        return 0;
    }
    
    // Phase 1: Comprehensive type checking
    if (operation == 0) {
        // Test all type checking functions on the root item
        cJSON_bool is_invalid = cJSON_IsInvalid(root);
        cJSON_bool is_false = cJSON_IsFalse(root);
        cJSON_bool is_true = cJSON_IsTrue(root);
        cJSON_bool is_bool = cJSON_IsBool(root);
        cJSON_bool is_null = cJSON_IsNull(root);
        cJSON_bool is_number = cJSON_IsNumber(root);
        cJSON_bool is_string = cJSON_IsString(root);
        cJSON_bool is_array = cJSON_IsArray(root);
        cJSON_bool is_object = cJSON_IsObject(root);
        cJSON_bool is_raw = cJSON_IsRaw(root);
        
        // Test value extraction functions based on type
        if (is_string) {
            const char* str_value = cJSON_GetStringValue(root);
            // Note: cJSON_GetStringLength doesn't exist in the header, skipping
        }
        
        if (is_number) {
            double num_value = cJSON_GetNumberValue(root);
        }
        
        // If root is an array or object, test type checking on child items
        if (is_array || is_object) {
            int child_count = cJSON_GetArraySize(root);
            if (child_count > 0 && fdp.remaining_bytes() > 0) {
                // Test a few child items
                int test_count = fdp.ConsumeIntegralInRange<int>(0, child_count);
                for (int i = 0; i < test_count && i < child_count; i++) {
                    cJSON* child = cJSON_GetArrayItem(root, i);
                    if (child != NULL) {
                        // Test type checking on child
                        cJSON_bool child_is_invalid = cJSON_IsInvalid(child);
                        cJSON_bool child_is_number = cJSON_IsNumber(child);
                        cJSON_bool child_is_string = cJSON_IsString(child);
                        
                        if (child_is_string) {
                            const char* child_str = cJSON_GetStringValue(child);
                        }
                        if (child_is_number) {
                            double child_num = cJSON_GetNumberValue(child);
                        }
                    }
                }
            }
        }
        
        // Test cJSON_HasObjectItem if root is an object
        if (is_object && fdp.remaining_bytes() > 0) {
            size_t test_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            if (fdp.remaining_bytes() >= test_key_len) {
                std::string test_key = fdp.ConsumeBytesAsString(test_key_len);
                cJSON_bool has_item = cJSON_HasObjectItem(root, test_key.c_str());
            }
        }
        
        // Test cJSON_GetErrorPtr (always available)
        const char* error_ptr = cJSON_GetErrorPtr();
    }
    
    // Phase 2: Detachment and reference operations
    else if (operation == 1) {
        // Create a parent structure for detachment operations
        cJSON* parent = NULL;
        bool parent_is_array = fdp.ConsumeBool();
        
        if (parent_is_array) {
            parent = cJSON_CreateArray();
            if (parent != NULL) {
                // Add the root item to the array
                cJSON_AddItemToArray(parent, root);
                
                // Also add some other items
                int extra_items = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; i < extra_items && fdp.remaining_bytes() > 0; i++) {
                    uint8_t extra_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                    cJSON* extra = NULL;
                    
                    switch (extra_type) {
                        case 0:
                            extra = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 1:
                            extra = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                        case 2: {
                            size_t extra_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                            if (fdp.remaining_bytes() < extra_str_len) break;
                            std::string extra_str = fdp.ConsumeBytesAsString(extra_str_len);
                            extra = cJSON_CreateString(extra_str.c_str());
                            break;
                        }
                    }
                    
                    if (extra != NULL) {
                        cJSON_AddItemToArray(parent, extra);
                    }
                }
                
                // Test detachment from array
                int array_size = cJSON_GetArraySize(parent);
                if (array_size > 0 && fdp.remaining_bytes() > 0) {
                    int detach_index = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
                    cJSON* detached = cJSON_DetachItemFromArray(parent, detach_index);
                    if (detached != NULL) {
                        // Test type checking on detached item
                        cJSON_IsNumber(detached);
                        cJSON_IsString(detached);
                        cJSON_IsBool(detached);
                        
                        // Clean up detached item
                        cJSON_Delete(detached);
                    }
                }
                
                // Test cJSON_AddItemReferenceToArray
                if (fdp.remaining_bytes() > 0) {
                    cJSON* ref_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                    if (ref_item != NULL) {
                        cJSON_AddItemReferenceToArray(parent, ref_item);
                        // Note: Don't delete ref_item since it's now referenced by parent
                    }
                }
            }
        } else {
            parent = cJSON_CreateObject();
            if (parent != NULL) {
                // Add the root item to the object
                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= key_len) {
                    std::string key = fdp.ConsumeBytesAsString(key_len);
                    cJSON_AddItemToObject(parent, key.c_str(), root);
                    
                    // Also add some other items
                    int extra_items = fdp.ConsumeIntegralInRange<int>(0, 3);
                    for (int i = 0; i < extra_items && fdp.remaining_bytes() > 0; i++) {
                        size_t extra_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                        if (fdp.remaining_bytes() < extra_key_len) break;
                        std::string extra_key = fdp.ConsumeBytesAsString(extra_key_len);
                        
                        uint8_t extra_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                        
                        switch (extra_type) {
                            case 0:
                                cJSON_AddNumberToObject(parent, extra_key.c_str(), 
                                                        fdp.ConsumeFloatingPoint<double>());
                                break;
                            case 1:
                                cJSON_AddBoolToObject(parent, extra_key.c_str(), fdp.ConsumeBool());
                                break;
                            case 2: {
                                size_t extra_val_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                                if (fdp.remaining_bytes() < extra_val_len) break;
                                std::string extra_val = fdp.ConsumeBytesAsString(extra_val_len);
                                cJSON_AddStringToObject(parent, extra_key.c_str(), extra_val.c_str());
                                break;
                            }
                        }
                    }
                    
                    // Test detachment from object
                    if (fdp.remaining_bytes() > 0) {
                        size_t detach_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                        if (fdp.remaining_bytes() >= detach_key_len) {
                            std::string detach_key = fdp.ConsumeBytesAsString(detach_key_len);
                            cJSON* detached = cJSON_DetachItemFromObject(parent, detach_key.c_str());
                            if (detached != NULL) {
                                // Test type checking on detached item
                                cJSON_IsNumber(detached);
                                cJSON_IsString(detached);
                                cJSON_IsBool(detached);
                                
                                // Clean up detached item
                                cJSON_Delete(detached);
                            }
                        }
                    }
                    
                    // Test cJSON_AddItemReferenceToObject
                    if (fdp.remaining_bytes() > 0) {
                        size_t ref_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                        if (fdp.remaining_bytes() >= ref_key_len) {
                            std::string ref_key = fdp.ConsumeBytesAsString(ref_key_len);
                            cJSON* ref_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            if (ref_item != NULL) {
                                cJSON_AddItemReferenceToObject(parent, ref_key.c_str(), ref_item);
                                // Note: Don't delete ref_item since it's now referenced by parent
                            }
                        }
                    }
                }
            }
        }
        
        // Test cJSON_DetachItemViaPointer if we have a parent with items
        if (parent != NULL) {
            int parent_size = cJSON_GetArraySize(parent);
            if (parent_size > 0) {
                // Get first item
                cJSON* first_item = cJSON_GetArrayItem(parent, 0);
                if (first_item != NULL) {
                    cJSON* detached_via_pointer = cJSON_DetachItemViaPointer(parent, first_item);
                    if (detached_via_pointer != NULL) {
                        // Test type checking
                        cJSON_IsInvalid(detached_via_pointer);
                        cJSON_IsNumber(detached_via_pointer);
                        cJSON_IsString(detached_via_pointer);
                        
                        // Test cJSON_ReplaceItemViaPointer
                        cJSON* replacement = cJSON_CreateBool(fdp.ConsumeBool());
                        if (replacement != NULL) {
                            cJSON_ReplaceItemViaPointer(parent, detached_via_pointer, replacement);
                            // replacement is now owned by parent
                            // Note: cJSON_ReplaceItemViaPointer frees detached_via_pointer,
                            // so we should NOT call cJSON_Delete(detached_via_pointer) here
                        }
                        
                        // Clean up - NOT needed for detached_via_pointer since it was freed by ReplaceItemViaPointer
                    }
                }
            }
            // Clean up parent
            cJSON_Delete(parent);
        } else {
            // Clean up root if parent creation failed
            cJSON_Delete(root);
        }
        
        return 0; // Early return since root is now part of parent or deleted
    }
    
    // Phase 3: Mixed operations
    else if (operation == 2) {
        // Test a combination of type checking and detachment
        
        // First do type checking
        cJSON_bool is_array = cJSON_IsArray(root);
        cJSON_bool is_object = cJSON_IsObject(root);
        cJSON_bool is_string = cJSON_IsString(root);
        cJSON_bool is_number = cJSON_IsNumber(root);
        
        // Test value extraction
        if (is_string) {
            const char* str_val = cJSON_GetStringValue(root);
        }
        if (is_number) {
            double num_val = cJSON_GetNumberValue(root);
        }
        
        // Test cJSON_GetErrorPtr
        const char* error_ptr = cJSON_GetErrorPtr();
        
        // If it's an array or object, test detachment operations
        if ((is_array || is_object) && fdp.remaining_bytes() > 0) {
            // Create a copy for detachment tests
            cJSON* copy = cJSON_Duplicate(root, 1);
            if (copy != NULL) {
                int copy_size = cJSON_GetArraySize(copy);
                if (copy_size > 0) {
                    // Test detachment
                    int detach_idx = fdp.ConsumeIntegralInRange<int>(0, copy_size - 1);
                    cJSON* detached = cJSON_DetachItemFromArray(copy, detach_idx);
                    if (detached != NULL) {
                        // Test type checking on detached item
                        cJSON_IsInvalid(detached);
                        cJSON_IsFalse(detached);
                        cJSON_IsTrue(detached);
                        cJSON_IsBool(detached);
                        cJSON_IsNull(detached);
                        cJSON_IsNumber(detached);
                        cJSON_IsString(detached);
                        cJSON_IsArray(detached);
                        cJSON_IsObject(detached);
                        cJSON_IsRaw(detached);
                        
                        // Test value extraction if applicable
                        if (cJSON_IsString(detached)) {
                            const char* det_str = cJSON_GetStringValue(detached);
                        }
                        if (cJSON_IsNumber(detached)) {
                            double det_num = cJSON_GetNumberValue(detached);
                        }
                        
                        cJSON_Delete(detached);
                    }
                }
                cJSON_Delete(copy);
            }
        }
    }
    
    // Clean up root if it hasn't been deleted already
    if (root != NULL) {
        cJSON_Delete(root);
    }
    
    return 0;
}

} // namespace harness_005
