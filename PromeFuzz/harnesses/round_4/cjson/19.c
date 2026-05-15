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

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from the fuzz input for the key name
    // Allocate one extra byte for null terminator
    char *key = (char *)malloc(Size + 1);
    if (key == NULL) {
        return 0;
    }
    
    // Copy the fuzz data and null-terminate
    if (Size > 0) {
        memcpy(key, Data, Size);
    }
    key[Size] = '\0';
    
    // Create a JSON object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        free(key);
        return 0;
    }
    
    // Add a null item to the object with the fuzzed key
    cJSON *null_item = cJSON_AddNullToObject(obj, key);
    
    // Clean up
    cJSON_Delete(obj);
    free(key);
    
    return 0;
}