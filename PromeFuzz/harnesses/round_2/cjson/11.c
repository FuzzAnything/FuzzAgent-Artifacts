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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    (void)Data;  // Unused parameter in this specific test
    (void)Size;  // Unused parameter in this specific test
    
    // Create three null items
    cJSON *null1 = cJSON_CreateNull();
    cJSON *null2 = cJSON_CreateNull();
    cJSON *null3 = cJSON_CreateNull();
    
    // Create an array
    cJSON *array = cJSON_CreateArray();
    
    // Add items to array (if allocations succeeded)
    if (array != NULL) {
        // Store the items in temporary variables before adding
        cJSON *temp1 = null1;
        cJSON *temp2 = null2;
        cJSON *temp3 = null3;
        
        if (temp1 != NULL) {
            cJSON_AddItemToArray(array, temp1);
        }
        if (temp2 != NULL) {
            cJSON_AddItemToArray(array, temp2);
        }
        if (temp3 != NULL) {
            cJSON_AddItemToArray(array, temp3);
        }
    }
    
    // Clean up - always free the array if it was created
    if (array != NULL) {
        cJSON_free(array);
    }
    
    // If array creation failed, we need to free the null items individually
    if (array == NULL) {
        if (null1 != NULL) {
            cJSON_free(null1);
        }
        if (null2 != NULL) {
            cJSON_free(null2);
        }
        if (null3 != NULL) {
            cJSON_free(null3);
        }
    }
    
    return 0;
}