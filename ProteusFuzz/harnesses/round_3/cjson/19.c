// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_string(char *str) {
    if (str) {
        free(str);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Use input data to generate a key string
    // Ensure null termination and limit length to prevent overflow
    size_t key_len = Size;
    if (key_len > 256) {
        key_len = 256;
    }

    char *key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        cJSON_Delete(obj);
        return 0;
    }

    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Invoke cJSON_AddNullToObject
    cJSON *null_item = cJSON_AddNullToObject(obj, key);
    
    // Cleanup key
    free_string(key);

    // Cleanup JSON object
    cJSON_Delete(obj);

    return 0;
}