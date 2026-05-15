// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Create a cJSON object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Create a null-terminated key string from fuzzed data
    // Use at least 1 byte for key, but ensure we don't overflow
    if (Size == 0) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create key - use min(Size, 256) to avoid huge allocations
    size_t key_len = Size > 256 ? 256 : Size;
    char *key = (char*)malloc(key_len + 1);
    if (!key) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Copy data and null-terminate
    memcpy(key, Data, key_len);
    key[key_len] = '\0';
    
    // Use first byte to determine which operation to perform
    uint8_t operation = Data[0] % 6;
    
    switch (operation) {
        case 0:
            // First add an item, then delete it
            cJSON_AddTrueToObject(root, key);
            cJSON_DeleteItemFromObject(root, key);
            break;
            
        case 1:
            // Add true boolean
            cJSON_AddTrueToObject(root, key);
            break;
            
        case 2:
            // Add false boolean
            cJSON_AddFalseToObject(root, key);
            break;
            
        case 3:
            // Add null item
            cJSON_AddNullToObject(root, key);
            break;
            
        case 4:
            // Add then detach item
            cJSON_AddTrueToObject(root, key);
            cJSON *detached = cJSON_DetachItemFromObject(root, key);
            if (detached) {
                cJSON_Delete(detached);
            }
            break;
            
        case 5:
            // Add then detach with case sensitivity
            cJSON_AddTrueToObject(root, key);
            cJSON *detached_case = cJSON_DetachItemFromObjectCaseSensitive(root, key);
            if (detached_case) {
                cJSON_Delete(detached_case);
            }
            break;
    }
    
    // Clean up
    cJSON_Delete(root);
    free(key);
    
    return 0;
}