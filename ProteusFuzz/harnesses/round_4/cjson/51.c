// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a copy of input to ensure null termination for string operations
    char *input_str = NULL;
    char *key_str = NULL;
    
    // Allocate buffers for JSON string and key
    size_t json_len = Size;
    input_str = (char *)malloc(json_len + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, json_len);
    input_str[json_len] = '\0';

    // Parse the JSON input
    cJSON *root = cJSON_Parse(input_str);
    free(input_str);

    if (root == NULL) {
        // Invalid JSON, try to create a simple object for testing functions that need an object
        root = cJSON_CreateObject();
        if (!root) {
            return 0;
        }
    }

    // Generate a key string from the input data for testing object operations
    size_t key_len = (Size > 10) ? 10 : Size;
    key_str = (char *)malloc(key_len + 1);
    if (!key_str) {
        cleanup_cjson(root);
        return 0;
    }
    memcpy(key_str, Data, key_len);
    key_str[key_len] = '\0';

    // Test cJSON_IsObject
    cJSON_bool is_obj = cJSON_IsObject(root);
    (void)is_obj; // Suppress unused warning

    // Test cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(root, key_str);
    (void)has_item;

    // Test cJSON_GetObjectItem
    cJSON *item = cJSON_GetObjectItem(root, key_str);
    if (item) {
        // Test cJSON_GetStringValue if the item is a string
        char *str_val = cJSON_GetStringValue(item);
        (void)str_val;
    }

    // Test cJSON_DetachItemFromObject
    // We need a mutable object, so if root is const or we want to ensure we can detach,
    // we might need to create a new object if root is not an object or is read-only in some contexts.
    // However, cJSON_DetachItemFromObject takes a non-const cJSON*.
    // If root is not an object, this might fail or do nothing, which is fine for fuzzing.
    cJSON *detached = cJSON_DetachItemFromObject(root, key_str);
    if (detached) {
        // Detached item should be freed or handled
        cJSON_Delete(detached);
    }

    // Test cJSON_AddBoolToObject
    // Add a boolean item with the generated key
    cJSON *bool_item = cJSON_AddBoolToObject(root, key_str, (cJSON_bool)(Size % 2));
    if (bool_item) {
        // Verify it was added
        cJSON_bool exists = cJSON_HasObjectItem(root, key_str);
        (void)exists;
    }

    // Cleanup
    cleanup_cjson(root);
    free(key_str);

    return 0;
}