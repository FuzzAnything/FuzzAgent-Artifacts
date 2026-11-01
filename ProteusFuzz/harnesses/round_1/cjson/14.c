// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    double num1 = 0.0;
    double num2 = 0.0;

    if (Size >= 8) {
        memcpy(&num1, Data, sizeof(double));
    }
    if (Size >= 16) {
        memcpy(&num2, Data + 8, sizeof(double));
    }

    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    cJSON *item1 = cJSON_CreateNumber(num1);
    if (!item1) {
        cJSON_Delete(obj);
        return 0;
    }

    cJSON *item2 = cJSON_CreateNumber(num2);
    if (!item2) {
        cJSON_Delete(item1);
        cJSON_Delete(obj);
        return 0;
    }

    const char *key1 = "key1";
    const char *key2 = "key2";

    if (cJSON_AddItemToObject(obj, key1, item1)) {
        if (cJSON_AddItemToObject(obj, key2, item2)) {
            cJSON *item3 = cJSON_CreateNumber((double)Size);
            if (item3) {
                cJSON_ReplaceItemInObject(obj, key1, item3);
            }
        }
    }

    cJSON_Delete(obj);

    return 0;
}