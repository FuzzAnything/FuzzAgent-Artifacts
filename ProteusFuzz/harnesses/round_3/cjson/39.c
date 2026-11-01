// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "cJSON.h"

/* Helper to create a simple JSON array with random elements for fuzzing */
static cJSON *create_fuzz_array(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateArray();
    if (!root) return NULL;

    size_t idx = 0;
    while (idx < size) {
        uint8_t type = data[idx] % 3; /* 0: Number, 1: String, 2: Null */
        cJSON *item = NULL;

        if (type == 0) {
            /* Number: use next 4 bytes as int if available, else 0 */
            int val = 0;
            if (idx + 4 <= size) {
                memcpy(&val, data + idx + 1, 4);
            }
            item = cJSON_CreateNumber((double)val);
            idx += 5;
        } else if (type == 1) {
            /* String: use next byte as length, then that many bytes */
            uint8_t len = (idx + 1 < size) ? data[idx + 1] : 0;
            if (len > 100) len = 100; /* Cap length */
            if (idx + 2 + len <= size) {
                char *str = (char *)malloc(len + 1);
                if (str) {
                    memcpy(str, data + idx + 2, len);
                    str[len] = '\0';
                    item = cJSON_CreateString(str);
                    free(str);
                }
                idx += 2 + len;
            } else {
                item = cJSON_CreateString("");
                idx += 2;
            }
        } else {
            /* Null */
            item = cJSON_CreateNull();
            idx += 1;
        }

        if (item) {
            cJSON_AddItemToArray(root, item);
        }
    }
    return root;
}

/* Helper to create a replacement item */
static cJSON *create_fuzz_item(const uint8_t *data, size_t size) {
    if (size < 1) return cJSON_CreateNull();
    uint8_t type = data[0] % 3;
    if (type == 0) {
        int val = 0;
        if (size >= 5) {
            memcpy(&val, data + 1, 4);
        }
        return cJSON_CreateNumber((double)val);
    } else if (type == 1) {
        uint8_t len = (size > 1) ? data[1] : 0;
        if (len > 50) len = 50;
        if (size >= 2 + len) {
            char *str = (char *)malloc(len + 1);
            if (str) {
                memcpy(str, data + 2, len);
                str[len] = '\0';
                cJSON *item = cJSON_CreateString(str);
                free(str);
                return item;
            }
        }
        return cJSON_CreateString("fuzz");
    }
    return cJSON_CreateNull();
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Split input: first half for array creation, second half for item creation */
    size_t split = Size / 2;
    cJSON *array = create_fuzz_array(Data, split);
    if (!array) return 0;

    cJSON *newitem = create_fuzz_item(Data + split, Size - split);
    if (!newitem) {
        cJSON_Delete(array);
        return 0;
    }

    /* Extract index from input data safely */
    int index = 0;
    /* Ensure we have enough data to read 4 bytes for the index */
    size_t index_offset = split;
    if (Size - split >= 4) {
        /* Use the first 4 bytes of the second half for the index */
        memcpy(&index, Data + index_offset, 4);
    } else if (Size - split >= 1) {
        /* Fallback to single byte if not enough data */
        index = (int)Data[index_offset];
    } else {
        index = 0;
    }

    /* Normalize index to avoid extreme values causing crashes in other ways */
    if (index < -1000) index = -1000;
    if (index > 1000) index = 1000;

    /* Test 1: cJSON_GetArrayItem */
    cJSON *item = cJSON_GetArrayItem(array, index);
    (void)item; /* Suppress unused warning */

    /* Test 2: cJSON_GetArraySize */
    int size = cJSON_GetArraySize(array);
    (void)size;

    /* Test 3: cJSON_DetachItemFromArray */
    cJSON *detached = cJSON_DetachItemFromArray(array, index);
    if (detached) {
        cJSON_Delete(detached);
    }

    /* Test 4: cJSON_InsertItemInArray */
    cJSON_bool insert_ok = cJSON_InsertItemInArray(array, index, newitem);
    (void)insert_ok;
    /* Note: newitem is now owned by array if insert_ok is true, or still owned by us if false.
       We will handle deletion later based on ownership. */

    /* Test 5: cJSON_ReplaceItemInArray */
    /* Create another item for replacement */
    cJSON *replace_item = create_fuzz_item(Data, Size);
    if (replace_item) {
        cJSON_bool replace_ok = cJSON_ReplaceItemInArray(array, index, replace_item);
        (void)replace_ok;
        /* If replace_ok is true, replace_item is now owned by array.
           If false, we must delete it. */
        if (!replace_ok) {
            cJSON_Delete(replace_item);
        }
    }

    /* Test 6: cJSON_ReplaceItemViaPointer */
    /* Need a valid item in the array to replace via pointer */
    cJSON *target = cJSON_GetArrayItem(array, 0);
    cJSON *replacement = create_fuzz_item(Data, Size);
    if (target && replacement) {
        cJSON_bool replace_ptr_ok = cJSON_ReplaceItemViaPointer(array, target, replacement);
        (void)replace_ptr_ok;
        /* If replace_ptr_ok is true, replacement is now owned by array.
           If false, we must delete it. */
        if (!replace_ptr_ok) {
            cJSON_Delete(replacement);
        }
    }

    /* Cleanup */
    cJSON_Delete(array);
    return 0;
}