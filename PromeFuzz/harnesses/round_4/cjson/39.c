// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_Print(const cJSON *item) {
    if (item == NULL) return;
    
    char *result = cJSON_Print(item);
    if (result != NULL) {
        cJSON_free(result);
    }
}

static void fuzz_cJSON_Minify(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *json = (char *)malloc(size + 1);
    if (json == NULL) return;
    
    memcpy(json, data, size);
    json[size] = '\0';
    
    cJSON_Minify(json);
    free(json);
}

static void fuzz_cJSON_PrintBuffered(const cJSON *item, int prebuffer, cJSON_bool fmt) {
    if (item == NULL) return;
    
    char *result = cJSON_PrintBuffered(item, prebuffer, fmt);
    if (result != NULL) {
        cJSON_free(result);
    }
}

static void fuzz_cJSON_PrintPreallocated(cJSON *item, const uint8_t *data, size_t size) {
    if (item == NULL) return;
    
    int buffer_len = (size > 0) ? (data[0] % 256) + 5 : 10;
    char *buffer = (char *)malloc(buffer_len);
    if (buffer == NULL) return;
    
    cJSON_bool format = (size > 1) ? (data[1] % 2) : 0;
    cJSON_PrintPreallocated(item, buffer, buffer_len, format);
    free(buffer);
}

static void fuzz_cJSON_GetStringValue(const cJSON *item) {
    if (item == NULL) return;
    
    char *result = cJSON_GetStringValue(item);
    (void)result;
}

static void fuzz_cJSON_PrintUnformatted(const cJSON *item) {
    if (item == NULL) return;
    
    char *result = cJSON_PrintUnformatted(item);
    if (result != NULL) {
        cJSON_free(result);
    }
}

static cJSON *create_test_json(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    char *json_str = (char *)malloc(size + 1);
    if (json_str == NULL) return NULL;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    free(json_str);
    
    if (json == NULL) {
        static const char *fallback_json = "{\"test\":\"value\",\"number\":42,\"array\":[1,2,3]}";
        json = cJSON_Parse(fallback_json);
    }
    
    return json;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    cJSON *json = create_test_json(data, size);
    if (json == NULL) return 0;
    
    fuzz_cJSON_Print(json);
    fuzz_cJSON_Minify(data, size);
    
    int prebuffer = (size > 0) ? (data[0] % 1024) : 64;
    cJSON_bool fmt = (size > 1) ? (data[1] % 2) : 0;
    fuzz_cJSON_PrintBuffered(json, prebuffer, fmt);
    
    fuzz_cJSON_PrintPreallocated(json, data, size);
    fuzz_cJSON_GetStringValue(json);
    fuzz_cJSON_PrintUnformatted(json);
    
    if (json->child != NULL) {
        fuzz_cJSON_GetStringValue(json->child);
    }
    
    cJSON_Delete(json);
    return 0;
}