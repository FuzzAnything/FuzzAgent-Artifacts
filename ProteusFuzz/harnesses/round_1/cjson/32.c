// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        return 0;
    }

    // Test cJSON_IsArray
    cJSON_IsArray(array);
    cJSON_IsArray(NULL);

    // Test cJSON_AddItemToArray with various inputs
    cJSON *item1 = cJSON_CreateNumber(1.0);
    if (item1) {
        cJSON_AddItemToArray(array, item1);
    }

    // Test cJSON_DeleteItemFromArray
    if (Size > 0) {
        int currentSize = cJSON_GetArraySize(array);
        int index = Data[0] % (currentSize + 2);
        cJSON_DeleteItemFromArray(array, index);
        cJSON_DeleteItemFromArray(array, -1);
        cJSON_DeleteItemFromArray(array, 1000);
    }

    // Test cJSON_InsertItemInArray
    cJSON *item2 = cJSON_CreateNumber(2.0);
    if (item2) {
        if (Size > 1) {
            int insertIndex = (int)Data[1];
            cJSON_InsertItemInArray(array, insertIndex, item2);
        } else {
            cJSON_InsertItemInArray(array, 0, item2);
        }
        cJSON_InsertItemInArray(array, -1, NULL);
        cJSON_InsertItemInArray(NULL, 0, item2);
    }

    // Test cJSON_CreateFloatArray
    if (Size > 2) {
        int count = Data[2] % 100;
        if (count > 0) {
            float *floats = (float *)malloc(count * sizeof(float));
            if (floats) {
                for (int i = 0; i < count; i++) {
                    if (3 + i < Size) {
                        floats[i] = (float)(Data[3 + i] % 100) / 10.0f;
                    } else {
                        floats[i] = 0.0f;
                    }
                }
                cJSON *floatArray = cJSON_CreateFloatArray(floats, count);
                if (floatArray) {
                    cJSON_IsArray(floatArray);
                    cleanup_cjson(floatArray);
                }
                free(floats);
            }
        }
    }

    // Cleanup
    cleanup_cjson(array);

    return 0;
}