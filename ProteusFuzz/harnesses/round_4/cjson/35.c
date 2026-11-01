// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_cjson(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a base array to perform operations on
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Parse input data to generate test values
    int op = Data[0] % 6;
    int index = (Size > 1) ? (int)(Data[1] % 100) : 0;
    int count = (Size > 2) ? (int)(Data[2] % 50) : 0;

    // Prepare temporary arrays for CreateIntArray and CreateDoubleArray
    int int_buf[50];
    double double_buf[50];
    for (int i = 0; i < 50; i++) {
        int_buf[i] = (int)(Data[(i + 3) % Size]);
        double_buf[i] = (double)(Data[(i + 3) % Size]) * 0.1;
    }

    switch (op) {
        case 0: {
            // Test cJSON_DeleteItemFromArray
            // First, populate array with some items to delete
            for (int i = 0; i < 5 && i < count; i++) {
                cJSON_AddItemToArray(array, cJSON_CreateNumber(i));
            }
            // Ensure index is within bounds for safety, though API handles out of bounds
            int safe_index = (index >= 0 && index < cJSON_GetArraySize(array)) ? index : 0;
            cJSON_DeleteItemFromArray(array, safe_index);
            break;
        }
        case 1: {
            // Test cJSON_CreateArray (already done at start, but test again)
            cJSON *temp = cJSON_CreateArray();
            if (temp != NULL) {
                cJSON_Delete(temp);
            }
            break;
        }
        case 2: {
            // Test cJSON_AddItemReferenceToArray
            cJSON *item = cJSON_CreateNumber(123);
            if (item != NULL) {
                cJSON_AddItemReferenceToArray(array, item);
                // Note: Do not delete 'item' separately as it's now owned by array
            }
            break;
        }
        case 3: {
            // Test cJSON_CreateDoubleArray
            cJSON *double_arr = cJSON_CreateDoubleArray(double_buf, count);
            if (double_arr != NULL) {
                cJSON_Delete(double_arr);
            }
            break;
        }
        case 4: {
            // Test cJSON_InsertItemInArray
            cJSON *new_item = cJSON_CreateNumber(999);
            if (new_item != NULL) {
                // Insert at index or append if out of bounds
                cJSON_InsertItemInArray(array, index, new_item);
                // new_item is now owned by array, don't delete separately
            }
            break;
        }
        case 5: {
            // Test cJSON_CreateIntArray
            cJSON *int_arr = cJSON_CreateIntArray(int_buf, count);
            if (int_arr != NULL) {
                cJSON_Delete(int_arr);
            }
            break;
        }
    }

    cleanup_cjson(array);
    return 0;
}