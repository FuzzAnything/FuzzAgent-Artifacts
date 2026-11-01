// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_json(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Interpret input as JSON string
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        return 0;
    }

    // Ensure root is an object for object-specific operations
    if (!cJSON_IsObject(root)) {
        // If not an object, try to create a dummy object to test functions
        cJSON *dummy_obj = cJSON_CreateObject();
        if (!dummy_obj) {
            cleanup_json(root);
            return 0;
        }
        // Swap root to be the dummy object for testing
        cJSON *temp = root;
        root = dummy_obj;
        cleanup_json(temp);
    }

    // Create a key string from input data (ensure null termination)
    char key[256];
    size_t key_len = Size < 255 ? Size : 255;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Test cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(root, key);
    (void)has_item; // Use result to avoid unused warning

    // Test cJSON_GetObjectItem
    cJSON *item = cJSON_GetObjectItem(root, key);
    if (item) {
        // Test cJSON_GetStringValue if item is a string
        char *str_val = cJSON_GetStringValue(item);
        (void)str_val; // Use result to avoid unused warning
    }

    // Test cJSON_AddBoolToObject
    cJSON *added_bool = cJSON_AddBoolToObject(root, key, (cJSON_bool)(Data[0] % 2));
    if (added_bool) {
        // Successfully added, can be detached later
    }

    // Test cJSON_IsObject on root and children
    cJSON_bool is_obj = cJSON_IsObject(root);
    (void)is_obj;
    if (root && root->child) {
        cJSON_bool child_is_obj = cJSON_IsObject(root->child);
        (void)child_is_obj;
    }

    // Test cJSON_DetachItemFromObject
    cJSON *detached = cJSON_DetachItemFromObject(root, key);
    if (detached) {
        // Detached item should be freed separately
        cJSON_Delete(detached);
    }

    // Cleanup
    cleanup_json(root);

    return 0;
}