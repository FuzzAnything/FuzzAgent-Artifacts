// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
#include <stdio.h>

static void test_cJSON_PrintPreallocated(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        free(json_str);
        return;
    }
    
    int buffer_len = size * 2 + 5;
    char *buffer = (char *)malloc(buffer_len);
    if (buffer) {
        cJSON_bool format = (data[0] & 1) ? 1 : 0;
        cJSON_PrintPreallocated(json, buffer, buffer_len, format);
        free(buffer);
    }
    
    cJSON_Delete(json);
    free(json_str);
}

static void test_cJSON_PrintBuffered(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        free(json_str);
        return;
    }
    
    int prebuffer = (data[0] << 8) | data[1 % size];
    if (prebuffer < 0) prebuffer = -prebuffer;
    if (prebuffer == 0) prebuffer = 1;
    
    cJSON_bool fmt = (data[0] & 2) ? 1 : 0;
    char *result = cJSON_PrintBuffered(json, prebuffer, fmt);
    if (result) free(result);
    
    cJSON_Delete(json);
    free(json_str);
}

static void test_cJSON_GetStringValue(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *str = (char *)malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    cJSON *json = cJSON_CreateString(str);
    if (json) {
        char *value = cJSON_GetStringValue(json);
        (void)value;
        cJSON_Delete(json);
    }
    
    free(str);
}

static void test_cJSON_Print(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        free(json_str);
        return;
    }
    
    char *result = cJSON_Print(json);
    if (result) free(result);
    
    cJSON_Delete(json);
    free(json_str);
}

static void test_cJSON_Minify(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *json = (char *)malloc(size + 1);
    if (!json) return;
    
    memcpy(json, data, size);
    json[size] = '\0';
    
    cJSON_Minify(json);
    
    free(json);
}

static void test_cJSON_PrintUnformatted(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        free(json_str);
        return;
    }
    
    char *result = cJSON_PrintUnformatted(json);
    if (result) free(result);
    
    cJSON_Delete(json);
    free(json_str);
}

int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    test_cJSON_PrintPreallocated(data, size);
    test_cJSON_PrintBuffered(data, size);
    test_cJSON_GetStringValue(data, size);
    test_cJSON_Print(data, size);
    test_cJSON_Minify(data, size);
    test_cJSON_PrintUnformatted(data, size);
    
    return 0;
}