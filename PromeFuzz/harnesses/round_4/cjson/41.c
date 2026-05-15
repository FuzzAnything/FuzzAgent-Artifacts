// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_object(const uint8_t *Data, size_t Size) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;
    
    // Add some items to the object
    cJSON_AddItemToObject(root, "key1", cJSON_CreateNumber(42));
    cJSON_AddItemToObject(root, "key2", cJSON_CreateString("test"));
    cJSON_AddItemToObject(root, "Key3", cJSON_CreateBool(1)); // Mixed case
    cJSON_AddItemToObject(root, "KEY3", cJSON_CreateBool(0)); // Same key different case
    
    // Add an item using fuzzer data if available
    if (Size > 0) {
        char *key = malloc(Size + 1);
        if (key) {
            memcpy(key, Data, Size);
            key[Size] = '\0';
            cJSON_AddItemToObject(root, key, cJSON_CreateNumber(Size));
            free(key);
        }
    }
    
    return root;
}

static void cleanup_detached(cJSON *detached) {
    if (detached) {
        cJSON_Delete(detached);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create test object
    cJSON *root = create_test_object(Data, Size);
    if (!root) return 0;
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;
    
    // Prepare key string from fuzzer data
    size_t key_len = (Size - 1) < 100 ? (Size - 1) : 100;
    char *key = malloc(key_len + 1);
    if (!key) {
        cJSON_Delete(root);
        return 0;
    }
    if (Size > 1) {
        memcpy(key, Data + 1, key_len);
    }
    key[key_len] = '\0';
    
    switch (selector) {
        case 0:
            cJSON_DeleteItemFromObject(root, key);
            break;
            
        case 1:
            cJSON_DeleteItemFromObjectCaseSensitive(root, key);
            break;
            
        case 2: {
            cJSON *detached = cJSON_DetachItemFromObject(root, key);
            cleanup_detached(detached);
            break;
        }
            
        case 3: {
            // Get an item pointer to detach
            cJSON *item_to_detach = cJSON_GetObjectItem(root, "key1");
            if (item_to_detach) {
                cJSON *detached = cJSON_DetachItemViaPointer(root, item_to_detach);
                cleanup_detached(detached);
            }
            break;
        }
            
        case 4: {
            cJSON *true_item = cJSON_CreateTrue();
            if (true_item) {
                // Add it to the object and then delete it
                cJSON_AddItemToObject(root, "new_true", true_item);
                cJSON_DeleteItemFromObject(root, "new_true");
            }
            break;
        }
            
        case 5: {
            cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, key);
            cleanup_detached(detached);
            break;
        }
    }
    
    // Cleanup
    free(key);
    cJSON_Delete(root);
    
    return 0;
}