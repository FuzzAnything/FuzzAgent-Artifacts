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
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_Parse(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed = cJSON_Parse(json_str);
    if (parsed) {
        cJSON_Delete(parsed);
    } else {
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    }
    
    free(json_str);
}

static void fuzz_cJSON_ParseWithLength(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *json_str = (char *)malloc(size);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    
    cJSON *parsed = cJSON_ParseWithLength(json_str, size);
    if (parsed) {
        cJSON_Delete(parsed);
    }
    
    free(json_str);
}

static void fuzz_cJSON_ParseWithOpts(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    const char *parse_end = NULL;
    
    cJSON *parsed1 = cJSON_ParseWithOpts(json_str, &parse_end, 1);
    if (parsed1) {
        cJSON_Delete(parsed1);
    }
    
    cJSON *parsed2 = cJSON_ParseWithOpts(json_str, &parse_end, 0);
    if (parsed2) {
        cJSON_Delete(parsed2);
    }
    
    free(json_str);
}

static void fuzz_cJSON_Minify(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON_Minify(json_str);
    
    free(json_str);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    const char *version = cJSON_Version();
    (void)version;
    
    fuzz_cJSON_Parse(data, size);
    fuzz_cJSON_ParseWithLength(data, size);
    fuzz_cJSON_ParseWithOpts(data, size);
    fuzz_cJSON_Minify(data, size);
    
    return 0;
}