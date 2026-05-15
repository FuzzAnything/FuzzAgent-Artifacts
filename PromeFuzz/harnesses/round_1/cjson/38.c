// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void fuzz_cjson_apis(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    /* Create a root object */
    cJSON *root = cJSON_CreateObject();
    if (!root) return;
    
    /* Create some child items */
    cJSON *item1 = cJSON_CreateString("value1");
    cJSON *item2 = cJSON_CreateNumber(42);
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateString("value4");
    cJSON *item5 = cJSON_CreateNumber(3.14);
    
    if (!item1 || !item2 || !item3 || !item4 || !item5) {
        goto cleanup_items;
    }
    
    /* Generate keys from fuzzer data */
    size_t key1_len = size / 5;
    size_t key2_len = size / 5;
    size_t key3_len = size / 5;
    
    if (key1_len == 0 || key2_len == 0 || key3_len == 0) {
        goto cleanup_items;
    }
    
    char *key1 = malloc(key1_len + 1);
    char *key2 = malloc(key2_len + 1);
    char *key3 = malloc(key3_len + 1);
    
    if (!key1 || !key2 || !key3) {
        goto cleanup_keys;
    }
    
    memcpy(key1, data, key1_len);
    key1[key1_len] = '\0';
    
    memcpy(key2, data + key1_len, key2_len);
    key2[key2_len] = '\0';
    
    memcpy(key3, data + key1_len + key2_len, key3_len);
    key3[key3_len] = '\0';
    
    /* Test cJSON_AddItemToObject */
    cJSON_AddItemToObject(root, key1, item1);
    item1 = NULL; /* item1 is now owned by root */
    
    /* Test cJSON_HasObjectItem */
    cJSON_bool has_item = cJSON_HasObjectItem(root, key1);
    (void)has_item;
    
    /* Test cJSON_AddItemToObjectCS with literal string */
    cJSON_AddItemToObjectCS(root, "literal_key", item2);
    item2 = NULL; /* item2 is now owned by root */
    
    /* Test cJSON_ReplaceItemInObject */
    cJSON_AddItemToObject(root, key2, item3);
    item3 = NULL; /* item3 is now owned by root */
    cJSON_ReplaceItemInObject(root, key2, item4);
    item4 = NULL; /* item4 is now owned by root */
    
    /* Test cJSON_ReplaceItemInObjectCaseSensitive */
    cJSON_AddItemToObject(root, key3, item5);
    item5 = NULL; /* item5 is now owned by root */
    cJSON *new_item = cJSON_CreateString("new_value");
    if (new_item) {
        cJSON_ReplaceItemInObjectCaseSensitive(root, key3, new_item);
        /* new_item is now owned by root, don't delete it separately */
    }
    
    /* Test cJSON_AddObjectToObject */
    cJSON *new_obj = cJSON_AddObjectToObject(root, "new_object");
    if (new_obj) {
        cJSON_AddItemToObject(new_obj, "nested_key", cJSON_CreateString("nested_value"));
    }
    
    /* Clean up dynamically allocated keys */
cleanup_keys:
    free(key1);
    free(key2);
    free(key3);
    
cleanup_items:
    /* Clean up any items that weren't added to the tree */
    cJSON_Delete(item1);
    cJSON_Delete(item2);
    cJSON_Delete(item3);
    cJSON_Delete(item4);
    cJSON_Delete(item5);
    /* Don't delete new_item here - it's either owned by root or NULL */
    
    /* Clean up the root object (this will delete all child items) */
    cJSON_Delete(root);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *data, size_t size) {
    fuzz_cjson_apis(data, size);
    return 0;
}