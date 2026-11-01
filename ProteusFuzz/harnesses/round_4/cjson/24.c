// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    cJSON *array1 = cJSON_CreateArray();
    if (!array1) {
        return 0;
    }

    cJSON *array2 = cJSON_CreateArray();
    if (!array2) {
        cleanup_cjson(array1);
        return 0;
    }

    cJSON *array3 = cJSON_CreateArray();
    if (!array3) {
        cleanup_cjson(array1);
        cleanup_cjson(array2);
        return 0;
    }

    cJSON *item1 = cJSON_CreateNumber((double)Data[0]);
    cJSON *item2 = cJSON_CreateNumber((double)Data[1]);
    cJSON *item3 = cJSON_CreateNumber((double)Data[2]);

    if (!item1 || !item2 || !item3) {
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
        if (item3) cJSON_Delete(item3);
        cleanup_cjson(array1);
        cleanup_cjson(array2);
        cleanup_cjson(array3);
        return 0;
    }

    cJSON_AddItemToArray(array1, item1);
    cJSON_AddItemToArray(array2, item2);
    cJSON_AddItemToArray(array3, item3);

    cJSON *dup = cJSON_Duplicate(array1, 1);
    if (dup) {
        cJSON *detached_dup = cJSON_DetachItemFromArray(dup, 0);
        if (detached_dup) {
            cJSON_Delete(detached_dup);
        }
        cJSON_Delete(dup);
    }

    cJSON *detached = cJSON_DetachItemFromArray(array1, 0);
    if (detached) {
        cJSON_Delete(detached);
    }

    cleanup_cjson(array1);
    cleanup_cjson(array2);
    cleanup_cjson(array3);

    return 0;
}