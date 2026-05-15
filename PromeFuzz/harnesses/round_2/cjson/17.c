// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Create a JSON object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Use fuzzer input to generate name strings for the boolean items
    // We'll create two names from the input data
    char name1[256] = {0};
    char name2[256] = {0};
    
    // Copy data for first name (ensure null termination)
    size_t name1_len = Size > 255 ? 255 : Size;
    if (name1_len > 0) {
        memcpy(name1, Data, name1_len);
        name1[name1_len] = '\0';
    } else {
        name1[0] = '\0';
    }
    
    // Copy data for second name (use remaining data or wrap around)
    size_t name2_len = 0;
    if (Size > 255) {
        name2_len = (Size - 255) > 255 ? 255 : (Size - 255);
        if (name2_len > 0) {
            memcpy(name2, Data + 255, name2_len);
            name2[name2_len] = '\0';
        }
    } else {
        // If we don't have enough data, use the same data but reversed
        if (name1_len > 0) {
            for (size_t i = 0; i < name1_len; i++) {
                name2[i] = name1[name1_len - 1 - i];
            }
            name2[name1_len] = '\0';
        }
    }
    
    // Generate boolean values from input data
    cJSON_bool bool1 = 0;
    cJSON_bool bool2 = 0;
    
    if (Size > 0) {
        bool1 = (Data[0] & 1) ? 1 : 0;
    }
    if (Size > 1) {
        bool2 = (Data[1] & 1) ? 1 : 0;
    }
    
    // Add first boolean to object
    cJSON *item1 = cJSON_AddBoolToObject(root, name1, bool1);
    
    // Add second boolean to object
    cJSON *item2 = cJSON_AddBoolToObject(root, name2, bool2);
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}