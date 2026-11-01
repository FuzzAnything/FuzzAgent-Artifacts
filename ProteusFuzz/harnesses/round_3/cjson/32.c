// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

/* Helper to create a simple array with random items for testing */
static cJSON *create_test_array(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return NULL;

    /* Use input data to generate random values to add to the array */
    for (size_t i = 0; i < size && i < 100; i++) {
        cJSON *item;
        int type = data[i] % 5;
        
        switch (type) {
            case 0:
                item = cJSON_CreateNumber((double)data[i]);
                break;
            case 1:
                item = cJSON_CreateString("test_string");
                break;
            case 2:
                item = cJSON_CreateTrue();
                break;
            case 3:
                item = cJSON_CreateFalse();
                break;
            case 4:
                item = cJSON_CreateNull();
                break;
            default:
                item = cJSON_CreateNumber(0);
                break;
        }
        
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    return array;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test 1: cJSON_CreateObjectReference with various inputs */
    cJSON *obj_ref = cJSON_CreateObjectReference(NULL);
    if (obj_ref) cJSON_Delete(obj_ref);

    cJSON *test_arr = create_test_array(Data, Size);
    if (test_arr) {
        /* Test 2: cJSON_GetArraySize */
        int size = cJSON_GetArraySize(test_arr);
        (void)size; /* Use return value to suppress unused warnings */

        /* Test 3: cJSON_AddItemReferenceToArray */
        if (cJSON_GetArraySize(test_arr) > 0) {
            cJSON *first_item = cJSON_GetArrayItem(test_arr, 0);
            if (first_item) {
                cJSON_AddItemReferenceToArray(test_arr, first_item);
            }
        }

        /* Test 4: cJSON_AddItemToArray with a new item */
        cJSON *new_item = cJSON_CreateNumber((double)Data[0]);
        if (new_item) {
            cJSON_AddItemToArray(test_arr, new_item);
        }

        /* Test 5: cJSON_DetachItemViaPointer */
        if (cJSON_GetArraySize(test_arr) > 1) {
            cJSON *item_to_detach = cJSON_GetArrayItem(test_arr, 1);
            if (item_to_detach) {
                cJSON *detached = cJSON_DetachItemViaPointer(test_arr, item_to_detach);
                if (detached) {
                    /* Detached item is not freed, so we must free it manually */
                    cJSON_Delete(detached);
                }
            }
        }

        /* Test 6: cJSON_DeleteItemFromArray */
        if (cJSON_GetArraySize(test_arr) > 0) {
            int idx = Data[0] % cJSON_GetArraySize(test_arr);
            cJSON_DeleteItemFromArray(test_arr, idx);
        }

        /* Cleanup */
        cJSON_Delete(test_arr);
    }

    /* Edge case: Test with NULL inputs where applicable */
    /* Create a new item, add it to a parent, delete via parent, then delete parent. 
       Do NOT delete the item again as it is already freed by the parent deletion. */
    cJSON *null_item = cJSON_CreateNull();
    if (null_item) {
        cJSON *null_parent = cJSON_CreateArray();
        if (null_parent) {
            cJSON_AddItemToArray(null_parent, null_item);
            /* This frees null_item internally */
            cJSON_DeleteItemFromArray(null_parent, 0);
            /* Free the parent array */
            cJSON_Delete(null_parent);
        }
        /* null_item is already freed, do not call cJSON_Delete on it again */
    }

    return 0;
}