// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create an empty array
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        return 0;
    }

    // Create a float array from input data if size is sufficient
    if (Size >= 4) {
        float *floats = (float *)malloc(Size);
        if (floats) {
            memcpy(floats, Data, Size);
            int count = Size / sizeof(float);
            cJSON *floatArray = cJSON_CreateFloatArray(floats, count);
            free(floats); // Free the raw buffer immediately

            if (floatArray && cJSON_IsArray(floatArray)) {
                // Iterate and detach items to transfer ownership to 'array'
                // We must detach before deleting the container to avoid double-free
                cJSON *item = floatArray->child;
                while (item) {
                    // DetachItemFromArray returns the item and removes it from floatArray
                    // We then add it to our main array
                    cJSON *detached = cJSON_DetachItemViaPointer(floatArray, item);
                    if (detached) {
                        cJSON_AddItemToArray(array, detached);
                    }
                    item = floatArray->child; // Re-fetch child as the list head might change
                }
            }
            // Delete the container (floatArray). Its children are now detached or null.
            cJSON_Delete(floatArray);
        }
    }

    // Try to insert a new item at various positions
    cJSON *newItem = cJSON_CreateNumber(1.0);
    if (newItem) {
        int which = 0;
        if (Size >= 2) {
            int arraySize = cJSON_GetArraySize(array);
            int divisor = (arraySize + 1 > 0) ? (arraySize + 1) : 1;
            int val1 = Data[0] % divisor;
            int val2 = Data[1] % 2;
            which = val1 - val2;
        } else if (Size == 1) {
            int arraySize = cJSON_GetArraySize(array);
            int divisor = (arraySize + 1 > 0) ? (arraySize + 1) : 1;
            which = Data[0] % divisor;
        }
        
        if (!cJSON_InsertItemInArray(array, which, newItem)) {
            cJSON_Delete(newItem);
        }
    }

    // Try to insert another item
    cJSON *newItem2 = cJSON_CreateNumber(2.0);
    if (newItem2) {
        int which2 = 0;
        if (Size >= 2) {
            int arraySize = cJSON_GetArraySize(array);
            int divisor = (arraySize + 1 > 0) ? (arraySize + 1) : 1;
            int val1 = Data[0] % divisor;
            int val2 = Data[1] % 2;
            which2 = val1 - val2;
        } else if (Size == 1) {
            int arraySize = cJSON_GetArraySize(array);
            int divisor = (arraySize + 1 > 0) ? (arraySize + 1) : 1;
            which2 = Data[0] % divisor;
        }

        if (!cJSON_InsertItemInArray(array, which2, newItem2)) {
            cJSON_Delete(newItem2);
        }
    }

    // Try to delete an item at a random index
    int currentSize = cJSON_GetArraySize(array);
    if (currentSize > 0) {
        int deleteIndex = 0;
        if (Size >= 3) {
            deleteIndex = Data[2] % (currentSize + 1);
        } else if (Size == 2) {
            deleteIndex = Data[1] % (currentSize + 1);
        } else {
            deleteIndex = 0;
        }
        cJSON_DeleteItemFromArray(array, deleteIndex);
    }

    // Try to add another item
    cJSON *anotherItem = cJSON_CreateString("test");
    if (anotherItem) {
        if (!cJSON_AddItemToArray(array, anotherItem)) {
            cJSON_Delete(anotherItem);
        }
    }

    // Cleanup
    cleanup_cjson(array);

    return 0;
}