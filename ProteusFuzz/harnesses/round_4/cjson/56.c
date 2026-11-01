// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"

static void cleanup_json_tree(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a copy of the input to ensure null-termination for cJSON_Parse
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // Parse JSON
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (root == NULL) {
        // Parsing failed, which is a valid state to test
        return 0;
    }

    // Ensure we have a valid object to work with for object-specific functions
    // If the root is not an object, we try to find an object child or create a dummy one
    cJSON *target_obj = NULL;
    
    if (cJSON_IsObject(root)) {
        target_obj = root;
    } else {
        // If root is not an object, create a temporary object to test object functions
        target_obj = cJSON_CreateObject();
        if (target_obj != NULL) {
            // We will clean up this temporary object separately if needed, 
            // but for this test, we just use it for the API calls.
            // Note: We don't delete 'root' yet, we'll delete both at the end.
        }
    }

    // Fuzzing Logic
    
    // 1. Test cJSON_IsObject on various nodes
    cJSON_IsObject(root);
    cJSON_IsObject(target_obj);
    cJSON_IsObject(NULL);
    
    // Traverse to test on children
    if (root != NULL) {
        cJSON *child = root->child;
        while (child != NULL) {
            cJSON_IsObject(child);
            child = child->next;
        }
    }

    // 2. Test cJSON_HasObjectItem and cJSON_GetObjectItem
    // We use a subset of the input data as a key string to generate diverse keys
    if (target_obj != NULL) {
        // Create a safe key string from input
        char key_buf[256];
        size_t key_len = (Size < 255) ? Size : 255;
        memcpy(key_buf, Data, key_len);
        key_buf[key_len] = '\0';
        
        cJSON_bool has_item = cJSON_HasObjectItem(target_obj, key_buf);
        (void)has_item; // Suppress unused warning

        cJSON *item = cJSON_GetObjectItem(target_obj, key_buf);
        if (item != NULL) {
            // If we found an item, test GetStringValue
            char *str_val = cJSON_GetStringValue(item);
            (void)str_val; // Suppress unused warning
            
            // Test DetachItemFromObject (only if target_obj is the real root or a valid object)
            // We need a writable object for Detach. If target_obj was created above, it's writable.
            // If target_obj is root, it's also writable (cJSON_Parse returns a modifiable tree).
            cJSON *detached = cJSON_DetachItemFromObject(target_obj, key_buf);
            if (detached != NULL) {
                // The item is detached, we should free it to avoid leaks in the fuzzer
                cJSON_Delete(detached);
            }
        }
    }

    // 3. Test cJSON_AddBoolToObject
    // We need a valid object to add to. If target_obj exists, use it.
    if (target_obj != NULL) {
        // Use first byte of input to determine boolean value
        cJSON_bool bool_val = (Data[0] % 2 == 0) ? true : false;
        
        // Generate a key for the new item
        char new_key[64];
        size_t new_key_len = (Size < 63) ? Size : 63;
        memcpy(new_key, Data, new_key_len);
        new_key[new_key_len] = '\0';
        
        cJSON *added_item = cJSON_AddBoolToObject(target_obj, new_key, bool_val);
        if (added_item != NULL) {
            // Successfully added, no extra cleanup needed as it's part of the tree
            // But we can test GetStringValue on it (should be NULL since it's a bool)
            char *str = cJSON_GetStringValue(added_item);
            (void)str;
        }
    }

    // 4. Test cJSON_GetStringValue on non-string items to ensure robustness
    // Try to find a number or boolean in the tree and call GetStringValue
    if (root != NULL) {
        cJSON *curr = root;
        // Simple traversal
        cJSON *stack[100];
        int top = 0;
        stack[top++] = root;
        
        while (top > 0) {
            curr = stack[--top];
            if (curr != NULL) {
                // Call GetStringValue regardless of type to test safety
                cJSON_GetStringValue(curr);
                
                // Push children to stack
                if (curr->child != NULL) {
                    cJSON *c = curr->child;
                    while (c != NULL && top < 100) {
                        stack[top++] = c;
                        c = c->next;
                    }
                }
            }
        }
    }

    // Cleanup
    // If target_obj was a temporary object created because root wasn't an object,
    // we must delete it. If target_obj was root, we delete root.
    // To avoid double deletion, we check if target_obj is the same as root.
    if (target_obj != NULL) {
        if (target_obj == root) {
            cJSON_Delete(root);
        } else {
            // target_obj was the temporary one
            cJSON_Delete(target_obj);
            // root still needs to be deleted if it wasn't the target
            if (root != NULL) {
                cJSON_Delete(root);
            }
        }
    } else {
        if (root != NULL) {
            cJSON_Delete(root);
        }
    }

    return 0;
}