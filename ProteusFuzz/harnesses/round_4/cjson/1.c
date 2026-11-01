// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void safe_cleanup(cJSON *obj, char *str) {
    if (obj) {
        cJSON_Delete(obj);
    }
    if (str) {
        free(str);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Ensure the input data is treated as a null-terminated string
    // by creating a temporary buffer with a guaranteed null terminator.
    char *safe_str = (char *)malloc(Size + 1);
    if (!safe_str) {
        safe_cleanup(root, NULL);
        return 0;
    }
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';

    cJSON *str_item = cJSON_CreateString(safe_str);
    free(safe_str); // Free the temporary buffer

    if (!str_item) {
        safe_cleanup(root, NULL);
        return 0;
    }
    cJSON_AddItemToObject(root, "fuzz_string", str_item);

    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        safe_cleanup(root, NULL);
        return 0;
    }
    cJSON_AddItemToObject(root, "fuzz_array", arr);

    cJSON *nested_obj = cJSON_CreateObject();
    if (!nested_obj) {
        safe_cleanup(root, NULL);
        return 0;
    }
    cJSON_AddItemToArray(arr, nested_obj);

    // Safely access Data elements
    double val1 = 0.0;
    double val2 = 0.0;
    
    if (Size > 0) {
        val1 = (double)Data[0] * 100.0;
    }
    if (Size > 1) {
        val2 = (double)Data[1] - 50.0;
    }

    cJSON *num1 = cJSON_CreateNumber(val1);
    if (num1) {
        cJSON_AddItemToObject(nested_obj, "num1", num1);
    }

    cJSON *num2 = cJSON_CreateNumber(val2);
    if (num2) {
        cJSON_AddItemToObject(nested_obj, "num2", num2);
    }

    char *json_str = cJSON_Print(root);
    if (json_str) {
        free(json_str);
    }

    safe_cleanup(root, NULL);
    return 0;
}