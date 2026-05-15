// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
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

static void fuzz_cjson_apis(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return;
    
    // Use first byte to determine boolean value
    cJSON_bool bool_val = (data[0] & 1) ? 1 : 0;
    
    // Create name from data (ensure null-terminated)
    char *name = malloc(size + 1);
    if (!name) {
        cJSON_Delete(root);
        return;
    }
    memcpy(name, data, size);
    name[size] = '\0';
    
    // Test cJSON_AddBoolToObject
    cJSON *bool_item = cJSON_AddBoolToObject(root, name, bool_val);
    
    // Test cJSON_IsBool
    if (bool_item) {
        cJSON_IsBool(bool_item);
    }
    
    // Test cJSON_IsTrue
    if (bool_item) {
        cJSON_IsTrue(bool_item);
    }
    
    // Test cJSON_CreateFalse
    cJSON *false_item = cJSON_CreateFalse();
    
    // Test cJSON_IsFalse
    if (false_item) {
        cJSON_IsFalse(false_item);
    }
    
    // Test cJSON_Compare with various combinations
    cJSON_bool case_sensitive = (size > 1) ? (data[1] & 1) : 0;
    
    // Compare bool_item with false_item
    if (bool_item && false_item) {
        cJSON_Compare(bool_item, false_item, case_sensitive);
    }
    
    // Compare root with itself
    cJSON_Compare(root, root, case_sensitive);
    
    // Create another object with same boolean for comparison
    cJSON *root2 = cJSON_CreateObject();
    if (root2) {
        cJSON *bool_item2 = cJSON_AddBoolToObject(root2, name, bool_val);
        if (bool_item && bool_item2) {
            cJSON_Compare(bool_item, bool_item2, case_sensitive);
        }
        cJSON_Delete(root2);
    }
    
    // Cleanup
    free(name);
    if (false_item) {
        cJSON_Delete(false_item);
    }
    cJSON_Delete(root);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
    fuzz_cjson_apis(data, size);
    return 0;
}