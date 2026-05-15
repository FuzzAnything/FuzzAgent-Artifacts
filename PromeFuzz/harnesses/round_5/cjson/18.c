// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create a JSON object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Prepare a null-terminated key string from fuzzer input
    char *key = NULL;
    if (Size > 0) {
        key = (char *)malloc(Size + 1);
        if (key == NULL) {
            cJSON_Delete(obj);
            return 0;
        }
        memcpy(key, Data, Size);
        key[Size] = '\0';
    } else {
        // If input is empty, use a default key
        key = strdup("key");
        if (key == NULL) {
            cJSON_Delete(obj);
            return 0;
        }
    }

    // Add a null item with the generated key
    cJSON *null_item = cJSON_AddNullToObject(obj, key);
    (void)null_item; // Use the result to avoid unused variable warning

    // Clean up
    free(key);
    cJSON_Delete(obj);

    return 0;
}