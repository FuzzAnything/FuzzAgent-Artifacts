// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

static cJSON *create_test_array(const uint8_t *data, size_t size) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    if (size == 0) {
        return arr;
    }

    // Create a few items based on input data to vary the array content
    for (size_t i = 0; i < size && i < 100; i++) {
        cJSON *item = cJSON_CreateNumber((double)data[i]);
        if (!item) {
            break;
        }
        if (!cJSON_AddItemToArray(arr, item)) {
            cJSON_Delete(item);
            break;
        }
    }
    return arr;
}

static cJSON *create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    if (size == 1) {
        return cJSON_CreateNumber((double)data[0]);
    }
    // Create a string from data if size is sufficient
    char *str = (char *)malloc(size + 1);
    if (!str) return cJSON_CreateNull();
    memcpy(str, data, size);
    str[size] = '\0';
    cJSON *item = cJSON_CreateString(str);
    free(str);
    return item;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input: first byte controls operation, rest is data
    int op = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    cJSON *array = NULL;
    cJSON *item = NULL;
    cJSON *replacement = NULL;
    cJSON *detached = NULL;
    int index;
    cJSON_bool result;

    // Initialize array
    array = create_test_array(payload, payload_size);
    if (!array) {
        return 0;
    }

    // Derive index from payload
    if (payload_size >= 2) {
        index = (payload[0] << 8) | payload[1];
        // Clamp index to reasonable range for safety in fuzzing logic, 
        // though APIs handle negatives/bounds internally.
        if (index > 10000) index = 10000;
        if (index < -10000) index = -10000;
    } else {
        index = 0;
    }

    switch (op) {
        case 0: {
            // Test cJSON_GetArraySize
            int size = cJSON_GetArraySize(array);
            (void)size; // Use return value to prevent unused warnings
            break;
        }
        case 1: {
            // Test cJSON_GetArrayItem
            cJSON *retrieved = cJSON_GetArrayItem(array, index);
            if (retrieved) {
                // Do nothing, just ensure it doesn't crash
            }
            break;
        }
        case 2: {
            // Test cJSON_DetachItemFromArray
            detached = cJSON_DetachItemFromArray(array, index);
            if (detached) {
                // We must free the detached item manually as it's removed from the tree
                cleanup_cjson(detached);
            }
            break;
        }
        case 3: {
            // Test cJSON_AddItemReferenceToArray
            // Create a separate item to reference
            item = create_test_item(payload, payload_size);
            if (item) {
                result = cJSON_AddItemReferenceToArray(array, item);
                (void)result;
                // Note: We don't delete 'item' here because it's now referenced by 'array'.
                // It will be deleted when 'array' is deleted.
            }
            break;
        }
        case 4: {
            // Test cJSON_ReplaceItemInArray
            replacement = create_test_item(payload, payload_size);
            if (replacement) {
                result = cJSON_ReplaceItemInArray(array, index, replacement);
                (void)result;
                // If successful, 'replacement' is now owned by 'array'.
                // If failed, we must delete 'replacement'.
                if (!result) {
                    cleanup_cjson(replacement);
                }
            }
            break;
        }
        case 5: {
            // Test cJSON_ReplaceItemViaPointer
            // We need a valid item in the array to replace
            cJSON *target = cJSON_GetArrayItem(array, index);
            if (target) {
                replacement = create_test_item(payload, payload_size);
                if (replacement) {
                    result = cJSON_ReplaceItemViaPointer(array, target, replacement);
                    (void)result;
                    if (!result) {
                        cleanup_cjson(replacement);
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    // Cleanup
    cleanup_cjson(array);
    // Note: 'item' might be referenced in array, so we don't delete it separately here
    // 'replacement' might be in array, handled by array deletion.
    // 'detached' was already deleted if it existed.

    return 0;
}