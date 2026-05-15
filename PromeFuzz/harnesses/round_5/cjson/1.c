// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create first object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Prepare string data from fuzzer input
    char *string_data = NULL;
    if (Size > 0) {
        string_data = (char *)malloc(Size + 1);
        if (!string_data) {
            cJSON_Delete(root);
            return 0;
        }
        memcpy(string_data, Data, Size);
        string_data[Size] = '\0';
    } else {
        string_data = strdup("");
        if (!string_data) {
            cJSON_Delete(root);
            return 0;
        }
    }
    
    // Add string to object
    cJSON_AddStringToObject(root, "test_string", string_data);
    
    // Add array to object
    cJSON *array = cJSON_AddArrayToObject(root, "test_array");
    
    // Create second object
    cJSON *child = cJSON_CreateObject();
    
    // Add numbers to child object
    // Use fuzzer data to generate numbers - use first 8 bytes for first number,
    // next 8 bytes for second number (if available)
    double num1 = 0.0;
    double num2 = 0.0;
    
    if (Size >= sizeof(double)) {
        memcpy(&num1, Data, sizeof(double));
    }
    if (Size >= 2 * sizeof(double)) {
        memcpy(&num2, Data + sizeof(double), sizeof(double));
    }
    
    cJSON_AddNumberToObject(child, "number1", num1);
    cJSON_AddNumberToObject(child, "number2", num2);
    
    // Add child to array
    if (array && child) {
        cJSON_AddItemToArray(array, child);
    } else {
        if (child) cJSON_Delete(child);
    }
    
    // Print the JSON
    char *printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }
    
    // Cleanup
    free(string_data);
    cJSON_Delete(root);
    
    return 0;
}