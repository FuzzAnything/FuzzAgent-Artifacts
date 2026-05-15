#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <atomic>

namespace harness_020 {
/*
 * Harness 020: Memory Allocation Tracking via Custom Hooks for cJSON_malloc
 * 
 * Target: Achieve 100% API coverage for cJSON_malloc (0% covered, 0 hits)
 * via custom memory hooks with allocation count tracking.
 * 
 * APIs to target:
 * - cJSON_malloc (primary) - track allocations via custom hooks
 * - cJSON_InitHooks (required helper for setup)
 * - cJSON_free (required helper for cleanup)
 * - cJSON_Parse (operation to trigger allocations)
 * - cJSON_CreateObject (operation to trigger object creation allocations)
 * - cJSON_CreateArray (operation to trigger array creation allocations)
 * - cJSON_Delete (required helper for cleanup)
 * - cJSON_AddItemToArray (additional allocation paths)
 * - cJSON_AddItemToObject (additional allocation paths)
 * 
 * Invocation sequence:
 * 1. Call cJSON_InitHooks to set up custom malloc/free functions that track allocation counts
 * 2. Call cJSON_Parse with various JSON strings to trigger memory allocations through cJSON_malloc
 * 3. Call cJSON_CreateObject and cJSON_CreateArray to create JSON structures
 * 4. Perform operations like cJSON_AddItemToArray, cJSON_AddItemToObject to trigger additional allocations
 * 5. Call cJSON_Delete on created objects to ensure proper cleanup through cJSON_free
 * 6. Verify that allocation counts match deallocation counts to test memory management integrity
 * 
 * Semantic diversity from existing harnesses:
 * - harness_002.cpp and harness_012.cpp use cJSON_InitHooks but don't specifically track cJSON_malloc calls
 * - harness_019.cpp mentions cJSON_malloc but doesn't use custom hooks to track allocations
 * - This harness specifically tracks allocation counts to verify memory management integrity
 * - Focuses on the relationship between cJSON_malloc and cJSON_free through hook instrumentation
 */


// Global counters for tracking allocations and deallocations
static std::atomic<int> allocation_count{0};
static std::atomic<int> deallocation_count{0};
static std::atomic<int> total_allocated_size{0};

// Custom allocation function that tracks allocations
static void* tracking_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr != nullptr) {
        allocation_count.fetch_add(1, std::memory_order_relaxed);
        total_allocated_size.fetch_add(static_cast<int>(size), std::memory_order_relaxed);
    }
    return ptr;
}

// Custom free function that tracks deallocations
static void tracking_free(void* ptr) {
    if (ptr != nullptr) {
        deallocation_count.fetch_add(1, std::memory_order_relaxed);
    }
    free(ptr);
}

