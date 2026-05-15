// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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
#include <stdio.h>

static void cleanup_string(char *str) {
    if (str) {
        free(str);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Create null-terminated string from fuzzer input
    char *json_string = malloc(Size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';

    // Parse JSON from the input string
    cJSON *json = cJSON_Parse(json_string);
    if (!json) {
        free(json_string);
        return 0;
    }

    // Test cJSON_Print
    char *printed = cJSON_Print(json);
    cleanup_string(printed);

    // Test cJSON_PrintUnformatted
    char *unformatted = cJSON_PrintUnformatted(json);
    cleanup_string(unformatted);

    // Test cJSON_PrintBuffered with various parameters
    char *buffered1 = cJSON_PrintBuffered(json, 0, 0);
    cleanup_string(buffered1);
    
    char *buffered2 = cJSON_PrintBuffered(json, 1024, 1);
    cleanup_string(buffered2);
    
    char *buffered3 = cJSON_PrintBuffered(json, Size, 0);
    cleanup_string(buffered3);

    // Test cJSON_PrintPreallocated
    int buffer_size = Size + 100; // Add some margin
    if (buffer_size > 0) {
        char *buffer = malloc(buffer_size);
        if (buffer) {
            cJSON_PrintPreallocated(json, buffer, buffer_size, 0);
            cJSON_PrintPreallocated(json, buffer, buffer_size, 1);
            
            // Also test with smaller buffer to trigger failure cases
            if (buffer_size > 1) {
                cJSON_PrintPreallocated(json, buffer, 1, 0);
                cJSON_PrintPreallocated(json, buffer, 1, 1);
            }
            free(buffer);
        }
    }

    // Test cJSON_GetStringValue
    char *str_value = cJSON_GetStringValue(json);
    (void)str_value; // Avoid unused variable warning

    // Also test on child elements if they exist
    if (json->child) {
        char *child_str = cJSON_GetStringValue(json->child);
        (void)child_str;
    }

    // Test cJSON_Minify
    // Create a writable copy of the original JSON string
    char *writable_json = malloc(Size + 1);
    if (writable_json) {
        memcpy(writable_json, json_string, Size + 1);
        cJSON_Minify(writable_json);
        free(writable_json);
    }

    // Cleanup
    cJSON_Delete(json);
    free(json_string);

    return 0;
}