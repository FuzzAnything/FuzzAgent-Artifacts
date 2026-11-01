// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_json_tree(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *parent = cJSON_CreateObject();
    if (parent == NULL) {
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
        cleanup_json_tree(parent);
        return 0;
    }

    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Attempt to add a nested object
    cJSON *child = cJSON_AddObjectToObject(parent, key);
    
    // If child was created successfully, we can optionally add more data to it
    // to explore more states, though the task only requires the specific call order.
    // For robustness, we don't strictly need to add more, but it helps fuzzing depth.
    if (child != NULL) {
        // Add a simple string item to the child to ensure it's a valid structure
        cJSON_AddStringToObject(child, "fuzz_key", "fuzz_value");
    }

    // Cleanup
    free(key);
    cleanup_json_tree(parent);

    return 0;
}