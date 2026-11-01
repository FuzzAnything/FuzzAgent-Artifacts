// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    cJSON *obj = NULL;
    cJSON *item = NULL;
    char *key = NULL;

    if (Size < 1) {
        return 0;
    }

    obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    /* Allocate buffer for key with space for null terminator */
    key = (char *)malloc(Size + 1);
    if (key == NULL) {
        cJSON_Delete(obj);
        return 0;
    }

    /* Copy data and ensure null termination */
    memcpy(key, Data, Size);
    key[Size] = '\0';

    item = cJSON_AddFalseToObject(obj, key);
    if (item != NULL) {
        /* Successfully added false item */
    }

    cJSON_Delete(obj);
    free(key);

    return 0;
}