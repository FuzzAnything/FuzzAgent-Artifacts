// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    cJSON *array1 = NULL;
    cJSON *array2 = NULL;
    cJSON *array3 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *item3 = NULL;
    cJSON *duplicate = NULL;
    cJSON *detached = NULL;

    /* Step 1: Create three arrays */
    array1 = cJSON_CreateArray();
    if (!array1) return 0;

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

    /* Step 2: Add items to arrays */
    /* Create items to add. Using array2 and array3 as items to add to array1 */
    /* Note: Adding an array to an array is valid in cjson */
    if (!cJSON_AddItemToArray(array1, array2)) {
        cJSON_Delete(array1);
        cJSON_Delete(array2);
        cJSON_Delete(array3);
        return 0;
    }
    
    /* Add array3 to array1 as well */
    if (!cJSON_AddItemToArray(array1, array3)) {
        cJSON_Delete(array1);
        return 0;
    }

    /* Step 3: Duplicate the main array */
    /* We duplicate array1. recurse=1 for deep copy, 0 for shallow. 
       Let's try both based on input data if possible, or just one for stability.
       Using Data[0] % 2 to decide recurse flag. */
    cJSON_bool recurse = (Size > 0) ? (Data[0] % 2) : 1;
    duplicate = cJSON_Duplicate(array1, recurse);
    
    /* Step 4: Detach an item from the original array */
    /* Use input data to determine index, ensuring bounds */
    int index = 0;
    if (Size > 1) {
        index = (int)(Data[1] % 5); // Limit to small range
    }
    
    /* Try to detach from array1. If index is out of bounds, it returns NULL safely. */
    detached = cJSON_DetachItemFromArray(array1, index);
    
    /* If we successfully detached, we own 'detached' now. 
       If 'duplicate' exists, we should clean it up. 
       If 'detached' is not NULL, we must delete it to avoid leaks. */
    if (duplicate) {
        cJSON_Delete(duplicate);
    }
    
    if (detached) {
        cJSON_Delete(detached);
    }

    /* Step 5: Delete the main structure (array1) */
    /* This will recursively delete array1 and any remaining children.
       Note: array2 and array3 were added to array1. 
       If we detached one of them, the other remains in array1.
       If we detached nothing, both remain.
       cJSON_Delete(array1) handles the cleanup of the tree rooted at array1. */
    cJSON_Delete(array1);

    /* Cleanup any remaining standalone pointers if logic varied, 
       but in this flow, array2 and array3 are children of array1 or detached.
       If detached, we deleted them above. 
       If not detached, they are deleted via array1.
       So no further cleanup needed for array2/array3 specifically. */

    return 0;
}