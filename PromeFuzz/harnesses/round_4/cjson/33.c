// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static cJSON* create_test_item(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    size_t len = size % 100;
    if (len == 0) len = 1;
    
    switch (data[0] % 7) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateFalse();
        case 3:
            return cJSON_CreateNumber((double)size);
        case 4: {
            char* str = (char*)malloc(len + 1);
            if (str) {
                for (size_t i = 0; i < len; i++) {
                    str[i] = data[i % size] % 128;
                }
                str[len] = '\0';
                cJSON* item = cJSON_CreateString(str);
                free(str);
                return item;
            }
            return cJSON_CreateNull();
        }
        case 5: {
            cJSON* array = cJSON_CreateArray();
            if (array && size > 1) {
                for (size_t i = 0; i < (size % 5); i++) {
                    cJSON_AddItemToArray(array, create_test_item(data + i, size - i));
                }
            }
            return array;
        }
        case 6: {
            cJSON* obj = cJSON_CreateObject();
            if (obj && size > 1) {
                for (size_t i = 0; i < (size % 3); i++) {
                    char key[16];
                    snprintf(key, sizeof(key), "key%zu", i);
                    cJSON_AddItemToObject(obj, key, create_test_item(data + i, size - i));
                }
            }
            return obj;
        }
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;
    
    cJSON* array = cJSON_CreateArray();
    if (!array) return 0;
    
    size_t num_items = (Data[0] % 10) + 1;
    
    for (size_t i = 0; i < num_items; i++) {
        cJSON* item = create_test_item(Data + (i % Size), Size - (i % Size));
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    int which = (int)Data[1];
    if (Size > 2) {
        which = (int)(Data[1] * 256 + Data[2]) - 32768;
    }
    
    if (cJSON_GetArraySize(array) > 0) {
        cJSON* detached = cJSON_DetachItemFromArray(array, which);
        if (detached) {
            cJSON_AddItemReferenceToArray(array, detached);
            cJSON_Delete(detached);
        }
    }
    
    cJSON* new_item = create_test_item(Data, Size);
    if (new_item) {
        cJSON_AddItemToArray(array, new_item);
        
        if (cJSON_GetArraySize(array) > 0) {
            int replace_idx = (int)(Data[0] % (cJSON_GetArraySize(array) + 1));
            cJSON_ReplaceItemInArray(array, replace_idx, create_test_item(Data + 1, Size - 1));
            
            if (cJSON_GetArraySize(array) > 0) {
                cJSON* first_item = cJSON_GetArrayItem(array, 0);
                if (first_item) {
                    cJSON_ReplaceItemViaPointer(array, first_item, create_test_item(Data + 2, Size - 2));
                }
            }
            
            int insert_idx = (int)(Data[1] % (cJSON_GetArraySize(array) + 2));
            cJSON_InsertItemInArray(array, insert_idx, create_test_item(Data + 3, Size - 3));
        }
    }
    
    cJSON_Delete(array);
    return 0;
}