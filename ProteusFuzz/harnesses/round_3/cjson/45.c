// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Create a copy of the input data as a null-terminated string
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *obj = cJSON_Parse(json_str);
    free(json_str);
    
    return obj;
}

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a JSON object from input data
    cJSON *obj = create_test_object(Data, Size);
    
    // If parsing failed, try to create a simple object manually
    if (!obj) {
        obj = cJSON_CreateObject();
    }
    
    if (!obj) {
        return 0;
    }

    // Ensure the object is of type cJSON_Object for these functions
    if (obj->type != cJSON_Object) {
        cJSON_Delete(obj);
        obj = cJSON_CreateObject();
    }

    // Create a dummy item to attach/replace
    cJSON *item = cJSON_CreateString("test_value");
    if (!item) {
        cJSON_Delete(obj);
        return 0;
    }

    // Extract key from data if possible, otherwise use a default
    // We must ensure the key string is null-terminated and valid
    char key_buf[256];
    const char *key = "key";
    
    if (Size > 0) {
        size_t key_len = Size < 255 ? Size : 255;
        // Ensure null termination
        memcpy(key_buf, Data, key_len);
        key_buf[key_len] = '\0';
        
        // Replace any null bytes in the middle with a safe character to ensure validity
        for (size_t i = 0; i < key_len; i++) {
            if (key_buf[i] == '\0') {
                key_buf[i] = ' ';
            }
        }
        key = key_buf;
    }

    // Test 1: Add item to object
    cJSON_AddItemToObject(obj, key, item);
    
    // Test 2: Detach item from object
    cJSON *detached = cJSON_DetachItemFromObject(obj, key);
    // detached is now owned by caller, we should free it or re-add it
    if (detached) {
        // Re-add it to keep it in the tree for further testing, or delete it
        // Here we re-add to ensure we can test other functions on a populated object
        cJSON_AddItemToObject(obj, key, detached);
    }
    
    // Test 3: Detach item case sensitive
    cJSON *detached_cs = cJSON_DetachItemFromObjectCaseSensitive(obj, key);
    if (detached_cs) {
        cJSON_AddItemToObject(obj, key, detached_cs);
    }
    
    // Test 4: Replace item in object
    cJSON *new_item = cJSON_CreateNumber(123);
    if (new_item) {
        cJSON_ReplaceItemInObject(obj, key, new_item);
        // new_item is now owned by obj, don't delete separately
    }
    
    // Test 5: Delete item from object
    cJSON_DeleteItemFromObject(obj, key);
    
    // Test 6: Delete item from object case sensitive
    cJSON_DeleteItemFromObjectCaseSensitive(obj, key);
    
    // Cleanup
    cleanup_cjson(obj);
    
    return 0;
}