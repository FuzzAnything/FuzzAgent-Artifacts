// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a writable, null-terminated copy of the input data
    char *writable_buffer = (char *)malloc(Size + 1);
    if (writable_buffer == NULL) {
        return 0;
    }
    memcpy(writable_buffer, Data, Size);
    writable_buffer[Size] = '\0';

    // Test cJSON_Version (doesn't depend on input)
    const char *version = cJSON_Version();
    (void)version; // Suppress unused variable warning

    // Test cJSON_Parse
    cJSON *parsed = cJSON_Parse(writable_buffer);
    if (parsed == NULL) {
        // Get error pointer if parsing failed
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr; // Suppress unused variable warning
    } else {
        cJSON_Delete(parsed);
    }

    // Test cJSON_ParseWithLength
    cJSON *parsed_with_length = cJSON_ParseWithLength(writable_buffer, Size);
    if (parsed_with_length != NULL) {
        cJSON_Delete(parsed_with_length);
    }

    // Test cJSON_ParseWithOpts with different options
    const char *parse_end = NULL;
    
    // Test without requiring null termination
    cJSON *parsed_opts1 = cJSON_ParseWithOpts(writable_buffer, &parse_end, 0);
    if (parsed_opts1 != NULL) {
        cJSON_Delete(parsed_opts1);
    }
    
    // Test requiring null termination
    cJSON *parsed_opts2 = cJSON_ParseWithOpts(writable_buffer, &parse_end, 1);
    if (parsed_opts2 != NULL) {
        cJSON_Delete(parsed_opts2);
    }

    // Test cJSON_Minify on writable buffer
    cJSON_Minify(writable_buffer);

    // Test parsing the minified result
    cJSON *minified_parsed = cJSON_Parse(writable_buffer);
    if (minified_parsed != NULL) {
        cJSON_Delete(minified_parsed);
    }

    free(writable_buffer);
    return 0;
}