// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Use the first 8 bytes to create a double for the first number
    double num1 = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num1, Data, sizeof(double));
    }
    
    // Use the next 8 bytes to create a double for the second number
    double num2 = 0.0;
    if (Size >= 2 * sizeof(double)) {
        memcpy(&num2, Data + sizeof(double), sizeof(double));
    }
    
    // Create number nodes
    cJSON *num_item1 = cJSON_CreateNumber(num1);
    cJSON *num_item2 = cJSON_CreateNumber(num2);
    
    // Create an object to add items to
    cJSON *object = cJSON_CreateObject();
    
    if (object && num_item1) {
        // Use remaining bytes as key string for AddItemToObject
        // Ensure null termination for the key
        size_t key_len = 0;
        const uint8_t *key_data = Data + 2 * sizeof(double);
        size_t remaining_size = Size - 2 * sizeof(double);
        
        if (remaining_size > 0 && remaining_size <= Size) {
            key_len = remaining_size;
        }
        
        char *key = NULL;
        if (key_len > 0) {
            key = (char *)malloc(key_len + 1);
            if (key) {
                memcpy(key, key_data, key_len);
                key[key_len] = '\0';
            }
        }
        
        // If no valid key from input, use a default
        if (!key) {
            key = strdup("test_key");
        }
        
        // Add first number item to object
        cJSON_AddItemToObject(object, key, num_item1);
        
        // Replace the item with the second number
        if (num_item2) {
            cJSON_ReplaceItemInObject(object, key, num_item2);
        }
        
        // Clean up
        free(key);
    }
    
    // Delete the object (which will delete all child items)
    cJSON_Delete(object);
    
    // If num_item2 wasn't added to object, delete it separately
    if (num_item2 && (!object || !cJSON_ReplaceItemInObject(object, "test_key", num_item2))) {
        cJSON_Delete(num_item2);
    }
    
    return 0;
}