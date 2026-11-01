// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Extract parameters from input
    int count = Data[0];
    int index = (int)(Data[1] & 0x7F); // Ensure non-negative for index logic
    int op_type = Data[1] >> 7; // Use high bit to select operation type

    // Create a buffer for int array if needed
    int *numbers = NULL;
    cJSON *array = NULL;
    cJSON *item = NULL;
    cJSON *ref_item = NULL;

    // Prepare int array for cJSON_CreateIntArray
    if (count > 0 && Size > 2) {
        // Limit count to prevent huge allocations
        if (count > 1000) count = 1000;
        numbers = (int *)malloc(count * sizeof(int));
        if (numbers) {
            for (int i = 0; i < count; i++) {
                if (2 + i < Size) {
                    numbers[i] = (int)(Data[2 + i] & 0xFF);
                } else {
                    numbers[i] = 0;
                }
            }
        }
    }

    // Operation 0: Test cJSON_CreateIntArray and subsequent array operations
    if (op_type == 0) {
        if (numbers && count >= 0) {
            array = cJSON_CreateIntArray(numbers, count);
        } else {
            // Test with NULL or negative count
            array = cJSON_CreateIntArray(NULL, -1);
        }
    }

    // Operation 1: Test cJSON_GetArrayItem
    if (op_type == 1) {
        if (numbers && count >= 0) {
            array = cJSON_CreateIntArray(numbers, count);
        } else {
            array = cJSON_CreateIntArray(NULL, 0);
        }
        // Get item by index (may be out of bounds)
        cJSON *retrieved = cJSON_GetArrayItem(array, index);
        // Cleanup retrieved if it's a reference or if we need to free it (usually not needed for GetArrayItem)
        // But if we created a specific item to test, we handle it.
        // Here retrieved is just a pointer, no need to delete unless we detached it.
        cleanup_cjson(array);
        // Do not call cleanup_cjson(numbers) here, numbers is int array, not cJSON
        free(numbers);
        return 0;
    }

    // Operation 2: Test cJSON_AddItemReferenceToArray
    if (op_type == 2) {
        // Create an array
        if (numbers && count >= 0) {
            array = cJSON_CreateIntArray(numbers, count);
        } else {
            array = cJSON_CreateIntArray(NULL, 0);
        }
        if (!array) {
            free(numbers);
            return 0;
        }

        // Create an item to reference
        ref_item = cJSON_CreateNumber(123.0);
        if (ref_item) {
            cJSON_bool success = cJSON_AddItemReferenceToArray(array, ref_item);
            if (!success) {
                // If adding reference failed, we must free ref_item manually
                cleanup_cjson(ref_item);
                ref_item = NULL;
            }
        }
    }

    // Operation 3: Test cJSON_DeleteItemFromArray
    if (op_type == 3) {
        if (numbers && count >= 0) {
            array = cJSON_CreateIntArray(numbers, count);
        } else {
            array = cJSON_CreateIntArray(NULL, 0);
        }
        if (array) {
            // Try to delete at index (might be out of bounds)
            cJSON_DeleteItemFromArray(array, index);
        }
    }

    // Operation 4: Test cJSON_GetArraySize
    if (op_type == 4) {
        if (numbers && count >= 0) {
            array = cJSON_CreateIntArray(numbers, count);
        } else {
            array = cJSON_CreateIntArray(NULL, 0);
        }
        int size = cJSON_GetArraySize(array);
        (void)size; // Use size to avoid unused warning
    }

    // Operation 5: Test cJSON_AddItemToArray
    if (op_type == 5) {
        if (numbers && count >= 0) {
            array = cJSON_CreateIntArray(numbers, count);
        } else {
            array = cJSON_CreateIntArray(NULL, 0);
        }
        if (array) {
            // Create a new item to add
            item = cJSON_CreateString("fuzz_string");
            if (item) {
                cJSON_AddItemToArray(array, item);
                // item is now owned by array, so we don't delete it separately
            }
        }
    }

    // Cleanup
    cleanup_cjson(array);
    if (numbers) {
        free(numbers);
    }
    // Note: ref_item and item are managed by the array or deleted via array cleanup.
    // In op_type 2, if add failed, we freed ref_item. If success, array owns the reference (but not the memory of ref_item itself? 
    // Actually cJSON_AddItemReferenceToArray does NOT take ownership of the item's memory, it just adds a reference.
    // So if we added ref_item successfully, we MUST free it manually after deleting the array, 
    // because cJSON_Delete(array) will NOT delete the referenced item, it just removes the reference.
    // Wait, let's check cjson behavior: 
    // "Append reference to item... don't want to corrupt your existing cJSON."
    // This implies the caller retains ownership of 'item'.
    // So if we created ref_item and added it by reference, we must delete ref_item ourselves.
    // However, if we delete the array first, the reference is gone, but the item still exists.
    // So we need to delete ref_item if it was created and not already freed.
    
    // Correction for op_type 2:
    // If ref_item was created and added successfully, we need to free it.
    // If it was added and then we delete array, ref_item is still alive.
    // So we should free ref_item here if it is not NULL.
    if (ref_item) {
        cleanup_cjson(ref_item);
    }

    return 0;
}