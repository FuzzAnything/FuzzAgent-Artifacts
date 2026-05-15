// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_functions(cJSON *json, const char *key, int index) {
    // Test cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(json, key);
    (void)item1;
    
    // Test cJSON_GetObjectItem
    cJSON *item2 = cJSON_GetObjectItem(json, key);
    (void)item2;
    
    // Test cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(json, key);
    (void)has_item;
    
    // Test cJSON_GetArrayItem
    cJSON *array_item = cJSON_GetArrayItem(json, index);
    (void)array_item;
    
    // Test cJSON_IsObject
    cJSON_bool is_obj = cJSON_IsObject(json);
    (void)is_obj;
    
    // Test cJSON_Compare
    cJSON_bool compare_result = cJSON_Compare(json, json, 1);
    (void)compare_result;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create a JSON string from the input data
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) return 0;
    
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    // Parse the JSON
    cJSON *json = cJSON_Parse(json_str);
    free(json_str);
    
    if (json) {
        // Generate key from input data (use first few bytes)
        size_t key_len = Size > 32 ? 32 : Size;
        char *key = (char *)malloc(key_len + 1);
        if (key) {
            memcpy(key, Data, key_len);
            key[key_len] = '\0';
            
            // Generate index from input data
            int index = 0;
            if (Size > 0) {
                index = Data[0] % 100;  // Limit to reasonable range
            }
            
            // Test all target functions
            test_functions(json, key, index);
            
            // Also test with NULL key
            test_functions(json, NULL, index);
            
            // Test with empty key
            test_functions(json, "", index);
            
            free(key);
        }
        
        // Test with child items if available
        if (json->child) {
            size_t child_key_len = Size > 16 ? 16 : Size;
            char *child_key = (char *)malloc(child_key_len + 1);
            if (child_key) {
                memcpy(child_key, Data, child_key_len);
                child_key[child_key_len] = '\0';
                
                int child_index = 0;
                if (Size > 1) {
                    child_index = Data[1] % 50;
                }
                
                test_functions(json->child, child_key, child_index);
                free(child_key);
            }
        }
        
        // Test comparison with different case sensitivity
        if (json->child) {
            cJSON_Compare(json, json->child, 0);
            cJSON_Compare(json, json->child, 1);
        }
        
        // Clean up
        cJSON_Delete(json);
    }
    
    // Test with NULL JSON object
    test_functions(NULL, "dummy", 0);
    
    return 0;
}