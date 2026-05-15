// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_null_items(cJSON *null1, cJSON *null2, cJSON *null3, cJSON *array) {
    if (array) {
        cJSON_free(array);
    } else {
        if (null1) cJSON_free(null1);
        if (null2) cJSON_free(null2);
        if (null3) cJSON_free(null3);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create three null items
    cJSON *null1 = cJSON_CreateNull();
    cJSON *null2 = cJSON_CreateNull();
    cJSON *null3 = cJSON_CreateNull();
    
    // Create array
    cJSON *array = cJSON_CreateArray();
    
    // Check if allocations succeeded
    if (!null1 || !null2 || !null3 || !array) {
        cleanup_null_items(null1, null2, null3, array);
        return 0;
    }
    
    // Add items to array
    cJSON_bool success1 = cJSON_AddItemToArray(array, null1);
    cJSON_bool success2 = cJSON_AddItemToArray(array, null2);
    cJSON_bool success3 = cJSON_AddItemToArray(array, null3);
    
    // Clean up - array now owns the items, so we only free the array
    if (array) {
        cJSON_free(array);
    }
    
    return 0;
}