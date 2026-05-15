// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from the fuzzer input
    char *json_string = malloc(Size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';

    // Parse the JSON string
    cJSON *json = cJSON_Parse(json_string);
    free(json_string);

    if (json) {
        // Test cJSON_PrintUnformatted
        char *unformatted = cJSON_PrintUnformatted(json);
        if (unformatted) {
            free(unformatted);
        }

        // Test cJSON_PrintBuffered with various parameters
        char *buffered1 = cJSON_PrintBuffered(json, 64, 0);
        if (buffered1) {
            free(buffered1);
        }
        
        char *buffered2 = cJSON_PrintBuffered(json, 128, 1);
        if (buffered2) {
            free(buffered2);
        }

        // Test cJSON_Print
        char *printed = cJSON_Print(json);
        if (printed) {
            free(printed);
        }

        // Test cJSON_PrintPreallocated with various buffer sizes
        int buffer_len = 256;
        char *buffer = malloc(buffer_len);
        if (buffer) {
            cJSON_PrintPreallocated(json, buffer, buffer_len, 0);
            cJSON_PrintPreallocated(json, buffer, buffer_len, 1);
            free(buffer);
        }

        // Clean up
        cJSON_Delete(json);
    }

    // Test cJSON_Minify with a writable copy of the input
    char *minify_input = malloc(Size + 1);
    if (minify_input) {
        memcpy(minify_input, Data, Size);
        minify_input[Size] = '\0';
        cJSON_Minify(minify_input);
        free(minify_input);
    }

    // Test cJSON_CreateIntArray with various sizes
    if (Size >= sizeof(int)) {
        size_t max_count = Size / sizeof(int);
        if (max_count > 0) {
            // Limit to reasonable size to avoid excessive memory usage
            size_t count = max_count > 100 ? 100 : max_count;
            int *numbers = malloc(count * sizeof(int));
            if (numbers) {
                // Copy integer data from fuzzer input
                memcpy(numbers, Data, count * sizeof(int));
                cJSON *int_array = cJSON_CreateIntArray(numbers, (int)count);
                if (int_array) {
                    // Test printing functions on the created array
                    char *array_print = cJSON_PrintUnformatted(int_array);
                    if (array_print) {
                        free(array_print);
                    }
                    cJSON_Delete(int_array);
                }
                free(numbers);
            }
        }
    }

    return 0;
}