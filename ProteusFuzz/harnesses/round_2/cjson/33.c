// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
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

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use input data to drive logic
    int op = Data[0] % 6; // 0-5 for the 6 functions
    int index = (Size > 1) ? (int)(Data[1] % 100) : 0;
    int count = (Size > 2) ? (int)(Data[2] % 50) : 0;
    
    // Ensure non-negative count for array creation
    if (count < 0) count = 0;
    
    // Prepare data for array creation functions
    int *int_data = NULL;
    double *double_data = NULL;
    
    // Check if we have enough data to fill the arrays
    size_t available_data = (Size > 3) ? (Size - 3) : 0;
    if (count > (int)available_data) {
        count = (int)available_data;
    }

    if (count > 0) {
        int_data = (int *)malloc(count * sizeof(int));
        double_data = (double *)malloc(count * sizeof(double));
        if (!int_data || !double_data) {
            free(int_data);
            free(double_data);
            return 0;
        }
        for (int i = 0; i < count; i++) {
            int_data[i] = (int)Data[3 + i];
            double_data[i] = (double)Data[3 + i];
        }
    }

    cJSON *array = NULL;
    cJSON *item = NULL;
    cJSON *newitem = NULL;
    cJSON_bool result = cJSON_False;

    switch (op) {
        case 0: // cJSON_CreateArray
            array = cJSON_CreateArray();
            if (array) {
                // Try to add a reference to itself (edge case)
                cJSON_AddItemReferenceToArray(array, array);
            }
            break;

        case 1: // cJSON_CreateIntArray
            if (int_data && count > 0) {
                array = cJSON_CreateIntArray(int_data, count);
            }
            break;

        case 2: // cJSON_CreateDoubleArray
            if (double_data && count > 0) {
                array = cJSON_CreateDoubleArray(double_data, count);
            }
            break;

        case 3: // cJSON_InsertItemInArray
            array = cJSON_CreateArray();
            if (array) {
                newitem = cJSON_CreateNumber((int)Data[0]);
                if (newitem) {
                    result = cJSON_InsertItemInArray(array, index, newitem);
                    // newitem is now owned by array, don't delete separately
                }
            }
            break;

        case 4: // cJSON_AddItemReferenceToArray
            array = cJSON_CreateArray();
            if (array) {
                item = cJSON_CreateNumber((int)Data[0]);
                if (item) {
                    result = cJSON_AddItemReferenceToArray(array, item);
                    // item is still owned by caller, but array holds a reference
                    // We must delete item separately if we want to clean up fully
                }
            }
            break;

        case 5: // cJSON_DeleteItemFromArray
            array = cJSON_CreateArray();
            if (array) {
                // Add some items first
                for (int i = 0; i < count && i < 10; i++) {
                    cJSON *temp = cJSON_CreateNumber(i);
                    if (temp) {
                        cJSON_AddItemToArray(array, temp);
                    }
                }
                // Delete item at index
                cJSON_DeleteItemFromArray(array, index);
            }
            break;
    }

    // Cleanup
    cleanup_cjson(array);
    // For case 4, item is not owned by array, so we must delete it if it exists
    if (op == 4 && item) {
        cleanup_cjson(item);
    }
    // For case 3, newitem is owned by array, so no need to delete separately if array is deleted
    // But if array creation failed, newitem might be leaked?
    // In case 3: if array is NULL, newitem is created but not added. We need to delete it.
    if (op == 3 && !array && newitem) {
        cleanup_cjson(newitem);
    }

    free(int_data);
    free(double_data);

    return 0;
}