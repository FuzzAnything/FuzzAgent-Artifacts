// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    // Create a new JSON object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Use the fuzzing input to generate a key name
    // Ensure the key is null-terminated and not empty
    size_t key_len = Size;
    if (key_len > 255) {
        key_len = 255;
    }

    char key[256];
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Add a boolean true item to the object
    cJSON_AddTrueToObject(obj, key);

    // Clean up the JSON object
    cJSON_Delete(obj);

    return 0;
}