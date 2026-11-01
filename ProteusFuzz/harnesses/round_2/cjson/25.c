// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static cJSON *create_test_array(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateArray();
    if (!root) return NULL;

    size_t idx = 0;
    while (idx < size) {
        int type = data[idx] % 3; // 0: Number, 1: String, 2: Null
        cJSON *item = NULL;

        if (type == 0) {
            double val = (double)(data[idx] - 128);
            item = cJSON_CreateNumber(val);
        } else if (type == 1) {
            size_t len = 0;
            // Check if there is enough data for the length byte
            if (idx + 1 < size) {
                len = data[idx + 1];
            }
            
            // Safety limit for string length
            if (len > 100) len = 100;
            
            // Check if there is enough data for the string content
            // We need: 1 byte for type (idx), 1 byte for len (idx+1), len bytes for content (idx+2 to idx+1+len)
            // Total needed: idx + 2 + len <= size
            if (idx + 2 + len <= size) {
                char *str = (char *)malloc(len + 1);
                if (str) {
                    memcpy(str, data + idx + 2, len);
                    str[len] = '\0';
                    item = cJSON_CreateString(str);
                    free(str);
                    idx += 1 + len; // Skip type, len, and content
                } else {
                    item = cJSON_CreateString("malloc_failed");
                }
            } else {
                // Not enough data for the string, treat as null or skip
                item = cJSON_CreateNull();
            }
        } else {
            item = cJSON_CreateNull();
        }

        if (item) {
            if (!cJSON_AddItemToArray(root, item)) {
                cJSON_Delete(item);
            }
        }
        idx++;
    }
    return root;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cJSON *array = create_test_array(Data, Size);
    if (!array) return 0;

    int index = Data[0];
    int index2 = Data[1];

    // Test 1: cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Use result to avoid unused warning

    // Test 2: cJSON_GetArrayItem
    cJSON *item = cJSON_GetArrayItem(array, index);
    if (item) {
        // Test 3: cJSON_DetachItemFromArray (on a copy or specific index)
        // We need a valid index for detach, let's use index2 if valid
        cJSON *detached = cJSON_DetachItemFromArray(array, index2);
        if (detached) {
            // Test 4: cJSON_AddItemReferenceToArray
            cJSON *refArray = cJSON_CreateArray();
            if (refArray) {
                cJSON_AddItemReferenceToArray(refArray, detached);
                cJSON_Delete(refArray);
            }
            // Re-attach or delete detached item to avoid leak
            if (!cJSON_AddItemToArray(array, detached)) {
                cJSON_Delete(detached);
            }
        }
    }

    // Test 5: cJSON_ReplaceItemInArray
    cJSON *newItem2 = cJSON_CreateNumber(999.0);
    if (newItem2) {
        cJSON_bool res = cJSON_ReplaceItemInArray(array, index, newItem2);
        if (!res) {
            // Replacement failed, newItem2 was not added, so we must free it.
            cJSON_Delete(newItem2);
        }
        // If res is true, newItem2 is now part of 'array' and will be freed when 'array' is deleted.
    }

    // Test 6: cJSON_ReplaceItemViaPointer
    cJSON *target = cJSON_GetArrayItem(array, index);
    if (target) {
        cJSON *replacement = cJSON_CreateString("replaced");
        if (replacement) {
            cJSON_bool res2 = cJSON_ReplaceItemViaPointer(array, target, replacement);
            if (!res2) {
                // Replacement failed, replacement was not added, so we must free it.
                cJSON_Delete(replacement);
            }
            // If res2 is true, replacement is now part of 'array'.
        }
    }

    // Cleanup
    cJSON_Delete(array);

    return 0;
}