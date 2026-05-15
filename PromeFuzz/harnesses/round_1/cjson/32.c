// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
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

static void test_cJSON_ParseWithOpts(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *null_terminated = (char *)malloc(size + 1);
    if (!null_terminated) return;
    
    memcpy(null_terminated, data, size);
    null_terminated[size] = '\0';
    
    const char *parse_end = NULL;
    cJSON *json = cJSON_ParseWithOpts(null_terminated, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
    }
    
    free(null_terminated);
}

static void test_cJSON_ParseWithLength(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *buffer = (char *)malloc(size);
    if (!buffer) return;
    
    memcpy(buffer, data, size);
    cJSON *json = cJSON_ParseWithLength(buffer, size);
    if (json) {
        cJSON_Delete(json);
    }
    
    free(buffer);
}

static void test_cJSON_Parse(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *null_terminated = (char *)malloc(size + 1);
    if (!null_terminated) return;
    
    memcpy(null_terminated, data, size);
    null_terminated[size] = '\0';
    
    cJSON *json = cJSON_Parse(null_terminated);
    if (json) {
        cJSON_Delete(json);
    }
    
    free(null_terminated);
}

static void test_cJSON_Minify(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *writable_buffer = (char *)malloc(size + 1);
    if (!writable_buffer) return;
    
    memcpy(writable_buffer, data, size);
    writable_buffer[size] = '\0';
    
    cJSON_Minify(writable_buffer);
    
    free(writable_buffer);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Test cJSON_ParseWithOpts
    test_cJSON_ParseWithOpts(Data, Size);
    
    // Test cJSON_ParseWithLength
    test_cJSON_ParseWithLength(Data, Size);
    
    // Test cJSON_Parse
    test_cJSON_Parse(Data, Size);
    
    // Test cJSON_Minify
    test_cJSON_Minify(Data, Size);
    
    // Test cJSON_Version - always safe to call
    const char *version = cJSON_Version();
    (void)version;
    
    // Test cJSON_GetErrorPtr - call after potential parse failures
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr;
    
    return 0;
}