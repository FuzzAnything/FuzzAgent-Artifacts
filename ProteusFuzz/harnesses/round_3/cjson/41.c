// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

static cJSON *create_test_array(const uint8_t *data, size_t size) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    // Create items based on input data to vary array size and content
    size_t count = 0;
    if (size > 0) {
        count = (size % 100) + 1; // Limit to 100 items to prevent memory exhaustion
    }

    for (size_t i = 0; i < count; i++) {
        cJSON *item = cJSON_CreateNumber((double)i);
        if (!item) {
            cJSON_Delete(arr);
            return NULL;
        }
        cJSON_AddItemToArray(arr, item);
    }
    return arr;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cJSON *array = create_test_array(Data, Size);
    if (!array) {
        return 0;
    }

    // Extract parameters from input data for fuzzing
    int index = (int)(Data[0] % 256);
    // Allow negative indices to test error handling
    if (Data[0] & 1) {
        index = -index;
    }
    // Expand range slightly
    index = index * 2 - 128;

    // Create a new item for replacement/insertion
    cJSON *new_item = cJSON_CreateNumber((double)(Data[1] % 256));
    if (!new_item) {
        cleanup_json(array);
        return 0;
    }

    // Create a reference item for AddItemReferenceToArray
    cJSON *ref_item = cJSON_CreateString("fuzz_string");
    if (!ref_item) {
        cJSON_Delete(new_item);
        cleanup_json(array);
        return 0;
    }

    // 1. Test cJSON_GetArraySize
    int size_before = cJSON_GetArraySize(array);

    // 2. Test cJSON_DetachItemFromArray
    // Detach an item to reuse it or test removal
    cJSON *detached = cJSON_DetachItemFromArray(array, index);
    if (detached) {
        // We detached it, so we should delete it if we don't use it elsewhere
        // But for fuzzing, let's try to re-add it or just delete it
        cJSON_Delete(detached);
    }

    // 3. Test cJSON_InsertItemInArray
    // Insert new_item at index
    cJSON_InsertItemInArray(array, index, new_item);
    // Note: new_item is now owned by array, do not delete it manually

    // 4. Test cJSON_ReplaceItemInArray
    cJSON *replace_item = cJSON_CreateNumber(999.0);
    if (replace_item) {
        cJSON_ReplaceItemInArray(array, index, replace_item);
        // replace_item is now owned by array
    }

    // 5. Test cJSON_ReplaceItemViaPointer
    // We need a valid item in the array to replace via pointer
    cJSON *target_item = cJSON_GetArrayItem(array, 0);
    cJSON *replacement_item = cJSON_CreateNumber(888.0);
    if (target_item && replacement_item) {
        cJSON_ReplaceItemViaPointer(array, target_item, replacement_item);
        // replacement_item is now owned by array
    }

    // 6. Test cJSON_AddItemReferenceToArray
    // Add ref_item by reference
    cJSON_AddItemReferenceToArray(array, ref_item);
    // ref_item is still owned by the caller, but referenced by array
    // We must delete ref_item only when we are done with the array or if we detach it
    // To be safe in fuzzing, we will detach the reference before deleting the array if needed,
    // or just let the array delete the reference if it was added as a child (which it is).
    // Actually, AddItemReferenceToArray adds the item to the array's child list.
    // If we delete the array, it will try to delete the item.
    // Since ref_item was created here, it's safe to let the array delete it, 
    // BUT AddItemReferenceToArray is meant to share ownership. 
    // To avoid double free or use-after-free in this simple driver:
    // We will detach the reference item before deleting the array to ensure we delete it ourselves.
    int current_size = cJSON_GetArraySize(array);
    if (current_size > 0) {
        cJSON *detached_ref = cJSON_DetachItemFromArray(array, current_size - 1);
        if (detached_ref) {
            cJSON_Delete(detached_ref);
        }
    }

    // Cleanup
    cleanup_json(array);
    
    // Note: new_item, replace_item, replacement_item were transferred to 'array' ownership.
    // ref_item was detached and deleted above.
    
    return 0;
}