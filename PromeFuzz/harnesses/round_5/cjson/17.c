// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create parent object
    cJSON *parent = cJSON_CreateObject();
    if (parent == NULL) {
        return 0;
    }

    // Create null-terminated name string from fuzz data
    char *name = (char *)malloc(Size + 1);
    if (name == NULL) {
        cJSON_Delete(parent);
        return 0;
    }
    memcpy(name, Data, Size);
    name[Size] = '\0';

    // Add child object to parent
    cJSON *child = cJSON_AddObjectToObject(parent, name);
    
    // Clean up
    free(name);
    cJSON_Delete(parent);
    
    return 0;
}