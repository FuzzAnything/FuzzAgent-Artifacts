// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzer input for the name parameter
    char *name = NULL;
    if (Size > 0) {
        name = (char *)malloc(Size + 1);
        if (name == NULL) {
            return 0;
        }
        memcpy(name, Data, Size);
        name[Size] = '\0';
    } else {
        // Handle empty input by using a default name
        const char *default_name = "fuzz";
        name = (char *)malloc(strlen(default_name) + 1);
        if (name == NULL) {
            return 0;
        }
        strcpy(name, default_name);
    }

    // Create JSON object
    cJSON *json_object = cJSON_CreateObject();
    if (json_object == NULL) {
        free(name);
        return 0;
    }

    // Add false boolean to object with fuzzer-generated name
    cJSON *added_item = cJSON_AddFalseToObject(json_object, name);
    
    // Clean up
    free(name);
    cJSON_Delete(json_object);
    
    return 0;
}