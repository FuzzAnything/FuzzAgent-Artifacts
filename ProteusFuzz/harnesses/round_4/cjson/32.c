// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

static cJSON *create_random_array(const uint8_t *data, size_t size) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    size_t idx = 0;
    while (idx < size) {
        uint8_t type = data[idx] % 3; // 0: number, 1: string, 2: null
        cJSON *item = NULL;
        idx++; // Consume the type byte

        if (type == 0) {
            // Number
            int val = 0;
            if (idx + sizeof(int) <= size) {
                memcpy(&val, data + idx, sizeof(int));
                idx += sizeof(int);
            }
            item = cJSON_CreateNumber((double)val);
        } else if (type == 1) {
            // String
            int len = 0;
            if (idx + sizeof(int) <= size) {
                memcpy(&len, data + idx, sizeof(int));
                idx += sizeof(int);
            }
            // Ensure len is non-negative and reasonable to prevent huge allocations
            if (len < 0) len = 0;
            if (len > 1000) len = 1000;
            
            if (len > 0 && idx + (size_t)len <= size) {
                char *str = (char *)malloc(len + 1);
                if (str) {
                    memcpy(str, data + idx, len);
                    str[len] = '\0';
                    item = cJSON_CreateString(str);
                    free(str);
                    idx += len;
                }
            }
        } else {
            // Null
            item = cJSON_CreateNull();
        }

        if (item) {
            cJSON_AddItemToArray(arr, item);
        }
    }
    return arr;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int op = Data[0] % 6; // Select operation 0-5
    int index_val = 0;
    if (Size > 1) {
        index_val = Data[1];
        if (index_val > 100) index_val = 100;
        if (index_val < -10) index_val = -10;
    }

    cJSON *array = create_random_array(Data + 2, Size - 2);
    if (!array) return 0;

    cJSON *item_to_add = NULL;
    cJSON *item_to_replace = NULL;
    cJSON *detached = NULL;
    cJSON *new_item = NULL;

    switch (op) {
        case 0: { // cJSON_AddItemReferenceToArray
            if (cJSON_GetArraySize(array) > 0) {
                item_to_add = cJSON_GetArrayItem(array, 0);
                if (item_to_add) {
                    cJSON_AddItemReferenceToArray(array, item_to_add);
                }
            } else {
                // Create a dummy item to reference
                item_to_add = cJSON_CreateNumber(123);
                if (item_to_add) {
                    cJSON_AddItemReferenceToArray(array, item_to_add);
                    // Note: The array now holds a reference to item_to_add.
                    // cJSON_Delete(array) will delete item_to_add.
                    // We must not delete item_to_add manually here.
                    item_to_add = NULL; 
                }
            }
            break;
        }
        case 1: { // cJSON_ReplaceItemInArray
            item_to_replace = cJSON_CreateNumber(999);
            if (item_to_replace) {
                if (!cJSON_ReplaceItemInArray(array, index_val, item_to_replace)) {
                    // If replacement failed, we own item_to_replace
                    cJSON_Delete(item_to_replace);
                    item_to_replace = NULL;
                }
            }
            break;
        }
        case 2: { // cJSON_ReplaceItemViaPointer
            if (cJSON_GetArraySize(array) > 0) {
                cJSON *target = cJSON_GetArrayItem(array, 0);
                cJSON *replacement = cJSON_CreateNumber(777);
                if (target && replacement) {
                    if (!cJSON_ReplaceItemViaPointer(array, target, replacement)) {
                        cJSON_Delete(replacement);
                    }
                } else if (replacement) {
                    cJSON_Delete(replacement);
                }
            }
            break;
        }
        case 3: { // cJSON_GetArraySize
            int size = cJSON_GetArraySize(array);
            (void)size; // Use the result to prevent unused warning
            break;
        }
        case 4: { // cJSON_DetachItemFromArray
            detached = cJSON_DetachItemFromArray(array, index_val);
            if (detached) {
                // Detached item is now owned by us, so we must delete it manually.
                cJSON_Delete(detached);
            }
            break;
        }
        case 5: { // cJSON_InsertItemInArray
            new_item = cJSON_CreateNumber(555);
            if (new_item) {
                if (!cJSON_InsertItemInArray(array, index_val, new_item)) {
                    // If insert failed, we own new_item
                    cJSON_Delete(new_item);
                    new_item = NULL;
                }
            }
            break;
        }
    }

    cleanup_cjson(array);
    return 0;
}