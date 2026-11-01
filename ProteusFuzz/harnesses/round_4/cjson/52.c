// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Parse the input data as JSON
    cJSON *root = cJSON_ParseWithLength((const char *)Data, Size);
    if (!root) {
        return 0;
    }

    // Ensure we have a valid object to work with for most operations
    if (cJSON_IsObject(root)) {
        // Test cJSON_HasObjectItem
        cJSON_bool has_item = cJSON_HasObjectItem(root, "test_key");
        (void)has_item;

        // Test cJSON_GetObjectItem
        cJSON *item = cJSON_GetObjectItem(root, "test_key");
        if (item) {
            // Test cJSON_GetStringValue
            char *str_val = cJSON_GetStringValue(item);
            (void)str_val;
        }

        // Test cJSON_AddBoolToObject
        cJSON *bool_item = cJSON_AddBoolToObject(root, "fuzz_bool", 1);
        if (bool_item) {
            // Try to detach the item we just added
            cJSON *detached = cJSON_DetachItemFromObject(root, "fuzz_bool");
            if (detached) {
                cleanup_cjson(detached);
            }
        }
    }

    // Test functions with NULL or non-object inputs to ensure robustness
    cJSON *null_item = NULL;
    cJSON_bool is_obj = cJSON_IsObject(null_item);
    (void)is_obj;

    char *null_str = cJSON_GetStringValue(null_item);
    (void)null_str;

    cJSON *null_detach = cJSON_DetachItemFromObject(null_item, "key");
    (void)null_detach;

    cJSON *null_get = cJSON_GetObjectItem(null_item, "key");
    (void)null_get;

    cJSON_bool null_has = cJSON_HasObjectItem(null_item, "key");
    (void)null_has;

    cJSON *null_add = cJSON_AddBoolToObject(null_item, "key", 0);
    (void)null_add;

    // Cleanup the root object
    cleanup_cjson(root);

    return 0;
}