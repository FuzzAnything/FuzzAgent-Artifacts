// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Create a null-terminated string from the input data
    // We allocate Size + 1 to ensure a null terminator
    char *safe_string = (char *)malloc(Size + 1);
    if (!safe_string) {
        cJSON_Delete(root);
        return 0;
    }
    memcpy(safe_string, Data, Size);
    safe_string[Size] = '\0';

    // Create and add a string
    cJSON *strItem = cJSON_CreateString(safe_string);
    free(safe_string); // Free the temporary buffer

    if (strItem) {
        cJSON_AddItemToObject(root, "input_string", strItem);
    }

    // Create and add an array
    cJSON *arrItem = cJSON_CreateArray();
    if (arrItem) {
        cJSON_AddItemToObject(root, "input_array", arrItem);

        // Create nested object inside array
        cJSON *nestedObj = cJSON_CreateObject();
        if (nestedObj) {
            cJSON_AddItemToArray(arrItem, nestedObj);

            // Create and add number 1 to nested object
            // Use Data[0] safely since Size > 0
            cJSON *num1 = cJSON_CreateNumber((double)Data[0]);
            if (num1) {
                cJSON_AddItemToObject(nestedObj, "num1", num1);
            }

            // Create and add number 2 to nested object
            double val2 = 0.0;
            if (Size > 1) {
                val2 = (double)Data[1];
            }
            cJSON *num2 = cJSON_CreateNumber(val2);
            if (num2) {
                cJSON_AddItemToObject(nestedObj, "num2", num2);
            }
        }
    }

    // Print the JSON structure
    char *json_str = cJSON_Print(root);
    if (json_str) {
        free(json_str);
    }

    // Cleanup
    cJSON_Delete(root);

    return 0;
}