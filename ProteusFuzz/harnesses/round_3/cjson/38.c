// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_json(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to a mutable buffer for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        return 0;
    }

    // Ensure we have an array to work with for most operations
    cJSON *target_array = NULL;
    cJSON *temp_array = cJSON_CreateArray();
    
    if (cJSON_IsArray(root)) {
        target_array = root;
    } else if (temp_array) {
        // If root is not an array, we use a temporary array for testing
        target_array = temp_array;
    } else {
        cleanup_json(root);
        return 0;
    }

    // Helper to get a random index within bounds or a safe default
    int random_index = (Size > 0) ? (Data[0] % 10) : 0;
    
    // 1. Test cJSON_GetArraySize
    int size = cJSON_GetArraySize(target_array);
    (void)size; // Use return value to prevent unused warning

    // 2. Test cJSON_AddItemToArray
    // Create a dummy item to add
    cJSON *new_item = cJSON_CreateNumber(123);
    if (new_item) {
        cJSON_AddItemToArray(target_array, new_item);
        // new_item ownership is transferred, so we shouldn't delete it here
    }

    // 3. Test cJSON_CreateObjectReference and cJSON_AddItemReferenceToArray
    // We need an existing item to reference. Let's try to get one from the array or create one.
    cJSON *item_to_ref = NULL;
    if (cJSON_GetArraySize(target_array) > 0) {
        item_to_ref = cJSON_GetArrayItem(target_array, 0);
    } else {
        // Create a temporary item just for reference testing
        cJSON *temp_ref_target = cJSON_CreateString("ref_target");
        if (temp_ref_target) {
            item_to_ref = temp_ref_target;
        }
    }

    if (item_to_ref) {
        cJSON *ref_item = cJSON_CreateObjectReference(item_to_ref);
        if (ref_item) {
            cJSON_AddItemToArray(target_array, ref_item);
            // ref_item ownership transferred
        }

        // Also test AddItemReferenceToArray directly
        // Fix: cJSON_AddItemReferenceToArray returns cJSON_bool (int), not cJSON*
        cJSON_bool ref_result = cJSON_AddItemReferenceToArray(target_array, item_to_ref);
        (void)ref_result; // Just checking the call
    }

    // 4. Test cJSON_DetachItemViaPointer
    // We need a parent and a child. target_array is parent.
    if (cJSON_GetArraySize(target_array) > 0) {
        cJSON *child = cJSON_GetArrayItem(target_array, 0);
        if (child) {
            cJSON *detached = cJSON_DetachItemViaPointer(target_array, child);
            if (detached) {
                // Detached item is now orphaned, we should clean it up or re-add
                // For fuzzing, let's just delete it to avoid leaks
                cJSON_Delete(detached);
            }
        }
    }

    // 5. Test cJSON_DeleteItemFromArray
    // Try to delete an item at a calculated index
    int current_size = cJSON_GetArraySize(target_array);
    if (current_size > 0) {
        int idx = (Data[0] % current_size);
        cJSON_DeleteItemFromArray(target_array, idx);
    }

    // Cleanup
    if (target_array == temp_array) {
        // If we used a temp array, delete it (which deletes the root if root was not used)
        cJSON_Delete(temp_array);
    } else {
        // If target_array was root, delete root
        cleanup_json(root);
    }

    return 0;
}