// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
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

static void test_cjson_functions(const uint8_t *data, size_t size) {
    // Create a null-terminated string for functions that require it
    char *null_terminated_str = (char *)malloc(size + 1);
    if (!null_terminated_str) return;
    
    memcpy(null_terminated_str, data, size);
    null_terminated_str[size] = '\0';
    
    // Test cJSON_Version - always safe to call
    const char *version = cJSON_Version();
    (void)version; // Use version to avoid unused variable warning
    
    // Test cJSON_Parse
    cJSON *parsed1 = cJSON_Parse(null_terminated_str);
    if (parsed1) {
        cJSON_Delete(parsed1);
    }
    
    // Test cJSON_ParseWithLength
    cJSON *parsed2 = cJSON_ParseWithLength(null_terminated_str, size);
    if (parsed2) {
        cJSON_Delete(parsed2);
    }
    
    // Test cJSON_ParseWithOpts with different combinations
    const char *error_ptr = NULL;
    
    // Test with require_null_terminated = 0
    cJSON *parsed3 = cJSON_ParseWithOpts(null_terminated_str, &error_ptr, 0);
    if (parsed3) {
        cJSON_Delete(parsed3);
    } else {
        // Test cJSON_GetErrorPtr after failed parse
        const char *error_location = cJSON_GetErrorPtr();
        (void)error_location; // Use error_location to avoid unused variable warning
    }
    
    // Test with require_null_terminated = 1
    cJSON *parsed4 = cJSON_ParseWithOpts(null_terminated_str, NULL, 1);
    if (parsed4) {
        cJSON_Delete(parsed4);
    }
    
    // Test cJSON_Minify - requires writable buffer
    char *writable_buffer = (char *)malloc(size + 1);
    if (writable_buffer) {
        memcpy(writable_buffer, data, size);
        writable_buffer[size] = '\0';
        cJSON_Minify(writable_buffer);
        free(writable_buffer);
    }
    
    free(null_terminated_str);
}

int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    test_cjson_functions(data, size);
    
    return 0;
}