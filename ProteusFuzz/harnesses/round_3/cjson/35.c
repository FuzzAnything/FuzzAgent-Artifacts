// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to char* for cJSON_Parse
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        // Parsing failed, try to create a minimal valid array manually to test array functions
        root = cJSON_CreateArray();
        if (!root) {
            return 0;
        }
        // Add a dummy item to ensure the array is not empty for some tests
        cJSON_AddItemToArray(root, cJSON_CreateNumber(0));
    }

    // Check if the root is an array using the type bitmask
    // cJSON_IsArray is a macro or constant, not a function. 
    // Standard cjson defines types like cJSON_Array (1<<2). 
    // We check the type field directly.
    if ((root->type & cJSON_Array) != 0) {
        // Fuzz indices based on input data to explore various states
        int index = 0;
        if (Size > 0) {
            index = Data[0];
            // Introduce negative indices to test boundary checks
            if (Data[0] & 0x80) {
                index = -index;
            }
        }

        // 1. Test cJSON_GetArrayItem
        cJSON *item = cJSON_GetArrayItem(root, index);
        (void)item; // Suppress unused variable warning

        // 2. Test cJSON_GetArraySize
        int size = cJSON_GetArraySize(root);
        (void)size;

        // Create a replacement item for modification tests
        cJSON *new_item = cJSON_CreateNumber(12345);
        if (new_item) {
            // 3. Test cJSON_InsertItemInArray
            int insert_idx = (Size > 1) ? (int)Data[1] : 0;
            if (insert_idx < 0) insert_idx = 0;
            cJSON_InsertItemInArray(root, insert_idx, new_item);

            // 4. Test cJSON_ReplaceItemInArray
            int replace_idx = (Size > 2) ? (int)Data[2] : 0;
            if (replace_idx < 0) replace_idx = 0;
            cJSON *replacement = cJSON_CreateNumber(54321);
            if (replacement) {
                cJSON_ReplaceItemInArray(root, replace_idx, replacement);
                // Note: replacement is now owned by the array, do not delete it here
            }

            // 5. Test cJSON_DetachItemFromArray
            int detach_idx = (Size > 3) ? (int)Data[3] : 0;
            if (detach_idx < 0) detach_idx = 0;
            cJSON *detached = cJSON_DetachItemFromArray(root, detach_idx);
            if (detached) {
                cJSON_Delete(detached);
            }

            // 6. Test cJSON_ReplaceItemViaPointer
            // We need a valid item in the array to replace via pointer
            cJSON *target_item = cJSON_GetArrayItem(root, 0);
            if (target_item) {
                cJSON *via_ptr_replacement = cJSON_CreateNumber(99999);
                if (via_ptr_replacement) {
                    cJSON_ReplaceItemViaPointer(root, target_item, via_ptr_replacement);
                    // via_ptr_replacement is now owned by the array
                }
            }
        }
    }

    cleanup_cjson(root);
    return 0;
}