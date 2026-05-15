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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Step 1: Create array
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Step 2: Create number using fuzz data
    double num = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num, Data, sizeof(double));
    } else if (Size > 0) {
        // Use available bytes to form a simple number
        num = (double)Data[0];
    }
    cJSON *number_item = cJSON_CreateNumber(num);
    if (number_item == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Step 3: Add number to array
    cJSON_AddItemToArray(array, number_item);

    // Step 4: Create array reference
    cJSON *array_ref = cJSON_CreateArrayReference(array);
    if (array_ref == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Step 5: Delete original array
    cJSON_Delete(array);

    // Step 6: Delete array reference
    cJSON_Delete(array_ref);

    return 0;
}