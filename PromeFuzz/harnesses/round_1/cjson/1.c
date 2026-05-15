// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
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
    // Create a null-terminated string from fuzzer input for string creation
    char *input_string = NULL;
    if (Size > 0) {
        input_string = (char *)malloc(Size + 1);
        if (input_string) {
            memcpy(input_string, Data, Size);
            input_string[Size] = '\0';
        }
    } else {
        // If no data, use a default string
        input_string = strdup("fuzz");
    }
    
    if (!input_string) {
        return 0;
    }
    
    // 1. Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        free(input_string);
        return 0;
    }
    
    // 2. Create string item
    cJSON *string_item = cJSON_CreateString(input_string);
    if (!string_item) {
        cJSON_Delete(root);
        free(input_string);
        return 0;
    }
    
    // 3. Add string item to root object
    cJSON_AddItemToObject(root, "string_key", string_item);
    
    // 4. Create array
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        cJSON_Delete(root);
        free(input_string);
        return 0;
    }
    
    // 5. Add array to root object
    cJSON_AddItemToObject(root, "array_key", array);
    
    // 6. Create inner object
    cJSON *inner_object = cJSON_CreateObject();
    if (!inner_object) {
        cJSON_Delete(root);
        free(input_string);
        return 0;
    }
    
    // 7. Add inner object to array
    cJSON_AddItemToArray(array, inner_object);
    
    // 8. Create first number
    double num1 = (Size > 0) ? (double)(*Data) : 42.0;
    cJSON *number1 = cJSON_CreateNumber(num1);
    if (!number1) {
        cJSON_Delete(root);
        free(input_string);
        return 0;
    }
    
    // 9. Add first number to inner object
    cJSON_AddItemToObject(inner_object, "number1_key", number1);
    
    // 10. Create second number
    double num2 = (Size > 1) ? (double)(Data[Size-1]) : 3.14;
    cJSON *number2 = cJSON_CreateNumber(num2);
    if (!number2) {
        cJSON_Delete(root);
        free(input_string);
        return 0;
    }
    
    // 11. Add second number to inner object
    cJSON_AddItemToObject(inner_object, "number2_key", number2);
    
    // 12. Print the JSON structure
    char *printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }
    
    // 13. Clean up
    cJSON_Delete(root);
    free(input_string);
    
    return 0;
}