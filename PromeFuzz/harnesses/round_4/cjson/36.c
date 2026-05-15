// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_functions(cJSON *item1, cJSON *item2, const char *key) {
    // Test cJSON_Duplicate with both recursive and non-recursive modes
    cJSON *dup1 = cJSON_Duplicate(item1, 0);
    cJSON *dup2 = cJSON_Duplicate(item1, 1);
    cJSON *dup3 = cJSON_Duplicate(NULL, 0);
    cJSON *dup4 = cJSON_Duplicate(NULL, 1);
    
    // Clean up duplicates
    if (dup1) cJSON_Delete(dup1);
    if (dup2) cJSON_Delete(dup2);
    if (dup3) cJSON_Delete(dup3);
    if (dup4) cJSON_Delete(dup4);
    
    // Test cJSON_Compare with case sensitive and insensitive modes
    cJSON_Compare(item1, item2, 1);
    cJSON_Compare(item1, item2, 0);
    cJSON_Compare(NULL, item2, 1);
    cJSON_Compare(item1, NULL, 1);
    cJSON_Compare(NULL, NULL, 1);
    
    // Test type checking functions
    cJSON_IsRaw(item1);
    cJSON_IsRaw(item2);
    cJSON_IsRaw(NULL);
    
    cJSON_IsString(item1);
    cJSON_IsString(item2);
    cJSON_IsString(NULL);
    
    cJSON_IsTrue(item1);
    cJSON_IsTrue(item2);
    cJSON_IsTrue(NULL);
    
    // Test object item retrieval
    if (item1 && item1->type == cJSON_Object) {
        cJSON_GetObjectItemCaseSensitive(item1, key);
        cJSON_GetObjectItemCaseSensitive(item1, "non_existent_key");
    }
    if (item2 && item2->type == cJSON_Object) {
        cJSON_GetObjectItemCaseSensitive(item2, key);
    }
    cJSON_GetObjectItemCaseSensitive(NULL, key);
    cJSON_GetObjectItemCaseSensitive(item1, NULL);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from input data for parsing
    char *json_string = NULL;
    if (Size > 0) {
        json_string = (char *)malloc(Size + 1);
        if (!json_string) return 0;
        memcpy(json_string, Data, Size);
        json_string[Size] = '\0';
    } else {
        // Handle empty input
        json_string = strdup("");
        if (!json_string) return 0;
    }
    
    // Create a key string from input data
    char *key_string = NULL;
    if (Size > 0) {
        // Use first 64 bytes or less for key
        size_t key_len = Size > 64 ? 64 : Size;
        key_string = (char *)malloc(key_len + 1);
        if (key_string) {
            memcpy(key_string, Data, key_len);
            key_string[key_len] = '\0';
        }
    }
    
    // Parse JSON from input data
    cJSON *item1 = cJSON_Parse(json_string);
    
    // Create some test JSON objects for comparison
    cJSON *item2 = NULL;
    
    // Create a simple object for testing
    cJSON *test_obj = cJSON_CreateObject();
    if (test_obj) {
        cJSON_AddStringToObject(test_obj, "key", "value");
        cJSON_AddNumberToObject(test_obj, "num", 42);
        cJSON_AddTrueToObject(test_obj, "bool");
        
        // Use item1 if valid, otherwise use test_obj
        if (item1 && item1->type != cJSON_Invalid) {
            item2 = item1;
        } else {
            item2 = test_obj;
        }
        
        // Test with various combinations
        test_functions(item1, item2, key_string ? key_string : "key");
        
        // Also test with raw JSON type
        cJSON *raw_item = cJSON_CreateRaw("{\"raw\":\"data\"}");
        if (raw_item) {
            test_functions(raw_item, item2, key_string ? key_string : "key");
            cJSON_Delete(raw_item);
        }
        
        // Test with string type
        cJSON *string_item = cJSON_CreateString("test string");
        if (string_item) {
            test_functions(string_item, item2, key_string ? key_string : "key");
            cJSON_Delete(string_item);
        }
        
        // Test with boolean true
        cJSON *true_item = cJSON_CreateTrue();
        if (true_item) {
            test_functions(true_item, item2, key_string ? key_string : "key");
            cJSON_Delete(true_item);
        }
        
        cJSON_Delete(test_obj);
    }
    
    // Clean up
    if (item1) cJSON_Delete(item1);
    free(json_string);
    if (key_string) free(key_string);
    
    return 0;
}