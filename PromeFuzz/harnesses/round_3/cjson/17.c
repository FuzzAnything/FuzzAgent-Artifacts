// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzer input for the key name
    char *key_name = NULL;
    if (Size > 0) {
        key_name = (char *)malloc(Size + 1);
        if (key_name == NULL) {
            return 0;
        }
        memcpy(key_name, Data, Size);
        key_name[Size] = '\0';
    } else {
        // If input is empty, use a default key name
        key_name = strdup("key");
        if (key_name == NULL) {
            return 0;
        }
    }

    // Create JSON object
    cJSON *json_object = cJSON_CreateObject();
    if (json_object == NULL) {
        free(key_name);
        return 0;
    }

    // Add null item to object with the generated key name
    cJSON *null_item = cJSON_AddNullToObject(json_object, key_name);
    
    // Clean up
    cJSON_Delete(json_object);
    free(key_name);
    
    return 0;
}