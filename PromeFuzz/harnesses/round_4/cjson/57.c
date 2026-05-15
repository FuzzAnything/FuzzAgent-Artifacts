// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static char* extract_string(const uint8_t* data, size_t size, size_t* offset, size_t max_len) {
    if (*offset >= size) {
        return NULL;
    }
    
    size_t remaining = size - *offset;
    size_t len = remaining > max_len ? max_len : remaining;
    
    char* str = malloc(len + 1);
    if (!str) {
        return NULL;
    }
    
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len;
    
    return str;
}

static double extract_double(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(double) > size) {
        *offset = size;
        return 0.0;
    }
    
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    
    return value;
}

int LLVMFuzzerTestOneInput_57(const uint8_t* Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Create main object
    cJSON* root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }
    
    // Extract and add string items
    for (int i = 0; i < 3 && offset < Size; i++) {
        char* key = extract_string(Data, Size, &offset, 16);
        if (!key) break;
        
        char* value = extract_string(Data, Size, &offset, 32);
        if (!value) {
            free(key);
            break;
        }
        
        cJSON_AddStringToObject(root, key, value);
        
        free(key);
        free(value);
    }
    
    // Extract and add number items
    for (int i = 0; i < 3 && offset < Size; i++) {
        char* key = extract_string(Data, Size, &offset, 16);
        if (!key) break;
        
        double num = extract_double(Data, Size, &offset);
        cJSON_AddNumberToObject(root, key, num);
        
        free(key);
    }
    
    // Check if root is an object
    cJSON_IsObject(root);
    
    // Check for existing items
    if (cJSON_GetArraySize(root) > 0) {
        cJSON* first_child = cJSON_GetArrayItem(root, 0);
        if (first_child && first_child->string) {
            cJSON_HasObjectItem(root, first_child->string);
        }
    }
    
    // Add nested objects
    for (int i = 0; i < 2 && offset < Size; i++) {
        char* key = extract_string(Data, Size, &offset, 16);
        if (!key) break;
        
        cJSON* nested = cJSON_AddObjectToObject(root, key);
        if (nested) {
            // Add some items to nested object
            char* nested_key = extract_string(Data, Size, &offset, 8);
            if (nested_key) {
                char* nested_value = extract_string(Data, Size, &offset, 16);
                if (nested_value) {
                    cJSON_AddStringToObject(nested, nested_key, nested_value);
                    free(nested_value);
                }
                free(nested_key);
            }
            
            double nested_num = extract_double(Data, Size, &offset);
            cJSON_AddNumberToObject(nested, "nested_num", nested_num);
        }
        
        free(key);
    }
    
    // Cleanup
    cJSON_Delete(root);
    
    return 0;
}