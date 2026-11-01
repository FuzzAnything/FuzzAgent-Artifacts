// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    int op = Data[0] % 7;
    int index = (Data[1] % 100) - 50; // Range -50 to 49
    int value = Data[2] % 1000;

    cJSON *array = NULL;
    cJSON *item = NULL;
    cJSON *ref_item = NULL;

    // Initialize array
    array = cJSON_CreateArray();
    if (!array) {
        return 0;
    }

    // Pre-populate array with some items to test edge cases
    for (int i = 0; i < 5; i++) {
        cJSON *temp = cJSON_CreateNumber((double)(i + value));
        if (temp) {
            cJSON_AddItemToArray(array, temp);
        }
    }

    switch (op) {
        case 0: // cJSON_InsertItemInArray
            item = cJSON_CreateNumber((double)value);
            if (item) {
                cJSON_InsertItemInArray(array, index, item);
                // If insertion failed, item is still owned by us, delete it
                // Check if item is in array by checking if its parent is array
                // A simpler heuristic: if we inserted, the item's parent should be set.
                // However, cJSON doesn't expose parent directly in the struct easily for checking ownership status post-call without traversing.
                // The API documentation says: "If which exceeds array length, it appends the item. Otherwise, it splices..."
                // If it returns false, the item is NOT added.
                // We can check the return value if we stored it, but here we just assume if we don't delete it, it's in the array.
                // Actually, cJSON_InsertItemInArray returns cJSON_bool.
                // Let's re-implement the check properly.
            }
            break;

        case 1: // cJSON_AddItemReferenceToArray
            ref_item = cJSON_CreateNumber((double)value);
            if (ref_item) {
                cJSON_AddItemReferenceToArray(array, ref_item);
                // Reference added. The array holds a pointer to ref_item, but does NOT own its memory.
                // We must delete ref_item manually to avoid leaks, unless we want to test dangling pointers (which is bad for fuzzing stability).
                // Standard practice: delete the referenced item when done.
                cJSON_Delete(ref_item); 
            }
            break;

        case 2: // cJSON_DeleteItemFromArray
            cJSON_DeleteItemFromArray(array, index);
            break;

        case 3: // cJSON_GetArraySize
            (void)cJSON_GetArraySize(array);
            break;

        case 4: // cJSON_AddItemToArray
            item = cJSON_CreateNumber((double)value);
            if (item) {
                cJSON_AddItemToArray(array, item);
                // item is now owned by array, do not delete
            }
            break;

        case 5: // cJSON_IsArray
            (void)cJSON_IsArray(array);
            // item might be NULL here if not created in this branch, but we don't use it.
            break;

        case 6: // Stress test: Create a new array and try operations on it
            cJSON *temp_array = cJSON_CreateArray();
            if (temp_array) {
                cJSON *temp_item = cJSON_CreateNumber((double)value);
                if (temp_item) {
                    cJSON_AddItemToArray(temp_array, temp_item);
                    cJSON_DeleteItemFromArray(temp_array, 0);
                    cJSON_Delete(temp_array);
                }
            }
            break;
    }

    // Re-implement case 0 logic correctly to avoid double free
    if (op == 0) {
        if (item) {
            // Check if item was successfully inserted. 
            // If the return value of InsertItemInArray was false, item is not in the array.
            // Since we didn't capture the return value, we can't be 100% sure without re-checking logic.
            // However, the previous code had a heuristic: if size <= 5, delete.
            // But size could be 5 if we inserted at index 5 (append) or if insertion failed.
            // Let's rely on the fact that if we don't delete it here, it's assumed to be in the array.
            // But wait, if insertion failed, we MUST delete it.
            // Let's just delete it if we are not sure, or better:
            // We will re-run the insertion logic to check return value? No, that modifies state.
            // Correct approach: Capture the return value.
            // Since I cannot change the switch case structure easily without rewriting, 
            // I will assume the previous heuristic was flawed and just delete the item if it's not in the array.
            // How to check? Traverse the array.
            cJSON *curr = array->child;
            int found = 0;
            while (curr) {
                if (curr == item) {
                    found = 1;
                    break;
                }
                curr = curr->next;
            }
            if (!found) {
                cJSON_Delete(item);
            }
        }
    }

    cleanup_cjson(array);
    return 0;
}