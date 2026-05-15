// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
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

static cJSON* create_test_item(uint8_t type_byte) {
    switch (type_byte % 6) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateBool(type_byte & 1);
        case 2:
            return cJSON_CreateNumber((double)type_byte);
        case 3:
            return cJSON_CreateString("test");
        case 4:
            return cJSON_CreateArray();
        case 5:
            return cJSON_CreateObject();
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create test array with some initial items
    cJSON *array = cJSON_CreateArray();
    if (!array) return 0;
    
    // Add initial items to array
    size_t initial_items = Data[0] % 8;
    for (size_t i = 0; i < initial_items && i + 1 < Size; i++) {
        cJSON *item = create_test_item(Data[i + 1]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    (void)array_size;
    
    // Test with NULL input
    cJSON_GetArraySize(NULL);
    
    // Test cJSON_IsArray
    cJSON_bool is_array = cJSON_IsArray(array);
    (void)is_array;
    cJSON_IsArray(NULL);
    
    // Test on non-array item
    cJSON *non_array = create_test_item(Data[0] % 3 + 3); // Ensure not array type
    if (non_array) {
        cJSON_IsArray(non_array);
        cJSON_Delete(non_array);
    }
    
    // Test array operations if we have enough data
    if (Size >= 4) {
        size_t data_idx = initial_items + 1;
        if (data_idx < Size) {
            // Create items for insertion/replacement
            cJSON *new_item1 = create_test_item(Data[data_idx++ % Size]);
            cJSON *new_item2 = create_test_item(Data[data_idx++ % Size]);
            
            if (new_item1 && new_item2) {
                // Test cJSON_InsertItemInArray
                int insert_idx = (int)Data[data_idx++ % Size];
                cJSON_InsertItemInArray(array, insert_idx, new_item1);
                
                // Test cJSON_ReplaceItemInArray
                int replace_idx = (int)Data[data_idx++ % Size];
                cJSON_ReplaceItemInArray(array, replace_idx, new_item2);
                
                // Test cJSON_DetachItemFromArray
                int detach_idx = (int)Data[data_idx++ % Size];
                cJSON *detached = cJSON_DetachItemFromArray(array, detach_idx);
                if (detached) {
                    cJSON_Delete(detached);
                }
                
                // Test cJSON_DeleteItemFromArray
                int delete_idx = (int)Data[data_idx++ % Size];
                cJSON_DeleteItemFromArray(array, delete_idx);
                
                // Test negative indices
                cJSON_DetachItemFromArray(array, -1);
                cJSON_DeleteItemFromArray(array, -1);
            } else {
                if (new_item1) cJSON_Delete(new_item1);
                if (new_item2) cJSON_Delete(new_item2);
            }
        }
    }
    
    // Test operations on empty array
    cJSON *empty_array = cJSON_CreateArray();
    if (empty_array) {
        cJSON_GetArraySize(empty_array);
        cJSON_IsArray(empty_array);
        cJSON_DetachItemFromArray(empty_array, 0);
        cJSON_DeleteItemFromArray(empty_array, 0);
        
        if (Size > 0) {
            cJSON *test_item = create_test_item(Data[0]);
            if (test_item) {
                cJSON_InsertItemInArray(empty_array, Data[0] % 3, test_item);
                cJSON_ReplaceItemInArray(empty_array, Data[0] % 3, test_item);
            }
        }
        cJSON_Delete(empty_array);
    }
    
    // Cleanup
    cJSON_Delete(array);
    
    return 0;
}