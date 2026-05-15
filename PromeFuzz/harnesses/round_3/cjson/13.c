// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create three arrays
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    cJSON *array3 = cJSON_CreateArray();
    
    if (!array1 || !array2 || !array3) {
        // Clean up any allocated arrays
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
        if (array3) cJSON_Delete(array3);
        return 0;
    }
    
    // Create items to add to arrays using fuzz data
    // Use first part of data for string values
    char *str1 = NULL;
    char *str2 = NULL;
    char *str3 = NULL;
    
    if (Size > 0) {
        size_t str1_len = Size > 100 ? 100 : Size;
        str1 = (char*)malloc(str1_len + 1);
        if (str1) {
            memcpy(str1, Data, str1_len);
            str1[str1_len] = '\0';
        }
    }
    
    if (Size > 100) {
        size_t str2_len = Size - 100 > 100 ? 100 : Size - 100;
        str2 = (char*)malloc(str2_len + 1);
        if (str2) {
            memcpy(str2, Data + 100, str2_len);
            str2[str2_len] = '\0';
        }
    }
    
    // Create JSON items from fuzz data
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *item3 = NULL;
    
    if (str1) {
        item1 = cJSON_CreateString(str1);
    } else {
        item1 = cJSON_CreateNumber(Size);
    }
    
    if (str2) {
        item2 = cJSON_CreateString(str2);
    } else {
        item2 = cJSON_CreateNumber(Size * 2.0);
    }
    
    // Use remaining data for third item
    if (Size > 0) {
        double num_value = 0.0;
        if (Size >= sizeof(double)) {
            memcpy(&num_value, Data + (Size > 200 ? 200 : Size - sizeof(double)), sizeof(double));
        } else {
            num_value = (double)Data[0];
        }
        item3 = cJSON_CreateNumber(num_value);
    } else {
        item3 = cJSON_CreateNull();
    }
    
    // Add items to arrays
    if (item1) cJSON_AddItemToArray(array1, item1);
    if (item2) cJSON_AddItemToArray(array2, item2);
    if (item3) cJSON_AddItemToArray(array3, item3);
    
    // Duplicate one of the arrays
    cJSON *duplicated = cJSON_Duplicate(array1, 1); // recurse = true
    
    // Detach an item from array (if it has items)
    cJSON *detached = NULL;
    if (array1 && array1->child) {
        detached = cJSON_DetachItemFromArray(array1, 0);
    }
    
    // Clean up
    if (detached) {
        cJSON_Delete(detached);
    }
    
    if (duplicated) {
        cJSON_Delete(duplicated);
    }
    
    cJSON_Delete(array1);
    cJSON_Delete(array2);
    cJSON_Delete(array3);
    
    // Free temporary strings
    if (str1) free(str1);
    if (str2) free(str2);
    
    return 0;
}