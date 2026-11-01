// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Extract key names and boolean values from input
    // Use first byte as length for first key, second byte for second key
    size_t key1_len = Data[0] % 32;
    size_t key2_len = Data[1] % 32;
    
    // Ensure we have enough data for keys
    if (Size < 2 + key1_len + key2_len) {
        cJSON_Delete(root);
        return 0;
    }

    // Create temporary buffers for keys
    char key1[33];
    char key2[33];
    
    if (key1_len > 0) {
        memcpy(key1, Data + 2, key1_len);
        key1[key1_len] = '\0';
    } else {
        strcpy(key1, "key1");
    }
    
    if (key2_len > 0) {
        memcpy(key2, Data + 2 + key1_len, key2_len);
        key2[key2_len] = '\0';
    } else {
        strcpy(key2, "key2");
    }

    // Extract boolean values from remaining data
    cJSON_bool bool1 = (Size > 2 + key1_len + key2_len) ? (Data[2 + key1_len + key2_len] & 1) : 0;
    cJSON_bool bool2 = (Size > 3 + key1_len + key2_len) ? (Data[3 + key1_len + key2_len] & 1) : 0;

    // Add first boolean
    cJSON *item1 = cJSON_AddBoolToObject(root, key1, bool1);
    if (item1 == NULL) {
        // Allocation failed or key exists, but we continue to test error handling
    }

    // Add second boolean
    cJSON *item2 = cJSON_AddBoolToObject(root, key2, bool2);
    if (item2 == NULL) {
        // Allocation failed or key exists
    }

    // Clean up
    cJSON_Delete(root);

    return 0;
}