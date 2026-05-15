// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
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

static void fuzz_cJSON_PrintBuffered(const cJSON *item) {
    if (item == NULL) return;
    
    // Test with various prebuffer sizes and formats
    char *result1 = cJSON_PrintBuffered(item, 64, 0);
    if (result1 != NULL) {
        cJSON_free(result1);
    }
    
    char *result2 = cJSON_PrintBuffered(item, 128, 1);
    if (result2 != NULL) {
        cJSON_free(result2);
    }
    
    char *result3 = cJSON_PrintBuffered(item, 0, 0);
    if (result3 != NULL) {
        cJSON_free(result3);
    }
    
    char *result4 = cJSON_PrintBuffered(item, 256, 1);
    if (result4 != NULL) {
        cJSON_free(result4);
    }
}

static void fuzz_cJSON_Print(const cJSON *item) {
    if (item == NULL) return;
    
    char *result = cJSON_Print(item);
    if (result != NULL) {
        cJSON_free(result);
    }
}

static cJSON *fuzz_cJSON_ParseWithLength(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return NULL;
    
    // Create a null-terminated copy for safety
    char *json_str = (char *)cJSON_malloc(size + 1);
    if (json_str == NULL) return NULL;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed = cJSON_ParseWithLength(json_str, size);
    cJSON_free(json_str);
    
    return parsed;
}

static void fuzz_cJSON_Minify(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    // Create a writable copy as required by cJSON_Minify
    char *writable_json = (char *)cJSON_malloc(size + 1);
    if (writable_json == NULL) return;
    
    memcpy(writable_json, data, size);
    writable_json[size] = '\0';
    
    cJSON_Minify(writable_json);
    cJSON_free(writable_json);
}

static void fuzz_cJSON_malloc_free(size_t size) {
    if (size > 1024 * 1024) return; // Limit allocation size
    
    void *ptr = cJSON_malloc(size);
    if (ptr != NULL) {
        cJSON_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Test cJSON_malloc and cJSON_free
    fuzz_cJSON_malloc_free(Size % 1024);
    
    // Test cJSON_ParseWithLength
    cJSON *parsed_json = fuzz_cJSON_ParseWithLength(Data, Size);
    
    if (parsed_json != NULL) {
        // Test cJSON_Print
        fuzz_cJSON_Print(parsed_json);
        
        // Test cJSON_PrintBuffered
        fuzz_cJSON_PrintBuffered(parsed_json);
        
        // Clean up parsed JSON
        cJSON_Delete(parsed_json);
    }
    
    // Test cJSON_Minify
    fuzz_cJSON_Minify(Data, Size);
    
    return 0;
}