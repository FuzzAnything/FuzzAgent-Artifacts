// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create null-terminated copy of input for parsing
    char *input_copy = malloc(Size + 1);
    if (!input_copy) return 0;
    memcpy(input_copy, Data, Size);
    input_copy[Size] = '\0';

    // Parse JSON with options
    cJSON *json = cJSON_ParseWithOpts(input_copy, NULL, 0);
    
    if (json) {
        // Test cJSON_PrintBuffered with different parameters
        char *buffered_result = cJSON_PrintBuffered(json, Size % 1024, Size % 2);
        if (buffered_result) {
            free(buffered_result);
        }
        
        // Test cJSON_Print
        char *printed_result = cJSON_Print(json);
        if (printed_result) {
            free(printed_result);
        }
        
        // Test cJSON_PrintUnformatted
        char *unformatted_result = cJSON_PrintUnformatted(json);
        if (unformatted_result) {
            free(unformatted_result);
        }
        
        // Test cJSON_Minify on a writable copy of the input
        char *minify_input = malloc(Size + 1);
        if (minify_input) {
            memcpy(minify_input, Data, Size);
            minify_input[Size] = '\0';
            cJSON_Minify(minify_input);
            free(minify_input);
        }
        
        // Clean up
        cJSON_Delete(json);
    }
    
    free(input_copy);
    return 0;
}