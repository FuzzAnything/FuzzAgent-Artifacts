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
    // Create parent object
    cJSON *parent = cJSON_CreateObject();
    if (parent == NULL) {
        return 0;
    }

    // Create child name from fuzzer input
    char *child_name = NULL;
    if (Size > 0) {
        child_name = (char *)malloc(Size + 1);
        if (child_name == NULL) {
            cJSON_Delete(parent);
            return 0;
        }
        memcpy(child_name, Data, Size);
        child_name[Size] = '\0';
    } else {
        // Use default name if no input
        child_name = strdup("child");
        if (child_name == NULL) {
            cJSON_Delete(parent);
            return 0;
        }
    }

    // Add child object to parent
    cJSON *child = cJSON_AddObjectToObject(parent, child_name);
    
    // Clean up
    free(child_name);
    cJSON_Delete(parent);
    
    return 0;
}