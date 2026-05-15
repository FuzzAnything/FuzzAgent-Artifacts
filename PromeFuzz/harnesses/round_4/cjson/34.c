// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Parse input as JSON
    cJSON *parsed = cJSON_ParseWithLength((const char*)Data, Size);
    
    // Test cJSON_Duplicate with various recursion settings
    if (parsed != NULL) {
        cJSON *duplicate_no_recurse = cJSON_Duplicate(parsed, 0);
        cJSON *duplicate_recurse = cJSON_Duplicate(parsed, 1);
        
        // Test cJSON_Compare with different case sensitivity settings
        if (duplicate_no_recurse != NULL) {
            cJSON_Compare(parsed, duplicate_no_recurse, 0); // case insensitive
            cJSON_Compare(parsed, duplicate_no_recurse, 1); // case sensitive
        }
        
        if (duplicate_recurse != NULL) {
            cJSON_Compare(parsed, duplicate_recurse, 0);
            cJSON_Compare(parsed, duplicate_recurse, 1);
        }
        
        // Test type checking functions
        cJSON_IsString(parsed);
        cJSON_IsTrue(parsed);
        cJSON_IsNull(parsed);
        
        // Test GetObjectItemCaseSensitive if parsed is an object
        if (parsed->type == cJSON_Object) {
            // Create a key from the input data
            char key[256];
            size_t key_len = Size < 255 ? Size : 255;
            if (key_len > 0) {
                memcpy(key, Data, key_len);
                key[key_len] = '\0';
                cJSON_GetObjectItemCaseSensitive(parsed, key);
            }
        }
        
        // Clean up duplicates
        if (duplicate_no_recurse != NULL) {
            cJSON_Delete(duplicate_no_recurse);
        }
        if (duplicate_recurse != NULL) {
            cJSON_Delete(duplicate_recurse);
        }
    }
    
    // Test functions with NULL input
    cJSON_Duplicate(NULL, 0);
    cJSON_Duplicate(NULL, 1);
    cJSON_Compare(NULL, NULL, 0);
    cJSON_Compare(NULL, NULL, 1);
    cJSON_IsString(NULL);
    cJSON_IsTrue(NULL);
    cJSON_IsNull(NULL);
    cJSON_GetObjectItemCaseSensitive(NULL, "test");
    
    // Test with fixed cJSON objects
    cJSON *fixed_string = cJSON_CreateString("test");
    cJSON *fixed_true = cJSON_CreateTrue();
    cJSON *fixed_null = cJSON_CreateNull();
    cJSON *fixed_object = cJSON_CreateObject();
    
    if (fixed_object != NULL) {
        cJSON_AddStringToObject(fixed_object, "key1", "value1");
        cJSON_AddBoolToObject(fixed_object, "key2", 1);
        cJSON_AddNullToObject(fixed_object, "key3");
    }
    
    // Test cJSON_Compare with fixed objects and parsed object
    if (parsed != NULL) {
        cJSON_Compare(parsed, fixed_string, 0);
        cJSON_Compare(parsed, fixed_true, 0);
        cJSON_Compare(parsed, fixed_null, 0);
        cJSON_Compare(parsed, fixed_object, 0);
    }
    
    // Test GetObjectItemCaseSensitive on fixed object
    if (fixed_object != NULL) {
        cJSON_GetObjectItemCaseSensitive(fixed_object, "key1");
        cJSON_GetObjectItemCaseSensitive(fixed_object, "KEY1"); // Different case
        cJSON_GetObjectItemCaseSensitive(fixed_object, "nonexistent");
    }
    
    // Test type checking on fixed objects
    if (fixed_string != NULL) {
        cJSON_IsString(fixed_string);
        cJSON_IsTrue(fixed_string);
        cJSON_IsNull(fixed_string);
    }
    
    if (fixed_true != NULL) {
        cJSON_IsString(fixed_true);
        cJSON_IsTrue(fixed_true);
        cJSON_IsNull(fixed_true);
    }
    
    if (fixed_null != NULL) {
        cJSON_IsString(fixed_null);
        cJSON_IsTrue(fixed_null);
        cJSON_IsNull(fixed_null);
    }
    
    // Clean up fixed objects
    if (fixed_string != NULL) cJSON_Delete(fixed_string);
    if (fixed_true != NULL) cJSON_Delete(fixed_true);
    if (fixed_null != NULL) cJSON_Delete(fixed_null);
    if (fixed_object != NULL) cJSON_Delete(fixed_object);
    
    // Clean up parsed object
    if (parsed != NULL) {
        cJSON_Delete(parsed);
    }
    
    return 0;
}