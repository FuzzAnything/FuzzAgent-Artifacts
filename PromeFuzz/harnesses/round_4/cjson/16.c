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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Prepare a name string for the child object from fuzzer input
    // Ensure we have at least 1 byte for null termination
    if (Size == 0) {
        cJSON_Delete(root);
        return 0;
    }

    // Copy fuzzer data to a null-terminated string for the name
    size_t name_len = Size > 100 ? 100 : Size; // Limit name length
    char *name = (char *)malloc(name_len + 1);
    if (name == NULL) {
        cJSON_Delete(root);
        return 0;
    }
    
    memcpy(name, Data, name_len);
    name[name_len] = '\0';

    // Add child object to root
    cJSON *child = cJSON_AddObjectToObject(root, name);
    
    // Clean up
    free(name);
    cJSON_Delete(root);
    
    return 0;
}