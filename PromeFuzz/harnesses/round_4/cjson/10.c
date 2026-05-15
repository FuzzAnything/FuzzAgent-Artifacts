// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArrayReference at cJSON.c:2571:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Step 1: Create an array
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Step 2: Create a number using fuzzer data
    double num_value = 0.0;
    if (Size >= sizeof(double)) {
        // Use first 8 bytes of fuzzer data as double value
        memcpy(&num_value, Data, sizeof(double));
    } else if (Size > 0) {
        // Use available bytes to create a small number
        num_value = (double)Data[0];
    }
    
    cJSON *number_item = cJSON_CreateNumber(num_value);
    if (number_item == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Step 3: Add the number to the array
    cJSON_AddItemToArray(array, number_item);

    // Step 4: Create an array reference to the array
    cJSON *array_ref = cJSON_CreateArrayReference(array);
    if (array_ref == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Step 5 & 6: Delete both the array and the array reference
    // Note: array_ref is a reference, so deleting array first is safe
    cJSON_Delete(array);
    cJSON_Delete(array_ref);

    return 0;
}