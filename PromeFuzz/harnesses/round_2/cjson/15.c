// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void fuzz_cjson_operations(const uint8_t *data, size_t size) {
    // Create three arrays as specified
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    cJSON *array3 = cJSON_CreateArray();
    
    if (!array1 || !array2 || !array3) {
        // Clean up any created arrays if allocation failed
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
        if (array3) cJSON_Delete(array3);
        return;
    }
    
    // Create some items to add to arrays
    // Use fuzzer data to create diverse items
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *item3 = NULL;
    
    if (size > 0) {
        // Create different types of items based on fuzzer input
        uint8_t type_selector = data[0] % 4;
        switch (type_selector) {
            case 0:
                item1 = cJSON_CreateNumber((double)(data[0]));
                break;
            case 1:
                item1 = cJSON_CreateString("test");
                break;
            case 2:
                item1 = cJSON_CreateBool(data[0] & 1);
                break;
            case 3:
                item1 = cJSON_CreateNull();
                break;
        }
    } else {
        item1 = cJSON_CreateNull();
    }
    
    if (size > 1) {
        uint8_t type_selector = data[1] % 4;
        switch (type_selector) {
            case 0:
                item2 = cJSON_CreateNumber((double)(data[1]));
                break;
            case 1:
                item2 = cJSON_CreateString("test2");
                break;
            case 2:
                item2 = cJSON_CreateBool(data[1] & 1);
                break;
            case 3:
                item2 = cJSON_CreateNull();
                break;
        }
    } else {
        item2 = cJSON_CreateNull();
    }
    
    if (size > 2) {
        uint8_t type_selector = data[2] % 4;
        switch (type_selector) {
            case 0:
                item3 = cJSON_CreateNumber((double)(data[2]));
                break;
            case 1:
                item3 = cJSON_CreateString("test3");
                break;
            case 2:
                item3 = cJSON_CreateBool(data[2] & 1);
                break;
            case 3:
                item3 = cJSON_CreateNull();
                break;
        }
    } else {
        item3 = cJSON_CreateNull();
    }
    
    // Add items to arrays (order specified: 3 calls to cJSON_AddItemToArray)
    if (item1) {
        cJSON_AddItemToArray(array1, item1);
    }
    
    if (item2) {
        cJSON_AddItemToArray(array2, item2);
    }
    
    if (item3) {
        cJSON_AddItemToArray(array3, item3);
    }
    
    // Duplicate one of the arrays
    // Use fuzzer data to decide which array to duplicate and recurse parameter
    cJSON_bool recurse = (size > 3) ? (data[3] & 1) : 1;
    cJSON *duplicated = cJSON_Duplicate(array1, recurse);
    
    // Detach an item from array
    // Use fuzzer data to determine which index (including negative)
    int which_index = 0;
    if (size > 4) {
        // Convert to signed to allow negative values
        which_index = (int8_t)data[4];
    }
    cJSON *detached = cJSON_DetachItemFromArray(array2, which_index);
    
    // Clean up detached item if any
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Clean up duplicated array if created
    if (duplicated) {
        cJSON_Delete(duplicated);
    }
    
    // Final cleanup - delete all arrays
    cJSON_Delete(array1);
    cJSON_Delete(array2);
    cJSON_Delete(array3);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    fuzz_cjson_operations(Data, Size);
    return 0;
}