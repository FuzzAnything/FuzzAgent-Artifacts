// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
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
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    cJSON *null1 = cJSON_CreateNull();
    if (null1 == NULL) {
        return 0;
    }

    cJSON *null2 = cJSON_CreateNull();
    if (null2 == NULL) {
        cJSON_free(null1);
        return 0;
    }

    cJSON *null3 = cJSON_CreateNull();
    if (null3 == NULL) {
        cJSON_free(null1);
        cJSON_free(null2);
        return 0;
    }

    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        cJSON_free(null1);
        cJSON_free(null2);
        cJSON_free(null3);
        return 0;
    }

    cJSON_AddItemToArray(array, null1);
    cJSON_AddItemToArray(array, null2);
    cJSON_AddItemToArray(array, null3);

    cJSON_Delete(array);

    return 0;
}