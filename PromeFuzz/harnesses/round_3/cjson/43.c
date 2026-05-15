// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void dummy_cleanup(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Extract key and value from fuzzer input
    size_t key_len = Size / 2;
    size_t val_len = Size - key_len;
    
    char *key = (char*)malloc(key_len + 1);
    char *value = (char*)malloc(val_len + 1);
    
    if (!key || !value) {
        free(key);
        free(value);
        dummy_cleanup(root);
        return 0;
    }
    
    memcpy(key, Data, key_len);
    key[key_len] = '\0';
    memcpy(value, Data + key_len, val_len);
    value[val_len] = '\0';

    // Create initial items
    cJSON *item1 = cJSON_CreateString(value);
    cJSON *item2 = cJSON_CreateNumber((double)Size);
    
    if (!item1 || !item2) {
        free(key);
        free(value);
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        dummy_cleanup(root);
        return 0;
    }

    // Test cJSON_AddItemToObject
    cJSON_AddItemToObject(root, key, item1);
    
    // Test cJSON_AddItemToObjectCS with literal string
    cJSON_AddItemToObjectCS(root, "literalKey", item2);
    
    // Test cJSON_GetObjectItem
    cJSON *retrieved = cJSON_GetObjectItem(root, key);
    
    // Test cJSON_HasObjectItem
    int has_item = cJSON_HasObjectItem(root, key);
    
    // Create a new item for replacement to avoid use-after-free
    cJSON *item3 = cJSON_CreateString("replacement");
    if (item3) {
        // Test cJSON_ReplaceItemInObject (may fail if key doesn't exist)
        cJSON_ReplaceItemInObject(root, key, item3);
    }
    
    // Create another new item for the second replacement
    cJSON *item4 = cJSON_CreateNumber(42.0);
    if (item4) {
        // Test cJSON_ReplaceItemInObjectCaseSensitive
        cJSON_ReplaceItemInObjectCaseSensitive(root, "literalKey", item4);
    }
    
    // Create another object for more complex testing
    cJSON *nested = cJSON_CreateObject();
    if (nested) {
        cJSON *nested_item1 = cJSON_CreateString(key);
        if (nested_item1) {
            cJSON_AddItemToObject(nested, "nestedKey", nested_item1);
        }
        
        cJSON_AddItemToObject(root, "nestedObj", nested);
        
        // Test retrieval from nested object
        cJSON *nested_retrieved = cJSON_GetObjectItem(nested, "nestedKey");
        int has_nested = cJSON_HasObjectItem(nested, "nestedKey");
        
        // Test replace in nested object with a new item
        cJSON *nested_replacement = cJSON_CreateNumber(99.0);
        if (nested_replacement) {
            cJSON_ReplaceItemInObject(nested, "nestedKey", nested_replacement);
        }
    }

    // Cleanup
    free(key);
    free(value);
    dummy_cleanup(root);
    
    return 0;
}