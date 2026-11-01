// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    cJSON *item1 = cJSON_CreateNull();
    if (item1 == NULL) {
        return 0;
    }

    cJSON *item2 = cJSON_CreateNull();
    if (item2 == NULL) {
        cJSON_free(item1);
        return 0;
    }

    cJSON *item3 = cJSON_CreateNull();
    if (item3 == NULL) {
        cJSON_free(item1);
        cJSON_free(item2);
        return 0;
    }

    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        cJSON_free(item1);
        cJSON_free(item2);
        cJSON_free(item3);
        return 0;
    }

    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);
    cJSON_AddItemToArray(array, item3);

    cJSON_Delete(array);

    return 0;
}