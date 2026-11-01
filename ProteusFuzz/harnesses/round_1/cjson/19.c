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

static void safe_cleanup(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *parent = NULL;
    cJSON *child = NULL;
    const char *key = NULL;

    // Create a new parent object
    parent = cJSON_CreateObject();
    if (parent == NULL) {
        return 0;
    }

    // Use input data to determine the key name
    // Ensure we don't read past the buffer and ensure null termination
    size_t key_len = Size;
    if (key_len > 256) {
        key_len = 256;
    }

    char *key_buf = NULL;
    if (key_len > 0) {
        key_buf = (char *)malloc(key_len + 1);
        if (key_buf == NULL) {
            safe_cleanup(parent);
            return 0;
        }
        memcpy(key_buf, Data, key_len);
        key_buf[key_len] = '\0';
        key = key_buf;
    } else {
        key = "default_key";
    }

    // Add a nested object to the parent
    child = cJSON_AddObjectToObject(parent, key);
    if (child != NULL) {
        // Optional: Add more data to the child to exercise more code paths if needed
        // For this specific task, we just need to call the three functions in order.
        // We can try to add a simple string to the child to ensure it's not empty
        cJSON_AddStringToObject(child, "fuzz_test", "value");
    }

    // Cleanup the entire tree
    safe_cleanup(parent);

    // Free the allocated key buffer if we allocated it
    if (key_buf != NULL) {
        free(key_buf);
    }

    return 0;
}