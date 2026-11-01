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
#include <stdint.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    double num;
    if (Size >= sizeof(double)) {
        num = *(const double *)Data;
    } else {
        num = (double)(Data[0]);
    }

    cJSON *number = cJSON_CreateNumber(num);
    if (number == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    cJSON_AddItemToArray(array, number);

    cJSON *ref = cJSON_CreateArrayReference(array);
    if (ref == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    cJSON_Delete(ref);
    cJSON_Delete(array);

    return 0;
}