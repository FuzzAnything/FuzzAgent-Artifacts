// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static void fuzz_parse_functions(const uint8_t *data, size_t size) {
    // Create null-terminated string for functions that require it
    char *null_terminated = malloc(size + 1);
    if (null_terminated == NULL) {
        return;
    }
    if (size > 0) {
        memcpy(null_terminated, data, size);
    }
    null_terminated[size] = '\0';
    
    // Create writable copy for minify
    char *writable = malloc(size + 1);
    if (writable == NULL) {
        free(null_terminated);
        return;
    }
    memcpy(writable, null_terminated, size + 1);
    
    // Test cJSON_Parse
    cJSON *parsed = cJSON_Parse(null_terminated);
    if (parsed != NULL) {
        cJSON_Delete(parsed);
    } else {
        // Get error pointer when parse fails
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr; // Avoid unused variable warning
    }
    
    // Test cJSON_ParseWithLength
    cJSON *parsed_with_length = cJSON_ParseWithLength(null_terminated, size);
    if (parsed_with_length != NULL) {
        cJSON_Delete(parsed_with_length);
    }
    
    // Test cJSON_ParseWithOpts with various options
    const char *parse_end = NULL;
    cJSON *parsed_with_opts = cJSON_ParseWithOpts(null_terminated, &parse_end, 0);
    if (parsed_with_opts != NULL) {
        cJSON_Delete(parsed_with_opts);
    }
    
    // Test with require_null_terminated = 1
    parsed_with_opts = cJSON_ParseWithOpts(null_terminated, &parse_end, 1);
    if (parsed_with_opts != NULL) {
        cJSON_Delete(parsed_with_opts);
    }
    
    // Test cJSON_ParseWithLengthOpts with various options
    parsed_with_opts = cJSON_ParseWithLengthOpts(null_terminated, size, &parse_end, 0);
    if (parsed_with_opts != NULL) {
        cJSON_Delete(parsed_with_opts);
    }
    
    // Test with require_null_terminated = 1
    parsed_with_opts = cJSON_ParseWithLengthOpts(null_terminated, size, &parse_end, 1);
    if (parsed_with_opts != NULL) {
        cJSON_Delete(parsed_with_opts);
    }
    
    // Test cJSON_Minify
    cJSON_Minify(writable);
    
    // Try parsing the minified result
    cJSON *minified_parsed = cJSON_Parse(writable);
    if (minified_parsed != NULL) {
        cJSON_Delete(minified_parsed);
    }
    
    // Cleanup
    free(null_terminated);
    free(writable);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
    // Call the main fuzzing function
    fuzz_parse_functions(data, size);
    
    return 0;
}