// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
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

static char* create_string_from_fuzz(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(2);
        if (str) {
            str[0] = 'a';
            str[1] = '\0';
        }
        return str;
    }
    
    size_t len = size > max_len ? max_len : size;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        str[i] = data[i] ? data[i] : 'a';
    }
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Extract two double values from fuzz data
    double num1, num2;
    memcpy(&num1, Data, sizeof(double));
    memcpy(&num2, Data + sizeof(double), sizeof(double));
    
    // Create key string from remaining fuzz data
    size_t key_size = Size - 2 * sizeof(double);
    char* key = create_string_from_fuzz(Data + 2 * sizeof(double), key_size, 64);
    if (!key) return 0;
    
    // Create object to work with
    cJSON* object = cJSON_CreateObject();
    if (!object) {
        free(key);
        return 0;
    }
    
    // First cJSON_CreateNumber
    cJSON* number1 = cJSON_CreateNumber(num1);
    if (!number1) {
        cJSON_Delete(object);
        free(key);
        return 0;
    }
    
    // Second cJSON_CreateNumber
    cJSON* number2 = cJSON_CreateNumber(num2);
    if (!number2) {
        cJSON_Delete(number1);
        cJSON_Delete(object);
        free(key);
        return 0;
    }
    
    // cJSON_AddItemToObject
    cJSON_AddItemToObject(object, key, number1);
    
    // cJSON_ReplaceItemInObject
    cJSON_ReplaceItemInObject(object, key, number2);
    
    // Cleanup
    cJSON_Delete(object);
    free(key);
    
    return 0;
}