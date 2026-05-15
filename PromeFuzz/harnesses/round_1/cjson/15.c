// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Create three arrays
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    cJSON *array3 = cJSON_CreateArray();
    
    // Check if allocations succeeded
    if (!array1 || !array2 || !array3) {
        // Clean up any allocated arrays
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
        if (array3) cJSON_Delete(array3);
        return 0;
    }
    
    // Add items to array1
    // First add array2 to array1
    cJSON_AddItemToArray(array1, array2);
    // Then add array3 to array1
    cJSON_AddItemToArray(array1, array3);
    // Create a simple number item to add as third item
    // We'll create a number using fuzzer data if available
    cJSON *number_item = NULL;
    if (Size > 0) {
        // Use first byte as a number value
        number_item = cJSON_CreateNumber((double)Data[0]);
    } else {
        number_item = cJSON_CreateNumber(42.0);
    }
    
    if (number_item) {
        cJSON_AddItemToArray(array1, number_item);
    } else {
        // If number creation failed, create a null item instead
        cJSON *null_item = cJSON_CreateNull();
        if (null_item) {
            cJSON_AddItemToArray(array1, null_item);
        }
    }
    
    // Duplicate array1
    // Use fuzzer data to decide recursion depth
    cJSON_bool recurse = (Size > 1) ? (Data[1] % 2) : 1;
    cJSON *duplicated = cJSON_Duplicate(array1, recurse);
    
    // Detach an item from array1
    // Use fuzzer data to determine which index to detach
    int which = 0;
    if (Size > 2) {
        which = (int)Data[2] % 3;  // 0, 1, or 2
    }
    cJSON *detached = cJSON_DetachItemFromArray(array1, which);
    
    // Clean up detached item if it exists
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Clean up duplicated item if it exists
    if (duplicated) {
        cJSON_Delete(duplicated);
    }
    
    // Finally delete the main array
    cJSON_Delete(array1);
    
    return 0;
}