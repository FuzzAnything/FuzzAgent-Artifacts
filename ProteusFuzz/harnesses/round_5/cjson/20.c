// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Create a key from the fuzz input, ensuring it is null-terminated
    // Limit the key length to prevent excessive allocation
    size_t key_len = (Size > 100) ? 100 : Size;
    char key[101];
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Add null to object
    cJSON_AddNullToObject(obj, key);

    // Cleanup
    cleanup_cjson(obj);

    return 0;
}