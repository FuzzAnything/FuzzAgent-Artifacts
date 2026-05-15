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
    // Create JSON object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }
    
    // Prepare key names from fuzzer input
    // We need at least 2 bytes for keys, but handle smaller inputs gracefully
    if (Size >= 2) {
        // Create first key (use first byte as length, but ensure null termination)
        size_t key1_len = Data[0] % 32;  // Limit key length
        if (key1_len > Size - 1) key1_len = Size > 1 ? Size - 1 : 0;
        char *key1 = (char*)malloc(key1_len + 1);
        if (key1) {
            memcpy(key1, Data + 1, key1_len);
            key1[key1_len] = '\0';
            
            // Use second byte for boolean value (0 = false, non-zero = true)
            cJSON_bool bool1 = (Data[1] & 1) ? 1 : 0;
            
            // Add first boolean
            cJSON_AddBoolToObject(root, key1, bool1);
            free(key1);
        }
        
        // Create second key if we have enough data
        if (Size >= 4) {
            size_t key2_len = Data[2] % 32;  // Limit key length
            size_t key2_start = 3;
            if (key2_len > Size - key2_start) key2_len = Size > key2_start ? Size - key2_start : 0;
            char *key2 = (char*)malloc(key2_len + 1);
            if (key2) {
                memcpy(key2, Data + key2_start, key2_len);
                key2[key2_len] = '\0';
                
                // Use fourth byte for boolean value
                cJSON_bool bool2 = (Data[3] & 1) ? 1 : 0;
                
                // Add second boolean
                cJSON_AddBoolToObject(root, key2, bool2);
                free(key2);
            }
        }
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}