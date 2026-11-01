// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json_tree(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    // Use input data to generate keys and values safely
    char key1[64], key2[64];
    memset(key1, 0, sizeof(key1));
    memset(key2, 0, sizeof(key2));

    // Ensure we don't read past the buffer or create negative sizes
    size_t key1_len = 0;
    size_t key2_len = 0;

    if (Size >= 1) {
        key1_len = (Size > 32) ? 32 : Size;
        if (key1_len > 0) {
            memcpy(key1, Data, key1_len);
            key1[key1_len] = '\0';
        }
    } else {
        strcpy(key1, "default_key1");
    }

    // Calculate remaining size for key2
    size_t remaining = (Size > key1_len) ? (Size - key1_len) : 0;
    if (remaining > 0) {
        key2_len = (remaining > 32) ? 32 : remaining;
        if (key2_len > 0) {
            memcpy(key2, Data + key1_len, key2_len);
            key2[key2_len] = '\0';
        }
    } else {
        strcpy(key2, "default_key2");
    }

    // Add initial items to the object
    cJSON_AddFalseToObject(obj, key1);
    cJSON_AddBoolToObject(obj, key2, (Data[0] % 2) == 0);
    
    cJSON *nested = cJSON_AddObjectToObject(obj, "nested");
    if (nested) {
        cJSON_AddFalseToObject(nested, "inner_false");
        // Ensure we have enough data for the second byte check
        cJSON_bool inner_bool_val = (Size > 1) ? ((Data[1] % 2) == 0) : 1;
        cJSON_AddBoolToObject(nested, "inner_bool", inner_bool_val);
    }

    // Create a new item to replace
    double num_val = (Size > 2) ? (double)(Data[2] % 100) : 0.0;
    cJSON *new_item = cJSON_CreateNumber(num_val);
    if (new_item) {
        // Try replacing an existing key
        cJSON_ReplaceItemInObject(obj, key1, new_item);
        
        // Try replacing a key that might not exist
        cJSON *temp_item = cJSON_CreateString("replacement_string");
        if (temp_item) {
            cJSON_ReplaceItemInObject(obj, "nonexistent_key", temp_item);
            // Since ReplaceItemInObject doesn't free on failure if key is missing, we must free temp_item
            cJSON_Delete(temp_item);
        }
    }

    // Test deletion
    cJSON_DeleteItemFromObject(obj, key1);
    cJSON_DeleteItemFromObject(obj, "nonexistent_key");
    if (nested) {
        cJSON_DeleteItemFromObject(nested, "inner_false");
    }

    // Cleanup
    cleanup_json_tree(obj);

    return 0;
}