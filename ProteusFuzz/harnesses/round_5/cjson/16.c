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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    cJSON *root = NULL;
    cJSON *item = NULL;
    char *key = NULL;
    size_t key_len = 0;

    if (Size == 0) {
        return 0;
    }

    // Determine key length, limiting to avoid excessive allocation
    if (Size > 1024) {
        key_len = 1024;
    } else {
        key_len = Size;
    }

    // Allocate a new buffer for the key to ensure it is null-terminated
    // We copy 'key_len' bytes from input and add a null terminator
    key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        return 0;
    }

    memcpy(key, Data, key_len);
    key[key_len] = '\0'; // Explicitly null-terminate the string

    // Step 1: Create an empty JSON object
    root = cJSON_CreateObject();
    if (root == NULL) {
        free(key);
        return 0;
    }

    // Step 2: Add a false boolean item to the object using the safe key
    item = cJSON_AddFalseToObject(root, key);
    
    // Step 3: Delete the JSON tree to prevent memory leaks
    cJSON_Delete(root);

    // Free the allocated key buffer
    free(key);

    return 0;
}