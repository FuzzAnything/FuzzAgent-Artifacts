// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *new_item = NULL;
    cJSON *dup_item = NULL;
    int which;
    cJSON_bool recurse;

    // Ensure the input is null-terminated for cJSON_Parse
    // Allocate a new buffer with an extra byte for the null terminator
    char *safe_input = (char *)malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    // Parse input as JSON to create a root structure if possible
    root = cJSON_Parse(safe_input);
    
    // Free the temporary buffer
    free(safe_input);

    if (!root) {
        // If parsing fails, create a simple dummy structure
        root = cJSON_CreateArray();
    }

    // Ensure we have an array to work with
    if (root && root->type != cJSON_Array) {
        cJSON_Delete(root);
        root = cJSON_CreateArray();
    }
    array = root;

    // Create a new item to use in operations
    new_item = cJSON_CreateString("fuzz_test");
    if (!new_item) {
        cleanup_cjson(root);
        return 0;
    }

    // Extract parameters from input data safely
    which = (int)Data[0];
    recurse = (Size > 1) ? (cJSON_bool)(Data[1] % 2) : 0;

    // Test cJSON_AddItemReferenceToArray
    // Note: This function adds a reference, so the caller does not own the item anymore
    // However, new_item is created fresh here, so it's safe to pass.
    // We must be careful not to delete new_item later if it was successfully added.
    cJSON_AddItemReferenceToArray(array, new_item);
    // Since we added a reference, we should NOT delete new_item here.
    // Ownership is transferred to the array.

    // Test cJSON_InsertItemInArray
    cJSON_InsertItemInArray(array, which, cJSON_CreateNumber(123));

    // Test cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Suppress unused variable warning

    // Test cJSON_Duplicate
    if (array && array->child) {
        dup_item = cJSON_Duplicate(array->child, recurse);
        if (dup_item) {
            // Try to replace using the duplicate
            cJSON_ReplaceItemInArray(array, 0, dup_item);
            // Ownership of dup_item is transferred to the array, so we do not delete it here
        }
    }

    // Test cJSON_ReplaceItemViaPointer
    if (array && array->child) {
        cJSON *target = array->child;
        cJSON *replacement = cJSON_CreateBool(1);
        if (replacement) {
            cJSON_ReplaceItemViaPointer(array, target, replacement);
            // Ownership of replacement is transferred, so no cleanup needed for replacement
        }
    }

    // Test cJSON_ReplaceItemInArray with various indices
    cJSON_ReplaceItemInArray(array, which, cJSON_CreateNumber(456));
    cJSON_ReplaceItemInArray(array, -1, cJSON_CreateNumber(789)); // Invalid index test

    // Cleanup
    cleanup_cjson(root);
    // new_item was added via reference, so it is part of root and will be deleted with root.
    // Do not call cleanup_cjson(new_item) here.

    return 0;
}