// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a root object to add items to
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Extract key name from input data
    // Use a safe substring to avoid buffer overflows
    size_t key_len = Size;
    if (key_len > 100) {
        key_len = 100;
    }
    
    // Ensure the key is null-terminated
    char *key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        cleanup_cjson(root);
        return 0;
    }
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Fuzz cJSON_AddBoolToObject
    cJSON_bool bool_val = (Data[0] % 2);
    cJSON *bool_item = cJSON_AddBoolToObject(root, key, bool_val);
    // No need to delete bool_item separately as it's part of root

    // Fuzz cJSON_AddNullToObject
    cJSON *null_item = cJSON_AddNullToObject(root, key);
    // No need to delete null_item separately as it's part of root

    // Fuzz cJSON_CreateFalse
    cJSON *false_node = cJSON_CreateFalse();
    if (false_node != NULL) {
        // Try to add the created false node to the object
        cJSON_AddItemToObjectCS(root, "fuzz_false", false_node);
        // false_node is now owned by root, don't delete
    }

    // Fuzz cJSON_CreateTrue
    cJSON *true_node = cJSON_CreateTrue();
    if (true_node != NULL) {
        cJSON_AddItemToObjectCS(root, "fuzz_true", true_node);
        // true_node is now owned by root, don't delete
    }

    // Fuzz cJSON_CreateNull
    cJSON *null_node = cJSON_CreateNull();
    if (null_node != NULL) {
        cJSON_AddItemToObjectCS(root, "fuzz_null", null_node);
        // null_node is now owned by root, don't delete
    }

    // Additional fuzzing: Try adding items with different keys derived from input
    // Fixed: Ensure we have enough data before accessing Data + 1
    if (Size > 1) {
        char key2[101];
        // Calculate remaining size safely
        size_t remaining = Size - 1;
        size_t key2_len = remaining;
        if (key2_len > 100) {
            key2_len = 100;
        }
        
        memcpy(key2, Data + 1, key2_len);
        key2[key2_len] = '\0';

        cJSON_AddBoolToObject(root, key2, (Data[1] % 2));
        cJSON_AddNullToObject(root, key2);
    }

    // Cleanup
    free(key);
    cleanup_cjson(root);

    return 0;
}