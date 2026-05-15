// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Create some test items
    cJSON *null_item = cJSON_CreateNull();
    cJSON *string_item = cJSON_CreateString("test");
    cJSON *number_item = cJSON_CreateNumber(42.0);
    cJSON *bool_item = cJSON_CreateBool(1);
    
    // Generate key from input data
    size_t key_len = Size > 64 ? 64 : Size;
    char *key = (char *)malloc(key_len + 1);
    if (!key) {
        cJSON_Delete(root);
        cJSON_Delete(null_item);
        cJSON_Delete(string_item);
        cJSON_Delete(number_item);
        cJSON_Delete(bool_item);
        return 0;
    }
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Test cJSON_AddItemToObject
    cJSON_AddItemToObject(root, key, cJSON_Duplicate(string_item, 1));
    
    // Test cJSON_HasObjectItem
    cJSON_HasObjectItem(root, key);
    
    // Test cJSON_AddNullToObject
    char null_key[32];
    snprintf(null_key, sizeof(null_key), "null_%s", key);
    cJSON_AddNullToObject(root, null_key);
    
    // Test cJSON_AddItemToObjectCS with const strings
    static const char *const_key1 = "const_key1";
    static const char *const_key2 = "const_key2";
    cJSON_AddItemToObjectCS(root, const_key1, cJSON_Duplicate(number_item, 1));
    cJSON_AddItemToObjectCS(root, const_key2, cJSON_Duplicate(bool_item, 1));
    
    // Test cJSON_ReplaceItemInObject
    cJSON_ReplaceItemInObject(root, key, cJSON_Duplicate(null_item, 1));
    
    // Test cJSON_ReplaceItemInObjectCaseSensitive
    char replace_key[32];
    snprintf(replace_key, sizeof(replace_key), "replace_%s", key);
    cJSON_AddItemToObject(root, replace_key, cJSON_Duplicate(string_item, 1));
    cJSON_ReplaceItemInObjectCaseSensitive(root, replace_key, cJSON_Duplicate(number_item, 1));
    
    // Cleanup
    free(key);
    cJSON_Delete(root);
    cJSON_Delete(null_item);
    cJSON_Delete(string_item);
    cJSON_Delete(number_item);
    cJSON_Delete(bool_item);
    
    return 0;
}