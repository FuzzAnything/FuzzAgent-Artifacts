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
#include "cJSON.h"

static void safe_cleanup(cJSON *item, char *json_str) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
    if (json_str != NULL) {
        free(json_str);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root_obj = cJSON_CreateObject();
    if (root_obj == NULL) {
        return 0;
    }

    // Prepare string data for AddStringToObject
    // Ensure null termination for string value
    char *str_value = (char *)malloc(Size + 1);
    if (str_value == NULL) {
        safe_cleanup(root_obj, NULL);
        return 0;
    }
    memcpy(str_value, Data, Size);
    str_value[Size] = '\0';

    // 1. cJSON_AddStringToObject
    cJSON_AddStringToObject(root_obj, "fuzz_string", str_value);
    free(str_value);

    // 2. cJSON_AddArrayToObject
    cJSON *arr = cJSON_AddArrayToObject(root_obj, "fuzz_array");
    if (arr == NULL) {
        safe_cleanup(root_obj, NULL);
        return 0;
    }

    // 3. cJSON_CreateObject (nested)
    cJSON *nested_obj = cJSON_CreateObject();
    if (nested_obj == NULL) {
        safe_cleanup(root_obj, NULL);
        return 0;
    }

    // 4. cJSON_AddNumberToObject (nested)
    double num1 = 0.0;
    double num2 = 0.0;
    
    // Safely extract numbers from fuzz data only if enough bytes are available
    if (Size >= 8) {
        memcpy(&num1, Data, 8);
    }
    // Ensure we don't read past the buffer for the second number
    if (Size >= 16) {
        memcpy(&num2, Data + 8, 8);
    } else if (Size > 8) {
        // If we have some bytes but not enough for a full double, 
        // we could pad or just use 0. Here we use 0 for safety.
        num2 = 0.0;
    }

    cJSON_AddNumberToObject(nested_obj, "num1", num1);
    cJSON_AddNumberToObject(nested_obj, "num2", num2);

    // 5. cJSON_AddItemToArray
    cJSON_AddItemToArray(arr, nested_obj);

    // 6. cJSON_Print
    char *json_str = cJSON_Print(root_obj);

    // 7. cJSON_Delete
    safe_cleanup(root_obj, json_str);

    return 0;
}