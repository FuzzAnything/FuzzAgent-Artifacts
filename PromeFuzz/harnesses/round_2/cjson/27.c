// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
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

static void fuzz_cJSON_AddStringToObject(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) return;
    
    size_t key_len = size / 2;
    size_t val_len = size - key_len;
    
    char *key = malloc(key_len + 1);
    char *val = malloc(val_len + 1);
    
    if (key == NULL || val == NULL) {
        free(key);
        free(val);
        cJSON_Delete(object);
        return;
    }
    
    memcpy(key, data, key_len);
    memcpy(val, data + key_len, val_len);
    key[key_len] = '\0';
    val[val_len] = '\0';
    
    cJSON *result = cJSON_AddStringToObject(object, key, val);
    (void)result;
    
    free(key);
    free(val);
    cJSON_Delete(object);
}

static void fuzz_cJSON_AddRawToObject(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) return;
    
    size_t key_len = size / 2;
    size_t raw_len = size - key_len;
    
    char *key = malloc(key_len + 1);
    char *raw = malloc(raw_len + 1);
    
    if (key == NULL || raw == NULL) {
        free(key);
        free(raw);
        cJSON_Delete(object);
        return;
    }
    
    memcpy(key, data, key_len);
    memcpy(raw, data + key_len, raw_len);
    key[key_len] = '\0';
    raw[raw_len] = '\0';
    
    cJSON *result = cJSON_AddRawToObject(object, key, raw);
    (void)result;
    
    free(key);
    free(raw);
    cJSON_Delete(object);
}

static void fuzz_cJSON_AddArrayToObject(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) return;
    
    char *key = malloc(size + 1);
    if (key == NULL) {
        cJSON_Delete(object);
        return;
    }
    
    memcpy(key, data, size);
    key[size] = '\0';
    
    cJSON *result = cJSON_AddArrayToObject(object, key);
    (void)result;
    
    free(key);
    cJSON_Delete(object);
}

static void fuzz_cJSON_AddNullToObject(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) return;
    
    char *key = malloc(size + 1);
    if (key == NULL) {
        cJSON_Delete(object);
        return;
    }
    
    memcpy(key, data, size);
    key[size] = '\0';
    
    cJSON *result = cJSON_AddNullToObject(object, key);
    (void)result;
    
    free(key);
    cJSON_Delete(object);
}

static void fuzz_cJSON_AddObjectToObject(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) return;
    
    char *key = malloc(size + 1);
    if (key == NULL) {
        cJSON_Delete(object);
        return;
    }
    
    memcpy(key, data, size);
    key[size] = '\0';
    
    cJSON *result = cJSON_AddObjectToObject(object, key);
    (void)result;
    
    free(key);
    cJSON_Delete(object);
}

static void fuzz_cJSON_AddNumberToObject(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) return;
    
    size_t key_len = size / 2;
    size_t num_len = size - key_len;
    
    char *key = malloc(key_len + 1);
    char *num_str = malloc(num_len + 1);
    
    if (key == NULL || num_str == NULL) {
        free(key);
        free(num_str);
        cJSON_Delete(object);
        return;
    }
    
    memcpy(key, data, key_len);
    memcpy(num_str, data + key_len, num_len);
    key[key_len] = '\0';
    num_str[num_len] = '\0';
    
    double number = atof(num_str);
    cJSON *result = cJSON_AddNumberToObject(object, key, number);
    (void)result;
    
    free(key);
    free(num_str);
    cJSON_Delete(object);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uint8_t selector = data[0];
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size - 1;
    
    switch (selector % 6) {
        case 0:
            fuzz_cJSON_AddStringToObject(remaining_data, remaining_size);
            break;
        case 1:
            fuzz_cJSON_AddRawToObject(remaining_data, remaining_size);
            break;
        case 2:
            fuzz_cJSON_AddArrayToObject(remaining_data, remaining_size);
            break;
        case 3:
            fuzz_cJSON_AddNullToObject(remaining_data, remaining_size);
            break;
        case 4:
            fuzz_cJSON_AddObjectToObject(remaining_data, remaining_size);
            break;
        case 5:
            fuzz_cJSON_AddNumberToObject(remaining_data, remaining_size);
            break;
    }
    
    return 0;
}