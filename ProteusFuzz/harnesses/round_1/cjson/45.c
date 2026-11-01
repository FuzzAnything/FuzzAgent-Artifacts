// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_json(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Use the input data to generate keys and values safely
    char key[64];
    int key_len = Size < 63 ? Size : 63;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Ensure key is valid (no null bytes in the middle for safety)
    for (int i = 0; i < key_len; i++) {
        if (key[i] == '\0') {
            key[i] = 'x';
        }
    }

    // Test cJSON_AddBoolToObject
    cJSON_AddBoolToObject(root, key, (Data[0] % 2) != 0);

    // Test cJSON_HasObjectItem
    cJSON_bool exists = cJSON_HasObjectItem(root, key);
    (void)exists; // Suppress unused variable warning

    // Create a new item to replace or add
    // Ensure Size >= 2 before accessing Data[1]
    cJSON *new_item = NULL;
    if (Size >= 2) {
        new_item = cJSON_CreateNumber((double)(Data[1] % 100));
    } else {
        new_item = cJSON_CreateNumber(0.0);
    }

    if (new_item) {
        // Test cJSON_ReplaceItemInObjectCaseSensitive
        cJSON_ReplaceItemInObjectCaseSensitive(root, key, new_item);
        
        // Test cJSON_ReplaceItemInObject
        cJSON *new_item2 = cJSON_CreateString("replaced");
        if (new_item2) {
            cJSON_ReplaceItemInObject(root, key, new_item2);
        }
    }

    // Test cJSON_AddItemToObject
    cJSON *item_to_add = cJSON_CreateArray();
    if (item_to_add) {
        cJSON_AddItemToObject(root, "array_key", item_to_add);
    }

    // Test cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, key);

    // Cleanup
    cleanup_json(root);

    return 0;
}