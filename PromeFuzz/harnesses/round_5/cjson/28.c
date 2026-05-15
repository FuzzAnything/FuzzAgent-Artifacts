// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
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

static void fuzz_cjson_apis(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return;
    
    // Use first byte to determine which APIs to call
    uint8_t selector = data[0];
    
    // Create key from input data (ensure null termination)
    size_t key_len = (size - 1) < 256 ? (size - 1) : 256;
    char *key = (char *)malloc(key_len + 1);
    if (!key) {
        cJSON_Delete(root);
        return;
    }
    memcpy(key, data + 1, key_len);
    key[key_len] = '\0';
    
    // Test various API functions based on selector
    if ((selector & 0x01) && key_len > 0) {
        // Create string value from remaining data
        size_t str_len = key_len < 128 ? key_len : 128;
        char *str_val = (char *)malloc(str_len + 1);
        if (str_val) {
            memcpy(str_val, key, str_len);
            str_val[str_len] = '\0';
            cJSON_AddStringToObject(root, key, str_val);
            free(str_val);
        }
    }
    
    if ((selector & 0x02) && key_len > 0) {
        cJSON_bool bool_val = (selector & 0x04) ? 1 : 0;
        cJSON_AddBoolToObject(root, key, bool_val);
    }
    
    if ((selector & 0x08) && key_len > 0) {
        cJSON_AddNullToObject(root, key);
    }
    
    if ((selector & 0x10) && key_len > 0) {
        // First create an item, then try to replace it
        cJSON *old_item = cJSON_AddStringToObject(root, key, "old_value");
        if (old_item) {
            cJSON *new_item = cJSON_CreateNumber(42.5);
            if (new_item) {
                cJSON_ReplaceItemInObject(root, key, new_item);
            }
        }
    }
    
    if ((selector & 0x20) && key_len > 0) {
        cJSON_AddArrayToObject(root, key);
    }
    
    if ((selector & 0x40) && key_len > 0) {
        cJSON_AddFalseToObject(root, key);
    }
    
    // Create another object for additional testing
    cJSON *second_obj = cJSON_CreateObject();
    if (second_obj) {
        if (key_len > 0) {
            cJSON_AddStringToObject(second_obj, "test_key", key);
        }
        cJSON_Delete(second_obj);
    }
    
    // Clean up
    free(key);
    cJSON_Delete(root);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    fuzz_cjson_apis(data, size);
    return 0;
}