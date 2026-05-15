// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cJSON.h"

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create a JSON object
    cJSON *json_object = cJSON_CreateObject();
    if (json_object == NULL) {
        return 0;
    }

    // Generate a key name from fuzzer input
    // Use a fixed buffer to avoid issues with non-null-terminated data
    char key_buffer[256];
    size_t key_len = Size < sizeof(key_buffer) - 1 ? Size : sizeof(key_buffer) - 1;
    if (key_len > 0) {
        memcpy(key_buffer, Data, key_len);
    }
    key_buffer[key_len] = '\0';

    // Add a true boolean to the object
    cJSON *true_item = cJSON_AddTrueToObject(json_object, key_buffer);
    // Note: We don't need to check return value for fuzzing purposes

    // Clean up
    cJSON_Delete(json_object);

    return 0;
}