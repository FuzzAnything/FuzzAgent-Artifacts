// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item(int seed) {
    switch (seed % 6) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateFalse();
        case 3:
            return cJSON_CreateNumber(seed * 1.5);
        case 4:
            return cJSON_CreateString("test_string");
        case 5:
            return cJSON_CreateArray();
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create test array with some initial items
    cJSON *array = cJSON_CreateArray();
    if (!array) return 0;
    
    // Add some initial items to the array
    for (int i = 0; i < 5 && i < Size; i++) {
        cJSON *item = create_test_item(Data[i]);
        cJSON_AddItemToArray(array, item);
    }
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    (void)array_size; // Use result to avoid unused variable warning
    
    // Test cJSON_IsArray
    cJSON_bool is_array = cJSON_IsArray(array);
    (void)is_array;
    
    // Test with NULL input for cJSON_GetArraySize
    cJSON_GetArraySize(NULL);
    
    // Test with NULL input for cJSON_IsArray
    cJSON_IsArray(NULL);
    
    // Test cJSON_DetachItemFromArray with various indices
    if (cJSON_GetArraySize(array) > 0) {
        // Use fuzz data to determine indices
        int index1 = Data[0] % (cJSON_GetArraySize(array) + 2) - 1; // -1 to 1+size
        cJSON *detached = cJSON_DetachItemFromArray(array, index1);
        if (detached) {
            cJSON_Delete(detached);
        }
        
        // Test with another index
        if (cJSON_GetArraySize(array) > 0) {
            int index2 = Data[1] % (cJSON_GetArraySize(array) + 2) - 1;
            detached = cJSON_DetachItemFromArray(array, index2);
            if (detached) {
                cJSON_Delete(detached);
            }
        }
    }
    
    // Test cJSON_DeleteItemFromArray
    if (cJSON_GetArraySize(array) > 0) {
        int index3 = Data[2] % (cJSON_GetArraySize(array) + 2) - 1;
        cJSON_DeleteItemFromArray(array, index3);
    }
    
    // Test cJSON_AddItemToArray with new items
    for (int i = 3; i < 7 && i < Size; i++) {
        cJSON *new_item = create_test_item(Data[i]);
        cJSON_AddItemToArray(array, new_item);
    }
    
    // Test cJSON_ReplaceItemInArray
    if (cJSON_GetArraySize(array) > 0) {
        int index4 = Data[3] % (cJSON_GetArraySize(array) + 2) - 1;
        cJSON *replacement = create_test_item(Data[4]);
        cJSON_ReplaceItemInArray(array, index4, replacement);
    }
    
    // Test with empty array
    cJSON *empty_array = cJSON_CreateArray();
    cJSON_GetArraySize(empty_array);
    cJSON_IsArray(empty_array);
    cJSON_DetachItemFromArray(empty_array, 0);
    cJSON_DeleteItemFromArray(empty_array, 0);
    cJSON_Delete(empty_array);
    
    // Test with non-array item
    cJSON *number = cJSON_CreateNumber(42);
    cJSON_GetArraySize(number);
    cJSON_IsArray(number);
    cJSON_Delete(number);
    
    // Clean up
    cJSON_Delete(array);
    
    return 0;
}