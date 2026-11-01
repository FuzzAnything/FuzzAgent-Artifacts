// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *new_item = NULL;
    cJSON *bool_item = NULL;
    const char *key_str = NULL;
    char *key_copy = NULL;
    char *input_copy = NULL;

    // Allocate buffer for input + null terminator
    input_copy = (char *)malloc(Size + 1);
    if (!input_copy) {
        return 0;
    }
    memcpy(input_copy, Data, Size);
    input_copy[Size] = '\0';

    // Attempt to parse the input as JSON
    root = cJSON_Parse(input_copy);
    
    // If parsing failed or result is not an object, try to create a dummy object
    // to ensure we have a valid object to test the APIs against.
    // Note: cJSON_IsReference, cJSON_IsArray, cJSON_Object are flags (integers).
    // The previous error was due to incorrect operator precedence or type mismatch.
    // We check if root is NULL, or if it is NOT an object (i.e., (root->type & cJSON_Object) == 0).
    // We also check if it's an array to be safe, though the object check covers it if flags are distinct.
    if (!root || (root->type & cJSON_Object) == 0) {
        // If root is not an object, we create a new empty object to test with
        if (root) {
            cJSON_Delete(root);
        }
        obj = cJSON_CreateObject();
    } else {
        obj = root;
    }

    if (!obj) {
        free(input_copy);
        return 0;
    }

    // Extract potential key and value from the input for testing
    // We use the input data as potential key strings
    if (Size > 0) {
        key_copy = (char *)malloc(Size + 1);
        if (key_copy) {
            memcpy(key_copy, Data, Size);
            key_copy[Size] = '\0';
            key_str = key_copy;
        }
    }

    // 1. Test cJSON_HasObjectItem
    // Safe to call with NULL key or NULL object (handled internally), but we pass valid obj
    if (key_str) {
        cJSON_HasObjectItem(obj, key_str);
    } else {
        cJSON_HasObjectItem(obj, "default_key");
    }
    cJSON_HasObjectItem(obj, NULL);
    cJSON_HasObjectItem(NULL, key_str ? key_str : "test");

    // 2. Test cJSON_ReplaceItemInObjectCaseSensitive
    // We need a new item to replace with
    if (key_str) {
        new_item = cJSON_CreateString("replacement_value");
        if (new_item) {
            cJSON_ReplaceItemInObjectCaseSensitive(obj, key_str, new_item);
            // If replacement failed (key not found), the new_item is still owned by us
            if (!cJSON_HasObjectItem(obj, key_str)) {
                cJSON_Delete(new_item);
            }
        }
    }

    // 3. Test cJSON_DeleteItemFromObjectCaseSensitive
    if (key_str) {
        cJSON_DeleteItemFromObjectCaseSensitive(obj, key_str);
    } else {
        cJSON_DeleteItemFromObjectCaseSensitive(obj, "test_key");
    }
    cJSON_DeleteItemFromObjectCaseSensitive(NULL, key_str ? key_str : "test");

    // 4. Test cJSON_AddItemToObject
    if (key_str) {
        new_item = cJSON_CreateNumber(123);
        if (new_item) {
            cJSON_AddItemToObject(obj, key_str, new_item);
            // If add failed, we must delete the item
            if (!cJSON_HasObjectItem(obj, key_str)) {
                cJSON_Delete(new_item);
            }
        }
    } else {
        new_item = cJSON_CreateNumber(456);
        if (new_item) {
            cJSON_AddItemToObject(obj, "fuzz_key", new_item);
            if (!cJSON_HasObjectItem(obj, "fuzz_key")) {
                cJSON_Delete(new_item);
            }
        }
    }

    // 5. Test cJSON_AddBoolToObject
    cJSON_bool bool_val = (Size % 2) ? 1 : 0;
    if (key_str) {
        bool_item = cJSON_AddBoolToObject(obj, key_str, bool_val);
        // If it returns NULL, the item was already deleted internally, nothing to do
    } else {
        bool_item = cJSON_AddBoolToObject(obj, "bool_key", bool_val);
    }

    // 6. Test cJSON_ReplaceItemInObject
    if (key_str) {
        new_item = cJSON_CreateString("updated_value");
        if (new_item) {
            cJSON_ReplaceItemInObject(obj, key_str, new_item);
            // If replacement failed, we own the new_item
            if (!cJSON_HasObjectItem(obj, key_str)) {
                cJSON_Delete(new_item);
            }
        }
    } else {
        new_item = cJSON_CreateString("updated_fuzz");
        if (new_item) {
            cJSON_ReplaceItemInObject(obj, "fuzz_key", new_item);
            if (!cJSON_HasObjectItem(obj, "fuzz_key")) {
                cJSON_Delete(new_item);
            }
        }
    }

    // Cleanup
    cleanup_json(obj);
    free(input_copy);
    free(key_copy);

    return 0;
}