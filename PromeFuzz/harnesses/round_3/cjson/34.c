// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_object(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;
    
    // Add some items with keys derived from fuzzer data
    if (size > 0) {
        char key1[32] = {0};
        char key2[32] = {0};
        
        // Create keys from first few bytes
        size_t key_len = size > 31 ? 31 : size;
        for (size_t i = 0; i < key_len; i++) {
            key1[i] = 'A' + (data[i] % 26);
            key2[i] = 'a' + (data[i] % 26);
        }
        key1[key_len] = '\0';
        key2[key_len] = '\0';
        
        cJSON_AddItemToObject(root, key1, cJSON_CreateNumber(42));
        cJSON_AddItemToObject(root, key2, cJSON_CreateString("test"));
        
        // Add a nested object
        cJSON *nested = cJSON_CreateObject();
        if (nested) {
            cJSON_AddItemToObject(nested, "nested_key", cJSON_CreateTrue());
            cJSON_AddItemToObject(root, "nested", nested);
        }
        
        // Add an array
        cJSON *array = cJSON_CreateArray();
        if (array) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
            cJSON_AddItemToArray(array, cJSON_CreateNumber(2));
            cJSON_AddItemToObject(root, "array", array);
        }
    }
    
    return root;
}

static void test_detach_functions(cJSON *obj, const char *key) {
    if (!obj || !key) return;
    
    // Test case-insensitive detach
    cJSON *detached = cJSON_DetachItemFromObject(obj, key);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Test case-sensitive detach
    detached = cJSON_DetachItemFromObjectCaseSensitive(obj, key);
    if (detached) {
        cJSON_Delete(detached);
    }
}

static void test_delete_functions(cJSON *obj, const char *key) {
    if (!obj || !key) return;
    
    // Test case-insensitive delete
    cJSON_DeleteItemFromObject(obj, key);
    
    // Test case-sensitive delete
    cJSON_DeleteItemFromObjectCaseSensitive(obj, key);
}

static void test_detach_via_pointer(cJSON *obj) {
    if (!obj) return;
    
    // Try to detach first child if exists
    if (obj->child) {
        cJSON *detached = cJSON_DetachItemViaPointer(obj, obj->child);
        if (detached) {
            cJSON_Delete(detached);
        }
    }
    
    // Try to detach via next pointer if exists
    if (obj->child && obj->child->next) {
        cJSON *detached = cJSON_DetachItemViaPointer(obj, obj->child->next);
        if (detached) {
            cJSON_Delete(detached);
        }
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Create test object
    cJSON *root = create_test_object(data, size);
    if (!root) return 0;
    
    // Extract key from fuzzer data
    char test_key[64] = {0};
    size_t key_len = size > 63 ? 63 : size;
    for (size_t i = 0; i < key_len; i++) {
        test_key[i] = 32 + (data[i] % 95);  // Printable ASCII
    }
    test_key[key_len] = '\0';
    
    // Test cJSON_CreateTrue
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        // Add it to object and then detach/delete
        cJSON_AddItemToObject(root, "fuzz_true", true_item);
        
        // Test detach via pointer on the true item
        cJSON *detached_true = cJSON_DetachItemViaPointer(root, true_item);
        if (detached_true) {
            cJSON_Delete(detached_true);
        }
    }
    
    // Test detach functions
    test_detach_functions(root, test_key);
    
    // Test delete functions
    test_delete_functions(root, test_key);
    
    // Test detach via pointer
    test_detach_via_pointer(root);
    
    // Test with empty key
    test_detach_functions(root, "");
    test_delete_functions(root, "");
    
    // Test with NULL key
    test_detach_functions(root, NULL);
    test_delete_functions(root, NULL);
    
    // Test with special keys that might exist
    test_detach_functions(root, "nested");
    test_detach_functions(root, "array");
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}