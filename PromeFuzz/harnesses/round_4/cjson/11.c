// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static void fuzz_cjson_operations(void) {
    // Create three null items
    cJSON *null1 = cJSON_CreateNull();
    cJSON *null2 = cJSON_CreateNull();
    cJSON *null3 = cJSON_CreateNull();
    
    // Create an array
    cJSON *array = cJSON_CreateArray();
    
    if (array != NULL) {
        // Add items to array if they were created successfully
        if (null1 != NULL) {
            cJSON_AddItemToArray(array, null1);
        } else {
            // If null1 failed, create a new null item to add
            cJSON *new_null1 = cJSON_CreateNull();
            if (new_null1 != NULL) {
                cJSON_AddItemToArray(array, new_null1);
            }
        }
        
        if (null2 != NULL) {
            cJSON_AddItemToArray(array, null2);
        } else {
            cJSON *new_null2 = cJSON_CreateNull();
            if (new_null2 != NULL) {
                cJSON_AddItemToArray(array, new_null2);
            }
        }
        
        if (null3 != NULL) {
            cJSON_AddItemToArray(array, null3);
        } else {
            cJSON *new_null3 = cJSON_CreateNull();
            if (new_null3 != NULL) {
                cJSON_AddItemToArray(array, new_null3);
            }
        }
        
        // Use cJSON_Delete to properly free the array and all its children
        cJSON_Delete(array);
    } else {
        // If array creation failed, free individual null items
        cJSON_Delete(null1);
        cJSON_Delete(null2);
        cJSON_Delete(null3);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Call the sequence of operations as specified
    fuzz_cjson_operations();
    
    // Try to explore more states by calling the operations multiple times
    // with different memory states
    for (int i = 0; i < 3 && i < (int)Size; i++) {
        fuzz_cjson_operations();
    }
    
    return 0;
}