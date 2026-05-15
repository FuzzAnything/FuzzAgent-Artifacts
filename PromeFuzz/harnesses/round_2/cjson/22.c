// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a root object to add items to
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Use the first byte to determine which function to test
    uint8_t function_selector = Data[0] % 6;
    
    // Prepare key and value strings from fuzzer input
    // Ensure we have at least some data for key
    size_t key_len = Size > 1 ? (Data[1] % 32) + 1 : 1;
    if (key_len > Size - 1) {
        key_len = Size > 1 ? Size - 1 : 1;
    }
    
    char *key = malloc(key_len + 1);
    if (key == NULL) {
        cJSON_Delete(root);
        return 0;
    }
    
    if (Size > 1) {
        memcpy(key, Data + 1, key_len);
    } else {
        key[0] = 'a';
    }
    key[key_len] = '\0';

    // Prepare value string if needed
    char *value_str = NULL;
    if (function_selector == 0 && Size > key_len + 1) {
        size_t value_len = Size - key_len - 1;
        if (value_len > 0) {
            value_str = malloc(value_len + 1);
            if (value_str != NULL) {
                memcpy(value_str, Data + 1 + key_len, value_len);
                value_str[value_len] = '\0';
            }
        }
    }

    // Call the selected API function
    switch (function_selector) {
        case 0:  // cJSON_AddStringToObject
            if (value_str != NULL) {
                cJSON_AddStringToObject(root, key, value_str);
            } else {
                cJSON_AddStringToObject(root, key, "default");
            }
            break;
            
        case 1:  // cJSON_AddNullToObject
            cJSON_AddNullToObject(root, key);
            break;
            
        case 2:  // cJSON_AddArrayToObject
            cJSON_AddArrayToObject(root, key);
            break;
            
        case 3:  // cJSON_AddObjectToObject
            cJSON_AddObjectToObject(root, key);
            break;
            
        case 4:  // cJSON_AddNumberToObject
            // Generate a double from fuzzer data
            double number = 0.0;
            if (Size > key_len + 1) {
                // Use up to 8 bytes to form a double
                size_t bytes_to_use = Size - key_len - 1;
                if (bytes_to_use > 8) bytes_to_use = 8;
                memcpy(&number, Data + 1 + key_len, bytes_to_use);
            }
            cJSON_AddNumberToObject(root, key, number);
            break;
            
        case 5:  // cJSON_AddTrueToObject
            cJSON_AddTrueToObject(root, key);
            break;
    }

    // Clean up
    free(key);
    if (value_str != NULL) {
        free(value_str);
    }
    
    // Also test with invalid inputs
    cJSON_AddStringToObject(NULL, "invalid", "test");
    cJSON_AddStringToObject(root, NULL, "test");
    cJSON_AddStringToObject(root, "key", NULL);
    
    cJSON_AddNullToObject(NULL, "invalid");
    cJSON_AddNullToObject(root, NULL);
    
    cJSON_AddArrayToObject(NULL, "invalid");
    cJSON_AddArrayToObject(root, NULL);
    
    cJSON_AddObjectToObject(NULL, "invalid");
    cJSON_AddObjectToObject(root, NULL);
    
    cJSON_AddNumberToObject(NULL, "invalid", 1.0);
    cJSON_AddNumberToObject(root, NULL, 1.0);
    
    cJSON_AddTrueToObject(NULL, "invalid");
    cJSON_AddTrueToObject(root, NULL);

    // Clean up root object
    cJSON_Delete(root);

    return 0;
}