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
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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

static void test_cjson_apis(const uint8_t *data, size_t size) {
    // Create a null-terminated string from fuzzer input
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Test cJSON_Version
    const char *version = cJSON_Version();
    (void)version; // Use version to avoid unused variable warning
    
    // Test cJSON_Parse
    cJSON *parsed = cJSON_Parse(json_str);
    if (parsed) {
        cJSON_Delete(parsed);
    }
    
    // Test cJSON_GetErrorPtr after parse
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr;
    
    // Test cJSON_ParseWithLength
    cJSON *parsed_with_length = cJSON_ParseWithLength(json_str, size);
    if (parsed_with_length) {
        cJSON_Delete(parsed_with_length);
    }
    
    // Test cJSON_ParseWithOpts with different options
    const char *parse_end = NULL;
    
    // Test with require_null_terminated = 1
    cJSON *parsed_opts1 = cJSON_ParseWithOpts(json_str, &parse_end, 1);
    if (parsed_opts1) {
        cJSON_Delete(parsed_opts1);
    }
    
    // Test with require_null_terminated = 0
    cJSON *parsed_opts2 = cJSON_ParseWithOpts(json_str, &parse_end, 0);
    if (parsed_opts2) {
        cJSON_Delete(parsed_opts2);
    }
    
    // Test cJSON_Minify - needs writable buffer
    char *minify_buf = (char *)malloc(size + 1);
    if (minify_buf) {
        memcpy(minify_buf, data, size);
        minify_buf[size] = '\0';
        cJSON_Minify(minify_buf);
        free(minify_buf);
    }
    
    free(json_str);
}

int LLVMFuzzerTestOneInput_58(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    test_cjson_apis(data, size);
    
    return 0;
}