// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArrayReference at cJSON.c:2571:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    // Step 1: Prepare environment and invoke target functions in order
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Use input data to generate a number for fuzzing diversity
    double num = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num, Data, sizeof(double));
    } else if (Size > 0) {
        // Fallback if data is too small
        num = (double)(Data[0]);
    }

    cJSON *number = cJSON_CreateNumber(num);
    if (number == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Add the number to the array
    if (!cJSON_AddItemToArray(array, number)) {
        cJSON_Delete(array);
        cJSON_Delete(number);
        return 0;
    }

    // Create an array reference to the number
    // Note: The referenced item must remain valid during the lifetime of the reference
    cJSON *ref_array = cJSON_CreateArrayReference(number);
    if (ref_array == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Step 2: Cleanup
    // Delete the reference array first
    cJSON_Delete(ref_array);
    
    // Delete the main array (which will also clean up the number item)
    cJSON_Delete(array);

    return 0;
}