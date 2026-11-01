// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static cJSON *create_random_item(const uint8_t *data, size_t size) {
    cJSON *item = cJSON_CreateNull();
    if (!item) return NULL;

    if (size > 0) {
        int type = data[0] % 6;
        if (type == 0) {
            cJSON_Delete(item);
            // Ensure we don't access out of bounds if size is 1
            if (size > 1) {
                item = cJSON_CreateNumber((double)(data[1] % 100));
            } else {
                item = cJSON_CreateNumber(0.0);
            }
        } else if (type == 1) {
            cJSON_Delete(item);
            // Ensure we have a null-terminated string if creating a string
            // We limit the length to avoid huge allocations and ensure safety
            size_t str_len = size > 1 ? size - 1 : 0;
            if (str_len > 0) {
                // Create a temporary buffer to ensure null termination
                char *temp_str = (char *)malloc(str_len + 1);
                if (temp_str) {
                    memcpy(temp_str, data + 1, str_len);
                    temp_str[str_len] = '\0';
                    item = cJSON_CreateString(temp_str);
                    free(temp_str);
                } else {
                    item = cJSON_CreateString("fuzz");
                }
            } else {
                item = cJSON_CreateString("fuzz");
            }
        } else if (type == 2) {
            cJSON_Delete(item);
            item = cJSON_CreateTrue();
        } else if (type == 3) {
            cJSON_Delete(item);
            item = cJSON_CreateFalse();
        } else if (type == 4) {
            cJSON_Delete(item);
            item = cJSON_CreateArray();
        } else {
            cJSON_Delete(item);
            item = cJSON_CreateObject();
        }
    }
    return item;
}

static void cleanup_json_tree(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cJSON *array = cJSON_CreateArray();
    if (!array) {
        return 0;
    }

    // Populate array with some random items to test array operations
    // Use Data[0] to determine number of items, but cap it to prevent huge allocations
    int num_items = (Data[0] % 10) + 1;
    
    for (int i = 0; i < num_items; i++) {
        // Pass the whole data buffer or a subset, ensuring size > 0
        cJSON *item = create_random_item(Data, Size);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }

    // Fuzz cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Use the result to avoid unused variable warning

    // Fuzz cJSON_GetArrayItem
    // Ensure index is within bounds to avoid undefined behavior, 
    // though the API handles out-of-bounds by returning NULL.
    // We use a safe range based on current size.
    int index_get = 0;
    if (Size > 1) {
        index_get = Data[1] % (size + 1); 
    }
    cJSON *item_get = cJSON_GetArrayItem(array, index_get);
    // DO NOT delete item_get here. It is part of the 'array' tree.
    // Deleting it causes use-after-free when the array is later deleted or accessed.
    if (item_get) {
        // Just access to ensure it's valid, e.g., check type
        (void)item_get->type; 
    }

    // Fuzz cJSON_DetachItemFromArray
    // This removes the item from the array and returns ownership.
    // We must delete the detached item manually.
    int index_detach = 0;
    if (Size > 1) {
        index_detach = Data[1] % (size + 2);
    }
    cJSON *detached = cJSON_DetachItemFromArray(array, index_detach);
    if (detached) {
        cJSON_Delete(detached);
    }

    // Fuzz cJSON_InsertItemInArray
    cJSON *new_item = create_random_item(Data, Size);
    if (new_item) {
        int index_insert = 0;
        if (Size > 1) {
            index_insert = Data[1] % (cJSON_GetArraySize(array) + 5);
        }
        cJSON_InsertItemInArray(array, index_insert, new_item);
    }

    // Fuzz cJSON_ReplaceItemInArray
    cJSON *replace_item = create_random_item(Data, Size);
    if (replace_item) {
        int index_replace = 0;
        if (Size > 1) {
            index_replace = Data[1] % (cJSON_GetArraySize(array) + 5);
        }
        cJSON_ReplaceItemInArray(array, index_replace, replace_item);
    }

    // Fuzz cJSON_ReplaceItemViaPointer
    // Only attempt if the array has children
    if (array->child) {
        cJSON *target = array->child;
        cJSON *replacement = create_random_item(Data, Size);
        if (replacement) {
            cJSON_ReplaceItemViaPointer(array, target, replacement);
        }
    }

    // Cleanup
    cleanup_json_tree(array);

    return 0;
}