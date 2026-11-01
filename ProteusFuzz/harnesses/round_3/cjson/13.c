// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    cJSON *array1 = NULL;
    cJSON *array2 = NULL;
    cJSON *array3 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *item3 = NULL;
    cJSON *duplicate = NULL;
    cJSON *detached = NULL;

    if (Size < 1) {
        return 0;
    }

    /* Step 1: Create three arrays */
    array1 = cJSON_CreateArray();
    if (!array1) { return 0; }

    array2 = cJSON_CreateArray();
    if (!array2) {
        cJSON_Delete(array1);
        return 0;
    }

    array3 = cJSON_CreateArray();
    if (!array3) {
        cJSON_Delete(array1);
        cJSON_Delete(array2);
        return 0;
    }

    /* Create items to add to arrays */
    item1 = cJSON_CreateNumber(0);
    item2 = cJSON_CreateNumber(1);
    item3 = cJSON_CreateNumber(2);

    if (!item1 || !item2 || !item3) {
        /* Cleanup partial items */
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
        if (item3) cJSON_Delete(item3);
        cJSON_Delete(array1);
        cJSON_Delete(array2);
        cJSON_Delete(array3);
        return 0;
    }

    /* Step 2: Add items to arrays */
    cJSON_AddItemToArray(array1, item1);
    cJSON_AddItemToArray(array2, item2);
    cJSON_AddItemToArray(array3, item3);

    /* Step 3: Duplicate an item (using recurse based on input data) */
    cJSON_bool recurse = (Data[0] % 2) == 1;
    duplicate = cJSON_Duplicate(array1, recurse);

    /* Step 4: Detach an item from an array (using index based on input data) */
    int index = (Size > 1) ? (Data[1] % 2) : 0;
    detached = cJSON_DetachItemFromArray(array2, index);

    /* Step 5: Delete the duplicate if it exists */
    if (duplicate) {
        cJSON_Delete(duplicate);
    }

    /* Step 6: Cleanup detached item if it exists */
    if (detached) {
        cJSON_Delete(detached);
    }

    /* Step 7: Final cleanup of all arrays */
    cJSON_Delete(array1);
    cJSON_Delete(array2);
    cJSON_Delete(array3);

    return 0;
}