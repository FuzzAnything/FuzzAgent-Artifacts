// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item_from_data(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    // Try to parse as JSON first
    cJSON* parsed = cJSON_ParseWithLength((const char*)data, size);
    if (parsed != NULL) {
        return parsed;
    }
    
    // If parsing fails, create a string from the data
    char* str = (char*)malloc(size + 1);
    if (str == NULL) {
        return cJSON_CreateNull();
    }
    memcpy(str, data, size);
    str[size] = '\0';
    cJSON* item = cJSON_CreateString(str);
    free(str);
    return item;
}

static char* extract_key_from_data(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return strdup("default_key");
    }
    
    size_t key_len = size - offset;
    if (key_len > 100) {
        key_len = 100;
    }
    
    char* key = (char*)malloc(key_len + 1);
    if (key == NULL) {
        return strdup("default_key");
    }
    
    memcpy(key, data + offset, key_len);
    key[key_len] = '\0';
    
    // Ensure the key is not empty
    if (key[0] == '\0') {
        free(key);
        return strdup("default_key");
    }
    
    return key;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    // Create a root JSON object
    cJSON* root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }
    
    // Extract keys from the input data
    size_t key1_offset = Size / 3;
    size_t key2_offset = (2 * Size) / 3;
    
    char* key1 = extract_key_from_data(Data, Size, 0);
    char* key2 = extract_key_from_data(Data, Size, key1_offset);
    char* key3 = extract_key_from_data(Data, Size, key2_offset);
    
    // Create test items from different parts of the input
    size_t item1_size = Size / 4;
    size_t item2_size = Size / 3;
    
    cJSON* item1 = create_test_item_from_data(Data, item1_size);
    cJSON* item2 = create_test_item_from_data(Data + item1_size, 
                                             (Size - item1_size > item2_size) ? item2_size : Size - item1_size);
    cJSON* item3 = create_test_item_from_data(Data, Size);
    
    // Test cJSON_AddItemToObject
    if (item1 != NULL) {
        cJSON_AddItemToObject(root, key1, item1);
    }
    
    // Test cJSON_AddItemToObjectCS with literal string
    if (item2 != NULL) {
        cJSON_AddItemToObjectCS(root, "literal_key", item2);
    }
    
    // Test cJSON_HasObjectItem
    cJSON_HasObjectItem(root, key1);
    cJSON_HasObjectItem(root, "literal_key");
    cJSON_HasObjectItem(root, key2);
    
    // Test cJSON_GetObjectItem
    cJSON* retrieved1 = cJSON_GetObjectItem(root, key1);
    cJSON* retrieved2 = cJSON_GetObjectItem(root, "literal_key");
    cJSON* retrieved3 = cJSON_GetObjectItem(root, key2);
    
    // Test cJSON_ReplaceItemInObject
    if (retrieved1 != NULL && item3 != NULL) {
        cJSON_ReplaceItemInObject(root, key1, item3);
    }
    
    // Create another item for replacement testing
    size_t remaining = (Size > item1_size + item2_size) ? Size - item1_size - item2_size : Size;
    cJSON* item4 = create_test_item_from_data(Data + item1_size + item2_size, remaining);
    
    // Test cJSON_ReplaceItemInObjectCaseSensitive
    if (retrieved2 != NULL && item4 != NULL) {
        cJSON_ReplaceItemInObjectCaseSensitive(root, "literal_key", item4);
    }
    
    // Try to replace with a non-existent key
    cJSON* item5 = cJSON_CreateNumber(42.0);
    if (item5 != NULL) {
        cJSON_ReplaceItemInObject(root, key3, item5);
        cJSON_ReplaceItemInObjectCaseSensitive(root, key3, item5);
    }
    
    // Add more items to test edge cases
    if (Size > 10) {
        cJSON* item6 = cJSON_CreateBool(cJSON_False);
        cJSON* item7 = cJSON_CreateBool(cJSON_True);
        
        cJSON_AddItemToObject(root, "bool_false", item6);
        cJSON_AddItemToObjectCS(root, "bool_true", item7);
        
        cJSON_HasObjectItem(root, "bool_false");
        cJSON_HasObjectItem(root, "bool_true");
        
        cJSON* retrieved4 = cJSON_GetObjectItem(root, "bool_false");
        cJSON* retrieved5 = cJSON_GetObjectItem(root, "bool_true");
        
        if (retrieved4 != NULL) {
            cJSON* item8 = cJSON_CreateNumber(100);
            cJSON_ReplaceItemInObject(root, "bool_false", item8);
        }
        
        if (retrieved5 != NULL) {
            cJSON* item9 = cJSON_CreateString("replaced");
            cJSON_ReplaceItemInObjectCaseSensitive(root, "bool_true", item9);
        }
    }
    
    // Cleanup
    free(key1);
    free(key2);
    free(key3);
    cJSON_Delete(root);
    
    return 0;
}