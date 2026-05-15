// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item(void) {
    // Create a simple test item with random type
    static int counter = 0;
    counter++;
    
    switch (counter % 4) {
        case 0:
            return cJSON_CreateString("test_value");
        case 1:
            return cJSON_CreateNumber(42.5);
        case 2:
            return cJSON_CreateBool(1);
        case 3:
            return cJSON_CreateNull();
        default:
            return cJSON_CreateString("default");
    }
}

static char* extract_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(2);
        if (str) {
            str[0] = 'a';
            str[1] = '\0';
        }
        return str;
    }
    
    size_t len = size > max_len ? max_len : size;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        // Ensure printable characters for keys
        str[i] = 32 + (data[i] % 95); // Printable ASCII range
    }
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Extract key from fuzzer input
    char *key1 = extract_string(Data, Size, 50);
    if (!key1) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Test 1: cJSON_AddItemToObject
    cJSON *item1 = create_test_item();
    if (item1) {
        cJSON_AddItemToObject(root, key1, item1);
    }
    
    // Test 2: cJSON_HasObjectItem
    cJSON_HasObjectItem(root, key1);
    
    // Test 3: cJSON_AddItemToObjectCS (with literal string)
    cJSON *item2 = create_test_item();
    if (item2) {
        cJSON_AddItemToObjectCS(root, "literal_key", item2);
    }
    
    // Test 4: cJSON_ReplaceItemInObject
    cJSON *new_item1 = create_test_item();
    if (new_item1) {
        cJSON_ReplaceItemInObject(root, key1, new_item1);
    }
    
    // Test 5: cJSON_ReplaceItemInObjectCaseSensitive
    char *key2 = extract_string(Data + (Size > 10 ? 10 : 0), 
                                Size > 10 ? Size - 10 : 0, 50);
    if (key2) {
        cJSON *item3 = create_test_item();
        if (item3) {
            cJSON_AddItemToObject(root, key2, item3);
        }
        
        cJSON *new_item2 = create_test_item();
        if (new_item2) {
            cJSON_ReplaceItemInObjectCaseSensitive(root, key2, new_item2);
        }
        
        free(key2);
    }
    
    // Test 6: cJSON_AddObjectToObject
    cJSON *nested_obj = cJSON_AddObjectToObject(root, "nested");
    if (nested_obj) {
        cJSON *nested_item = create_test_item();
        if (nested_item) {
            cJSON_AddItemToObject(nested_obj, "nested_key", nested_item);
        }
    }
    
    // Additional test: Try to replace non-existent key
    cJSON *dummy_item = create_test_item();
    if (dummy_item) {
        cJSON_ReplaceItemInObject(root, "non_existent_key", dummy_item);
    }
    
    // Cleanup
    free(key1);
    cJSON_Delete(root);
    
    return 0;
}