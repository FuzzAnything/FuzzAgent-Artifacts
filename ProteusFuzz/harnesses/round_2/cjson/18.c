// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    cJSON *obj = NULL;
    cJSON *null_item = NULL;
    char *key = NULL;

    if (Size == 0) {
        return 0;
    }

    // Allocate and initialize a key string from fuzz input
    key = (char *)malloc(Size + 1);
    if (key == NULL) {
        return 0;
    }
    if (Size > 256) {
        Size = 256; // Limit key size to avoid excessive allocations
    }
    for (size_t i = 0; i < Size; i++) {
        key[i] = (char)Data[i];
    }
    key[Size] = '\0';

    // Create a new JSON object
    obj = cJSON_CreateObject();
    if (obj == NULL) {
        free(key);
        return 0;
    }

    // Add a null item to the object
    null_item = cJSON_AddNullToObject(obj, key);
    // null_item can be NULL if key already exists or allocation fails, which is fine for fuzzing

    // Clean up the JSON object
    cJSON_Delete(obj);

    // Free the allocated key
    free(key);

    return 0;
}