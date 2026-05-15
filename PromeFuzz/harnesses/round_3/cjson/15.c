// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Create object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Prepare name string from fuzz data
    char *name = NULL;
    if (Size > 0) {
        // Allocate buffer for name with null terminator
        name = (char *)malloc(Size + 1);
        if (name == NULL) {
            cJSON_Delete(obj);
            return 0;
        }
        memcpy(name, Data, Size);
        name[Size] = '\0';
    } else {
        // Use empty string if no data
        name = (char *)malloc(1);
        if (name == NULL) {
            cJSON_Delete(obj);
            return 0;
        }
        name[0] = '\0';
    }

    // Add false value to object
    cJSON *false_item = cJSON_AddFalseToObject(obj, name);
    
    // Clean up
    free(name);
    cJSON_Delete(obj);
    
    return 0;
}