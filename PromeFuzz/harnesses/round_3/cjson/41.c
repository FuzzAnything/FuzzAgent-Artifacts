// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static cJSON *create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) return cJSON_CreateNull();
    
    switch (data[0] % 5) {
        case 0:
            return cJSON_CreateString((const char*)data);
        case 1:
            return cJSON_CreateNumber((double)size);
        case 2:
            return cJSON_CreateBool(size % 2);
        case 3: {
            cJSON *array = cJSON_CreateArray();
            if (array && size > 1) {
                cJSON_AddItemToArray(array, cJSON_CreateNumber(data[1]));
            }
            return array;
        }
        case 4: {
            cJSON *obj = cJSON_CreateObject();
            if (obj && size > 2) {
                cJSON_AddItemToObject(obj, "test", cJSON_CreateNumber(data[2]));
            }
            return obj;
        }
        default:
            return cJSON_CreateNull();
    }
}

static char *extract_string(const uint8_t *data, size_t size) {
    if (size == 0) return strdup("");
    
    char *str = (char*)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    for (size_t i = 0; i < size; i++) {
        if (str[i] == '\0') {
            str[i] = 'a';
        }
    }
    
    return str;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    size_t key1_len = Size / 3;
    size_t key2_len = Size / 3;
    size_t key3_len = Size - key1_len - key2_len;
    
    if (key1_len == 0) key1_len = 1;
    if (key2_len == 0) key2_len = 1;
    if (key3_len == 0) key3_len = 1;
    
    char *key1 = extract_string(Data, key1_len);
    char *key2 = extract_string(Data + key1_len, key2_len);
    char *key3 = extract_string(Data + key1_len + key2_len, key3_len);
    
    if (!key1 || !key2 || !key3) {
        free(key1);
        free(key2);
        free(key3);
        cJSON_Delete(root);
        return 0;
    }
    
    cJSON *item1 = create_test_item(Data, Size);
    cJSON *item2 = create_test_item(Data + 1, Size - 1);
    cJSON *item3 = create_test_item(Data + 2, Size - 2);
    
    if (!item1 || !item2 || !item3) {
        free(key1);
        free(key2);
        free(key3);
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cJSON_Delete(item3);
        cJSON_Delete(root);
        return 0;
    }
    
    int op = Data[0] % 6;
    
    switch (op) {
        case 0:
            cJSON_AddItemToObjectCS(root, key1, item1);
            break;
        case 1:
            cJSON_AddItemToObject(root, key1, item1);
            cJSON_ReplaceItemInObject(root, key1, item2);
            break;
        case 2:
            cJSON_AddItemToObject(root, key1, item1);
            cJSON_ReplaceItemInObjectCaseSensitive(root, key1, item2);
            break;
        case 3:
            cJSON_AddItemToObject(root, key1, item1);
            cJSON_AddItemToObject(root, key2, item2);
            cJSON_AddItemToObject(root, key3, item3);
            break;
        case 4:
            cJSON_AddItemToObject(root, key1, item1);
            cJSON_GetObjectItem(root, key1);
            cJSON_GetObjectItem(root, key2);
            break;
        case 5:
            cJSON_AddItemToObject(root, key1, item1);
            cJSON_HasObjectItem(root, key1);
            cJSON_HasObjectItem(root, key2);
            break;
    }
    
    free(key1);
    free(key2);
    free(key3);
    
    if (op != 0 && op != 3 && op != 4 && op != 5) {
        cJSON_Delete(item1);
    }
    if (op == 0 || op == 3 || op == 4 || op == 5) {
        cJSON_Delete(item2);
        cJSON_Delete(item3);
    }
    
    cJSON_Delete(root);
    return 0;
}