// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cJSON* create_test_item(int type, const uint8_t* data, size_t size) {
    cJSON* item = NULL;
    switch (type % 6) {
        case 0:
            item = cJSON_CreateNull();
            break;
        case 1:
            item = cJSON_CreateTrue();
            break;
        case 2:
            item = cJSON_CreateFalse();
            break;
        case 3: {
            char* str = (char*)malloc(size + 1);
            if (str) {
                memcpy(str, data, size);
                str[size] = '\0';
                item = cJSON_CreateString(str);
                free(str);
            }
            break;
        }
        case 4:
            item = cJSON_CreateNumber((double)size);
            break;
        case 5:
            item = cJSON_CreateArray();
            break;
    }
    return item;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy array for testing
    cJSON* array = cJSON_CreateArray();
    if (!array) return 0;
    
    // Populate array with some items
    for (int i = 0; i < 5 && i < (int)Size; i++) {
        cJSON* item = create_test_item(Data[i] + i, Data, Size);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    cJSON_GetArraySize(NULL); // Test with NULL
    
    // Test cJSON_AddItemReferenceToArray
    if (array_size > 0) {
        cJSON* first_item = cJSON_GetArrayItem(array, 0);
        if (first_item) {
            cJSON_AddItemReferenceToArray(array, first_item);
        }
    }
    
    // Test cJSON_ReplaceItemInArray
    if (array_size > 0) {
        int index = Data[0] % (array_size + 2); // Include out-of-bounds indices
        cJSON* new_item = create_test_item(Data[1], Data, Size);
        if (new_item) {
            cJSON_ReplaceItemInArray(array, index, new_item);
        }
    }
    
    // Test cJSON_DetachItemFromArray
    if (array_size > 0) {
        int index = Data[1] % (array_size + 2); // Include out-of-bounds indices
        cJSON* detached = cJSON_DetachItemFromArray(array, index);
        if (detached) {
            cJSON_Delete(detached);
        }
    }
    
    // Test cJSON_ReplaceItemViaPointer
    if (array_size > 1) {
        cJSON* second_item = cJSON_GetArrayItem(array, 1);
        cJSON* third_item = cJSON_GetArrayItem(array, 2);
        if (second_item && third_item) {
            cJSON_ReplaceItemViaPointer(array, second_item, third_item);
        }
    }
    
    // Test cJSON_AddItemToArray with new items
    for (int i = 0; i < 3 && i < (int)Size; i++) {
        cJSON* new_item = create_test_item(Data[i] + 10, Data, Size);
        if (new_item) {
            cJSON_AddItemToArray(array, new_item);
        }
    }
    
    // Cleanup
    cJSON_Delete(array);
    
    return 0;
}