// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static cJSON *create_test_object_from_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }

    // Create a copy of the input data to ensure null termination for string operations
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
        return cJSON_CreateObject();
    }
    memcpy(input_str, data, size);
    input_str[size] = '\0';

    // Attempt to parse the input as JSON to create a valid object
    cJSON *root = cJSON_Parse(input_str);
    free(input_str);

    if (root && cJSON_IsObject(root)) {
        return root;
    }

    // If parsing fails or result is not an object, create a fresh object
    if (root) {
        cJSON_Delete(root);
    }
    return cJSON_CreateObject();
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a valid JSON object
    cJSON *obj = create_test_object_from_input(Data, Size);
    if (!obj) {
        return 0;
    }

    // Ensure the object is actually an object type for safety
    if (!cJSON_IsObject(obj)) {
        cJSON_Delete(obj);
        obj = cJSON_CreateObject();
        if (!obj) return 0;
    }

    // Extract key and value data from the fuzz input
    const char *key = (const char *)Data;
    // Ensure key is null-terminated for safety in API calls
    char *safe_key = (char *)malloc(Size + 1);
    if (!safe_key) {
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(safe_key, Data, Size);
    safe_key[Size] = '\0';

    // Determine boolean value from input (simple heuristic)
    // Use 0 and 1 instead of false/true to avoid C99/C++ dependency issues if not enabled
    cJSON_bool bool_val = (Size > 0 && Data[0] % 2 == 0) ? 1 : 0;

    // 1. Test cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(obj, safe_key);
    (void)has_item; // Use result to avoid unused warning

    // 2. Test cJSON_GetObjectItem
    cJSON *item = cJSON_GetObjectItem(obj, safe_key);
    if (item) {
        // 3. Test cJSON_GetStringValue on retrieved item
        char *str_val = cJSON_GetStringValue(item);
        (void)str_val;
    }

    // 4. Test cJSON_AddBoolToObject
    // This adds a new boolean item or updates existing if overwrite is allowed (default behavior depends on cjson version, 
    // but typically overwrites or fails if strict).
    cJSON *added_item = cJSON_AddBoolToObject(obj, safe_key, bool_val);
    if (added_item) {
        // Verify the added item
        cJSON_bool check_bool = added_item->valueint;
        (void)check_bool;
    }

    // 5. Test cJSON_DetachItemFromObject
    // Try to detach the item we just added or one that might exist
    cJSON *detached = cJSON_DetachItemFromObject(obj, safe_key);
    if (detached) {
        // The detached item is now orphaned; we should delete it to prevent leaks
        cJSON_Delete(detached);
    }

    // Cleanup
    cJSON_Delete(obj);
    free(safe_key);

    return 0;
}