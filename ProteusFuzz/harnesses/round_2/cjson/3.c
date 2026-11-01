// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create the root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Create a string node
    // Ensure we have a null-terminated string by limiting size to Size-1
    size_t str_len = Size > 0 ? Size - 1 : 0;
    char *fuzz_str = (char *)malloc(str_len + 1);
    if (!fuzz_str) {
        cJSON_Delete(root);
        return 0;
    }
    memcpy(fuzz_str, Data, str_len);
    fuzz_str[str_len] = '\0';

    cJSON *str_node = cJSON_CreateString(fuzz_str);
    free(fuzz_str);

    if (!str_node) {
        cJSON_Delete(root);
        return 0;
    }

    // Add string to object
    cJSON_AddItemToObject(root, "fuzz_string", str_node);

    // Create an array
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        cJSON_Delete(root);
        return 0;
    }

    // Add array to object
    cJSON_AddItemToObject(root, "fuzz_array", array);

    // Create a nested object
    cJSON *nested_obj = cJSON_CreateObject();
    if (!nested_obj) {
        cJSON_Delete(root);
        return 0;
    }

    // Add nested object to array
    cJSON_AddItemToArray(array, nested_obj);

    // Create a number node
    // Use the first byte of input to generate a number, or 0 if empty
    double num_val = (Size > 0) ? (double)Data[0] : 0.0;
    // Also try to interpret more bytes if available to create larger numbers
    if (Size > 4) {
        uint32_t val = 0;
        memcpy(&val, Data, 4);
        num_val = (double)val;
    }

    cJSON *num_node = cJSON_CreateNumber(num_val);
    if (!num_node) {
        cJSON_Delete(root);
        return 0;
    }

    // Add number to nested object
    cJSON_AddItemToObject(nested_obj, "fuzz_number", num_node);

    // Create another number node
    double num_val2 = (Size > 1) ? (double)Data[1] : 1.0;
    cJSON *num_node2 = cJSON_CreateNumber(num_val2);
    if (!num_node2) {
        cJSON_Delete(root);
        return 0;
    }

    // Add second number to nested object
    cJSON_AddItemToObject(nested_obj, "fuzz_number_2", num_node2);

    // Print the JSON
    char *json_str = cJSON_Print(root);
    if (json_str) {
        free(json_str);
    }

    // Cleanup
    cJSON_Delete(root);

    return 0;
}