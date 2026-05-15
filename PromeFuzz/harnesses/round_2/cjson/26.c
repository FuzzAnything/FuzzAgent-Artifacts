// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Use first byte to determine which function to test
    uint8_t function_selector = Data[0] % 6;
    
    // Use remaining bytes as key string (null-terminated)
    size_t key_len = Size - 1;
    char *key = (char *)malloc(key_len + 1);
    if (!key) {
        return 0;
    }
    memcpy(key, Data + 1, key_len);
    key[key_len] = '\0';

    // Create a test JSON object with some items
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        free(key);
        return 0;
    }

    // Add some test items to the object
    cJSON_AddItemToObject(root, "test1", cJSON_CreateNumber(42));
    cJSON_AddItemToObject(root, "test2", cJSON_CreateString("hello"));
    cJSON_AddItemToObject(root, "Test1", cJSON_CreateBool(1)); // Different case
    cJSON_AddItemToObject(root, key, cJSON_CreateNull()); // Add item with our fuzzed key

    // Also add the key as a nested object for more complex testing
    cJSON *nested = cJSON_CreateObject();
    if (nested) {
        cJSON_AddItemToObject(nested, "nested_key", cJSON_CreateNumber(123));
        cJSON_AddItemToObject(root, "nested", nested);
    }

    switch (function_selector) {
        case 0: {
            // Test cJSON_DetachItemFromObjectCaseSensitive
            cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, key);
            if (detached) {
                cJSON_Delete(detached);
            }
            break;
        }
        case 1: {
            // Test cJSON_DetachItemFromObject
            cJSON *detached = cJSON_DetachItemFromObject(root, key);
            if (detached) {
                cJSON_Delete(detached);
            }
            break;
        }
        case 2: {
            // Test cJSON_DeleteItemFromObjectCaseSensitive
            cJSON_DeleteItemFromObjectCaseSensitive(root, key);
            break;
        }
        case 3: {
            // Test cJSON_DeleteItemFromObject
            cJSON_DeleteItemFromObject(root, key);
            break;
        }
        case 4: {
            // Test cJSON_CreateTrue
            cJSON *true_item = cJSON_CreateTrue();
            if (true_item) {
                cJSON_AddItemToObject(root, "new_true", true_item);
            }
            break;
        }
        case 5: {
            // Test cJSON_DetachItemViaPointer
            // First get an item from the object
            cJSON *item_to_detach = cJSON_GetObjectItem(root, "test1");
            if (item_to_detach) {
                cJSON *detached = cJSON_DetachItemViaPointer(root, item_to_detach);
                if (detached) {
                    cJSON_Delete(detached);
                }
            }
            break;
        }
    }

    // Cleanup
    cJSON_Delete(root);
    free(key);
    
    return 0;
}