// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Create a key name from the fuzzer input
    // Ensure we have a valid null-terminated string for the key
    char *key_name = NULL;
    if (Size > 0) {
        // Allocate one extra byte for null terminator
        key_name = (char *)malloc(Size + 1);
        if (key_name == NULL) {
            cJSON_Delete(root);
            return 0;
        }
        memcpy(key_name, Data, Size);
        key_name[Size] = '\0';
    } else {
        // If no data, use a default key
        key_name = strdup("fuzz_key");
        if (key_name == NULL) {
            cJSON_Delete(root);
            return 0;
        }
    }

    // Add a child object to the root
    cJSON *child = cJSON_AddObjectToObject(root, key_name);
    
    // Clean up the key name
    free(key_name);
    
    // Delete the entire structure
    cJSON_Delete(root);
    
    return 0;
}