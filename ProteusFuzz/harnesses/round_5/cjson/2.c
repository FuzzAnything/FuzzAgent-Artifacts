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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Create a string from input data, ensuring it's null-terminated
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        cJSON_Delete(root);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    cJSON *str_item = cJSON_CreateString(input_str);
    free(input_str);
    if (str_item) {
        cJSON_AddItemToObject(root, "input_string", str_item);
    }

    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToObject(root, "nested_array", array);

        cJSON *inner_obj = cJSON_CreateObject();
        if (inner_obj) {
            cJSON_AddItemToArray(array, inner_obj);

            // Add number to inner object
            double num_val = 0.0;
            if (Size > 0) {
                num_val = (double)(Data[0] & 0xFF);
                if (Size > 1) {
                    num_val += (double)(Data[1] & 0xFF) / 256.0;
                }
            }
            cJSON *num_item = cJSON_CreateNumber(num_val);
            if (num_item) {
                cJSON_AddItemToObject(inner_obj, "number_field", num_item);
            }

            // Add another number to inner object
            double num_val2 = 0.0;
            if (Size > 2) {
                num_val2 = (double)(Data[2] & 0xFF);
                if (Size > 3) {
                    num_val2 += (double)(Data[3] & 0xFF) / 256.0;
                }
            }
            cJSON *num_item2 = cJSON_CreateNumber(num_val2);
            if (num_item2) {
                cJSON_AddItemToObject(inner_obj, "number_field2", num_item2);
            }
        }
    }

    // Serialize to string
    char *json_str = cJSON_Print(root);
    if (json_str) {
        free(json_str);
    }

    // Cleanup
    cJSON_Delete(root);

    return 0;
}