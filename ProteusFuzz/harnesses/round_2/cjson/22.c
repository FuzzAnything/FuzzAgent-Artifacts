// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

/* Fuzz driver for cjson API functions */

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a base object to work with */
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    /* 
       Fix: Ensure the input data is null-terminated before passing to 
       cJSON_CreateRaw, which expects a valid C-string. 
       We allocate a new buffer, copy the data, and append a null terminator.
    */
    char *safe_raw = (char *)malloc(Size + 1);
    if (!safe_raw) {
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(safe_raw, Data, Size);
    safe_raw[Size] = '\0';

    cJSON *rawItem = cJSON_CreateRaw(safe_raw);
    free(safe_raw);

    if (!rawItem) {
        cJSON_Delete(obj);
        return 0;
    }

    /* Extract key name from input data (ensure null termination) */
    size_t keyLen = Size > 64 ? 64 : Size;
    char key[65];
    memcpy(key, Data, keyLen);
    key[keyLen] = '\0';

    /* Test cJSON_AddItemToObjectCS */
    cJSON_AddItemToObjectCS(obj, key, rawItem);

    /* Test cJSON_DetachItemFromObject */
    cJSON *detached = cJSON_DetachItemFromObject(obj, key);
    if (detached) {
        /* Re-attach or delete later */
        cJSON_AddItemToObjectCS(obj, key, detached);
    }

    /* 
       Fix: Similarly, ensure null termination for cJSON_AddRawToObject 
       by creating a temporary null-terminated string.
    */
    char *safe_raw2 = (char *)malloc(Size + 1);
    if (safe_raw2) {
        memcpy(safe_raw2, Data, Size);
        safe_raw2[Size] = '\0';
        cJSON_AddRawToObject(obj, key, safe_raw2);
        free(safe_raw2);
    }

    /* Test cJSON_DeleteItemFromObject */
    cJSON_DeleteItemFromObject(obj, key);

    /* Cleanup */
    cJSON_Delete(obj);

    return 0;
}