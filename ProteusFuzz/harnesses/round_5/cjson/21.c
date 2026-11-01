// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void safe_delete(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Create a key string from input data, ensuring it's null-terminated
    char *key = NULL;
    size_t key_len = (Size < 256) ? Size : 255;
    key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        safe_delete(obj);
        return 0;
    }

    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Attempt to add true to the object
    cJSON_AddTrueToObject(obj, key);

    // Cleanup
    free(key);
    safe_delete(obj);

    return 0;
}