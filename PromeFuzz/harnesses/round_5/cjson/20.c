// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create JSON object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Create key name from fuzz data
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
        // Use default key if no data
        key = strdup("fuzz_key");
        if (key == NULL) {
            cJSON_Delete(obj);
            return 0;
        }
    }

    // Add true value to object
    cJSON *true_item = cJSON_AddTrueToObject(obj, key);
    
    // Cleanup
    free(key);
    cJSON_Delete(obj);
    
    return 0;
}