// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char* create_string_from_data(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        str[i] = (char)(data[i % size] % 128);
        if (str[i] == '\0') str[i] = 'a';
    }
    str[len] = '\0';
    return str;
}

static cJSON* create_test_json(const uint8_t *data, size_t size) {
    cJSON* root = cJSON_CreateObject();
    if (!root) return NULL;
    
    size_t offset = 0;
    
    for (int i = 0; i < 10 && offset < size; i++) {
        size_t key_len = ((data[offset % size] % 16) + 1);
        offset = (offset + 1) % (size > 0 ? size : 1);
        
        char* key = create_string_from_data(data + (offset % size), 
                                           size - (offset % size), key_len);
        if (!key) continue;
        
        offset = (offset + key_len) % (size > 0 ? size : 1);
        
        uint8_t choice = data[offset % size] % 6;
        switch (choice) {
            case 0:
                cJSON_AddStringToObject(root, key, "test_string");
                break;
            case 1:
                cJSON_AddBoolToObject(root, key, 1);
                break;
            case 2:
                cJSON_AddNumberToObject(root, key, 42.5);
                break;
            case 3:
                cJSON_AddTrueToObject(root, key);
                break;
            case 4:
                cJSON_AddFalseToObject(root, key);
                break;
            case 5:
                cJSON_AddNullToObject(root, key);
                break;
        }
        
        free(key);
        offset = (offset + 1) % (size > 0 ? size : 1);
    }
    
    return root;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cJSON* json = create_test_json(Data, Size);
    if (!json) return 0;
    
    const char* keys[] = {"key1", "key2", "key3", "key4", "key5", "key6"};
    size_t num_keys = sizeof(keys) / sizeof(keys[0]);
    
    uint8_t idx0 = Data[0 % Size];
    uint8_t idx1 = Data[1 % Size];
    uint8_t idx2 = Data[2 % Size];
    
    cJSON* item1 = cJSON_GetObjectItemCaseSensitive(json, keys[idx0 % num_keys]);
    cJSON_bool is_str = cJSON_IsString(item1);
    
    cJSON* item2 = cJSON_GetObjectItemCaseSensitive(json, keys[(idx0 + 1) % num_keys]);
    cJSON_bool is_true = cJSON_IsTrue(item2);
    
    cJSON* item3 = cJSON_GetObjectItemCaseSensitive(json, keys[(idx0 + 2) % num_keys]);
    cJSON* item4 = cJSON_GetObjectItemCaseSensitive(json, keys[(idx0 + 3) % num_keys]);
    
    cJSON* duplicate = cJSON_Duplicate(item4, idx1 % 2);
    
    cJSON* item5 = cJSON_GetObjectItemCaseSensitive(json, keys[(idx0 + 4) % num_keys]);
    cJSON* item6 = cJSON_GetObjectItemCaseSensitive(json, keys[(idx0 + 5) % num_keys]);
    
    cJSON_bool compare_result = cJSON_Compare(item5, item6, idx2 % 2);
    
    cJSON_Delete(json);
    if (duplicate) {
        cJSON_Delete(duplicate);
    }
    
    return 0;
}