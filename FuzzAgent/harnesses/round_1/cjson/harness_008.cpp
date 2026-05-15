#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_008 {
/*
 * harness_008.cpp - cJSON array reference and detachment operations harness
 * 
 * This harness targets array manipulation functions with 0% coverage:
 * - cJSON_AddItemReferenceToArray (2 undiscovered branches)
 * - cJSON_DetachItemFromArray (16 undiscovered branches) 
 * - cJSON_DeleteItemFromArray (16 undiscovered branches)
 * 
 * Following the invocation sequence:
 * 1. Create array using cJSON_CreateArray
 * 2. Create multiple items using cJSON_CreateNumber, cJSON_CreateString, etc.
 * 3. Add items to array using cJSON_AddItemToArray
 * 4. Test cJSON_AddItemReferenceToArray to add references to existing items
 * 5. Test cJSON_DetachItemFromArray to remove items without deleting them
 * 6. Test cJSON_DeleteItemFromArray to delete items from specific positions
 * 7. Clean up using cJSON_Delete
 */


extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create array
    cJSON* array = cJSON_CreateArray();
    if (array == nullptr) {
        return 0;
    }
    
    // Step 2: Create multiple items of different types
    // Consume number of items to create (3-8 items)
    int num_items = fdp.ConsumeIntegralInRange<int>(3, 8);
    
    // Create and store items
    cJSON* items[8];
    int created_items = 0;
    
    for (int i = 0; i < num_items && fdp.remaining_bytes() > 10; i++) {
        // Determine item type based on fuzzed data
        uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (item_type) {
            case 0: {
                // Create number
                double num_value = fdp.ConsumeFloatingPoint<double>();
                items[i] = cJSON_CreateNumber(num_value);
                break;
            }
            case 1: {
                // Create string
                std::string str_value = fdp.ConsumeRandomLengthString(64);
                items[i] = cJSON_CreateString(str_value.c_str());
                break;
            }
            case 2: {
                // Create boolean
                bool bool_value = fdp.ConsumeBool();
                items[i] = cJSON_CreateBool(bool_value ? 1 : 0);
                break;
            }
            case 3: {
                // Create null
                items[i] = cJSON_CreateNull();
                break;
            }
        }
        
        if (items[i] != nullptr) {
            created_items++;
        }
    }
    
    // Step 3: Add items to array
    for (int i = 0; i < created_items; i++) {
        if (items[i] != nullptr) {
            cJSON_AddItemToArray(array, items[i]);
        }
    }
    
    // Get current array size
    int array_size = cJSON_GetArraySize(array);
    if (array_size == 0) {
        cJSON_Delete(array);
        return 0;
    }
    
    // Step 4: Test cJSON_AddItemReferenceToArray
    // Add references to existing items at random positions
    if (array_size > 0 && fdp.remaining_bytes() > 4) {
        for (int i = 0; i < 3 && i < array_size; i++) {
            // Get an item from the array to reference
            int ref_index = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON* item_to_ref = cJSON_GetArrayItem(array, ref_index);
            if (item_to_ref != nullptr) {
                // Try to add reference (this should fail if item is already in array)
                cJSON_AddItemReferenceToArray(array, item_to_ref);
            }
        }
    }
    // Step 5: Test cJSON_DetachItemFromArray
    // Detach items at random positions
    if (array_size > 0 && fdp.remaining_bytes() > 4) {
        int detach_attempts = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < detach_attempts && array_size > 0; i++) {
            int detach_index = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON* detached = cJSON_DetachItemFromArray(array, detach_index);
            if (detached != nullptr) {
                // Delete the detached item
                cJSON_Delete(detached);
                array_size--;  // Array size decreased after detachment
            }
        }
    }
    
    // Update array size after detachments
    array_size = cJSON_GetArraySize(array);
    
    // Step 6: Test cJSON_DeleteItemFromArray
    // Delete items at random positions
    if (array_size > 0 && fdp.remaining_bytes() > 4) {
        int delete_attempts = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < delete_attempts && array_size > 0; i++) {
            int delete_index = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON_DeleteItemFromArray(array, delete_index);
            array_size--;  // Array size decreased after deletion
        }
    }
    
    // Step 7: Test edge cases with invalid indices
    if (fdp.remaining_bytes() > 4) {
        // Try to detach/delete with out of bounds indices
        int invalid_index = fdp.ConsumeIntegralInRange<int>(-10, 100);
        if (invalid_index < 0 || invalid_index >= array_size) {
            // These should handle invalid indices gracefully
            cJSON* detached = cJSON_DetachItemFromArray(array, invalid_index);
            if (detached == nullptr) {
                // Expected behavior for invalid index
            }
            // DeleteItemFromArray should also handle invalid indices
            cJSON_DeleteItemFromArray(array, invalid_index);
        }
    }
    
    // Step 8: Cleanup
    cJSON_Delete(array);
    
    return 0;
}

} // namespace harness_008
