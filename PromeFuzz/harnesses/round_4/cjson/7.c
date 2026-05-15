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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cJSON.h"

static void cleanup_strings(char **strings, int count) {
    for (int i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            free(strings[i]);
            strings[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a null-terminated copy of the input for parsing
    char *json_string = (char *)malloc(Size + 1);
    if (json_string == NULL) {
        return 0;
    }
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';
    
    // Create a writable copy for minification
    char *minify_string = (char *)malloc(Size + 1);
    if (minify_string == NULL) {
        free(json_string);
        return 0;
    }
    memcpy(minify_string, Data, Size);
    minify_string[Size] = '\0';
    
    // Array to track allocated strings for cleanup
    char *allocated_strings[4] = {NULL, NULL, NULL, NULL};
    int string_index = 0;
    
    // Step 1: Parse the JSON
    const char *parse_end = NULL;
    cJSON *parsed_json = cJSON_ParseWithOpts(json_string, &parse_end, 0);
    
    if (parsed_json != NULL) {
        // Step 2: Print with buffered strategy
        char *buffered = cJSON_PrintBuffered(parsed_json, 1024, 1);
        if (buffered != NULL) {
            allocated_strings[string_index++] = buffered;
        }
        
        // Step 3: Print formatted
        char *printed = cJSON_Print(parsed_json);
        if (printed != NULL) {
            allocated_strings[string_index++] = printed;
        }
        
        // Step 4: Print unformatted
        char *unformatted = cJSON_PrintUnformatted(parsed_json);
        if (unformatted != NULL) {
            allocated_strings[string_index++] = unformatted;
        }
    }
    
    // Step 5: Minify the input string
    cJSON_Minify(minify_string);
    
    // Step 6: Clean up parsed JSON
    if (parsed_json != NULL) {
        cJSON_Delete(parsed_json);
    }
    
    // Clean up allocated strings
    cleanup_strings(allocated_strings, 4);
    
    // Free temporary buffers
    free(json_string);
    free(minify_string);
    
    return 0;
}