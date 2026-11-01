// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    cJSON *obj = NULL;
    cJSON *item = NULL;
    char *key = NULL;
    size_t key_len = 0;

    if (Size == 0) {
        return 0;
    }

    // Create a key from the input data, ensuring it is null-terminated
    // Limit key length to avoid excessive allocations
    key_len = Size;
    if (key_len > 1024) {
        key_len = 1024;
    }

    key = (char *)malloc(key_len + 1);
    if (!key) {
        return 0;
    }

    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Step 1: Create a JSON object
    obj = cJSON_CreateObject();
    if (!obj) {
        free(key);
        return 0;
    }

    // Step 2: Add a false boolean item to the object
    // This function returns NULL if the object is invalid or key already exists
    item = cJSON_AddFalseToObject(obj, key);
    // Note: If item is NULL, it might mean the key already existed or object was invalid.
    // Since we just created the object, it's likely valid unless key is NULL (handled above).
    // If key already existed (unlikely here as obj is new), the function deletes the created item.
    // We don't need to manually delete 'item' if it's NULL or if it was added successfully (ownership transferred to obj).

    // Step 3: Delete the JSON object to clean up
    cJSON_Delete(obj);

    // Cleanup allocated key
    free(key);

    return 0;
}