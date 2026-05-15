// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
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
    if (size == 0) return cJSON_CreateNull();
    
    // Use first byte to determine item type
    uint8_t type = data[0];
    switch (type % 4) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateBool(size % 2);
        case 2:
            return cJSON_CreateNumber((double)size);
        case 3: {
            // Create string from data (ensure null termination)
            char* str = (char*)malloc(size + 1);
            if (!str) return cJSON_CreateNull();
            memcpy(str, data, size);
            str[size] = '\0';
            cJSON* item = cJSON_CreateString(str);
            free(str);
            return item;
        }
        default:
            return cJSON_CreateNull();
    }
}

static char* extract_key(const uint8_t* data, size_t size, size_t* consumed) {
    if (size == 0) {
        *consumed = 0;
        char* key = (char*)malloc(2);
        if (key) {
            key[0] = 'k';
            key[1] = '\0';
        }
        return key;
    }
    
    // Find null terminator or use up to 64 bytes
    size_t key_len = 0;
    while (key_len < size && key_len < 64 && data[key_len] != 0) {
        key_len++;
    }
    
    char* key = (char*)malloc(key_len + 1);
    if (!key) {
        *consumed = 0;
        return NULL;
    }
    
    memcpy(key, data, key_len);
    key[key_len] = '\0';
    
    *consumed = key_len;
    if (key_len < size && data[key_len] == 0) {
        (*consumed)++; // Skip null terminator
    }
    
    return key;
}

int LLVMFuzzerTestOneInput_41(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Extract key from beginning of data
    size_t key_consumed = 0;
    char* key = extract_key(Data, Size, &key_consumed);
    if (!key) return 0;
    
    // Create test objects
    cJSON* root_object = cJSON_CreateObject();
    cJSON* test_object = cJSON_CreateObject();
    
    if (!root_object || !test_object) {
        free(key);
        if (root_object) cJSON_Delete(root_object);
        if (test_object) cJSON_Delete(test_object);
        return 0;
    }
    
    // Use remaining data for test items
    const uint8_t* remaining_data = Data + key_consumed;
    size_t remaining_size = Size - key_consumed;
    
    // Test cJSON_AddNullToObject
    cJSON* null_item = cJSON_AddNullToObject(root_object, key);
    (void)null_item; // Suppress unused warning
    
    // Test cJSON_AddItemToObjectCS with literal string
    cJSON* item1 = create_test_item(remaining_data, remaining_size % 32);
    if (item1) {
        cJSON_AddItemToObjectCS(root_object, "literal_key", item1);
    }
    
    // Test cJSON_AddItemToObject
    cJSON* item2 = create_test_item(remaining_data + 1, (remaining_size - 1) % 32);
    if (item2) {
        cJSON_AddItemToObject(root_object, key, item2);
    }
    
    // Test cJSON_ReplaceItemInObject (replace if exists)
    cJSON* item3 = create_test_item(remaining_data + 2, (remaining_size - 2) % 32);
    if (item3) {
        cJSON_ReplaceItemInObject(root_object, key, item3);
    }
    
    // Test cJSON_IsNull on various items
    cJSON_IsNull(root_object);
    cJSON_IsNull(item1);
    cJSON_IsNull(item2);
    cJSON_IsNull(item3);
    cJSON_IsNull(NULL);
    
    // Test cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root_object, key);
    cJSON_DeleteItemFromObject(root_object, "literal_key");
    cJSON_DeleteItemFromObject(root_object, "nonexistent_key");
    
    // Test with second object using different key
    char* key2 = extract_key(remaining_data, remaining_size % 16, &key_consumed);
    if (key2) {
        cJSON* item4 = create_test_item(remaining_data + key_consumed, 
                                       (remaining_size - key_consumed) % 16);
        if (item4) {
            cJSON_AddItemToObject(test_object, key2, item4);
            cJSON_ReplaceItemInObject(test_object, key2, 
                                     create_test_item(remaining_data, 1));
            cJSON_DeleteItemFromObject(test_object, key2);
        }
        free(key2);
    }
    
    // Cleanup
    free(key);
    cJSON_Delete(root_object);
    cJSON_Delete(test_object);
    
    return 0;
}