// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static void fuzz_cjson_operations(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create a cJSON array with some initial items
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some initial items to the array
    for (int i = 0; i < 5 && i < (int)size; i++) {
        cJSON *item = cJSON_CreateNumber(data[i]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Use fuzzer data to generate indices and operations
    size_t data_index = 0;
    
    // Test cJSON_IsArray
    (void)cJSON_IsArray(array);
    (void)cJSON_IsArray(NULL);
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    (void)cJSON_GetArraySize(NULL);
    
    if (array_size > 0) {
        // Generate indices from fuzzer data
        while (data_index + sizeof(int) <= size) {
            int index = *(int*)(data + data_index);
            data_index += sizeof(int);
            
            // Test cJSON_GetArrayItem with various indices
            cJSON *item = cJSON_GetArrayItem(array, index);
            (void)item;
            
            // Test cJSON_DetachItemFromArray
            cJSON *detached = cJSON_DetachItemFromArray(array, index);
            if (detached) {
                // Create new items for insertion/replacement
                cJSON *new_item = cJSON_CreateString("fuzz_item");
                if (new_item) {
                    // Test cJSON_InsertItemInArray
                    (void)cJSON_InsertItemInArray(array, index, new_item);
                    
                    // Create another item for replacement
                    cJSON *replace_item = cJSON_CreateNumber(index * 2.5);
                    if (replace_item) {
                        // Test cJSON_ReplaceItemInArray
                        (void)cJSON_ReplaceItemInArray(array, index, replace_item);
                    }
                }
                
                // Clean up detached item
                cJSON_Delete(detached);
            }
            
            // Break if we've consumed all data
            if (data_index >= size) break;
        }
    }
    
    // Additional tests with edge cases
    // Test with negative indices
    (void)cJSON_GetArrayItem(array, -1);
    (void)cJSON_DetachItemFromArray(array, -1);
    
    // Test with large indices
    (void)cJSON_GetArrayItem(array, INT_MAX);
    (void)cJSON_DetachItemFromArray(array, INT_MAX);
    
    // Test insertion/replacement with edge cases
    cJSON *edge_item = cJSON_CreateString("edge");
    if (edge_item) {
        (void)cJSON_InsertItemInArray(array, -1, edge_item);
        (void)cJSON_InsertItemInArray(array, array_size + 100, edge_item);
        (void)cJSON_ReplaceItemInArray(array, -1, edge_item);
        (void)cJSON_ReplaceItemInArray(array, array_size + 100, edge_item);
    }
    
    // Clean up
    cJSON_Delete(array);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    // Call the main fuzzing function
    fuzz_cjson_operations(data, size);
    
    // Also test with NULL input
    (void)cJSON_GetArraySize(NULL);
    (void)cJSON_IsArray(NULL);
    
    // Test with empty array
    cJSON *empty_array = cJSON_CreateArray();
    if (empty_array) {
        (void)cJSON_GetArraySize(empty_array);
        (void)cJSON_IsArray(empty_array);
        (void)cJSON_GetArrayItem(empty_array, 0);
        (void)cJSON_DetachItemFromArray(empty_array, 0);
        
        // Test insertion into empty array
        cJSON *new_item = cJSON_CreateNumber(42);
        if (new_item) {
            (void)cJSON_InsertItemInArray(empty_array, 0, new_item);
            (void)cJSON_ReplaceItemInArray(empty_array, 0, new_item);
        }
        
        cJSON_Delete(empty_array);
    }
    
    return 0;
}