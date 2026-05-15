#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_006 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create base JSON structures using fuzzer input
    // Determine structure type from fuzzer input
    int structure_type = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    cJSON* base_structure = NULL;
    
    switch (structure_type) {
        case 0:  // Create an object with various items
            base_structure = cJSON_CreateObject();
            if (base_structure != NULL) {
                // Add some items to the object
                int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
                    std::string key = fdp.ConsumeRandomLengthString(16);
                    int item_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                    
                    switch (item_type) {
                        case 0:  // String
                            {
                                std::string value = fdp.ConsumeRandomLengthString(32);
                                cJSON_AddStringToObject(base_structure, key.c_str(), value.c_str());
                            }
                            break;
                        case 1:  // Number
                            {
                                double value = fdp.ConsumeFloatingPoint<double>();
                                cJSON_AddNumberToObject(base_structure, key.c_str(), value);
                            }
                            break;
                        case 2:  // Boolean
                            {
                                bool value = fdp.ConsumeBool();
                                cJSON_AddBoolToObject(base_structure, key.c_str(), value);
                            }
                            break;
                        case 3:  // Array
                            {
                                cJSON* array = cJSON_CreateArray();
                                if (array != NULL) {
                                    int array_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                                    for (int j = 0; j < array_size && fdp.remaining_bytes() > 0; j++) {
                                        double num = fdp.ConsumeFloatingPoint<double>();
                                        cJSON_AddItemToArray(array, cJSON_CreateNumber(num));
                                    }
                                    cJSON_AddItemToObject(base_structure, key.c_str(), array);
                                }
                            }
                            break;
                    }
                }
            }
            break;
            
        case 1:  // Create an array with various items
            base_structure = cJSON_CreateArray();
            if (base_structure != NULL) {
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 10);
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    int item_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                    
                    switch (item_type) {
                        case 0:  // String
                            {
                                std::string value = fdp.ConsumeRandomLengthString(32);
                                cJSON_AddItemToArray(base_structure, cJSON_CreateString(value.c_str()));
                            }
                            break;
                        case 1:  // Number
                            {
                                double value = fdp.ConsumeFloatingPoint<double>();
                                cJSON_AddItemToArray(base_structure, cJSON_CreateNumber(value));
                            }
                            break;
                        case 2:  // Boolean
                            {
                                bool value = fdp.ConsumeBool();
                                cJSON_AddItemToArray(base_structure, cJSON_CreateBool(value));
                            }
                            break;
                        case 3:  // Object
                            {
                                cJSON* obj = cJSON_CreateObject();
                                if (obj != NULL) {
                                    std::string key = fdp.ConsumeRandomLengthString(8);
                                    std::string value = fdp.ConsumeRandomLengthString(16);
                                    cJSON_AddStringToObject(obj, key.c_str(), value.c_str());
                                    cJSON_AddItemToArray(base_structure, obj);
                                }
                            }
                            break;
                    }
                }
            }
            break;
            
        case 2:  // Create a string for string reference testing
            {
                std::string string_value = fdp.ConsumeRandomLengthString(64);
                base_structure = cJSON_CreateString(string_value.c_str());
            }
            break;
    }
    
    if (base_structure == NULL) {
        return 0;
    }
    
    // Step 2: Test cJSON_Duplicate with both shallow and deep copying
    bool deep_copy = fdp.ConsumeBool();
    cJSON* duplicate = cJSON_Duplicate(base_structure, deep_copy ? 1 : 0);
    
    // Step 3: Test reference creation APIs
    // Test cJSON_CreateStringReference
    std::string ref_string = fdp.ConsumeRandomLengthString(32);
    cJSON* string_ref = cJSON_CreateStringReference(ref_string.c_str());
    
    // Test cJSON_CreateObjectReference and cJSON_CreateArrayReference
    cJSON* object_ref = NULL;
    cJSON* array_ref = NULL;
    
    if (cJSON_IsObject(base_structure)) {
        object_ref = cJSON_CreateObjectReference(base_structure);
    }
    
    if (cJSON_IsArray(base_structure)) {
        array_ref = cJSON_CreateArrayReference(base_structure);
    }
    
    // Step 4: Create a container for testing reference addition APIs
    cJSON* container = cJSON_CreateObject();
    if (container != NULL) {
        // Add references to container using reference APIs
        if (string_ref != NULL) {
            std::string string_key = fdp.ConsumeRandomLengthString(16);
            cJSON_AddItemReferenceToObject(container, string_key.c_str(), string_ref);
        }
        
        if (object_ref != NULL) {
            std::string object_key = fdp.ConsumeRandomLengthString(16);
            cJSON_AddItemReferenceToObject(container, object_key.c_str(), object_ref);
        }
        
        // Also test adding references to arrays
        cJSON* ref_array = cJSON_CreateArray();
        if (ref_array != NULL) {
            if (string_ref != NULL) {
                cJSON_AddItemReferenceToArray(ref_array, string_ref);
            }
            if (object_ref != NULL) {
                cJSON_AddItemReferenceToArray(ref_array, object_ref);
            }
            if (array_ref != NULL) {
                cJSON_AddItemReferenceToArray(ref_array, array_ref);
            }
            
            // Add the ref_array to container
            cJSON_AddItemToObject(container, "ref_array", ref_array);
        }
    }
    
    // Step 5: Test detachment APIs
    if (cJSON_IsObject(base_structure) && container != NULL) {
        // Get an item from container to detach
        cJSON* item_to_detach = NULL;
        if (cJSON_IsObject(container) && container->child != NULL) {
            item_to_detach = container->child;
            
            // Test cJSON_DetachItemViaPointer
            if (item_to_detach != NULL) {
                cJSON* detached_via_pointer = cJSON_DetachItemViaPointer(container, item_to_detach);
                if (detached_via_pointer != NULL) {
                    cJSON_Delete(detached_via_pointer);
                }
            }
        }
        
        // Test cJSON_DetachItemFromObject and cJSON_DetachItemFromObjectCaseSensitive
        if (cJSON_IsObject(container) && container->child != NULL) {
            cJSON* first_child = container->child;
            if (first_child->string != NULL) {
                std::string key(first_child->string);
                
                // Test case-sensitive detach
                cJSON* detached_case_sensitive = cJSON_DetachItemFromObjectCaseSensitive(container, key.c_str());
                if (detached_case_sensitive != NULL) {
                    cJSON_Delete(detached_case_sensitive);
                }
                
                // Also test regular detach
                // Re-add an item first
                if (string_ref != NULL) {
                    std::string detach_key = fdp.ConsumeRandomLengthString(16);
                    cJSON_AddItemReferenceToObject(container, detach_key.c_str(), string_ref);
                    
                    cJSON* detached = cJSON_DetachItemFromObject(container, detach_key.c_str());
                    if (detached != NULL) {
                        cJSON_Delete(detached);
                    }
                }
            }
        }
    }
    
    if (cJSON_IsArray(base_structure)) {
        // Test cJSON_DetachItemFromArray
        int array_size = cJSON_GetArraySize(base_structure);
        if (array_size > 0) {
            int index_to_detach = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON* detached_from_array = cJSON_DetachItemFromArray(base_structure, index_to_detach);
            if (detached_from_array != NULL) {
                cJSON_Delete(detached_from_array);
            }
        }
    }
    
    // Step 6: Clean up all allocated memory
    // Clean up references first (they don't own the referenced data)
    if (string_ref != NULL) {
        cJSON_Delete(string_ref);
    }
    if (object_ref != NULL) {
        cJSON_Delete(object_ref);
    }
    if (array_ref != NULL) {
        cJSON_Delete(array_ref);
    }
    
    // Clean up duplicates
    if (duplicate != NULL) {
        cJSON_Delete(duplicate);
    }
    
    // Clean up container
    if (container != NULL) {
        cJSON_Delete(container);
    }
    
    // Clean up base structure last
    cJSON_Delete(base_structure);
    
    return 0;
}

} // namespace harness_006
