// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "cJSON.h"

static void cleanup_cjson_tree(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

static cJSON *create_random_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNumber(0);
    }
    
    int type_choice = data[0] % 5;
    cJSON *item = NULL;
    
    switch (type_choice) {
        case 0: // Number
            if (size >= 8) {
                double val = 0;
                memcpy(&val, data, sizeof(double));
                item = cJSON_CreateNumber(val);
            } else {
                item = cJSON_CreateNumber((int)data[0]);
            }
            break;
        case 1: // String
            {
                size_t len = size < 1024 ? size : 1024;
                char *str = (char *)malloc(len + 1);
                if (str) {
                    memcpy(str, data, len);
                    str[len] = '\0';
                    item = cJSON_CreateString(str);
                    free(str);
                } else {
                    item = cJSON_CreateString("default");
                }
            }
            break;
        case 2: // Boolean true
            item = cJSON_CreateTrue();
            break;
        case 3: // Boolean false
            item = cJSON_CreateFalse();
            break;
        case 4: // Null
            item = cJSON_CreateNull();
            break;
        default:
            item = cJSON_CreateNumber(0);
            break;
    }
    
    return item;
}

static cJSON *create_random_array(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return NULL;
    
    int count = 0;
    if (size > 0) {
        count = (data[0] % 10) + 1;
    } else {
        count = 1;
    }
    
    for (int i = 0; i < count; i++) {
        // Ensure we have enough data for the offset
        if (i >= size) break;
        cJSON *item = create_random_item(data + i, size - i);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    return array;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cJSON *array = create_random_array(Data, Size);
    if (!array) {
        return 0;
    }
    
    int array_size = cJSON_GetArraySize(array);
    
    int index1 = (Size > 0) ? (Data[0] % (array_size + 2)) : 0;
    int index2 = (Size > 1) ? (Data[1] % (array_size + 2)) : 0;
    
    // Test 1: cJSON_DeleteItemFromArray
    cJSON *array_copy1 = cJSON_Duplicate(array, 1);
    if (array_copy1) {
        cJSON_DeleteItemFromArray(array_copy1, index1);
        cJSON_DeleteItemFromArray(array_copy1, -1);
        cJSON_DeleteItemFromArray(array_copy1, INT_MAX);
        cleanup_cjson_tree(array_copy1);
    }
    
    // Test 2: cJSON_AddItemReferenceToArray
    cJSON *array_copy2 = cJSON_Duplicate(array, 1);
    if (array_copy2 && array_size > 0) {
        cJSON *item_to_ref = cJSON_GetArrayItem(array_copy2, 0);
        if (item_to_ref) {
            cJSON_AddItemReferenceToArray(array_copy2, item_to_ref);
        }
        if (item_to_ref) {
            cJSON_AddItemReferenceToArray(NULL, item_to_ref);
        }
        cJSON_AddItemReferenceToArray(array_copy2, NULL);
        cleanup_cjson_tree(array_copy2);
    }
    
    // Test 3: cJSON_DetachItemViaPointer
    cJSON *array_copy3 = cJSON_Duplicate(array, 1);
    if (array_copy3 && array_size > 0) {
        cJSON *item_to_detach = cJSON_GetArrayItem(array_copy3, 0);
        if (item_to_detach) {
            cJSON *detached = cJSON_DetachItemViaPointer(array_copy3, item_to_detach);
            if (detached) {
                cJSON_AddItemToArray(array_copy3, detached);
            }
        }
        if (item_to_detach) {
            cJSON_DetachItemViaPointer(NULL, item_to_detach);
        }
        cJSON_DetachItemViaPointer(array_copy3, NULL);
        cleanup_cjson_tree(array_copy3);
    }
    
    // Test 4: cJSON_CreateObjectReference
    cJSON *array_copy4 = cJSON_Duplicate(array, 1);
    if (array_copy4 && array_size > 0) {
        cJSON *child = cJSON_GetArrayItem(array_copy4, 0);
        if (child) {
            cJSON *ref = cJSON_CreateObjectReference(child);
            if (ref) {
                cJSON *new_array = cJSON_CreateArray();
                if (new_array) {
                    cJSON_AddItemToArray(new_array, ref);
                    cJSON_Delete(new_array);
                }
            }
            cJSON_CreateObjectReference(NULL);
        }
        cleanup_cjson_tree(array_copy4);
    }
    
    // Test 5: cJSON_GetArraySize
    cJSON *array_copy5 = cJSON_Duplicate(array, 1);
    if (array_copy5) {
        int size1 = cJSON_GetArraySize(array_copy5);
        int size2 = cJSON_GetArraySize(NULL);
        (void)size1;
        (void)size2;
        cleanup_cjson_tree(array_copy5);
    }
    
    // Test 6: cJSON_InsertItemInArray
    cJSON *array_copy6 = cJSON_Duplicate(array, 1);
    if (array_copy6) {
        cJSON *new_item = create_random_item(Data + 2, Size > 2 ? Size - 2 : 0);
        if (new_item) {
            cJSON_InsertItemInArray(array_copy6, index1, new_item);
            cJSON_InsertItemInArray(array_copy6, -1, new_item);
            cJSON_InsertItemInArray(array_copy6, 0, NULL);
            cJSON_InsertItemInArray(NULL, 0, new_item);
        }
        cleanup_cjson_tree(array_copy6);
    }
    
    cleanup_cjson_tree(array);
    
    return 0;
}