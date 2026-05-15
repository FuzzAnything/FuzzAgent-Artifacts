// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_json_from_data(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return NULL;
    }
    
    char *json_str = (char*)malloc(Size + 1);
    if (!json_str) {
        return NULL;
    }
    
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    free(json_str);
    
    return json;
}

static cJSON* create_fixed_test_json(void) {
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }
    
    cJSON_AddTrueToObject(root, "true_key");
    cJSON_AddFalseToObject(root, "false_key");
    cJSON_AddStringToObject(root, "string_key", "test_string");
    cJSON_AddNumberToObject(root, "number_key", 42.5);
    cJSON_AddRawToObject(root, "raw_key", "{\"nested\": \"raw\"}");
    
    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToArray(array, cJSON_CreateString("array_item1"));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(100));
        cJSON_AddItemToObject(root, "array_key", array);
    }
    
    return root;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Create JSON objects from fuzzer input
    cJSON *json1 = create_test_json_from_data(Data, Size);
    
    // Create second JSON object with offset data if possible
    cJSON *json2 = NULL;
    if (Size > 1) {
        json2 = create_test_json_from_data(Data + 1, Size - 1);
    }
    
    // Create fixed JSON for comparison
    cJSON *fixed_json = create_fixed_test_json();
    
    // Test cJSON_Duplicate
    if (json1) {
        cJSON *dup1 = cJSON_Duplicate(json1, 0);
        cJSON *dup2 = cJSON_Duplicate(json1, 1);
        
        if (dup1) {
            // Test cJSON_Compare with duplicates
            cJSON_Compare(json1, dup1, 1);
            cJSON_Compare(json1, dup1, 0);
            cJSON_Delete(dup1);
        }
        
        if (dup2) {
            cJSON_Compare(json1, dup2, 1);
            cJSON_Compare(json1, dup2, 0);
            cJSON_Delete(dup2);
        }
    }
    
    // Test cJSON_Compare between various objects
    if (json1 && json2) {
        cJSON_Compare(json1, json2, 1);
        cJSON_Compare(json1, json2, 0);
    }
    
    if (json1 && fixed_json) {
        cJSON_Compare(json1, fixed_json, 1);
        cJSON_Compare(json1, fixed_json, 0);
    }
    
    // Test type checking functions
    if (json1) {
        cJSON_IsRaw(json1);
        cJSON_IsString(json1);
        cJSON_IsTrue(json1);
        
        // Test on child items if they exist
        if (json1->child) {
            cJSON_IsRaw(json1->child);
            cJSON_IsString(json1->child);
            cJSON_IsTrue(json1->child);
        }
    }
    
    if (fixed_json) {
        cJSON_IsRaw(fixed_json);
        cJSON_IsString(fixed_json);
        cJSON_IsTrue(fixed_json);
        
        // Test cJSON_GetObjectItemCaseSensitive on fixed_json
        if (fixed_json->child) {
            cJSON_GetObjectItemCaseSensitive(fixed_json, "true_key");
            cJSON_GetObjectItemCaseSensitive(fixed_json, "string_key");
            cJSON_GetObjectItemCaseSensitive(fixed_json, "NON_EXISTENT_KEY");
            
            // Test with fuzzer data as key
            if (Size > 0) {
                char *key = (char*)malloc(Size + 1);
                if (key) {
                    memcpy(key, Data, Size);
                    key[Size] = '\0';
                    cJSON_GetObjectItemCaseSensitive(fixed_json, key);
                    free(key);
                }
            }
        }
    }
    
    // Cleanup
    if (json1) {
        cJSON_Delete(json1);
    }
    if (json2) {
        cJSON_Delete(json2);
    }
    if (fixed_json) {
        cJSON_Delete(fixed_json);
    }
    
    return 0;
}