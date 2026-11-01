// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    cJSON *array1 = cJSON_CreateArray();
    if (array1 == NULL) {
        return 0;
    }

    cJSON *array2 = cJSON_CreateArray();
    if (array2 == NULL) {
        cleanup_cjson(array1);
        return 0;
    }

    cJSON *array3 = cJSON_CreateArray();
    if (array3 == NULL) {
        cleanup_cjson(array1);
        cleanup_cjson(array2);
        return 0;
    }

    cJSON_bool res1 = cJSON_AddItemToArray(array1, array2);
    (void)res1;

    cJSON_bool res2 = cJSON_AddItemToArray(array1, array3);
    (void)res2;

    cJSON *duplicate = cJSON_Duplicate(array1, 1);
    if (duplicate == NULL) {
        cleanup_cjson(array1);
        return 0;
    }

    cJSON *detached = cJSON_DetachItemFromArray(duplicate, 0);
    if (detached != NULL) {
        cleanup_cjson(detached);
    }

    cleanup_cjson(duplicate);
    cleanup_cjson(array1);

    return 0;
}