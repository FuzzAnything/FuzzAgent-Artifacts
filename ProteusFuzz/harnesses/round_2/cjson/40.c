// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a copy of the input data to ensure it is null-terminated
    // cJSON_Parse expects a null-terminated string.
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Parse input data as JSON to create an initial object
    cJSON *root = cJSON_Parse(input_str);
    free(input_str);

    // If parsing fails, create a minimal valid object manually
    if (root == NULL) {
        root = cJSON_CreateObject();
        if (root == NULL) {
            return 0;
        }
    }

    // Ensure root is an object for the target functions
    if (root->type != cJSON_Object) {
        cleanup_cjson(root);
        root = cJSON_CreateObject();
        if (root == NULL) {
            return 0;
        }
    }

    // Extract key and value from input data if possible
    // Use a portion of the input as the key string
    size_t key_len = (Size > 10) ? 10 : Size;
    char key[11];
    memset(key, 0, sizeof(key));
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Create a dummy item to add/replace
    // Ensure we have enough data for a string value, otherwise use a number
    cJSON *item = NULL;
    if (Size > key_len) {
        // Check if the remaining data is null-terminated or safe to treat as string
        // We limit the string length to avoid reading past buffer
        size_t val_len = Size - key_len;
        if (val_len > 0) {
            // Create a temporary buffer for the string to ensure null termination
            char *val_buf = (char *)malloc(val_len + 1);
            if (val_buf != NULL) {
                memcpy(val_buf, Data + key_len, val_len);
                val_buf[val_len] = '\0';
                item = cJSON_CreateString(val_buf);
                free(val_buf);
            }
        }
    }
    
    if (item == NULL) {
        item = cJSON_CreateNumber(0.0);
        if (item == NULL) {
            cleanup_cjson(root);
            return 0;
        }
    }

    // Fuzz cJSON_AddItemToObject
    cJSON_AddItemToObject(root, key, item);

    // Fuzz cJSON_DetachItemFromObject
    cJSON *detached = cJSON_DetachItemFromObject(root, key);
    if (detached != NULL) {
        // Re-attach or delete the detached item
        cJSON_AddItemToObject(root, key, detached);
        // Or: cleanup_cjson(detached);
    }

    // Fuzz cJSON_DetachItemFromObjectCaseSensitive
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, key);
    if (detached != NULL) {
        cJSON_AddItemToObject(root, key, detached);
    }

    // Create a new item for replacement
    cJSON *new_item = cJSON_CreateNumber(123.456);
    if (new_item != NULL) {
        // Fuzz cJSON_ReplaceItemInObject
        cJSON_ReplaceItemInObject(root, key, new_item);
        // Note: new_item is now owned by root, do not delete it here
    }

    // Fuzz cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, key);

    // Fuzz cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, key);

    // Cleanup
    cleanup_cjson(root);

    return 0;
}