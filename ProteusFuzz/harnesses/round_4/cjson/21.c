// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cJSON.h"

static void cleanup(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Use input data to generate a key name
    // Ensure the key is null-terminated and not too long
    size_t key_len = Size;
    if (key_len > 255) {
        key_len = 255;
    }

    char *key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        cleanup(obj);
        return 0;
    }

    // Copy data to key, ensuring null termination
    if (Size > 0) {
        // Handle potential null bytes in input by replacing them or truncating
        // For fuzzing, we can just copy and ensure termination
        size_t i;
        for (i = 0; i < key_len && i < Size; i++) {
            if (Data[i] == '\0') {
                key[i] = ' '; // Replace null with space to avoid early termination
            } else {
                key[i] = (char)Data[i];
            }
        }
        key[i] = '\0';
    } else {
        key[0] = '\0';
    }

    // Invoke cJSON_AddNullToObject
    cJSON_AddNullToObject(obj, key);

    // Cleanup
    free(key);
    cleanup(obj);

    return 0;
}