// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create the root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Prepare a string from input for cJSON_AddStringToObject
    // Ensure null termination
    char *input_str = NULL;
    if (Size > 0) {
        input_str = (char *)malloc(Size + 1);
        if (input_str) {
            memcpy(input_str, Data, Size);
            input_str[Size] = '\0';
        }
    }

    // Add string to object
    cJSON_AddStringToObject(root, "input_string", input_str ? input_str : "");

    // Add array to object
    cJSON *arr = cJSON_AddArrayToObject(root, "items_array");
    if (!arr) {
        cJSON_Delete(root);
        free(input_str);
        return 0;
    }

    // Create a nested object
    cJSON *nested_obj = cJSON_CreateObject();
    if (!nested_obj) {
        cJSON_Delete(root);
        free(input_str);
        return 0;
    }

    // Add numbers to nested object
    // Use input data to derive numbers if possible, otherwise use defaults
    double num1 = 0.0;
    double num2 = 0.0;
    if (Size >= 1) num1 = (double)Data[0];
    if (Size >= 2) num2 = (double)Data[1];

    cJSON_AddNumberToObject(nested_obj, "number1", num1);
    cJSON_AddNumberToObject(nested_obj, "number2", num2);

    // Add nested object to array
    cJSON_AddItemToArray(arr, nested_obj);

    // Print the JSON
    char *json_str = cJSON_Print(root);
    if (json_str) {
        free(json_str);
    }

    // Cleanup
    cJSON_Delete(root);
    free(input_str);

    return 0;
}