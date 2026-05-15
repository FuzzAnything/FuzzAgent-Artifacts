// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a root object to work with
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Split the input data to create key and value strings
    size_t key_len = Size / 2;
    size_t value_len = Size - key_len;
    
    char *key = (char *)malloc(key_len + 1);
    char *value = (char *)malloc(value_len + 1);
    
    if (key == NULL || value == NULL) {
        free(key);
        free(value);
        cJSON_Delete(root);
        return 0;
    }
    
    memcpy(key, Data, key_len);
    key[key_len] = '\0';
    memcpy(value, Data + key_len, value_len);
    value[value_len] = '\0';

    // Test cJSON_AddNullToObject
    cJSON *null_item = cJSON_AddNullToObject(root, key);

    // Test cJSON_AddObjectToObject
    cJSON *object_item = cJSON_AddObjectToObject(root, key);
    if (object_item != NULL) {
        // Add some content to the nested object
        cJSON_AddNullToObject(object_item, value);
    }

    // Test cJSON_AddRawToObject
    cJSON *raw_item = cJSON_AddRawToObject(root, value, key);

    // Test cJSON_DetachItemFromObject
    cJSON *detached = cJSON_DetachItemFromObject(root, key);
    if (detached != NULL) {
        cJSON_Delete(detached);
    }

    // Test cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, value);

    // Test cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, key);

    // Cleanup
    free(key);
    free(value);
    cJSON_Delete(root);
    
    return 0;
}