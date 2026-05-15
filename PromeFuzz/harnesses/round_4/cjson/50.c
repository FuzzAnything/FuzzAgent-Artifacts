// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_Version - simple function call
    const char* version = cJSON_Version();
    if (version == NULL) {
        // Should never happen, but check anyway
        return 0;
    }

    // Create null-terminated string for functions that require it
    char* null_terminated = (char*)malloc(Size + 1);
    if (null_terminated == NULL) {
        return 0;
    }
    memcpy(null_terminated, Data, Size);
    null_terminated[Size] = '\0';

    // Test cJSON_Parse
    cJSON* parsed = cJSON_Parse(null_terminated);
    if (parsed != NULL) {
        cJSON_Delete(parsed);
    } else {
        // If parsing failed, test cJSON_GetErrorPtr
        const char* error_ptr = cJSON_GetErrorPtr();
        // error_ptr may be NULL or point to error location
    }

    // Test cJSON_ParseWithLength
    cJSON* parsed_with_length = cJSON_ParseWithLength((const char*)Data, Size);
    if (parsed_with_length != NULL) {
        cJSON_Delete(parsed_with_length);
    }

    // Test cJSON_ParseWithOpts with various options
    const char* parse_end = NULL;
    
    // Test with require_null_terminated = false
    cJSON* parsed_opts1 = cJSON_ParseWithOpts((const char*)Data, &parse_end, 0);
    if (parsed_opts1 != NULL) {
        cJSON_Delete(parsed_opts1);
    }
    
    // Test with require_null_terminated = true using null-terminated string
    cJSON* parsed_opts2 = cJSON_ParseWithOpts(null_terminated, &parse_end, 1);
    if (parsed_opts2 != NULL) {
        cJSON_Delete(parsed_opts2);
    }
    
    // Test with NULL return_parse_end
    cJSON* parsed_opts3 = cJSON_ParseWithOpts(null_terminated, NULL, 0);
    if (parsed_opts3 != NULL) {
        cJSON_Delete(parsed_opts3);
    }

    // Test cJSON_Minify - requires writable buffer
    char* minify_buffer = (char*)malloc(Size + 1);
    if (minify_buffer != NULL) {
        memcpy(minify_buffer, Data, Size);
        minify_buffer[Size] = '\0';
        cJSON_Minify(minify_buffer);
        free(minify_buffer);
    }

    // Test with different sizes to explore more states
    if (Size > 1) {
        // Test with first half of data
        size_t half_size = Size / 2;
        char* half_buffer = (char*)malloc(half_size + 1);
        if (half_buffer != NULL) {
            memcpy(half_buffer, Data, half_size);
            half_buffer[half_size] = '\0';
            
            cJSON* half_parsed = cJSON_Parse(half_buffer);
            if (half_parsed != NULL) {
                cJSON_Delete(half_parsed);
            }
            
            // Test minify on half buffer
            cJSON_Minify(half_buffer);
            free(half_buffer);
        }
    }

    free(null_terminated);
    return 0;
}