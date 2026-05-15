// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create a dummy key from fuzzer input
    size_t key_len = Size / 2;
    char *key = (char*)malloc(key_len + 1);
    if (!key) return 0;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';
    
    // Create a JSON object and populate it with test items
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        free(key);
        return 0;
    }
    
    // Add some items to the object
    cJSON_AddItemToObject(root, "test1", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "test2", cJSON_CreateFalse());
    cJSON_AddItemToObject(root, "test3", cJSON_CreateNumber(42));
    
    // Add an item with the fuzzer-generated key
    cJSON_AddItemToObject(root, key, cJSON_CreateString("fuzzer_value"));
    
    // Test cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, key);
    
    // Test cJSON_DetachItemFromObjectCaseSensitive
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "test1");
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Test cJSON_CreateTrue
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        cJSON_AddItemToObject(root, "new_true", true_item);
    }
    
    // Test cJSON_DetachItemViaPointer
    cJSON *item_to_detach = cJSON_GetObjectItem(root, "test2");
    if (item_to_detach) {
        cJSON *detached_via_ptr = cJSON_DetachItemViaPointer(root, item_to_detach);
        if (detached_via_ptr) {
            cJSON_Delete(detached_via_ptr);
        }
    }
    
    // Test cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, "test3");
    
    // Test cJSON_DetachItemFromObject
    cJSON *detached2 = cJSON_DetachItemFromObject(root, "new_true");
    if (detached2) {
        cJSON_Delete(detached2);
    }
    
    // Cleanup
    cJSON_Delete(root);
    free(key);
    
    return 0;
}