// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Create string items
    cJSON *string1 = cJSON_CreateString(input_str);
    cJSON *array = cJSON_CreateArray();
    cJSON *string2 = cJSON_CreateString(input_str);
    cJSON *string3 = cJSON_CreateString(input_str);
    cJSON *string4 = cJSON_CreateString(input_str);

    // Initialize hooks with NULL (reset to defaults)
    cJSON_InitHooks(NULL);

    // Delete one of the string items
    if (string1) {
        cJSON_Delete(string1);
    }

    // Get array size (array might be NULL)
    if (array) {
        int array_size = cJSON_GetArraySize(array);
        (void)array_size; // Use result to avoid unused variable warning

        // Add items to array if they exist
        if (string2) {
            cJSON_AddItemToArray(array, string2);
        }
        if (string3) {
            cJSON_AddItemToArray(array, string3);
        }

        // Add item to object (using array as object for fuzzing)
        if (string4) {
            cJSON_AddItemToObject(array, input_str, string4);
        }

        // Clean up remaining items
        cJSON_Delete(array);
    } else {
        // Clean up individual items if array creation failed
        if (string2) {
            cJSON_Delete(string2);
        }
        if (string3) {
            cJSON_Delete(string3);
        }
        if (string4) {
            cJSON_Delete(string4);
        }
    }

    free(input_str);
    return 0;
}