// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_random_json(const uint8_t* data, size_t size) {
    if (size == 0) return NULL;
    
    cJSON* item = NULL;
    switch (data[0] % 6) {
        case 0:
            item = cJSON_CreateNull();
            break;
        case 1:
            item = cJSON_CreateTrue();
            break;
        case 2:
            item = cJSON_CreateFalse();
            break;
        case 3:
            if (size > 1) {
                char* str = (char*)malloc(size);
                if (str) {
                    memcpy(str, data, size - 1);
                    str[size - 1] = '\0';
                    item = cJSON_CreateString(str);
                    free(str);
                }
            }
            break;
        case 4:
            if (size >= sizeof(double)) {
                double num;
                memcpy(&num, data, sizeof(double));
                item = cJSON_CreateNumber(num);
            }
            break;
        case 5:
            item = cJSON_CreateObject();
            break;
    }
    return item;
}

static char* extract_key(const uint8_t* data, size_t size) {
    if (size == 0) return NULL;
    
    size_t key_len = size > 100 ? 100 : size;
    char* key = (char*)malloc(key_len + 1);
    if (!key) return NULL;
    
    memcpy(key, data, key_len);
    key[key_len] = '\0';
    
    for (size_t i = 0; i < key_len; i++) {
        if (key[i] == '\0') {
            key[i] = 'a';
        }
    }
    
    return key;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    cJSON* root = cJSON_CreateObject();
    if (!root) return 0;
    
    size_t key_offset = size / 3;
    if (key_offset == 0) key_offset = 1;
    
    char* key1 = extract_key(data, key_offset);
    char* key2 = extract_key(data + key_offset, key_offset);
    
    if (!key1 || !key2) {
        free(key1);
        free(key2);
        cJSON_Delete(root);
        return 0;
    }
    
    size_t item_offset = key_offset * 2;
    if (item_offset >= size) item_offset = size - 1;
    
    cJSON* item1 = create_random_json(data + item_offset, size - item_offset);
    cJSON* item2 = create_random_json(data, size);
    cJSON* item3 = create_random_json(data + (size/2), size/2);
    
    if (item1) {
        cJSON_AddItemToObject(root, key1, item1);
        
        cJSON_bool has_item = cJSON_HasObjectItem(root, key1);
        (void)has_item;
        
        cJSON* retrieved = cJSON_GetObjectItem(root, key1);
        (void)retrieved;
    }
    
    if (item2) {
        cJSON_ReplaceItemInObjectCaseSensitive(root, key1, item2);
        
        if (item3) {
            cJSON_ReplaceItemInObject(root, key1, item3);
        } else {
            cJSON* null_item = cJSON_CreateNull();
            if (null_item) {
                cJSON_ReplaceItemInObject(root, key1, null_item);
            }
        }
        
        cJSON_AddItemToObjectCS(root, key2, cJSON_CreateString("const_string"));
    }
    
    free(key1);
    free(key2);
    cJSON_Delete(root);
    
    return 0;
}