// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    cJSON *array1 = cJSON_CreateArray();
    if (array1 == NULL) {
        return 0;
    }

    cJSON *array2 = cJSON_CreateArray();
    if (array2 == NULL) {
        cJSON_Delete(array1);
        return 0;
    }

    cJSON *array3 = cJSON_CreateArray();
    if (array3 == NULL) {
        cJSON_Delete(array1);
        cJSON_Delete(array2);
        return 0;
    }

    cJSON_AddItemToArray(array1, array2);
    cJSON_AddItemToArray(array2, array3);
    cJSON_AddItemToArray(array3, cJSON_CreateString("dummy"));

    cJSON *duplicate = cJSON_Duplicate(array1, 1);
    if (duplicate == NULL) {
        cJSON_Delete(array1);
        return 0;
    }

    cJSON *detached = cJSON_DetachItemFromArray(duplicate, 0);
    if (detached != NULL) {
        cJSON_Delete(detached);
    }

    cJSON_Delete(duplicate);
    cJSON_Delete(array1);

    return 0;
}