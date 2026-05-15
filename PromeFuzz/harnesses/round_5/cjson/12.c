// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
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
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Create three arrays
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    cJSON *array3 = cJSON_CreateArray();
    
    if (!array1 || !array2 || !array3) {
        // Clean up any created arrays
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
        if (array3) cJSON_Delete(array3);
        return 0;
    }
    
    // Create some items to add to arrays
    // Use fuzzer data to create different types of items
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *item3 = NULL;
    
    if (Size > 0) {
        // Create items based on fuzzer input
        if (Data[0] % 4 == 0) {
            item1 = cJSON_CreateNumber((double)(Data[0]));
        } else if (Data[0] % 4 == 1) {
            item1 = cJSON_CreateString("test");
        } else if (Data[0] % 4 == 2) {
            item1 = cJSON_CreateBool(Data[0] % 2);
        } else {
            item1 = cJSON_CreateNull();
        }
    } else {
        item1 = cJSON_CreateNull();
    }
    
    if (Size > 1) {
        if (Data[1] % 4 == 0) {
            item2 = cJSON_CreateNumber((double)(Data[1]));
        } else if (Data[1] % 4 == 1) {
            item2 = cJSON_CreateString("fuzz");
        } else if (Data[1] % 4 == 2) {
            item2 = cJSON_CreateBool(Data[1] % 2);
        } else {
            item2 = cJSON_CreateNull();
        }
    } else {
        item2 = cJSON_CreateNull();
    }
    
    if (Size > 2) {
        if (Data[2] % 4 == 0) {
            item3 = cJSON_CreateNumber((double)(Data[2]));
        } else if (Data[2] % 4 == 1) {
            item3 = cJSON_CreateString("data");
        } else if (Data[2] % 4 == 2) {
            item3 = cJSON_CreateBool(Data[2] % 2);
        } else {
            item3 = cJSON_CreateNull();
        }
    } else {
        item3 = cJSON_CreateNull();
    }
    
    // Add items to arrays
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
    // Use fuzzer data to decide which array to duplicate and recursion flag
    cJSON_bool recurse = (Size > 3) ? (Data[3] % 2) : 0;
    cJSON *duplicate = NULL;
    
    if (Size > 4) {
        switch (Data[4] % 3) {
            case 0:
                duplicate = cJSON_Duplicate(array1, recurse);
                break;
            case 1:
                duplicate = cJSON_Duplicate(array2, recurse);
                break;
            case 2:
                duplicate = cJSON_Duplicate(array3, recurse);
                break;
        }
    } else {
        duplicate = cJSON_Duplicate(array1, recurse);
    }
    
    // Detach an item from array
    // Use fuzzer data to decide which index to detach from
    int which = 0;
    if (Size > 5) {
        // Cast to signed char to get negative values
        which = (int)((int8_t)Data[5]);
    }
    
    cJSON *detached = cJSON_DetachItemFromArray(array1, which);
    
    // Clean up detached item if it exists
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Clean up duplicate if it exists
    if (duplicate) {
        cJSON_Delete(duplicate);
    }
    
    // Clean up all arrays
    cJSON_Delete(array1);
    cJSON_Delete(array2);
    cJSON_Delete(array3);
    
    return 0;
}