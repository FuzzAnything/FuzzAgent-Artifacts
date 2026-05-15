// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Create three null items
    cJSON *null1 = cJSON_CreateNull();
    cJSON *null2 = cJSON_CreateNull();
    cJSON *null3 = cJSON_CreateNull();
    
    // Create an array
    cJSON *array = cJSON_CreateArray();
    
    // Add null items to array if all allocations succeeded
    if (array && null1) {
        cJSON_AddItemToArray(array, null1);
    }
    if (array && null2) {
        cJSON_AddItemToArray(array, null2);
    }
    if (array && null3) {
        cJSON_AddItemToArray(array, null3);
    }
    
    // Clean up: free the array (which will also free its children)
    cJSON_free(array);
    
    // Free any null items that weren't added to the array
    // Since cJSON struct doesn't have a parent field, we need a different approach
    // We'll free null items if array creation failed or if they weren't successfully added
    // Note: Once added to array, items are freed when array is freed
    if (!array) {
        // If array creation failed, free all null items
        cJSON_free(null1);
        cJSON_free(null2);
        cJSON_free(null3);
    } else {
        // If array was created, we already freed it above
        // The null items that were added are now freed as part of the array
        // We don't need to free them separately
    }
    
    return 0;
}