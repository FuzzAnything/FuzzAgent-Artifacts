// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_json(cJSON *obj) {
    if (obj != NULL) {
        cJSON_Delete(obj);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 2) {
        return 0;
    }

    double num1 = *(const double *)Data;
    double num2 = *(const double *)(Data + sizeof(double));

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    cJSON *item1 = cJSON_CreateNumber(num1);
    if (item1 == NULL) {
        cleanup_json(obj);
        return 0;
    }

    cJSON *item2 = cJSON_CreateNumber(num2);
    if (item2 == NULL) {
        cJSON_Delete(item1);
        cleanup_json(obj);
        return 0;
    }

    if (!cJSON_AddItemToObject(obj, "key1", item1)) {
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cleanup_json(obj);
        return 0;
    }

    if (!cJSON_ReplaceItemInObject(obj, "key1", item2)) {
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cleanup_json(obj);
        return 0;
    }

    cleanup_json(obj);
    return 0;
}