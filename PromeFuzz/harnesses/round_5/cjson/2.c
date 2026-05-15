// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
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

static char* extract_string(const uint8_t* data, size_t size, size_t offset, size_t max_len) {
    if (offset >= size) {
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size - offset;
    if (len > max_len) len = max_len;
    
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + offset, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Extract strings for keys and values from fuzzing input
    char* key1 = extract_string(Data, Size, 0, 50);
    char* key2 = extract_string(Data, Size, 50, 50);
    char* key3 = extract_string(Data, Size, 100, 50);
    char* key4 = extract_string(Data, Size, 150, 50);
    char* string_val = extract_string(Data, Size, 200, 50);
    
    // Extract numbers from fuzzing input
    double num1 = 0.0, num2 = 0.0;
    if (Size >= 208) {
        memcpy(&num1, Data + 200, 8);
        memcpy(&num2, Data + 208, 8);
    } else if (Size >= 201) {
        num1 = (double)Data[200];
        if (Size >= 202) num2 = (double)Data[201];
    }
    
    // Create root object
    cJSON* root = cJSON_CreateObject();
    if (!root) {
        free(key1); free(key2); free(key3); free(key4); free(string_val);
        return 0;
    }
    
    // Create string item and add to root
    cJSON* string_item = cJSON_CreateString(string_val);
    if (string_item) {
        cJSON_AddItemToObject(root, key1, string_item);
    }
    
    // Create array and add to root
    cJSON* array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToObject(root, key2, array);
        
        // Create nested object and add to array
        cJSON* nested_obj = cJSON_CreateObject();
        if (nested_obj) {
            cJSON_AddItemToArray(array, nested_obj);
            
            // Create first number and add to nested object
            cJSON* num_item1 = cJSON_CreateNumber(num1);
            if (num_item1) {
                cJSON_AddItemToObject(nested_obj, key3, num_item1);
            }
            
            // Create second number and add to nested object
            cJSON* num_item2 = cJSON_CreateNumber(num2);
            if (num_item2) {
                cJSON_AddItemToObject(nested_obj, key4, num_item2);
            }
        }
    }
    
    // Print the JSON structure
    char* printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }
    
    // Cleanup
    cJSON_Delete(root);
    free(key1);
    free(key2);
    free(key3);
    free(key4);
    free(string_val);
    
    return 0;
}