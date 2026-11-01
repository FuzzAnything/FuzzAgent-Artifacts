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
#include <cJSON.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void safe_delete(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Create a key name from the input data
    char key[256];
    size_t key_len = (Size < 255) ? Size : 255;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Ensure key is not empty or invalid
    if (key_len == 0) {
        safe_delete(root);
        return 0;
    }

    cJSON *child = cJSON_AddObjectToObject(root, key);
    if (child == NULL) {
        // Allocation failed or parent invalid, clean up root
        safe_delete(root);
        return 0;
    }

    // Optionally add more data to the child object to explore more states
    if (Size > key_len + 1) {
        char value[256];
        size_t val_len = (Size - key_len - 1 < 255) ? (Size - key_len - 1) : 255;
        memcpy(value, Data + key_len + 1, val_len);
        value[val_len] = '\0';
        
        if (val_len > 0) {
            cJSON_AddStringToObject(child, "fuzz_value", value);
        }
    }

    // Clean up the entire tree
    safe_delete(root);

    return 0;
}