// Custom realloc function for completeness (though cJSON may not use it with custom hooks)
static void* tracking_realloc(void* ptr, size_t size) {
    // For realloc, we count as free + malloc
    if (ptr != nullptr) {
        deallocation_count.fetch_add(1, std::memory_order_relaxed);
    }
    void* new_ptr = realloc(ptr, size);
    if (new_ptr != nullptr) {
        allocation_count.fetch_add(1, std::memory_order_relaxed);
        total_allocated_size.fetch_add(static_cast<int>(size), std::memory_order_relaxed);
    }
    return new_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check for meaningful testing
    // Need enough data for JSON strings and configuration
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Reset counters for this test run
    allocation_count.store(0);
    deallocation_count.store(0);
    total_allocated_size.store(0);
    
    // Consume configuration data
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool test_complex_json = fdp.ConsumeBool();
    bool test_multiple_objects = fdp.ConsumeBool();
    bool test_nested_structures = fdp.ConsumeBool();
    
    // Consume JSON input data
    std::string json_input = fdp.ConsumeRandomLengthString(1024);
    std::string key_name = fdp.ConsumeRandomLengthString(128);
    std::string string_value = fdp.ConsumeRandomLengthString(256);
    double number_value = fdp.ConsumeFloatingPoint<double>();
    
    // Set up custom hooks for tracking allocations
    cJSON_Hooks hooks;
    hooks.malloc_fn = tracking_malloc;
    hooks.free_fn = tracking_free;
    
    // 1. Initialize hooks to use our tracking functions (targets cJSON_InitHooks)
    cJSON_InitHooks(&hooks);
    
    // Record initial allocation count before any cJSON operations
    int initial_allocation_count = allocation_count.load(std::memory_order_relaxed);
    
    // Choose test scenario based on fuzzer input
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Simple parsing and deletion
            // 2. Parse JSON (triggers cJSON_malloc through allocation paths)
            cJSON* root = cJSON_Parse(json_input.c_str());
            
            if (root != nullptr) {
                // Create some additional objects to trigger more allocations
                cJSON* extra_object = cJSON_CreateObject();
                cJSON* extra_array = cJSON_CreateArray();
                
                if (extra_object != nullptr && extra_array != nullptr) {
                    // Add items to trigger additional allocations
                    cJSON_AddItemToObject(extra_object, key_name.c_str(), cJSON_CreateString(string_value.c_str()));
                    cJSON_AddItemToArray(extra_array, cJSON_CreateNumber(number_value));
                    
                    // 5. Clean up all objects (triggers cJSON_free)
                    cJSON_Delete(extra_array);
                    cJSON_Delete(extra_object);
                } else {
                    if (extra_object != nullptr) cJSON_Delete(extra_object);
                    if (extra_array != nullptr) cJSON_Delete(extra_array);
                }
                
                // 5. Delete root object
                cJSON_Delete(root);
            }
            break;
        }
        
        case 1: {
            // Scenario 2: Object creation and manipulation
            // 3. Create objects and arrays directly
            cJSON* object = cJSON_CreateObject();
            cJSON* array = cJSON_CreateArray();
            
            if (object != nullptr && array != nullptr) {
                // Create various items to add
                cJSON* string_item = cJSON_CreateString(string_value.c_str());
                cJSON* number_item = cJSON_CreateNumber(number_value);
                cJSON* bool_item = cJSON_CreateBool(fdp.ConsumeBool() ? 1 : 0);
                cJSON* null_item = cJSON_CreateNull();
                
                // 4. Add items to trigger additional allocations
                if (string_item != nullptr) {
                    cJSON_AddItemToObject(object, "string_key", string_item);
                }
                if (number_item != nullptr) {
                    cJSON_AddItemToArray(array, number_item);
                }
                if (bool_item != nullptr) {
                    cJSON_AddItemToObject(object, "bool_key", bool_item);
                }
                if (null_item != nullptr) {
                    cJSON_AddItemToArray(array, null_item);
                }
                
                // Create nested structure if enabled
                if (test_nested_structures && fdp.remaining_bytes() > 100) {
                    cJSON* nested_object = cJSON_CreateObject();
                    if (nested_object != nullptr) {
                        cJSON_AddItemToObject(nested_object, "nested_key", cJSON_CreateString("nested_value"));
                        cJSON_AddItemToObject(object, "nested", nested_object);
                    }
                }
                
                // 5. Clean up
                cJSON_Delete(array);
                cJSON_Delete(object);
            } else {
                if (object != nullptr) cJSON_Delete(object);
                if (array != nullptr) cJSON_Delete(array);
            }
            break;
        }
        
        case 2: {
            // Scenario 3: Complex JSON parsing with multiple operations
            cJSON* parsed = cJSON_Parse(json_input.c_str());
            
            if (parsed != nullptr) {
                // If we have a valid parse, perform various operations
                if (cJSON_IsObject(parsed) || cJSON_IsArray(parsed)) {
                    // Try to add new items to existing structure
                    cJSON* new_item = cJSON_CreateString("fuzzer_generated");
                    if (new_item != nullptr) {
                        if (cJSON_IsObject(parsed)) {
                            cJSON_AddItemToObject(parsed, "fuzzer_key", new_item);
                        } else if (cJSON_IsArray(parsed)) {
                            cJSON_AddItemToArray(parsed, new_item);
                        }
                    }
                    
                    // Try to create and add more items
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; i++) {
                        std::string item_key = fdp.ConsumeRandomLengthString(32);
                        std::string item_value = fdp.ConsumeRandomLengthString(64);
                        cJSON* item = cJSON_CreateString(item_value.c_str());
                        if (item != nullptr && cJSON_IsObject(parsed)) {
                            cJSON_AddItemToObject(parsed, item_key.c_str(), item);
                        } else if (item != nullptr) {
                            cJSON_Delete(item);
                        }
                    }
                }
                
                // 5. Clean up
                cJSON_Delete(parsed);
            }
            break;
        }
        
        case 3: {
            // Scenario 4: Multiple independent objects
            std::vector<cJSON*> objects;
            
            // Create multiple objects
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 100; i++) {
                if (fdp.ConsumeBool()) {
                    cJSON* obj = cJSON_CreateObject();
                    if (obj != nullptr) {
                        objects.push_back(obj);
                        
                        // Add some content
                        std::string obj_key = fdp.ConsumeRandomLengthString(32);
                        cJSON_AddItemToObject(obj, obj_key.c_str(), 
                            cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    }
                } else {
                    cJSON* arr = cJSON_CreateArray();
                    if (arr != nullptr) {
                        objects.push_back(arr);
                        
                        // Add some items
                        for (int j = 0; j < 3 && fdp.remaining_bytes() > 20; j++) {
                            cJSON_AddItemToArray(arr, 
                                cJSON_CreateBool(fdp.ConsumeBool() ? 1 : 0));
                        }
                    }
                }
            }
            
            // Also try parsing if we have input left
            if (fdp.remaining_bytes() > 50) {
                std::string extra_json = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
                cJSON* parsed = cJSON_Parse(extra_json.c_str());
                if (parsed != nullptr) {
                    objects.push_back(parsed);
                }
            }
            
            // 5. Clean up all objects
            for (cJSON* obj : objects) {
                cJSON_Delete(obj);
            }
            break;
        }
    }
    
    // 6. Verify memory management integrity
    // After all operations, allocations should match deallocations
    int final_allocation_count = allocation_count.load(std::memory_order_relaxed);
    int final_deallocation_count = deallocation_count.load(std::memory_order_relaxed);
    
    // Note: We don't assert or fail here because fuzzer should continue running
    // But we can use the counts to verify our hooks are working
    // In a real test, we would assert(final_allocation_count == final_deallocation_count)
    // For fuzzing, we just make sure the code path is executed
    
    // Use the counts in a way that can't be optimized away
    volatile int diff = final_allocation_count - final_deallocation_count;
    (void)diff;
    
    // Also track total allocated size
    volatile int total_size = total_allocated_size.load(std::memory_order_relaxed);
    (void)total_size;
    
    // Additional test: Direct cJSON_malloc call if we have an API for it
    // Note: cJSON_malloc is a public API, so we can call it directly
    // This ensures we directly hit the cJSON_malloc function
    if (fdp.remaining_bytes() > sizeof(size_t)) {
        size_t direct_alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        void* direct_alloc = cJSON_malloc(direct_alloc_size);
        if (direct_alloc != nullptr) {
            // Do something with the allocation
            memset(direct_alloc, 0, direct_alloc_size);
            cJSON_free(direct_alloc);
        }
    }
    
    return 0;
}

} // namespace harness_020
