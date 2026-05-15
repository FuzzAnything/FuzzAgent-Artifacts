// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_random_item(const uint8_t *data, size_t size) {
    if (size == 0) return cJSON_CreateNull();
    
    switch (data[0] % 5) {
        case 0: return cJSON_CreateNull();
        case 1: return cJSON_CreateNumber((double)(data[0] % 100));
        case 2: {
            char str[32];
            size_t len = size < 31 ? size : 31;
            memcpy(str, data, len);
            str[len] = '\0';
            return cJSON_CreateString(str);
        }
        case 3: return cJSON_CreateBool(data[0] % 2);
        case 4: return cJSON_CreateArray();
        default: return cJSON_CreateNull();
    }
}

static char* create_random_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < 100 ? size : 100;
    char *str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Create a key from the first part of data
    size_t key_len = Size / 4;
    if (key_len > 50) key_len = 50;
    char *key = create_random_string(Data, key_len);
    if (!key) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Use different parts of the input for different operations
    size_t offset = key_len;
    if (offset >= Size) offset = 0;
    
    // Test cJSON_AddNullToObject
    cJSON *null_item = cJSON_AddNullToObject(root, key);
    if (null_item) {
        // Test cJSON_IsNull on the added item
        cJSON_bool is_null = cJSON_IsNull(null_item);
        (void)is_null; // Use result to avoid unused variable warning
    }
    
    // Test cJSON_AddItemToObject with a new item
    if (offset < Size) {
        cJSON *item1 = create_random_item(Data + offset, Size - offset);
        if (item1) {
            cJSON_AddItemToObject(root, "item1", item1);
        }
    }
    
    // Test cJSON_AddItemToObjectCS with a literal string
    cJSON *item2 = cJSON_CreateNumber(42.0);
    if (item2) {
        cJSON_AddItemToObjectCS(root, "literal_key", item2);
    }
    
    // Test cJSON_ReplaceItemInObject if we have a key
    if (offset < Size) {
        cJSON *new_item = create_random_item(Data + offset, Size - offset);
        if (new_item) {
            cJSON_ReplaceItemInObject(root, key, new_item);
        }
    }
    
    // Test cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, key);
    
    // Create another object to test more operations
    cJSON *obj2 = cJSON_CreateObject();
    if (obj2) {
        // Add some items to the second object
        cJSON_AddNullToObject(obj2, "test_null");
        
        cJSON *item3 = cJSON_CreateString("test");
        if (item3) {
            cJSON_AddItemToObject(obj2, "test_string", item3);
        }
        
        // Add the second object to the root
        cJSON_AddItemToObject(root, "nested", obj2);
    }
    
    // Test cJSON_IsNull on various items
    cJSON_IsNull(root);
    cJSON_IsNull(NULL);
    
    // Create an array and test IsNull on it
    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_IsNull(array);
        cJSON_Delete(array);
    }
    
    // Cleanup
    free(key);
    cJSON_Delete(root);
    
    return 0;
}