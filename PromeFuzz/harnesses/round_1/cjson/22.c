// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string from fuzzer input
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // Test cJSON_Parse first to get a cJSON object
    cJSON *json = cJSON_Parse(json_str);
    
    if (json) {
        // Test cJSON_PrintUnformatted
        char *printed = cJSON_PrintUnformatted(json);
        if (printed) {
            free(printed);
        }

        // Test cJSON_Print
        printed = cJSON_Print(json);
        if (printed) {
            free(printed);
        }

        // Test cJSON_PrintBuffered with various parameters
        printed = cJSON_PrintBuffered(json, Size % 1024, 0); // Unformatted
        if (printed) {
            free(printed);
        }
        
        printed = cJSON_PrintBuffered(json, Size % 1024, 1); // Formatted
        if (printed) {
            free(printed);
        }

        // Test cJSON_PrintPreallocated
        int buffer_size = Size % 1024 + 5; // +5 as per documentation
        char *buffer = (char *)malloc(buffer_size);
        if (buffer) {
            cJSON_PrintPreallocated(json, buffer, buffer_size, 0);
            cJSON_PrintPreallocated(json, buffer, buffer_size, 1);
            free(buffer);
        }

        // Test cJSON_GetStringValue on string items
        // First check if the root is a string
        char *str_val = cJSON_GetStringValue(json);
        // Also check child items if they exist
        if (json->child) {
            str_val = cJSON_GetStringValue(json->child);
        }

        cJSON_Delete(json);
    }

    // Test cJSON_Minify on the input string
    // Make a writable copy since cJSON_Minify modifies in place
    char *minify_str = (char *)malloc(Size + 1);
    if (minify_str) {
        memcpy(minify_str, Data, Size);
        minify_str[Size] = '\0';
        cJSON_Minify(minify_str);
        free(minify_str);
    }

    // Also test cJSON_GetStringValue on a newly created string item
    cJSON *string_item = cJSON_CreateString(json_str);
    if (string_item) {
        char *str_val = cJSON_GetStringValue(string_item);
        cJSON_Delete(string_item);
    }

    free(json_str);
    return 0;
}