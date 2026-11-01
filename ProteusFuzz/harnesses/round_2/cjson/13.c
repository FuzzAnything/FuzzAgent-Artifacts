// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArrayReference at cJSON.c:2571:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_cjson_tree(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *array = NULL;
    cJSON *number = NULL;
    cJSON *ref_array = NULL;

    // Step 1: Create an empty array
    array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Step 2: Create a number node from input data
    // Use the first byte as a seed for a double to ensure variety
    double num_value = 0.0;
    if (Size >= 1) {
        num_value = (double)Data[0];
        // Add some variation based on more bytes if available
        if (Size >= 2) {
            num_value += (double)Data[1] / 10.0;
        }
        if (Size >= 3) {
            num_value += (double)Data[2] / 100.0;
        }
        // Also try negative values
        if (Size >= 4) {
            if (Data[3] & 1) {
                num_value = -num_value;
            }
        }
    }
    number = cJSON_CreateNumber(num_value);
    if (number == NULL) {
        cleanup_cjson_tree(array);
        return 0;
    }

    // Step 3: Add the number to the array
    if (!cJSON_AddItemToArray(array, number)) {
        // If addition fails, clean up the created items
        cJSON_Delete(number);
        cleanup_cjson_tree(array);
        return 0;
    }

    // Step 4: Create an array reference to the number
    // Note: cJSON_CreateArrayReference creates a new array node referencing 'number'
    // This effectively wraps the 'number' in a new array context as a reference.
    ref_array = cJSON_CreateArrayReference(number);
    if (ref_array == NULL) {
        // If reference creation fails, we still need to clean up the main tree
        // Note: number is owned by 'array', so deleting 'array' will free 'number'
        cleanup_cjson_tree(array);
        return 0;
    }

    // Step 5: Delete the reference array first
    // This should not free 'number' because it is a reference
    cJSON_Delete(ref_array);

    // Step 6: Delete the main array
    // This will free 'number' and the 'array' structure itself
    cJSON_Delete(array);

    return 0;
}