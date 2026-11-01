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
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Create a string from input data, ensuring it is null-terminated
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        cJSON_Delete(root);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    cJSON *str_item = cJSON_CreateString(input_str);
    free(input_str);

    if (str_item != NULL) {
        cJSON_AddItemToObject(root, "input_string", str_item);
    }

    cJSON *array = cJSON_CreateArray();
    if (array != NULL) {
        cJSON_AddItemToObject(root, "array", array);

        cJSON *nested_obj = cJSON_CreateObject();
        if (nested_obj != NULL) {
            cJSON_AddItemToArray(array, nested_obj);

            // Create number from first byte
            double num1 = (double)Data[0];
            cJSON *num_item1 = cJSON_CreateNumber(num1);
            if (num_item1 != NULL) {
                cJSON_AddItemToObject(nested_obj, "number1", num_item1);
            }

            // Create number from second byte (or 0 if size is 1)
            double num2 = (Size > 1) ? (double)Data[1] : 0.0;
            cJSON *num_item2 = cJSON_CreateNumber(num2);
            if (num_item2 != NULL) {
                cJSON_AddItemToObject(nested_obj, "number2", num_item2);
            }
        }
    }

    char *json_str = cJSON_Print(root);
    if (json_str != NULL) {
        free(json_str);
    }

    cJSON_Delete(root);

    return 0;
}