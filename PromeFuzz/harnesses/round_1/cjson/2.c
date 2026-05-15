// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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
#include "cJSON.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create first object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Prepare string data from fuzzer input
    char *string_data = NULL;
    if (Size > 0) {
        string_data = (char *)malloc(Size + 1);
        if (string_data != NULL) {
            memcpy(string_data, Data, Size);
            string_data[Size] = '\0';
        }
    } else {
        string_data = strdup("");
    }

    // Add string to object
    cJSON_AddStringToObject(root, "test_key", string_data != NULL ? string_data : "");

    // Add array to object
    cJSON *array = cJSON_AddArrayToObject(root, "test_array");
    
    // Create second object
    cJSON *child_obj = cJSON_CreateObject();
    if (child_obj != NULL) {
        // Add numbers to second object using fuzzer-derived values
        double num1 = 0.0;
        double num2 = 0.0;
        
        if (Size >= sizeof(double)) {
            memcpy(&num1, Data, sizeof(double));
        }
        if (Size >= 2 * sizeof(double)) {
            memcpy(&num2, Data + sizeof(double), sizeof(double));
        }
        
        cJSON_AddNumberToObject(child_obj, "number1", num1);
        cJSON_AddNumberToObject(child_obj, "number2", num2);
        
        // Add second object to array
        cJSON_AddItemToArray(array, child_obj);
    }

    // Print JSON
    char *printed = cJSON_Print(root);
    if (printed != NULL) {
        free(printed);
    }

    // Cleanup
    if (string_data != NULL) {
        free(string_data);
    }
    cJSON_Delete(root);
    
    return 0;
}