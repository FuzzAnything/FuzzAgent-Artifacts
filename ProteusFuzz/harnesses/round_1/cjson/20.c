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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    /* Ensure the string is null-terminated by allocating size + 1 */
    char *str = (char *)malloc(size + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Step 1: Create a JSON object */
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    /* Step 2: Prepare a key name from fuzz input */
    /* Use a portion of the input as the key, ensuring it's not too long */
    size_t key_len = Size;
    if (key_len > 256) {
        key_len = 256;
    }

    char *key_str = create_safe_string(Data, key_len);
    if (!key_str) {
        cJSON_Delete(obj);
        return 0;
    }

    /* Step 3: Add a null item to the object */
    cJSON *null_item = cJSON_AddNullToObject(obj, key_str);
    /* null_item can be NULL if allocation fails or key exists, which is fine for fuzzing */

    /* Cleanup key string */
    free(key_str);

    /* Step 4: Delete the JSON object to free all resources */
    cJSON_Delete(obj);

    return 0;
}