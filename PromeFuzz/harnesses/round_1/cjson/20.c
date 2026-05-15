// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string for the key from fuzzer input
    char *key = NULL;
    if (Size > 0) {
        key = (char *)malloc(Size + 1);
        if (key == NULL) {
            return 0;
        }
        memcpy(key, Data, Size);
        key[Size] = '\0';
    } else {
        // If input is empty, use a default key
        key = strdup("key");
        if (key == NULL) {
            return 0;
        }
    }

    // Create JSON object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        free(key);
        return 0;
    }

    // Add null item to object using the fuzzer-generated key
    cJSON *null_item = cJSON_AddNullToObject(obj, key);
    
    // Clean up
    free(key);
    cJSON_Delete(obj);
    
    return 0;
}