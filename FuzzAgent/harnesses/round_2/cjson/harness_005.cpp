#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_005 {

extern "C" {
}

// Custom memory allocation tracking structure
struct AllocationTracker {
    size_t total_allocations;
    size_t total_deallocations;
    size_t total_allocated_bytes;
    size_t max_concurrent_allocations;
    size_t current_allocations;
    
    // For simulating allocation failures
    bool fail_next_allocation;
    int allocation_failure_countdown;
    
    AllocationTracker() : total_allocations(0), total_deallocations(0), 
                         total_allocated_bytes(0), max_concurrent_allocations(0),
                         current_allocations(0), fail_next_allocation(false),
                         allocation_failure_countdown(0) {}
};

// Global allocation tracker
static AllocationTracker g_tracker;

// Custom malloc function with tracking
void* CJSON_CDECL custom_malloc(size_t size) {
    // Check if we should simulate allocation failure
    if (g_tracker.fail_next_allocation) {
        g_tracker.fail_next_allocation = false;
        return NULL;
    }
    
    // Decrement failure countdown if active
    if (g_tracker.allocation_failure_countdown > 0) {
        g_tracker.allocation_failure_countdown--;
        if (g_tracker.allocation_failure_countdown == 0) {
            return NULL;
        }
    }
    
    void* ptr = malloc(size);
    if (ptr != NULL) {
        g_tracker.total_allocations++;
        g_tracker.current_allocations++;
        g_tracker.total_allocated_bytes += size;
        
        if (g_tracker.current_allocations > g_tracker.max_concurrent_allocations) {
            g_tracker.max_concurrent_allocations = g_tracker.current_allocations;
        }
    }
    return ptr;
}

