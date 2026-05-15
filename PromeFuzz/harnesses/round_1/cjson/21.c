// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Create a JSON object
    cJSON *object = cJSON_CreateObject();
    if (!object) {
        return 0;
    }

    // Prepare a null-terminated string for the key name
    char *name = NULL;
    if (Size > 0) {
        // Allocate buffer for name with null terminator
        name = (char *)malloc(Size + 1);
        if (!name) {
            cJSON_Delete(object);
            return 0;
        }
        memcpy(name, Data, Size);
        name[Size] = '\0';
    } else {
        // Use a default name if input is empty
        name = strdup("key");
        if (!name) {
            cJSON_Delete(object);
            return 0;
        }
    }

    // Add a true boolean to the object
    cJSON_AddTrueToObject(object, name);

    // Clean up
    free(name);
    cJSON_Delete(object);

    return 0;
}