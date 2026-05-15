// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_array(void) {
    cJSON* array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
        cJSON_AddItemToArray(array, cJSON_CreateString("test"));
        cJSON_AddItemToArray(array, cJSON_CreateBool(1));
        cJSON_AddItemToArray(array, cJSON_CreateNull());
        cJSON_AddItemToArray(array, cJSON_CreateObject());
    }
    return array;
}

static cJSON* create_test_object(void) {
    cJSON* obj = cJSON_CreateObject();
    if (obj) {
        cJSON_AddItemToObject(obj, "key1", cJSON_CreateNumber(42));
        cJSON_AddItemToObject(obj, "key2", cJSON_CreateString("value"));
        cJSON_AddItemToObject(obj, "key3", cJSON_CreateArray());
    }
    return obj;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to control behavior
    uint8_t mode = Data[0];
    
    // Create test structures
    cJSON* test_array = create_test_array();
    cJSON* test_object = create_test_object();
    cJSON* parsed_json = NULL;
    
    // Try to parse input as JSON (skip first byte)
    if (Size > 1) {
        char* json_string = (char*)malloc(Size);
        if (json_string) {
            memcpy(json_string, Data + 1, Size - 1);
            json_string[Size - 1] = '\0';
            parsed_json = cJSON_Parse(json_string);
            free(json_string);
        }
    }
    
    // Test cJSON_IsArray
    if (test_array) {
        cJSON_IsArray(test_array);
    }
    if (test_object) {
        cJSON_IsArray(test_object);  // Should return false
    }
    if (parsed_json) {
        cJSON_IsArray(parsed_json);
    }
    
    // Test cJSON_DeleteItemFromArray
    if (test_array) {
        int array_size = cJSON_GetArraySize(test_array);
        if (array_size > 0) {
            // Use second byte (if available) to determine index
            int index = 0;
            if (Size > 2) {
                index = Data[1] % array_size;
            }
            // Create a copy to delete from
            cJSON* array_copy = cJSON_Duplicate(test_array, 1);
            if (array_copy) {
                cJSON_DeleteItemFromArray(array_copy, index);
                cJSON_Delete(array_copy);
            }
        }
    }
    
    // Test cJSON_GetArrayItem
    if (test_array) {
        int array_size = cJSON_GetArraySize(test_array);
        for (int i = -2; i < array_size + 2; i++) {
            cJSON_GetArrayItem(test_array, i);
        }
    }
    
    // Test cJSON_IsObject
    if (test_object) {
        cJSON_IsObject(test_object);
    }
    if (test_array) {
        cJSON_IsObject(test_array);  // Should return false
    }
    if (parsed_json) {
        cJSON_IsObject(parsed_json);
    }
    
    // Test cJSON_Compare
    if (test_array && test_object) {
        // Compare different types
        cJSON_Compare(test_array, test_object, (mode & 1) ? 1 : 0);
        
        // Compare array with itself
        cJSON_Compare(test_array, test_array, (mode & 2) ? 1 : 0);
        
        // Compare object with itself
        cJSON_Compare(test_object, test_object, (mode & 4) ? 1 : 0);
    }
    
    // Test cJSON_GetArraySize
    if (test_array) {
        cJSON_GetArraySize(test_array);
    }
    if (test_object) {
        cJSON_GetArraySize(test_object);  // Also works for objects
    }
    if (parsed_json) {
        cJSON_GetArraySize(parsed_json);
    }
    
    // Additional tests with parsed JSON
    if (parsed_json) {
        // Test all functions on parsed JSON
        cJSON_bool is_array = cJSON_IsArray(parsed_json);
        cJSON_bool is_object = cJSON_IsObject(parsed_json);
        
        if (is_array || is_object) {
            int size = cJSON_GetArraySize(parsed_json);
            if (size > 0 && Size > 2) {
                int idx = Data[2] % (size + 2);  // Include out-of-range indices
                cJSON_GetArrayItem(parsed_json, idx);
                
                // Create copy for deletion test
                cJSON* copy = cJSON_Duplicate(parsed_json, 1);
                if (copy) {
                    if (idx >= 0 && idx < size) {
                        cJSON_DeleteItemFromArray(copy, idx);
                    }
                    cJSON_Delete(copy);
                }
            }
        }
        
        // Compare parsed JSON with test structures
        if (test_array) {
            cJSON_Compare(parsed_json, test_array, (mode & 8) ? 1 : 0);
        }
        if (test_object) {
            cJSON_Compare(parsed_json, test_object, (mode & 16) ? 1 : 0);
        }
    }
    
    // Cleanup
    if (test_array) cJSON_Delete(test_array);
    if (test_object) cJSON_Delete(test_object);
    if (parsed_json) cJSON_Delete(parsed_json);
    
    return 0;
}