// Custom free function with tracking
void CJSON_CDECL custom_free(void* ptr) {
    if (ptr != NULL) {
        g_tracker.total_deallocations++;
        g_tracker.current_allocations--;
    }
    free(ptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Reset allocation tracker
    g_tracker = AllocationTracker();
    
    // Configure allocation failure simulation from fuzzer input
    g_tracker.fail_next_allocation = fdp.ConsumeBool();
    if (fdp.ConsumeBool()) {
        g_tracker.allocation_failure_countdown = fdp.ConsumeIntegralInRange<int>(1, 10);
    }
    
    // Step 1: Set up custom memory hooks
    cJSON_Hooks custom_hooks;
    custom_hooks.malloc_fn = custom_malloc;
    custom_hooks.free_fn = custom_free;
    
    cJSON_InitHooks(&custom_hooks);
    
    // Step 2: Create complex JSON structures using custom allocators
    // Determine what type of structure to create from fuzzer input
    int structure_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    cJSON* root = NULL;
    
    switch (structure_type) {
        case 0: {
            // Create a complex object with nested structures
            root = cJSON_CreateObject();
            if (root == NULL) {
                return 0;  // Allocation failed, early exit
            }
            
            // Add various types of items to the object
            int num_items = fdp.ConsumeIntegralInRange<int>(1, 8);
            for (int i = 0; i < num_items; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                int item_type = fdp.ConsumeIntegralInRange<int>(0, 5);
                
                switch (item_type) {
                    case 0:
                        cJSON_AddNullToObject(root, key.c_str());
                        break;
                    case 1:
                        cJSON_AddTrueToObject(root, key.c_str());
                        break;
                    case 2:
                        cJSON_AddFalseToObject(root, key.c_str());
                        break;
                    case 3:
                        cJSON_AddNumberToObject(root, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 4: {
                        std::string str_val = fdp.ConsumeRandomLengthString(32);
                        cJSON_AddStringToObject(root, key.c_str(), str_val.c_str());
                        break;
                    }
                    case 5: {
                        // Add a nested array
                        cJSON* array = cJSON_CreateArray();
                        if (array != NULL) {
                            int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
                            for (int j = 0; j < array_size; j++) {
                                cJSON* array_item = NULL;
                                int array_item_type = fdp.ConsumeIntegralInRange<int>(0, 2);
                                switch (array_item_type) {
                                    case 0:
                                        array_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                        break;
                                    case 1: {
                                        std::string array_str = fdp.ConsumeRandomLengthString(20);
                                        array_item = cJSON_CreateString(array_str.c_str());
                                        break;
                                    }
                                    case 2:
                                        array_item = cJSON_CreateBool(fdp.ConsumeBool());
                                        break;
                                }
                                if (array_item != NULL) {
                                    cJSON_AddItemToArray(array, array_item);
                                }
                            }
                            cJSON_AddItemToObject(root, key.c_str(), array);
                        }
                        break;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Create a complex array
            root = cJSON_CreateArray();
            if (root == NULL) {
                return 0;
            }
            
            int array_size = fdp.ConsumeIntegralInRange<int>(3, 10);
            for (int i = 0; i < array_size; i++) {
                cJSON* item = NULL;
                int item_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                
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
                        std::string str_val = fdp.ConsumeRandomLengthString(24);
                        item = cJSON_CreateString(str_val.c_str());
                        break;
                    }
                    case 4: {
                        // Nested object in array
                        cJSON* nested_obj = cJSON_CreateObject();
                        if (nested_obj != NULL) {
                            std::string nested_key = fdp.ConsumeRandomLengthString(12);
                            std::string nested_val = fdp.ConsumeRandomLengthString(16);
                            cJSON_AddStringToObject(nested_obj, nested_key.c_str(), nested_val.c_str());
                            item = nested_obj;
                        }
                        break;
                    }
                }
                
                if (item != NULL) {
                    cJSON_AddItemToArray(root, item);
                }
            }
            break;
        }
        
        case 2: {
            // Create JSON by parsing a string (which will use custom allocators)
            std::string json_string = fdp.ConsumeRandomLengthString(256);
            root = cJSON_Parse(json_string.c_str());
            if (root == NULL) {
                // Parsing failed, create a simple object instead
                root = cJSON_CreateObject();
                if (root != NULL) {
                    std::string key = fdp.ConsumeRandomLengthString(12);
                    std::string value = fdp.ConsumeRandomLengthString(20);
                    cJSON_AddStringToObject(root, key.c_str(), value.c_str());
                }
            }
            break;
        }
        
        case 3: {
            // Mixed approach: create object and modify it
            root = cJSON_CreateObject();
            if (root == NULL) {
                return 0;
            }
            
            // Add initial items
            for (int i = 0; i < 3; i++) {
                std::string key = fdp.ConsumeRandomLengthString(10);
                cJSON_AddNumberToObject(root, key.c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            // Replace some items
            if (cJSON_GetArraySize(root) > 0) {
                // Get first item and potentially replace it
                cJSON* first_child = root->child;
                if (first_child != NULL && first_child->string != NULL) {
                    // Replace with a different type
                    std::string new_str = fdp.ConsumeRandomLengthString(15);
                    cJSON_ReplaceItemInObject(root, first_child->string, cJSON_CreateString(new_str.c_str()));
                }
            }
            break;
        }
    }
    
    // Step 3: Optionally test printing with custom allocators
    if (root != NULL && fdp.ConsumeBool()) {
        char* printed_json = NULL;
        int print_option = fdp.ConsumeIntegralInRange<int>(0, 2);
        
        switch (print_option) {
            case 0:
                printed_json = cJSON_Print(root);
                break;
            case 1:
                printed_json = cJSON_PrintUnformatted(root);
                break;
            case 2: {
                int prebuffer = fdp.ConsumeIntegralInRange<int>(1, 512);
                printed_json = cJSON_PrintBuffered(root, prebuffer, fdp.ConsumeBool());
                break;
            }
        }
        
        // Free the printed JSON using custom free
        if (printed_json != NULL) {
            free(printed_json);
        }
    }
    
    // Step 4: Clean up all allocations with cJSON_Delete (which will use custom free)
    if (root != NULL) {
        cJSON_Delete(root);
    }
    
    // Step 5: Reset hooks to default (optional but good practice)
    cJSON_InitHooks(NULL);
    
    // The allocation tracker statistics are not used further but would be available
    // for debugging or to guide fuzzer toward interesting allocation patterns
    
    return 0;
}

} // namespace harness_005
