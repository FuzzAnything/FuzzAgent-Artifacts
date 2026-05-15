// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

static cJSON* create_test_object(void) {
    cJSON *root = cJSON_CreateObject();
    if (root) {
        cJSON_AddStringToObject(root, "key1", "value1");
        cJSON_AddNumberToObject(root, "key2", 42);
        cJSON_AddTrueToObject(root, "key3");
        cJSON_AddFalseToObject(root, "key4");
        cJSON_AddNullToObject(root, "key5");
        
        cJSON *array = cJSON_CreateArray();
        if (array) {
            cJSON_AddItemToArray(array, cJSON_CreateString("array_item1"));
            cJSON_AddItemToArray(array, cJSON_CreateNumber(123));
            cJSON_AddItemToObject(root, "array_key", array);
        }
        
        cJSON *raw = cJSON_CreateRaw("{\"raw\": \"data\"}");
        if (raw) {
            cJSON_AddItemToObject(root, "raw_key", raw);
        }
    }
    return root;
}

static cJSON* create_test_string_item(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    char *str = (char*)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    cJSON *item = cJSON_CreateString(str);
    free(str);
    return item;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
    // Test 1: cJSON_Duplicate with various inputs
    cJSON *test_obj = create_test_object();
    if (test_obj) {
        // Use first byte to determine recurse parameter
        cJSON_bool recurse = (size > 0) ? (data[0] & 1) : 0;
        cJSON *duplicate = cJSON_Duplicate(test_obj, recurse);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        
        // Test duplicate with NULL input
        cJSON_Duplicate(NULL, recurse);
        
        cJSON_Delete(test_obj);
    }
    
    // Test 2: cJSON_Compare with various combinations
    cJSON *obj1 = create_test_object();
    cJSON *obj2 = create_test_object();
    
    if (obj1 && obj2) {
        // Use second byte (if available) for case_sensitive parameter
        cJSON_bool case_sensitive = (size > 1) ? ((data[1] & 1) ? 1 : 0) : 1;
        
        // Compare identical objects
        cJSON_Compare(obj1, obj2, case_sensitive);
        
        // Compare with NULL
        cJSON_Compare(NULL, obj2, case_sensitive);
        cJSON_Compare(obj1, NULL, case_sensitive);
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        // Compare with modified object
        if (obj2->child) {
            cJSON *first_child = obj2->child;
            if (first_child->valuestring) {
                // Modify the value to make objects different
                first_child->valuestring[0] = 'X';
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
        }
    }
    
    if (obj1) cJSON_Delete(obj1);
    if (obj2) cJSON_Delete(obj2);
    
    // Test 3: cJSON_IsRaw
    cJSON *raw_item = cJSON_CreateRaw("{\"test\": 123}");
    if (raw_item) {
        cJSON_IsRaw(raw_item);
        cJSON_IsRaw(NULL);
        cJSON_Delete(raw_item);
    }
    
    // Test with non-raw item
    cJSON *non_raw = cJSON_CreateNumber(42);
    if (non_raw) {
        cJSON_IsRaw(non_raw);
        cJSON_Delete(non_raw);
    }
    
    // Test 4: cJSON_IsString
    cJSON *string_item = create_test_string_item(data, size);
    if (string_item) {
        cJSON_IsString(string_item);
        cJSON_Delete(string_item);
    }
    
    // Test with non-string items
    cJSON *num_item = cJSON_CreateNumber(3.14);
    if (num_item) {
        cJSON_IsString(num_item);
        cJSON_Delete(num_item);
    }
    
    cJSON *bool_item = cJSON_CreateTrue();
    if (bool_item) {
        cJSON_IsString(bool_item);
        cJSON_Delete(bool_item);
    }
    
    cJSON_IsString(NULL);
    
    // Test 5: cJSON_GetObjectItemCaseSensitive
    cJSON *object_with_items = cJSON_CreateObject();
    if (object_with_items) {
        cJSON_AddStringToObject(object_with_items, "testKey", "testValue");
        cJSON_AddNumberToObject(object_with_items, "anotherKey", 999);
        
        // Try to get items with keys from fuzzer input
        if (size > 0) {
            // Use up to 100 bytes for key or whatever is available
            size_t key_len = (size > 100) ? 100 : size;
            char *key = (char*)malloc(key_len + 1);
            if (key) {
                memcpy(key, data, key_len);
                key[key_len] = '\0';
                
                cJSON_GetObjectItemCaseSensitive(object_with_items, key);
                
                // Also test with exact known key
                cJSON_GetObjectItemCaseSensitive(object_with_items, "testKey");
                cJSON_GetObjectItemCaseSensitive(object_with_items, "nonexistent");
                
                free(key);
            }
        }
        
        // Test with NULL inputs
        cJSON_GetObjectItemCaseSensitive(NULL, "test");
        cJSON_GetObjectItemCaseSensitive(object_with_items, NULL);
        
        cJSON_Delete(object_with_items);
    }
    
    // Test 6: cJSON_IsTrue
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        cJSON_IsTrue(true_item);
        cJSON_Delete(true_item);
    }
    
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_IsTrue(false_item);
        cJSON_Delete(false_item);
    }
    
    cJSON *null_item = cJSON_CreateNull();
    if (null_item) {
        cJSON_IsTrue(null_item);
        cJSON_Delete(null_item);
    }
    
    cJSON_IsTrue(NULL);
    
    // Additional test: Create a boolean item from fuzzer data
    if (size > 0) {
        cJSON *dynamic_bool = cJSON_CreateBool((data[0] & 1) ? 1 : 0);
        if (dynamic_bool) {
            cJSON_IsTrue(dynamic_bool);
            cJSON_Delete(dynamic_bool);
        }
    }
    
    return 0;
}