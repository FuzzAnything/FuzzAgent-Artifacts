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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create a JSON object
    cJSON *json_object = cJSON_CreateObject();
    if (json_object == NULL) {
        return 0;
    }

    // Create a key name from the fuzz input
    char *key_name = NULL;
    if (Size > 0) {
        // Allocate buffer for key name (null-terminated)
        key_name = (char *)malloc(Size + 1);
        if (key_name == NULL) {
            cJSON_Delete(json_object);
            return 0;
        }
        memcpy(key_name, Data, Size);
        key_name[Size] = '\0';
    } else {
        // Use a default key name if input is empty
        key_name = strdup("key");
        if (key_name == NULL) {
            cJSON_Delete(json_object);
            return 0;
        }
    }

    // Add false value to the object
    cJSON *false_item = cJSON_AddFalseToObject(json_object, key_name);
    
    // Clean up
    free(key_name);
    cJSON_Delete(json_object);
    
    return 0;
}