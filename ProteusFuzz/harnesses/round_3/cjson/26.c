// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_json_tree(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a copy of the input data to ensure null-termination for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (root == NULL) {
        // Parsing failed, try to fuzz with a simple valid JSON object
        root = cJSON_CreateObject();
        if (root == NULL) {
            return 0;
        }
    }

    // Extract a key string from the input data for API testing
    // We use a portion of the input as the key string
    char key_buf[256];
    size_t key_len = (Size > 255) ? 255 : Size;
    memcpy(key_buf, Data, key_len);
    key_buf[key_len] = '\0';

    // Fuzz cJSON_HasObjectItem
    if (cJSON_IsObject(root)) {
        cJSON_HasObjectItem(root, key_buf);
    }

    // Fuzz cJSON_IsObject
    cJSON_IsObject(root);
    if (root->child) {
        cJSON_IsObject(root->child);
    }

    // Fuzz cJSON_GetObjectItem
    if (cJSON_IsObject(root)) {
        cJSON *item = cJSON_GetObjectItem(root, key_buf);
        if (item != NULL) {
            // Fuzz cJSON_GetStringValue on the retrieved item
            cJSON_GetStringValue(item);
        }
    }

    // Fuzz cJSON_DetachItemFromObject
    // Create a mutable copy if root is not already mutable (cJSON_Parse returns const in some contexts, 
    // but standard usage implies ownership. We assume we own the tree).
    cJSON *mutable_root = root; 
    // Note: If root was parsed, it's owned by us. If we created it, we own it.
    // DetachItemFromObject modifies the object, so we need a non-const pointer.
    // The API signature takes cJSON *object.
    if (cJSON_IsObject(mutable_root)) {
        cJSON *detached = cJSON_DetachItemFromObject(mutable_root, key_buf);
        if (detached != NULL) {
            // The detached item is now owned by us, we should delete it or keep it.
            // To avoid leaks in fuzzing loop, we delete it immediately.
            cJSON_Delete(detached);
        }
    }

    // Fuzz cJSON_AddBoolToObject
    // We need an object to add to. Let's ensure we have one.
    if (cJSON_IsObject(mutable_root)) {
        cJSON_AddBoolToObject(mutable_root, key_buf, 1);
        cJSON_AddBoolToObject(mutable_root, key_buf, 0);
    } else {
        // If root is not an object, create one to test the function
        cJSON *temp_obj = cJSON_CreateObject();
        if (temp_obj != NULL) {
            cJSON_AddBoolToObject(temp_obj, key_buf, 1);
            cJSON_Delete(temp_obj);
        }
    }

    // Cleanup
    cleanup_json_tree(root);

    return 0;
}