// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
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
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy JSON object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Parse input data as a potential JSON string to add to the object
    // We use a subset of data to avoid parsing failures on binary data
    char *json_str = NULL;
    if (Size > 10) {
        json_str = (char *)malloc(Size + 1);
        if (json_str) {
            memcpy(json_str, Data, Size);
            json_str[Size] = '\0';
            
            // Try to parse the input as JSON to add items dynamically
            cJSON *parsed = cJSON_Parse(json_str);
            if (parsed && parsed->type == cJSON_Object) {
                // Merge parsed object into root
                cJSON *item;
                cJSON_ArrayForEach(item, parsed) {
                    if (item->string) {
                        cJSON_AddItemToObject(root, item->string, cJSON_Duplicate(item, 1));
                    }
                }
                cJSON_Delete(parsed);
            } else if (parsed) {
                // If not an object, just add as a raw string item
                cJSON_AddStringToObject(root, "input_data", json_str);
                cJSON_Delete(parsed);
            } else {
                // If parsing fails, add as raw string
                cJSON_AddStringToObject(root, "raw_input", json_str);
            }
            free(json_str);
        }
    }

    // Extract key and value from input for testing
    const char *key = (const char *)Data;
    size_t key_len = Size;
    if (key_len > 0 && key[key_len - 1] != '\0') {
        // Ensure null termination for key if not present
        char *safe_key = (char *)malloc(key_len + 1);
        if (safe_key) {
            memcpy(safe_key, key, key_len);
            safe_key[key_len] = '\0';
            key = safe_key;
        } else {
            key = "default_key";
        }
    } else {
        key = (const char *)Data;
    }

    // 1. Test cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(root, key);
    (void)has_item; // Suppress unused variable warning

    // 2. Test cJSON_AddBoolToObject
    cJSON_bool bool_val = (Size % 2) ? 1 : 0;
    cJSON *bool_item = cJSON_AddBoolToObject(root, "bool_test", bool_val);
    if (bool_item) {
        // Successfully added, no cleanup needed as it's part of root
    }

    // 3. Test cJSON_AddItemToObject with a new item
    cJSON *new_item = cJSON_CreateNumber((double)Size);
    if (new_item) {
        cJSON_bool added = cJSON_AddItemToObject(root, "number_test", new_item);
        if (!added) {
            // If failed, we must free new_item as ownership wasn't transferred
            cJSON_Delete(new_item);
        }
    }

    // 4. Test cJSON_ReplaceItemInObjectCaseSensitive
    cJSON *replace_item = cJSON_CreateString("replaced_value");
    if (replace_item) {
        cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "number_test", replace_item);
        if (!replaced) {
            cJSON_Delete(replace_item);
        }
    }

    // 5. Test cJSON_ReplaceItemInObject (case insensitive)
    cJSON *replace_item_ci = cJSON_CreateString("replaced_ci_value");
    if (replace_item_ci) {
        cJSON_bool replaced_ci = cJSON_ReplaceItemInObject(root, "number_test", replace_item_ci);
        if (!replaced_ci) {
            cJSON_Delete(replace_item_ci);
        }
    }

    // 6. Test cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, "bool_test");

    // Cleanup
    cleanup_json_tree(root);
    
    // Free safe_key if allocated
    if (Size > 0 && Data[Size - 1] != '\0') {
        free((void *)key);
    }

    return 0;
}