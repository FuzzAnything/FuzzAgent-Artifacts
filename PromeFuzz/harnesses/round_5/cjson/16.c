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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzer input for the key name
    char *key_name = NULL;
    if (Size > 0) {
        key_name = (char *)malloc(Size + 1);
        if (key_name == NULL) {
            return 0;
        }
        memcpy(key_name, Data, Size);
        key_name[Size] = '\0';
    } else {
        // If input is empty, use a default key name
        key_name = strdup("key");
        if (key_name == NULL) {
            return 0;
        }
    }

    // Create JSON object
    cJSON *json_obj = cJSON_CreateObject();
    if (json_obj == NULL) {
        free(key_name);
        return 0;
    }

    // Add false boolean value to the object
    cJSON *false_item = cJSON_AddFalseToObject(json_obj, key_name);
    // Note: false_item may be NULL on failure, but we continue to cleanup

    // Cleanup
    cJSON_Delete(json_obj);
    free(key_name);

    return 0;
}