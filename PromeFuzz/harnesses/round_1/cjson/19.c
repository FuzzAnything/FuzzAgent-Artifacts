// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Prepare a name string for the child object from fuzz data
    // Use a fixed-size buffer to avoid excessive memory allocation
    char name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (Size > 0) {
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
    } else {
        name[0] = '\0';
    }

    // Add a child JSON object to the root object
    cJSON *child = cJSON_AddObjectToObject(root, name);
    // Note: child may be NULL if addition fails, which is acceptable

    // Clean up the entire JSON structure
    cJSON_Delete(root);

    return 0;
}