// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"

/* Helper to create a simple cJSON object with some initial items for testing */
static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }
    
    /* Create a string from input data to use as JSON content */
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return NULL;
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);
    
    if (root && root->type == cJSON_Object) {
        return root;
    }
    
    cJSON_Delete(root);
    return cJSON_CreateObject();
}

/* Helper to create a simple cJSON item */
static cJSON *create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateString("default");
    }
    
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    
    cJSON *item = cJSON_CreateString(str);
    free(str);
    return item;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Split input data into parts for object and key */
    size_t key_len = Size / 2;
    if (key_len == 0) key_len = 1;
    
    const char *key_start = (const char *)Data;
    const char *key_end = (const char *)(Data + key_len);
    
    /* Create a null-terminated key string */
    char *key = (char *)malloc(key_len + 1);
    if (!key) return 0;
    memcpy(key, key_start, key_len);
    key[key_len] = '\0';
    
    /* Create test object */
    cJSON *obj = create_test_object(Data + key_len, Size - key_len);
    if (!obj) {
        free(key);
        return 0;
    }
    
    /* Add some initial items to the object to ensure there's something to delete/detach */
    cJSON *item1 = cJSON_CreateString("value1");
    cJSON *item2 = cJSON_CreateNumber(123);
    cJSON_AddItemToObject(obj, "key1", item1);
    cJSON_AddItemToObject(obj, "key2", item2);
    
    /* Test cJSON_DeleteItemFromObject */
    cJSON_DeleteItemFromObject(obj, key);
    
    /* Re-add items for next test */
    cJSON_AddItemToObject(obj, "key1", cJSON_CreateString("value1"));
    cJSON_AddItemToObject(obj, "key2", cJSON_CreateNumber(123));
    
    /* Test cJSON_DetachItemFromObject */
    cJSON *detached = cJSON_DetachItemFromObject(obj, key);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    /* Re-add items for next test */
    cJSON_AddItemToObject(obj, "key1", cJSON_CreateString("value1"));
    cJSON_AddItemToObject(obj, "key2", cJSON_CreateNumber(123));
    
    /* Test cJSON_DeleteItemFromObjectCaseSensitive */
    cJSON_DeleteItemFromObjectCaseSensitive(obj, key);
    
    /* Re-add items for next test */
    cJSON_AddItemToObject(obj, "key1", cJSON_CreateString("value1"));
    cJSON_AddItemToObject(obj, "key2", cJSON_CreateNumber(123));
    
    /* Test cJSON_DetachItemFromObjectCaseSensitive */
    detached = cJSON_DetachItemFromObjectCaseSensitive(obj, key);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    /* Re-add items for next test */
    cJSON_AddItemToObject(obj, "key1", cJSON_CreateString("value1"));
    cJSON_AddItemToObject(obj, "key2", cJSON_CreateNumber(123));
    
    /* Test cJSON_ReplaceItemInObject */
    cJSON *new_item = create_test_item(Data + key_len, Size - key_len);
    if (new_item) {
        cJSON_ReplaceItemInObject(obj, key, new_item);
    }
    
    /* Test cJSON_AddItemToObject */
    cJSON *add_item = create_test_item(Data, Size);
    if (add_item) {
        cJSON_AddItemToObject(obj, key, add_item);
    }
    
    /* Cleanup */
    cJSON_Delete(obj);
    free(key);
    
    return 0;
}