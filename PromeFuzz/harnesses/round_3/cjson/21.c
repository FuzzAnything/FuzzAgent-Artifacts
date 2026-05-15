// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Limit key size to avoid excessive memory usage
    size_t key_len = Size > 1024 ? 1024 : Size;
    
    // Create null-terminated key string from fuzzer input
    char *key = (char*)malloc(key_len + 1);
    if (!key) return 0;
    
    memcpy(key, Data, key_len);
    key[key_len] = '\0';
    
    // Create JSON object
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        free(key);
        return 0;
    }
    
    // Add true value to object
    cJSON_AddTrueToObject(obj, key);
    
    // Clean up
    cJSON_Delete(obj);
    free(key);
    
    return 0;
}