// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Create JSON object
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }
    
    // Prepare name and raw strings from fuzzer input
    // Split the input into two parts: name and raw value
    size_t name_len = Size / 2;
    size_t raw_len = Size - name_len;
    
    // Allocate and null-terminate the strings
    char *name = NULL;
    char *raw = NULL;
    
    if (name_len > 0) {
        name = (char *)malloc(name_len + 1);
        if (name) {
            memcpy(name, Data, name_len);
            name[name_len] = '\0';
        }
    }
    
    if (raw_len > 0) {
        raw = (char *)malloc(raw_len + 1);
        if (raw) {
            memcpy(raw, Data + name_len, raw_len);
            raw[raw_len] = '\0';
        }
    }
    
    // Call cJSON_AddRawToObject with various combinations
    if (name && raw) {
        // Normal case
        cJSON *added = cJSON_AddRawToObject(obj, name, raw);
        (void)added; // Use result to avoid unused variable warning
    }
    
    if (name) {
        // Case with empty raw string
        cJSON *added = cJSON_AddRawToObject(obj, name, "");
        (void)added;
    }
    
    if (raw) {
        // Case with empty name
        cJSON *added = cJSON_AddRawToObject(obj, "", raw);
        (void)added;
    }
    
    // Case with both empty
    cJSON *added = cJSON_AddRawToObject(obj, "", "");
    (void)added;
    
    // Case with NULL strings (should handle gracefully)
    cJSON *added_null = cJSON_AddRawToObject(obj, NULL, NULL);
    (void)added_null;
    
    // Clean up allocated strings
    if (name) free(name);
    if (raw) free(raw);
    
    // Delete the JSON object
    cJSON_Delete(obj);
    
    // Try creating and deleting multiple objects
    for (int i = 0; i < 3; i++) {
        cJSON *temp_obj = cJSON_CreateObject();
        if (temp_obj) {
            // Add some raw data
            char temp_name[32];
            char temp_raw[32];
            snprintf(temp_name, sizeof(temp_name), "key%d", i);
            snprintf(temp_raw, sizeof(temp_raw), "value%d", i);
            
            cJSON *temp_added = cJSON_AddRawToObject(temp_obj, temp_name, temp_raw);
            (void)temp_added;
            
            cJSON_Delete(temp_obj);
        }
    }
    
    // Try adding multiple raw values to same object
    cJSON *multi_obj = cJSON_CreateObject();
    if (multi_obj) {
        for (int i = 0; i < 5; i++) {
            char key[16];
            char value[16];
            snprintf(key, sizeof(key), "k%d", i);
            snprintf(value, sizeof(value), "v%d", i);
            
            cJSON *multi_added = cJSON_AddRawToObject(multi_obj, key, value);
            (void)multi_added;
        }
        cJSON_Delete(multi_obj);
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif