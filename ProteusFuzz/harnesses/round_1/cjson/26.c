// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdbool.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract parameters from input data
    int count = 0;
    int which = 0;
    bool create_double = true;
    bool create_int = true;
    bool insert = true;
    bool delete_item = true;
    bool add_ref = true;
    bool create_empty = true;

    if (Size > 0) count = Data[0] & 0x3F; // Limit count to 63
    if (Size > 1) which = Data[1] & 0x3F; // Limit index to 63
    if (Size > 2) create_double = (Data[2] & 0x01) != 0;
    if (Size > 3) create_int = (Data[3] & 0x01) != 0;
    if (Size > 4) insert = (Data[4] & 0x01) != 0;
    if (Size > 5) delete_item = (Data[5] & 0x01) != 0;
    if (Size > 6) add_ref = (Data[6] & 0x01) != 0;
    if (Size > 7) create_empty = (Data[7] & 0x01) != 0;

    // Generate test arrays
    // Safety cap for max_elements to prevent excessive allocation
    int max_elements = (Size > 8) ? (Data[8] & 0x1F) : 5;
    if (max_elements > 20) max_elements = 20; 

    // Ensure we have enough data bytes to read for the arrays
    // We need at least 9 bytes for the control flags and max_elements
    // plus max_elements bytes for the array data
    size_t required_size = 9 + max_elements;
    if (Size < required_size) {
        // If input is too small, just test with minimal data or skip array creation
        // We will proceed with max_elements = 0 for array creation to avoid overflow
        max_elements = 0;
    }

    double *double_arr = NULL;
    int *int_arr = NULL;
    cJSON *double_array = NULL;
    cJSON *int_array = NULL;
    cJSON *empty_array = NULL;
    cJSON *item_to_insert = NULL;
    cJSON *item_to_ref = NULL;

    if (create_double && max_elements > 0) {
        double_arr = (double *)malloc(max_elements * sizeof(double));
        if (double_arr) {
            for (int i = 0; i < max_elements; i++) {
                // Safe access: i starts at 0, max_elements is capped, and Size check passed
                double_arr[i] = (double)(Data[9 + i] * 100.0 - 50.0);
            }
            double_array = cJSON_CreateDoubleArray(double_arr, max_elements);
            free(double_arr);
        }
    }

    if (create_int && max_elements > 0) {
        int_arr = (int *)malloc(max_elements * sizeof(int));
        if (int_arr) {
            for (int i = 0; i < max_elements; i++) {
                int_arr[i] = (int)(Data[9 + i] * 10 - 5);
            }
            int_array = cJSON_CreateIntArray(int_arr, max_elements);
            free(int_arr);
        }
    }

    if (create_empty) {
        empty_array = cJSON_CreateArray();
    }

    // Create an item to insert and reference
    // Ensure we have data for this
    if (Size > 10) {
        item_to_insert = cJSON_CreateNumber((double)Data[10]);
        if (!item_to_insert) {
            item_to_insert = cJSON_CreateString("test");
        }
    } else {
        item_to_insert = cJSON_CreateNumber(0.0);
    }
    
    if (item_to_insert) {
        item_to_ref = cJSON_CreateNumber((double)Data[0]);
    }

    // Test cJSON_InsertItemInArray
    if (insert && item_to_insert) {
        if (double_array) {
            cJSON_InsertItemInArray(double_array, which, item_to_insert);
            // item_to_insert is now owned by double_array
            item_to_insert = NULL;
        } else if (int_array) {
            cJSON_InsertItemInArray(int_array, which, item_to_insert);
            item_to_insert = NULL;
        } else if (empty_array) {
            cJSON_InsertItemInArray(empty_array, which, item_to_insert);
            item_to_insert = NULL;
        }
    }

    // Test cJSON_DeleteItemFromArray
    if (delete_item) {
        if (double_array) {
            cJSON_DeleteItemFromArray(double_array, which);
        } else if (int_array) {
            cJSON_DeleteItemFromArray(int_array, which);
        } else if (empty_array) {
            cJSON_DeleteItemFromArray(empty_array, which);
        }
    }

    // Test cJSON_AddItemReferenceToArray
    if (add_ref && item_to_ref) {
        if (double_array) {
            cJSON_AddItemReferenceToArray(double_array, item_to_ref);
        } else if (int_array) {
            cJSON_AddItemReferenceToArray(int_array, item_to_ref);
        } else if (empty_array) {
            cJSON_AddItemReferenceToArray(empty_array, item_to_ref);
        }
        // item_to_ref is now referenced, do not delete it separately
        item_to_ref = NULL;
    }

    // Cleanup
    cleanup_cjson(double_array);
    cleanup_cjson(int_array);
    cleanup_cjson(empty_array);
    cleanup_cjson(item_to_insert);
    cleanup_cjson(item_to_ref);

    return 0;
}