// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_cjson_tree(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the input data as a JSON string to parse
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (root == NULL) {
        return 0;
    }

    // Ensure root is an object for most operations
    if (cJSON_IsObject(root) == 0) {
        cleanup_cjson_tree(root);
        return 0;
    }

    // Create a key string from the input data to use for lookups/deletions
    // We use a prefix to ensure it's a valid C string and potentially different from content
    char key[256];
    size_t key_len = (Size < 255) ? Size : 255;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Fuzz cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(root, key);
    (void)has_item; // Suppress unused variable warning

    // Fuzz cJSON_GetObjectItem
    cJSON *item = cJSON_GetObjectItem(root, key);
    if (item != NULL) {
        // Fuzz cJSON_GetStringValue on the retrieved item
        char *str_val = cJSON_GetStringValue(item);
        (void)str_val; // Suppress unused variable warning
    }

    // Fuzz cJSON_DetachItemFromObject
    cJSON *detached = cJSON_DetachItemFromObject(root, key);
    if (detached != NULL) {
        // Clean up the detached item manually since it's removed from the tree
        cJSON_Delete(detached);
    }

    // Re-parse to get a fresh object for deletion tests if needed, 
    // or reuse root if it still has items. 
    // For simplicity, let's try to delete from the current root state.
    // Note: If the item was detached above, this will do nothing safely.
    cJSON_DeleteItemFromObject(root, key);

    // Fuzz cJSON_DeleteItemFromObjectCaseSensitive
    // We need a fresh object or ensure the key exists. 
    // Since we don't know if the key exists after previous ops, 
    // we just call it; it handles missing keys safely.
    cJSON_DeleteItemFromObjectCaseSensitive(root, key);

    // Cleanup
    cleanup_cjson_tree(root);

    return 0;
}