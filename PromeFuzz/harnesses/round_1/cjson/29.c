// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Create a null-terminated copy for functions that require it
    char *null_terminated = NULL;
    if (Size > 0) {
        null_terminated = (char *)malloc(Size + 1);
        if (null_terminated) {
            memcpy(null_terminated, Data, Size);
            null_terminated[Size] = '\0';
        }
    }

    // Test cJSON_ParseWithOpts with various options
    const char *parse_end = NULL;
    cJSON *parsed_with_opts = NULL;
    
    if (null_terminated) {
        // Test with require_null_terminated = 0
        parsed_with_opts = cJSON_ParseWithOpts(null_terminated, &parse_end, 0);
        if (parsed_with_opts) {
            cJSON_Delete(parsed_with_opts);
            parsed_with_opts = NULL;
        }
        
        // Test with require_null_terminated = 1
        parsed_with_opts = cJSON_ParseWithOpts(null_terminated, &parse_end, 1);
        if (parsed_with_opts) {
            cJSON_Delete(parsed_with_opts);
            parsed_with_opts = NULL;
        }
        
        // Test with NULL return_parse_end
        parsed_with_opts = cJSON_ParseWithOpts(null_terminated, NULL, 0);
        if (parsed_with_opts) {
            cJSON_Delete(parsed_with_opts);
            parsed_with_opts = NULL;
        }
    }

    // Test cJSON_ParseWithLength
    cJSON *parsed_with_length = NULL;
    if (Size > 0) {
        parsed_with_length = cJSON_ParseWithLength((const char *)Data, Size);
        if (parsed_with_length) {
            cJSON_Delete(parsed_with_length);
            parsed_with_length = NULL;
        }
    }

    // Test cJSON_Version
    const char *version = cJSON_Version();
    (void)version; // Use version to avoid unused variable warning

    // Test cJSON_GetErrorPtr after parse attempts
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr; // Use error_ptr to avoid unused variable warning

    // Test cJSON_Minify - requires writable buffer
    if (null_terminated) {
        // Create a writable copy for minify
        char *minify_buffer = (char *)malloc(Size + 1);
        if (minify_buffer) {
            memcpy(minify_buffer, null_terminated, Size);
            minify_buffer[Size] = '\0';
            cJSON_Minify(minify_buffer);
            free(minify_buffer);
        }
        
        // Test with NULL input
        cJSON_Minify(NULL);
    }

    // Test cJSON_Parse
    cJSON *parsed = NULL;
    if (null_terminated) {
        parsed = cJSON_Parse(null_terminated);
        if (parsed) {
            cJSON_Delete(parsed);
            parsed = NULL;
        }
    }

    // Cleanup
    if (null_terminated) {
        free(null_terminated);
    }

    return 0;
}