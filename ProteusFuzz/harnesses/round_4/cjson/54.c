// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a copy of input data to ensure it is null-terminated for cJSON_Parse
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (root == NULL) {
        // Parsing failed, which is a valid state to test
        return 0;
    }

    // Create a separate object to test modification functions if root is not an object
    cJSON *test_obj = NULL;
    if (cJSON_IsObject(root)) {
        // If root is an object, we can use it directly for read-only checks,
        // but for modifications like Detach/Add, we might want a copy or a new object
        // to avoid side effects on the parsed structure if we wanted to re-parse.
        // However, for fuzzing, modifying the parsed tree is fine.
        test_obj = root;
    } else {
        // If root is not an object, create a new one for object-specific tests
        test_obj = cJSON_CreateObject();
    }

    if (test_obj == NULL) {
        cleanup_json(root);
        return 0;
    }

    // Fuzzing Strategy 1: Test cJSON_GetStringValue on various items
    // We traverse the tree to find string items or non-string items to test the NULL return
    cJSON *current = root;
    while (current != NULL) {
        char *str_val = cJSON_GetStringValue(current);
        // str_val is either a valid string or NULL. No action needed, just call it.
        
        // Move to next sibling
        current = current->next;
    }

    // Fuzzing Strategy 2: Test cJSON_IsObject
    // Test on root and children
    cJSON *item = root;
    while (item != NULL) {
        cJSON_IsObject(item);
        item = item->next;
    }
    // Test on test_obj specifically
    cJSON_IsObject(test_obj);

    // Fuzzing Strategy 3: Test cJSON_HasObjectItem and cJSON_GetObjectItem
    // We use the input data to generate keys to search for
    // We try to find keys that might exist or not exist
    for (size_t i = 0; i < Size; i++) {
        // Create a small key from the input buffer
        char key[256];
        size_t key_len = (Size - i) < 255 ? (Size - i) : 255;
        if (key_len > 0) {
            memcpy(key, &Data[i], key_len);
            key[key_len] = '\0';
        } else {
            key[0] = '\0';
        }

        if (cJSON_IsObject(test_obj)) {
            cJSON_HasObjectItem(test_obj, key);
            cJSON_GetObjectItem(test_obj, key);
        }
    }

    // Fuzzing Strategy 4: Test cJSON_AddBoolToObject
    // Add boolean items with keys derived from input
    if (cJSON_IsObject(test_obj)) {
        for (size_t i = 0; i < Size; i++) {
            char key[256];
            size_t key_len = (Size - i) < 255 ? (Size - i) : 255;
            if (key_len > 0) {
                memcpy(key, &Data[i], key_len);
                key[key_len] = '\0';
            } else {
                key[0] = '\0';
            }
            
            // Alternate between true (1) and false (0) based on index
            // Using 1 and 0 instead of true/false to avoid C89/C99 compatibility issues
            // if strict C89 is used, though cJSON.h usually defines them.
            // Explicitly casting to cJSON_bool to be safe.
            cJSON_bool val = (i % 2 == 0) ? 1 : 0;
            cJSON_AddBoolToObject(test_obj, key, val);
            
            // Limit iterations to prevent OOM on huge inputs
            if (i > 100) break;
        }
    }

    // Fuzzing Strategy 5: Test cJSON_DetachItemFromObject
    // Try to detach items using keys derived from input
    if (cJSON_IsObject(test_obj)) {
        for (size_t i = 0; i < Size; i++) {
            char key[256];
            size_t key_len = (Size - i) < 255 ? (Size - i) : 255;
            if (key_len > 0) {
                memcpy(key, &Data[i], key_len);
                key[key_len] = '\0';
            } else {
                key[0] = '\0';
            }

            cJSON *detached = cJSON_DetachItemFromObject(test_obj, key);
            if (detached != NULL) {
                // The detached item is still valid, we should free it to avoid leaks
                // In a real app, we might reuse it. Here we just delete it.
                cJSON_Delete(detached);
            }
            
            // Limit iterations
            if (i > 100) break;
        }
    }

    // Cleanup
    // If test_obj was root, we clean up root.
    // If test_obj was a new object created inside, we clean it up separately.
    // Note: If test_obj == root, we only delete once.
    if (test_obj == root) {
        cleanup_json(root);
    } else {
        cleanup_json(root);
        cleanup_json(test_obj);
    }

    return 0;
}