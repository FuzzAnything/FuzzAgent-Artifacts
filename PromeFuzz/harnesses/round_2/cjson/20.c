// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string for the key from fuzzer input
    // Use a reasonable maximum length to avoid excessive memory usage
    size_t key_len = Size;
    if (key_len > 1024) {
        key_len = 1024;
    }
    
    char *key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        return 0;
    }
    
    if (Size > 0) {
        memcpy(key, Data, key_len);
    }
    key[key_len] = '\0';
    
    // Create JSON object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        free(key);
        return 0;
    }
    
    // Add true value to object with the fuzzer-generated key
    cJSON *added_item = cJSON_AddTrueToObject(obj, key);
    
    // Clean up
    free(key);
    cJSON_Delete(obj);
    
    return 0;
}