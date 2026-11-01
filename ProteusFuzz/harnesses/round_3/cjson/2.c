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
#include "cJSON.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Create the root object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Step 2: Add a string to the object
    // Use input data as key and value, ensuring null termination
    char key_str[256];
    char val_str[256];
    size_t key_len = (Size < 128) ? Size : 128;
    size_t val_len = (Size < 128) ? Size : 128;
    
    // Ensure we don't read out of bounds
    if (key_len > 0) {
        memcpy(key_str, Data, key_len);
        key_str[key_len] = '\0';
    } else {
        strcpy(key_str, "key");
    }
    
    if (val_len > 0) {
        memcpy(val_str, Data, val_len);
        val_str[val_len] = '\0';
    } else {
        strcpy(val_str, "value");
    }

    cJSON_AddStringToObject(root, key_str, val_str);

    // Step 3: Add an array to the object
    cJSON *arr = cJSON_AddArrayToObject(root, "array_key");
    if (arr == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    // Step 4: Create a nested object
    cJSON *nested_obj = cJSON_CreateObject();
    if (nested_obj == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    // Step 5 & 6: Add numbers to the nested object
    // Use input bytes to generate double values
    double num1 = 0.0;
    double num2 = 0.0;
    
    if (Size >= 2) {
        num1 = (double)Data[0] / 255.0;
        num2 = (double)Data[1] / 255.0;
    }

    cJSON_AddNumberToObject(nested_obj, "num1", num1);
    cJSON_AddNumberToObject(nested_obj, "num2", num2);

    // Step 7: Add the nested object to the array
    cJSON_AddItemToArray(arr, nested_obj);

    // Step 8: Print the JSON
    char *json_str = cJSON_Print(root);
    if (json_str != NULL) {
        free(json_str);
    }

    // Step 9: Cleanup
    cJSON_Delete(root);

    return 0;
}