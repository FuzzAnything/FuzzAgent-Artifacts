// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a buffer to hold the input string, ensuring null termination
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test 1: cJSON_CreateStringReference
    cJSON *ref_node = cJSON_CreateStringReference(input_str);
    if (ref_node) {
        // ref_node points to input_str, so we must not free input_str yet
        // Just delete the node (which won't free the string)
        cJSON_Delete(ref_node);
    }

    // Test 2: cJSON_CreateString
    cJSON *str_node = cJSON_CreateString(input_str);
    if (str_node) {
        // Test 2a: cJSON_SetValuestring on a valid string node
        // Prepare a new value string
        char new_value[256];
        size_t new_len = (Size < 255) ? Size : 255;
        memcpy(new_value, input_str, new_len);
        new_value[new_len] = '\0';

        cJSON_SetValuestring(str_node, new_value);
        cleanup_cjson(str_node);
    }

    // Test 3: cJSON_AddStringToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj) {
        // Add a string to the object
        cJSON *added = cJSON_AddStringToObject(obj, "key1", input_str);
        if (!added) {
            // If addition failed, the object might still be valid but empty or partial
        }
        // Try adding another with a different key
        cJSON_AddStringToObject(obj, "key2", "static_value");
        cleanup_cjson(obj);
    }

    // Test 4: cJSON_CreateStringArray
    // Create an array of strings from the input
    // We'll split the input into chunks or just use the whole string multiple times
    const char *str_array[5];
    int count = 0;
    if (Size > 0) {
        str_array[0] = input_str;
        count = 1;
        if (Size > 10) {
            // Create a second string by taking a substring if possible, 
            // but for simplicity in fuzzing, we might just reuse or use static
            str_array[1] = "fuzzed_chunk";
            count = 2;
        }
    }

    cJSON *arr = cJSON_CreateStringArray(str_array, count);
    if (arr) {
        cleanup_cjson(arr);
    }

    // Test 5: cJSON_PrintBuffered
    // Create a simple object to print
    cJSON *print_obj = cJSON_CreateObject();
    if (print_obj) {
        cJSON_AddStringToObject(print_obj, "test", input_str);
        
        // Try with different buffer sizes and formats
        char *out1 = cJSON_PrintBuffered(print_obj, 0, 0); // Unformatted, guess 0
        if (out1) {
            free(out1);
        }
        
        char *out2 = cJSON_PrintBuffered(print_obj, 1024, 1); // Formatted, guess 1024
        if (out2) {
            free(out2);
        }
        
        char *out3 = cJSON_PrintBuffered(print_obj, -1, 0); // Invalid size
        if (out3) {
            free(out3);
        }

        cleanup_cjson(print_obj);
    }

    // Cleanup
    free(input_str);

    return 0;
}