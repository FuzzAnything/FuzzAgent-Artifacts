// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    // Use first byte to decide type
    uint8_t type = data[0] % 5;
    
    switch (type) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            if (size > 1) {
                char* str = (char*)malloc(size);
                if (str) {
                    memcpy(str, data, size - 1);
                    str[size - 1] = '\0';
                    cJSON* item = cJSON_CreateString(str);
                    free(str);
                    return item;
                }
            }
            return cJSON_CreateString("test");
        case 2:
            return cJSON_CreateNumber((double)(data[0]));
        case 3:
            return cJSON_CreateBool(data[0] % 2);
        case 4:
            return cJSON_CreateArray();
        default:
            return cJSON_CreateObject();
    }
}

static char* extract_key(const uint8_t* data, size_t size, size_t* consumed) {
    if (size == 0) {
        *consumed = 0;
        return strdup("key");
    }
    
    // Find first null byte or use up to 64 chars
    size_t key_len = 0;
    while (key_len < size && key_len < 64 && data[key_len] != 0) {
        key_len++;
    }
    
    char* key = (char*)malloc(key_len + 1);
    if (!key) {
        *consumed = 0;
        return strdup("key");
    }
    
    memcpy(key, data, key_len);
    key[key_len] = '\0';
    
    *consumed = key_len + (key_len < size && data[key_len] == 0 ? 1 : 0);
    return key;
}

int LLVMFuzzerTestOneInput_38(const uint8_t* Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    // Create root object
    cJSON* root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }
    
    // Extract first key
    size_t consumed1 = 0;
    char* key1 = extract_key(Data, Size, &consumed1);
    
    // Extract second key from remaining data
    size_t consumed2 = 0;
    char* key2 = NULL;
    if (consumed1 < Size) {
        key2 = extract_key(Data + consumed1, Size - consumed1, &consumed2);
    } else {
        key2 = strdup("key2");
    }
    
    // Create test items
    cJSON* item1 = create_test_item(Data, Size);
    cJSON* item2 = create_test_item(Data + (consumed1 + consumed2) / 2, 
                                   Size - (consumed1 + consumed2) / 2);
    cJSON* item3 = create_test_item(Data + Size / 2, Size - Size / 2);
    
    // Test cJSON_AddItemToObject
    cJSON_AddItemToObject(root, key1, cJSON_Duplicate(item1, 1));
    
    // Test cJSON_AddItemToObjectCS with literal string
    cJSON_AddItemToObjectCS(root, "literal_key", cJSON_Duplicate(item2, 1));
    
    // Test cJSON_HasObjectItem
    cJSON_HasObjectItem(root, key1);
    cJSON_HasObjectItem(root, key2);
    cJSON_HasObjectItem(root, "literal_key");
    
    // Test cJSON_GetObjectItem
    cJSON* retrieved = cJSON_GetObjectItem(root, key1);
    cJSON_GetObjectItem(root, "literal_key");
    
    // Test cJSON_ReplaceItemInObject (case-insensitive)
    if (retrieved) {
        cJSON_ReplaceItemInObject(root, key1, cJSON_Duplicate(item3, 1));
    }
    
    // Test cJSON_ReplaceItemInObjectCaseSensitive
    cJSON_ReplaceItemInObjectCaseSensitive(root, "literal_key", 
                                          cJSON_Duplicate(item1, 1));
    
    // Try to replace non-existent item
    cJSON_ReplaceItemInObject(root, "nonexistent", cJSON_Duplicate(item2, 1));
    cJSON_ReplaceItemInObjectCaseSensitive(root, "nonexistent2", 
                                          cJSON_Duplicate(item3, 1));
    
    // Cleanup
    free(key1);
    free(key2);
    cJSON_Delete(item1);
    cJSON_Delete(item2);
    cJSON_Delete(item3);
    cJSON_Delete(root);
    
    return 0;
}