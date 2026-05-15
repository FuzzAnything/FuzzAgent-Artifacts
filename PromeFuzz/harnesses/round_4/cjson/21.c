// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
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
#include "cJSON.h"

static void fuzz_cjson_operations(const uint8_t *data, size_t size) {
    // Create three arrays
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    cJSON *array3 = cJSON_CreateArray();
    
    if (!array1 || !array2 || !array3) {
        // Clean up any created arrays
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
        if (array3) cJSON_Delete(array3);
        return;
    }
    
    // Add items to arrays
    // First add: array2 to array1
    cJSON_AddItemToArray(array1, array2);
    
    // Second add: array3 to array1
    cJSON_AddItemToArray(array1, array3);
    
    // Third add: create a new array and add to array2
    cJSON *temp_array = cJSON_CreateArray();
    if (temp_array) {
        cJSON_AddItemToArray(array2, temp_array);
    }
    
    // Duplicate array1 with recurse flag based on input
    cJSON_bool recurse = (size > 0) ? (data[0] & 1) : 0;
    cJSON *duplicate = cJSON_Duplicate(array1, recurse);
    
    // Detach item from array1 with index based on input
    int which = (size > 1) ? (data[1] % 10) : 0;
    cJSON *detached = cJSON_DetachItemFromArray(array1, which);
    
    // Clean up detached item if any
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Clean up duplicate if created
    if (duplicate) {
        cJSON_Delete(duplicate);
    }
    
    // Final delete of the main structure
    cJSON_Delete(array1);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    fuzz_cjson_operations(Data, Size);
    return 0;
}