// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cJSON.h"

static void cleanup_detached_item(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

static void cleanup_object(cJSON *object) {
    if (object != NULL) {
        cJSON_Delete(object);
    }
}

static char* create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char *str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char *str = (char*)malloc(len + 1);
    if (str) {
        memcpy(str, data, len);
        str[len] = '\0';
    }
    return str;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create test strings from fuzzer input
    size_t str_len = Size > 100 ? 100 : Size;
    char *key_str = create_null_terminated_string(Data, str_len, 50);
    char *value_str = create_null_terminated_string(Data + (str_len/2), 
                                                   Size > str_len/2 ? Size - str_len/2 : 0, 
                                                   50);
    
    if (!key_str || !value_str) {
        free(key_str);
        free(value_str);
        return 0;
    }

    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        free(key_str);
        free(value_str);
        return 0;
    }

    // Test 1: cJSON_AddNumberToObject
    double number = (double)(Data[0] * 256 + Data[Size > 1 ? 1 : 0]);
    cJSON *num_item = cJSON_AddNumberToObject(root, key_str, number);
    (void)num_item; // Use result to avoid unused variable warning

    // Test 2: cJSON_AddStringToObject
    cJSON *str_item = cJSON_AddStringToObject(root, value_str, key_str);
    (void)str_item;

    // Test 3: cJSON_AddRawToObject
    // Create a simple raw JSON string
    char raw_json[] = "{\"test\":123}";
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_key", raw_json);
    (void)raw_item;

    // Test 4: cJSON_DetachItemFromObject
    cJSON *detached = cJSON_DetachItemFromObject(root, key_str);
    cleanup_detached_item(detached);

    // Test 5: cJSON_DetachItemFromObjectCaseSensitive
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, value_str);
    cleanup_detached_item(detached);

    // Test 6: cJSON_DeleteItemFromObjectCaseSensitive
    // First add an item to delete
    cJSON_AddNumberToObject(root, "delete_me", 42.0);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "delete_me");

    // Try to delete non-existent item (should do nothing)
    cJSON_DeleteItemFromObjectCaseSensitive(root, "non_existent_key");

    // Cleanup
    cleanup_object(root);
    free(key_str);
    free(value_str);

    return 0;
}