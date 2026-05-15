// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
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

static cJSON* create_dummy_item(int type, const uint8_t* data, size_t size) {
    switch (type % 4) {
        case 0:
            return cJSON_CreateNumber((double)(data[0] % 256));
        case 1:
            return cJSON_CreateString("dummy");
        case 2:
            return cJSON_CreateBool(data[0] % 2);
        case 3:
            return cJSON_CreateNull();
        default:
            return cJSON_CreateNull();
    }
}

static cJSON* create_array_with_items(size_t count, const uint8_t* data, size_t size) {
    cJSON* array = cJSON_CreateArray();
    if (!array) return NULL;
    
    for (size_t i = 0; i < count && i < 10; i++) {
        cJSON* item = create_dummy_item((data[i % size] + i) % 4, data, size);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    return array;
}

static int is_item_in_array(cJSON* array, cJSON* item) {
    if (!array || !item) return 0;
    
    cJSON* current = array->child;
    while (current) {
        if (current == item) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create a test array with some items
    size_t array_size = (Data[0] % 10) + 1;
    cJSON* array = create_array_with_items(array_size, Data, Size);
    if (!array) return 0;
    
    // Generate indices and other parameters from fuzzer data
    int index1 = (int)(Data[1] % 20) - 10;  // Range: -10 to 9
    int index2 = (int)(Data[2] % 20) - 10;  // Range: -10 to 9
    int index3 = (int)(Data[3] % 20) - 10;  // Range: -10 to 9
    
    // Test cJSON_GetArraySize
    int array_len = cJSON_GetArraySize(array);
    
    // Test cJSON_DeleteItemFromArray
    if (array_len > 0) {
        cJSON_DeleteItemFromArray(array, index1 % array_len);
    }
    
    // Test cJSON_DetachItemFromArray
    cJSON* detached = cJSON_DetachItemFromArray(array, index2);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Test cJSON_ReplaceItemInArray
    cJSON* new_item1 = create_dummy_item(Data[4] % 4, Data, Size);
    if (new_item1) {
        cJSON_bool replace_result = cJSON_ReplaceItemInArray(array, index3, new_item1);
        if (!replace_result) {
            cJSON_Delete(new_item1);
        }
    }
    
    // Test cJSON_InsertItemInArray
    cJSON* new_item2 = create_dummy_item(Data[5] % 4, Data, Size);
    if (new_item2) {
        int insert_index = (int)(Data[6] % 20) - 10;
        cJSON_bool insert_result = cJSON_InsertItemInArray(array, insert_index, new_item2);
        if (!insert_result) {
            cJSON_Delete(new_item2);
        }
    }
    
    // Test cJSON_ReplaceItemViaPointer
    if (array_len > 0) {
        cJSON* item_to_replace = cJSON_GetArrayItem(array, 0);
        cJSON* replacement = create_dummy_item(Data[7] % 4, Data, Size);
        if (item_to_replace && replacement) {
            cJSON_bool replace_ptr_result = cJSON_ReplaceItemViaPointer(array, item_to_replace, replacement);
            if (!replace_ptr_result) {
                cJSON_Delete(replacement);
            }
        } else if (replacement) {
            cJSON_Delete(replacement);
        }
    }
    
    // Clean up
    cJSON_Delete(array);
    
    return 0;
}