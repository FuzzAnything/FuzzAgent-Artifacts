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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Create an array
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Create a number using fuzzer input
    double num = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num, Data, sizeof(double));
    } else if (Size > 0) {
        num = (double)Data[0];
    }

    cJSON *number = cJSON_CreateNumber(num);
    if (number == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Add number to array
    cJSON_AddItemToArray(array, number);

    // Create array reference
    cJSON *array_ref = cJSON_CreateArrayReference(array);
    if (array_ref == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Delete array reference (doesn't delete the referenced array)
    cJSON_Delete(array_ref);

    // Delete the original array (which also deletes the number)
    cJSON_Delete(array);

    return 0;
}