// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create parent object
    cJSON *object = cJSON_CreateObject();
    if (!object) return 0;

    // Create some child items
    cJSON *item1 = cJSON_CreateNumber(42.0);
    cJSON *item2 = cJSON_CreateString("test");
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateNull();
    cJSON *item5 = cJSON_CreateArray();
    cJSON *item6 = cJSON_CreateObject();

    if (!item1 || !item2 || !item3 || !item4 || !item5 || !item6) {
        goto cleanup;
    }

    // Generate keys from fuzzer data
    // Use first half for key1, second half for key2
    size_t key1_len = Size / 2;
    size_t key2_len = Size - key1_len;
    
    char *key1 = malloc(key1_len + 1);
    char *key2 = malloc(key2_len + 1);
    
    if (!key1 || !key2) {
        goto cleanup;
    }
    
    memcpy(key1, Data, key1_len);
    memcpy(key2, Data + key1_len, key2_len);
    key1[key1_len] = '\0';
    key2[key2_len] = '\0';

    // Test cJSON_AddItemToObject
    cJSON_AddItemToObject(object, key1, item1);
    
    // Test cJSON_AddItemToObjectCS - use literal for safety
    cJSON_AddItemToObjectCS(object, "literal_key", item2);
    
    // Add another item for replacement testing
    cJSON_AddItemToObject(object, key2, item3);
    
    // Test cJSON_ReplaceItemInObject
    cJSON_ReplaceItemInObject(object, key1, item4);
    
    // Test cJSON_ReplaceItemInObjectCaseSensitive
    cJSON_ReplaceItemInObjectCaseSensitive(object, key2, item5);
    
    // Test cJSON_GetObjectItem
    cJSON *retrieved = cJSON_GetObjectItem(object, key1);
    (void)retrieved; // Use to avoid unused variable warning
    
    // Test cJSON_HasObjectItem
    int has_item = cJSON_HasObjectItem(object, key2);
    (void)has_item; // Use to avoid unused variable warning
    
    // Try to replace non-existent key
    cJSON_ReplaceItemInObject(object, "nonexistent", item6);
    
    // Clean up
cleanup:
    if (key1) free(key1);
    if (key2) free(key2);
    
    // Note: items are owned by object now, don't free them separately
    if (object) cJSON_Delete(object);
    
    return 0;
}