// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Parse input as JSON to create a base structure
    // We prepend a '[' to ensure valid JSON if possible, or let parser fail safely
    char *json_str = (char *)malloc(Size + 2);
    if (!json_str) return 0;

    json_str[0] = '[';
    memcpy(json_str + 1, Data, Size);
    json_str[Size + 1] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    // If parsing failed, try to create a minimal array manually for testing
    if (!root) {
        root = cJSON_CreateArray();
    }

    if (!root) {
        return 0;
    }

    // Ensure root is an array for most tests
    if (!cJSON_IsArray(root)) {
        // If it's an object or other type, wrap it or reset to array
        cJSON_Delete(root);
        root = cJSON_CreateArray();
        if (!root) return 0;
    }

    // Create some dummy items to manipulate
    cJSON *item1 = cJSON_CreateNumber(123.0);
    cJSON *item2 = cJSON_CreateString("test");
    cJSON *item3 = cJSON_CreateBool(1);

    if (!item1 || !item2 || !item3) {
        cJSON_Delete(root);
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
        if (item3) cJSON_Delete(item3);
        return 0;
    }

    // Test 1: cJSON_IsArray
    cJSON_IsArray(root);
    cJSON_IsArray(item1);
    cJSON_IsArray(NULL);

    // Test 2: cJSON_GetArraySize
    int size = cJSON_GetArraySize(root);
    (void)size; // Suppress unused warning
    cJSON_GetArraySize(NULL);

    // Test 3: cJSON_AddItemReferenceToArray
    // Add item1 by reference
    cJSON_AddItemReferenceToArray(root, item1);
    // Try adding NULL
    cJSON_AddItemReferenceToArray(root, NULL);
    // Try adding to NULL array
    cJSON_AddItemReferenceToArray(NULL, item2);

    // Test 4: cJSON_GetArrayItem
    cJSON *retrieved = cJSON_GetArrayItem(root, 0);
    cJSON_GetArrayItem(root, -1);
    cJSON_GetArrayItem(root, 9999);
    cJSON_GetArrayItem(NULL, 0);
    if (retrieved) {
        // No need to delete retrieved as it's a reference
    }

    // Test 5: cJSON_InsertItemInArray
    // Insert at beginning
    cJSON_InsertItemInArray(root, 0, item2);
    // Insert at end (index > size)
    cJSON_InsertItemInArray(root, 999, item3);
    // Insert with negative index
    cJSON_InsertItemInArray(root, -1, item1);
    // Insert NULL item
    cJSON_InsertItemInArray(root, 0, NULL);
    // Insert into NULL array
    cJSON_InsertItemInArray(NULL, 0, item1);

    // Test 6: cJSON_DeleteItemFromArray
    // Delete first item
    cJSON_DeleteItemFromArray(root, 0);
    // Delete out of bounds
    cJSON_DeleteItemFromArray(root, 999);
    // Delete negative index
    cJSON_DeleteItemFromArray(root, -1);
    // Delete from NULL array
    cJSON_DeleteItemFromArray(NULL, 0);

    // Cleanup
    // Note: item1, item2, item3 might be owned by root now if added by reference or value
    // Since we used AddItemReferenceToArray and InsertItemInArray, ownership is tricky.
    // To be safe, we delete the root which should handle the chain.
    cJSON_Delete(root);

    return 0;
}