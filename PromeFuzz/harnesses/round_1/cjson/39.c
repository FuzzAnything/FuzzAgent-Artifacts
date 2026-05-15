// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char* create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static cJSON* create_test_item(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    switch (data[0] % 6) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateFalse();
        case 3: {
            char* str = create_null_terminated_string(data, size, 50);
            cJSON* item = cJSON_CreateString(str);
            free(str);
            return item;
        }
        case 4: {
            double num = (double)(*(int32_t*)data);
            return cJSON_CreateNumber(num);
        }
        case 5: {
            cJSON* obj = cJSON_CreateObject();
            if (size > 1) {
                char* key = create_null_terminated_string(data + 1, size - 1, 20);
                cJSON* child = cJSON_CreateNumber(data[0]);
                cJSON_AddItemToObject(obj, key, child);
                free(key);
            }
            return obj;
        }
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create test object
    cJSON* root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Create keys from input data
    size_t key1_len = Size / 3;
    size_t key2_len = Size / 3;
    size_t key3_len = Size - 2 * (Size / 3);
    
    char* key1 = create_null_terminated_string(Data, key1_len, 100);
    char* key2 = create_null_terminated_string(Data + key1_len, key2_len, 100);
    char* key3 = create_null_terminated_string(Data + key1_len + key2_len, key3_len, 100);
    
    if (!key1 || !key2 || !key3) {
        free(key1);
        free(key2);
        free(key3);
        cJSON_Delete(root);
        return 0;
    }
    
    // Create test items
    cJSON* item1 = create_test_item(Data, Size);
    cJSON* item2 = create_test_item(Data + 1, Size - 1);
    cJSON* item3 = create_test_item(Data + 2, Size - 2);
    
    // Test cJSON_AddItemToObject
    if (item1) {
        cJSON_AddItemToObject(root, key1, item1);
    }
    
    // Test cJSON_AddItemToObjectCS with literal string
    if (item2) {
        cJSON_AddItemToObjectCS(root, "literal_key", item2);
    }
    
    // Test cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(root, key1);
    
    // Test cJSON_GetObjectItem
    cJSON* retrieved_item = cJSON_GetObjectItem(root, key1);
    
    // Test cJSON_ReplaceItemInObject
    if (item3 && retrieved_item) {
        cJSON_ReplaceItemInObject(root, key1, item3);
    }
    
    // Test cJSON_ReplaceItemInObjectCaseSensitive with new item
    cJSON* item4 = create_test_item(Data + 3, Size - 3);
    if (item4) {
        cJSON_ReplaceItemInObjectCaseSensitive(root, "literal_key", item4);
    }
    
    // Test with non-existent keys
    cJSON_HasObjectItem(root, "non_existent_key");
    cJSON_GetObjectItem(root, "non_existent_key");
    
    // Test replacement with non-existent key (should fail)
    cJSON* item5 = create_test_item(Data + 4, Size - 4);
    if (item5) {
        cJSON_ReplaceItemInObject(root, "another_non_existent_key", item5);
        cJSON_ReplaceItemInObjectCaseSensitive(root, "another_non_existent_key", item5);
        cJSON_Delete(item5);
    }
    
    // Cleanup
    free(key1);
    free(key2);
    free(key3);
    cJSON_Delete(root);
    
    return 0;
}