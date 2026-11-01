// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void safe_cleanup(cJSON *obj, char *json_str) {
    if (obj) {
        cJSON_Delete(obj);
    }
    if (json_str) {
        free(json_str);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Prepare key and string from input, ensuring null termination
    size_t key_len = (Size > 10) ? 10 : Size;
    char key[11] = {0};
    if (Size > 0) {
        memcpy(key, Data, key_len);
    }

    size_t str_len = (Size > 20) ? 20 : Size;
    char str_val[21] = {0};
    if (Size > 0) {
        memcpy(str_val, Data, str_len);
    }

    // 1. cJSON_AddStringToObject
    cJSON_AddStringToObject(root, key, str_val);

    // 2. cJSON_AddArrayToObject
    cJSON *arr = cJSON_AddArrayToObject(root, key);
    if (!arr) {
        safe_cleanup(root, NULL);
        return 0;
    }

    // 3. cJSON_CreateObject (nested)
    cJSON *nested_obj = cJSON_CreateObject();
    if (!nested_obj) {
        safe_cleanup(root, NULL);
        return 0;
    }

    // 4. cJSON_AddNumberToObject (first)
    double num1 = (Size > 0) ? (double)Data[0] : 0.0;
    cJSON_AddNumberToObject(nested_obj, key, num1);

    // 5. cJSON_AddNumberToObject (second)
    double num2 = (Size > 1) ? (double)Data[1] : 0.0;
    cJSON_AddNumberToObject(nested_obj, key, num2);

    // 6. cJSON_AddItemToArray
    cJSON_AddItemToArray(arr, nested_obj);

    // 7. cJSON_Print
    char *json_str = cJSON_Print(root);

    // Cleanup
    safe_cleanup(root, json_str);

    return 0;
}