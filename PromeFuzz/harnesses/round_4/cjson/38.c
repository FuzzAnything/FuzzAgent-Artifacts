// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item(const uint8_t *data, size_t size, int index) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    switch (index % 7) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateFalse();
        case 3:
            return cJSON_CreateNumber((double)(data[0] * 256 + data[1 % size]));
        case 4: {
            char str[32];
            size_t len = size < 31 ? size : 31;
            memcpy(str, data, len);
            str[len] = '\0';
            return cJSON_CreateString(str);
        }
        case 5: {
            cJSON *arr = cJSON_CreateArray();
            if (arr && size > 0) {
                cJSON_AddItemToArray(arr, cJSON_CreateNumber(data[0]));
            }
            return arr;
        }
        case 6: {
            cJSON *obj = cJSON_CreateObject();
            if (obj && size > 0) {
                cJSON_AddItemToObject(obj, "key", cJSON_CreateNumber(data[0]));
            }
            return obj;
        }
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cJSON *array = cJSON_CreateArray();
    if (!array) return 0;
    
    size_t num_items = (Data[0] % 10) + 1;
    cJSON **items = (cJSON**)malloc(num_items * sizeof(cJSON*));
    if (!items) {
        cJSON_Delete(array);
        return 0;
    }
    
    for (size_t i = 0; i < num_items; i++) {
        items[i] = create_test_item(Data + (i % Size), Size - (i % Size), i);
        if (items[i]) {
            cJSON_AddItemToArray(array, items[i]);
        }
    }
    
    if (cJSON_GetArraySize(array) > 0) {
        int which = (int)Data[1] % (cJSON_GetArraySize(array) + 5) - 2;
        
        cJSON *detached = cJSON_DetachItemFromArray(array, which);
        if (detached) {
            cJSON_AddItemReferenceToArray(array, detached);
            cJSON_Delete(detached);
        }
        
        cJSON *new_item = create_test_item(Data, Size, Data[2] % 7);
        if (new_item) {
            cJSON_AddItemToArray(array, new_item);
        }
        
        if (cJSON_GetArraySize(array) > 0) {
            int replace_idx = (int)Data[3] % cJSON_GetArraySize(array);
            cJSON *replace_item = create_test_item(Data + 4, Size - 4, Data[4] % 7);
            if (replace_item) {
                cJSON_ReplaceItemInArray(array, replace_idx, replace_item);
            }
            
            if (cJSON_GetArraySize(array) > 0) {
                int insert_idx = (int)Data[5] % (cJSON_GetArraySize(array) + 3);
                cJSON *insert_item = create_test_item(Data + 6, Size - 6, Data[6] % 7);
                if (insert_item) {
                    cJSON_InsertItemInArray(array, insert_idx, insert_item);
                }
                
                if (cJSON_GetArraySize(array) > 0) {
                    int via_ptr_idx = (int)Data[7] % cJSON_GetArraySize(array);
                    cJSON *item_to_replace = cJSON_GetArrayItem(array, via_ptr_idx);
                    cJSON *replacement = create_test_item(Data + 8, Size - 8, Data[8] % 7);
                    if (item_to_replace && replacement) {
                        cJSON_ReplaceItemViaPointer(array, item_to_replace, replacement);
                    }
                }
            }
        }
    }
    
    cJSON_Delete(array);
    free(items);
    
    return 0;
}