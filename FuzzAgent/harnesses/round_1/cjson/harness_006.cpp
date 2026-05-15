#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_006 {

extern "C" {
}


// Custom memory hook functions for tracking allocations
static size_t total_allocations = 0;
static size_t total_frees = 0;

void* custom_malloc(size_t size) {
    total_allocations++;
    return malloc(size);
}

void custom_free(void* ptr) {
    total_frees++;
    free(ptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing: need bytes for various parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Reset allocation counters
    total_allocations = 0;
    total_frees = 0;
    
    // 1. Define custom memory hooks and initialize them
    cJSON_Hooks hooks;
    hooks.malloc_fn = custom_malloc;
    hooks.free_fn = custom_free;
    cJSON_InitHooks(&hooks);
    
    // 2. Create root object and array using custom hooks
    cJSON* root_object = cJSON_CreateObject();
    if (root_object == nullptr) {
        return 0;
    }
    
    cJSON* root_array = cJSON_CreateArray();
    if (root_array == nullptr) {
        cJSON_Delete(root_object);
        return 0;
    }
    
    // 3. Create string items for testing
    std::string string1 = fdp.ConsumeRandomLengthString(64);
    std::string string2 = fdp.ConsumeRandomLengthString(64);
    
    cJSON* string_item1 = cJSON_CreateString(string1.c_str());
    cJSON* string_item2 = cJSON_CreateString(string2.c_str());
    
    // 4. Create references using reference functions
    // Create object reference
    cJSON* object_reference = cJSON_CreateObjectReference(root_object);
    
    // Create string reference - uses a fuzzed string that won't be freed by cJSON_Delete
    std::string external_string = fdp.ConsumeRandomLengthString(64);
    cJSON* string_reference = cJSON_CreateStringReference(external_string.c_str());
    
    // Create array reference
    cJSON* array_reference = cJSON_CreateArrayReference(root_array);
    
    // 5. Add items and references to arrays
    if (string_item1 != nullptr) {
        cJSON_AddItemToArray(root_array, string_item1);
    }
    
    if (string_item2 != nullptr) {
        cJSON_AddItemToArray(root_array, string_item2);
    }
    
    // Add references to the array
    if (object_reference != nullptr) {
        cJSON_AddItemToArray(root_array, object_reference);
    }
    
    if (string_reference != nullptr) {
        cJSON_AddItemToArray(root_array, string_reference);
    }
    
    if (array_reference != nullptr) {
        cJSON_AddItemToArray(root_array, array_reference);
    }
    
    // 6. Add items to objects
    std::string key1 = fdp.ConsumeRandomLengthString(32);
    std::string key2 = fdp.ConsumeRandomLengthString(32);
    
    if (string_item1 != nullptr) {
        cJSON* string_item1_copy = cJSON_Duplicate(string_item1, 1); // Recursive copy
        if (string_item1_copy != nullptr) {
            cJSON_AddItemToObject(root_object, key1.c_str(), string_item1_copy);
        }
    }
    
    // Create a number item for SetNumberHelper test
    cJSON* number_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
    if (number_item != nullptr) {
        cJSON_AddItemToObject(root_object, key2.c_str(), number_item);
    }
    
    // 7. Test duplication with custom hooks
    // Choose whether to duplicate recursively based on fuzzed input
    bool duplicate_recursive = fdp.ConsumeBool();
    cJSON* duplicated = nullptr;
    
    if (duplicate_recursive) {
        duplicated = cJSON_Duplicate(root_object, 1); // Recursive duplication
    } else {
        duplicated = cJSON_Duplicate(root_array, 0); // Non-recursive duplication
    }
    
    // 8. Test cJSON_SetNumberHelper
    if (number_item != nullptr) {
        double new_number = fdp.ConsumeFloatingPoint<double>();
        cJSON_SetNumberHelper(number_item, new_number);
        
        // Also test on duplicated object if it exists
        if (duplicated != nullptr) {
            // Find a number item in the duplicated object
            if (cJSON_IsObject(duplicated)) {
                cJSON* child = duplicated->child;
                while (child != nullptr) {
                    if (cJSON_IsNumber(child)) {
                        cJSON_SetNumberHelper(child, fdp.ConsumeFloatingPoint<double>());
                        break;
                    }
                    child = child->next;
                }
            }
        }
    }
    
    // 9. Cleanup
    // Note: References (string_reference) don't free the referenced string
    // Regular items will be freed by cJSON_Delete
    if (duplicated != nullptr) {
        cJSON_Delete(duplicated);
    }
    
    cJSON_Delete(root_array);
    cJSON_Delete(root_object);
    
    // string_item2 was added to root_array, so it will be freed by cJSON_Delete(root_array)
    // The string reference uses external_string which we shouldn't free
    // Output allocation statistics for debugging (commented out in production)
    // std::cout << "Allocations: " << total_allocations 
    //           << ", Frees: " << total_frees 
    //           << ", Reallocs: " << total_reallocs << std::endl;
    
    return 0;
}

} // namespace harness_006
