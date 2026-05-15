// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Create main JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Create keys and values from fuzzer input
    // Use first part of input for string key
    size_t key_len = Size > 0 ? (Data[0] % 100) + 1 : 10;
    if (key_len > Size) key_len = Size > 0 ? Size : 1;
    
    char *key1 = (char*)malloc(key_len + 1);
    if (!key1) {
        cJSON_Delete(root);
        return 0;
    }
    if (Size > 0) {
        memcpy(key1, Data, key_len < Size ? key_len : Size);
    }
    key1[key_len] = '\0';
    
    // Use next part for string value
    size_t val_start = key_len < Size ? key_len : 0;
    size_t val_len = Size > val_start ? (Size - val_start) : 0;
    char *str_val = (char*)malloc(val_len + 1);
    if (!str_val) {
        free(key1);
        cJSON_Delete(root);
        return 0;
    }
    if (val_len > 0) {
        memcpy(str_val, Data + val_start, val_len);
    }
    str_val[val_len] = '\0';
    
    // Add string to object
    cJSON_AddStringToObject(root, key1, str_val);
    
    // Create array key
    size_t array_key_len = Size > 0 ? (Data[0] % 50) + 1 : 5;
    if (array_key_len > Size) array_key_len = Size > 0 ? Size : 1;
    
    char *array_key = (char*)malloc(array_key_len + 1);
    if (!array_key) {
        free(key1);
        free(str_val);
        cJSON_Delete(root);
        return 0;
    }
    if (Size > 0) {
        memcpy(array_key, Data, array_key_len < Size ? array_key_len : Size);
    }
    array_key[array_key_len] = '\0';
    
    // Add array to object
    cJSON *array = cJSON_AddArrayToObject(root, array_key);
    
    // Create nested object
    cJSON *nested_obj = cJSON_CreateObject();
    if (nested_obj) {
        // Create number keys
        size_t num_key1_len = Size > 0 ? (Data[0] % 30) + 1 : 5;
        if (num_key1_len > Size) num_key1_len = Size > 0 ? Size : 1;
        
        char *num_key1 = (char*)malloc(num_key1_len + 1);
        if (num_key1) {
            if (Size > 0) {
                memcpy(num_key1, Data, num_key1_len < Size ? num_key1_len : Size);
            }
            num_key1[num_key1_len] = '\0';
            
            // Generate number from fuzzer data
            double num1 = 0.0;
            if (Size >= 8) {
                memcpy(&num1, Data, sizeof(double));
            } else if (Size > 0) {
                num1 = (double)Data[0];
            }
            
            cJSON_AddNumberToObject(nested_obj, num_key1, num1);
            free(num_key1);
        }
        
        size_t num_key2_len = Size > 1 ? (Data[1] % 30) + 1 : 5;
        if (num_key2_len > Size) num_key2_len = Size > 1 ? Size : 1;
        
        char *num_key2 = (char*)malloc(num_key2_len + 1);
        if (num_key2) {
            size_t offset = Size > 1 ? 1 : 0;
            if (Size > offset) {
                memcpy(num_key2, Data + offset, num_key2_len < (Size - offset) ? num_key2_len : (Size - offset));
            }
            num_key2[num_key2_len] = '\0';
            
            // Generate second number from fuzzer data
            double num2 = 0.0;
            if (Size >= 16) {
                memcpy(&num2, Data + 8, sizeof(double));
            } else if (Size > 1) {
                num2 = (double)Data[1];
            }
            
            cJSON_AddNumberToObject(nested_obj, num_key2, num2);
            free(num_key2);
        }
        
        // Add nested object to array
        if (array) {
            cJSON_AddItemToArray(array, nested_obj);
        } else {
            cJSON_Delete(nested_obj);
        }
    }
    
    // Print the JSON
    char *printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }
    
    // Cleanup
    cJSON_Delete(root);
    free(key1);
    free(str_val);
    free(array_key);
    
    return 0;
}