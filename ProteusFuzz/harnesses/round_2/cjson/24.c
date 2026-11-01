// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a root array to serve as the main target for operations
    cJSON *root = cJSON_CreateArray();
    if (!root) {
        return 0;
    }

    // Use the first byte to decide how many items to pre-populate (0-10)
    // Ensure we don't exceed the available data size minus the header bytes
    int max_pre_count = (Size > 1) ? (int)(Size - 1) : 0;
    int pre_count = Data[0] % 11;
    if (pre_count > max_pre_count) {
        pre_count = max_pre_count;
    }

    cJSON *items[11] = {0};

    for (int i = 0; i < pre_count; i++) {
        // Create dummy items based on subsequent bytes
        // Safety check: ensure i + 1 is within bounds
        if (i + 1 >= Size) break;
        
        cJSON *item = cJSON_CreateNumber((double)(Data[i + 1] & 0xFF));
        if (item) {
            items[i] = item;
            cJSON_AddItemToArray(root, item);
        }
    }

    // Use the second byte to determine the operation mode
    int mode = Data[1] % 6;
    
    // Calculate index safely
    int index = 0;
    if (Size > 2) {
        index = (int)(Data[2] % 20);
    }
    
    int val = 0;
    if (Size > 3) {
        val = (int)(Data[3] & 0xFF);
    }

    switch (mode) {
        case 0: {
            // Test cJSON_AddItemReferenceToArray
            if (pre_count > 0 && items[0]) {
                cJSON_AddItemReferenceToArray(root, items[0]);
            } else {
                // Try with NULL item or create a new one to reference
                cJSON *new_ref = cJSON_CreateString("ref_test");
                if (new_ref) {
                    cJSON_AddItemReferenceToArray(root, new_ref);
                    // Don't delete new_ref here as it's now owned by root
                }
            }
            break;
        }
        case 1: {
            // Test cJSON_GetArraySize
            int size = cJSON_GetArraySize(root);
            (void)size; // Suppress unused warning
            break;
        }
        case 2: {
            // Test cJSON_GetArrayItem
            // Try valid and invalid indices
            cJSON *item = cJSON_GetArrayItem(root, index);
            if (item) {
                // Accessing item is safe
            }
            // Also test negative index
            cJSON *neg_item = cJSON_GetArrayItem(root, -1);
            (void)neg_item;
            break;
        }
        case 3: {
            // Test cJSON_DeleteItemFromArray
            // Ensure we have items to delete
            if (cJSON_GetArraySize(root) > 0) {
                cJSON_DeleteItemFromArray(root, index);
            }
            break;
        }
        case 4: {
            // Test cJSON_ReplaceItemInArray
            cJSON *new_item = cJSON_CreateNumber((double)val);
            if (new_item) {
                if (!cJSON_ReplaceItemInArray(root, index, new_item)) {
                    cJSON_Delete(new_item);
                }
            }
            break;
        }
        case 5: {
            // Test cJSON_AddItemToArray
            cJSON *add_item = cJSON_CreateString("fuzz_add");
            if (add_item) {
                cJSON_AddItemToArray(root, add_item);
            }
            break;
        }
    }

    // Cleanup
    cleanup_json(root);

    return 0;
}