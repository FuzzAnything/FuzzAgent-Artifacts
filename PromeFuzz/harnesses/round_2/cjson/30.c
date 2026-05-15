// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_operations(cJSON *root, const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Use data to create keys and values
    size_t key_len = (data[0] % (size - 1)) + 1;
    if (key_len >= size) key_len = size - 1;
    
    char *key = (char *)malloc(key_len + 1);
    if (!key) return;
    
    memcpy(key, data + 1, key_len);
    key[key_len] = '\0';
    
    // Add some items first
    cJSON_AddNullToObject(root, key);
    
    // Add raw item
    size_t raw_len = (data[0] % 16) + 1;
    char *raw = (char *)malloc(raw_len + 1);
    if (raw) {
        for (size_t i = 0; i < raw_len && i + key_len + 1 < size; i++) {
            raw[i] = data[key_len + 1 + i] ? data[key_len + 1 + i] : '0';
        }
        raw[raw_len] = '\0';
        cJSON_AddRawToObject(root, "raw_key", raw);
        free(raw);
    }
    
    // Perform detach operations
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, key);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Re-add for next operation
    cJSON_AddNullToObject(root, key);
    
    detached = cJSON_DetachItemFromObject(root, key);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Re-add for delete operation
    cJSON_AddNullToObject(root, key);
    cJSON_DeleteItemFromObject(root, key);
    
    // Test DetachItemViaPointer
    cJSON *item = cJSON_AddNullToObject(root, "test_item");
    if (item) {
        cJSON_DetachItemViaPointer(root, item);
        cJSON_Delete(item);
    }
    
    free(key);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Perform fuzzing operations
    fuzz_operations(root, Data, Size);
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}