// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a base array to perform operations on
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        return 0;
    }

    // Use the fuzzed data to create an int array for cJSON_CreateIntArray
    int *int_data = NULL;
    int count = 0;
    
    // Determine count based on input size, ensuring safety
    if (Size >= sizeof(int)) {
        count = Data[0] % 100; // Limit to 0-99
        if (count > 0 && Size >= (size_t)(sizeof(int) + count * sizeof(int))) {
            int_data = (int *)malloc(count * sizeof(int));
            if (int_data) {
                memcpy(int_data, Data + sizeof(int), count * sizeof(int));
            }
        }
    }

    // Fuzz cJSON_CreateIntArray
    cJSON *int_array = cJSON_CreateIntArray(int_data, count);
    if (int_array) {
        // Try to add the created int_array to our main array
        cJSON_AddItemToArray(array, int_array);
    }

    free(int_data);

    // Fuzz cJSON_AddItemToArray with a new item
    cJSON *new_item = cJSON_CreateNumber(123);
    if (new_item) {
        cJSON_AddItemToArray(array, new_item);
    }

    // Fuzz cJSON_AddItemReferenceToArray
    // Create a string item. When added via reference, ownership transfers to the array.
    // We do not delete ref_item separately here; it will be cleaned up with the array.
    cJSON *ref_item = cJSON_CreateString("test");
    if (ref_item) {
        cJSON_AddItemReferenceToArray(array, ref_item);
    }

    // Fuzz cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Use the result to avoid compiler warnings

    // Fuzz cJSON_GetArrayItem
    if (size > 0) {
        cJSON *item = cJSON_GetArrayItem(array, 0);
        if (item) {
            // Do nothing, just ensure it doesn't crash
        }
        // Try an out of bounds index
        cJSON *item_out = cJSON_GetArrayItem(array, size + 100);
        (void)item_out;
    }
    // Try with negative index
    cJSON *item_neg = cJSON_GetArrayItem(array, -1);
    (void)item_neg;

    // Fuzz cJSON_DeleteItemFromArray
    if (size > 0) {
        cJSON_DeleteItemFromArray(array, 0);
    }
    // Try deleting out of bounds
    cJSON_DeleteItemFromArray(array, size + 100);

    // Cleanup: Delete the main array, which will free all children including ref_item
    cleanup_cjson(array);

    return 0;
}