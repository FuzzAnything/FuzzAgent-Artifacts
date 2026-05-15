// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
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

static cJSON *create_test_object(void) {
    cJSON *obj = cJSON_CreateObject();
    if (obj) {
        cJSON_AddStringToObject(obj, "key1", "value1");
        cJSON_AddNumberToObject(obj, "key2", 42);
        cJSON_AddTrueToObject(obj, "key3");
        cJSON_AddRawToObject(obj, "key4", "{\"nested\": true}");
    }
    return obj;
}

static cJSON *create_test_array(void) {
    cJSON *arr = cJSON_CreateArray();
    if (arr) {
        cJSON_AddItemToArray(arr, cJSON_CreateString("array_item1"));
        cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));
        cJSON_AddItemToArray(arr, cJSON_CreateTrue());
    }
    return arr;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a null-terminated string from fuzzer input
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) return 0;
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    // Parse JSON from fuzzer input
    cJSON *parsed = cJSON_Parse(json_str);
    
    // Test cJSON_IsRaw
    if (parsed) {
        cJSON_IsRaw(parsed);
        cJSON_IsRaw(NULL);
        
        // Test on child items if available
        if (parsed->child) {
            cJSON_IsRaw(parsed->child);
        }
    }
    
    // Test cJSON_IsString
    if (parsed) {
        cJSON_IsString(parsed);
        cJSON_IsString(NULL);
        
        // Test on child items if available
        if (parsed->child) {
            cJSON_IsString(parsed->child);
        }
    }
    
    // Test cJSON_IsTrue
    if (parsed) {
        cJSON_IsTrue(parsed);
        cJSON_IsTrue(NULL);
        
        // Test on child items if available
        if (parsed->child) {
            cJSON_IsTrue(parsed->child);
        }
    }
    
    // Create additional test items
    cJSON *test_obj = create_test_object();
    cJSON *test_arr = create_test_array();
    cJSON *raw_item = cJSON_CreateRaw("{\"test\": true}");
    cJSON *string_item = cJSON_CreateString("test string");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *null_item = cJSON_CreateNull();
    
    // Test cJSON_Compare with various combinations
    if (parsed) {
        // Compare parsed with itself
        cJSON_Compare(parsed, parsed, 1);
        cJSON_Compare(parsed, parsed, 0);
        
        // Compare with other items
        if (test_obj) {
            cJSON_Compare(parsed, test_obj, 1);
            cJSON_Compare(test_obj, parsed, 0);
        }
        
        // Compare with NULL
        cJSON_Compare(parsed, NULL, 1);
        cJSON_Compare(NULL, parsed, 1);
        cJSON_Compare(NULL, NULL, 1);
    }
    
    // Compare test items with each other
    if (test_obj && test_arr) {
        cJSON_Compare(test_obj, test_arr, 1);
    }
    
    // Test cJSON_GetObjectItemCaseSensitive
    if (test_obj) {
        cJSON_GetObjectItemCaseSensitive(test_obj, "key1");
        cJSON_GetObjectItemCaseSensitive(test_obj, "KEY1"); // Different case
        cJSON_GetObjectItemCaseSensitive(test_obj, "nonexistent");
        cJSON_GetObjectItemCaseSensitive(NULL, "key1");
        cJSON_GetObjectItemCaseSensitive(test_obj, NULL);
        
        // Use fuzzer data as key
        if (Size > 0) {
            char key[256];
            size_t key_len = Size < 255 ? Size : 255;
            memcpy(key, Data, key_len);
            key[key_len] = '\0';
            cJSON_GetObjectItemCaseSensitive(test_obj, key);
        }
    }
    
    // Test cJSON_Duplicate
    if (parsed) {
        // Duplicate with recursion
        cJSON *dup_recursive = cJSON_Duplicate(parsed, 1);
        if (dup_recursive) {
            cJSON_Delete(dup_recursive);
        }
        
        // Duplicate without recursion
        cJSON *dup_nonrecursive = cJSON_Duplicate(parsed, 0);
        if (dup_nonrecursive) {
            cJSON_Delete(dup_nonrecursive);
        }
        
        // Duplicate NULL
        cJSON_Duplicate(NULL, 1);
        cJSON_Duplicate(NULL, 0);
    }
    
    // Duplicate other test items
    if (test_obj) {
        cJSON *dup_obj = cJSON_Duplicate(test_obj, 1);
        if (dup_obj) {
            cJSON_Delete(dup_obj);
        }
    }
    
    if (test_arr) {
        cJSON *dup_arr = cJSON_Duplicate(test_arr, 0);
        if (dup_arr) {
            cJSON_Delete(dup_arr);
        }
    }
    
    // Test with raw, string, and boolean items
    if (raw_item) {
        cJSON_IsRaw(raw_item);
        cJSON_IsString(raw_item);
        cJSON_Duplicate(raw_item, 1);
    }
    
    if (string_item) {
        cJSON_IsRaw(string_item);
        cJSON_IsString(string_item);
        cJSON_Duplicate(string_item, 1);
    }
    
    if (true_item) {
        cJSON_IsTrue(true_item);
        cJSON_IsString(true_item);
        cJSON_Duplicate(true_item, 1);
    }
    
    if (false_item) {
        cJSON_IsTrue(false_item);
        cJSON_Duplicate(false_item, 1);
    }
    
    // Test comparisons between various types
    if (true_item && false_item) {
        cJSON_Compare(true_item, false_item, 1);
    }
    
    if (string_item && raw_item) {
        cJSON_Compare(string_item, raw_item, 1);
    }
    
    // Cleanup
    if (parsed) cJSON_Delete(parsed);
    if (test_obj) cJSON_Delete(test_obj);
    if (test_arr) cJSON_Delete(test_arr);
    if (raw_item) cJSON_Delete(raw_item);
    if (string_item) cJSON_Delete(string_item);
    if (true_item) cJSON_Delete(true_item);
    if (false_item) cJSON_Delete(false_item);
    if (null_item) cJSON_Delete(null_item);
    
    free(json_str);
    return 0;
}