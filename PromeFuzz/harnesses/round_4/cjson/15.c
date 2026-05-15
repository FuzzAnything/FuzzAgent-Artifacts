// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
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

static double extract_double(const uint8_t *data, size_t size, size_t offset) {
    double value = 0.0;
    if (offset + sizeof(double) <= size) {
        memcpy(&value, data + offset, sizeof(double));
    } else if (offset < size) {
        memcpy(&value, data + offset, size - offset);
    }
    return value;
}

static char* extract_string(const uint8_t *data, size_t size, size_t offset, size_t max_len) {
    if (offset >= size) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t remaining = size - offset;
    size_t len = remaining < max_len ? remaining : max_len;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + offset, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Create first number
    double num1 = extract_double(Data, Size, 0);
    cJSON *number1 = cJSON_CreateNumber(num1);
    if (!number1) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create second number
    double num2 = extract_double(Data, Size, sizeof(double));
    cJSON *number2 = cJSON_CreateNumber(num2);
    if (!number2) {
        cJSON_Delete(number1);
        cJSON_Delete(root);
        return 0;
    }
    
    // Extract key for AddItemToObject
    size_t key_offset = 2 * sizeof(double);
    char *key1 = extract_string(Data, Size, key_offset, 32);
    if (!key1) {
        cJSON_Delete(number1);
        cJSON_Delete(number2);
        cJSON_Delete(root);
        return 0;
    }
    
    // Add first number to object
    cJSON_AddItemToObject(root, key1, number1);
    
    // Extract key for ReplaceItemInObject (could be same or different)
    size_t replace_key_offset = key_offset + 32;
    char *key2 = extract_string(Data, Size, replace_key_offset, 32);
    if (!key2) {
        free(key1);
        cJSON_Delete(number2);
        cJSON_Delete(root);
        return 0;
    }
    
    // Try to replace item - will fail if key doesn't exist
    cJSON_ReplaceItemInObject(root, key2, number2);
    
    // Cleanup
    free(key1);
    free(key2);
    cJSON_Delete(root);
    
    return 0;
}