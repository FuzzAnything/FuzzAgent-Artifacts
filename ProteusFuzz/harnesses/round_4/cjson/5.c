// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for parsing
    char *input = (char *)malloc(Size + 1);
    if (!input) {
        return 0;
    }
    memcpy(input, Data, Size);
    input[Size] = '\0';

    // 1. cJSON_Parse
    cJSON *root = cJSON_Parse(input);
    if (!root) {
        free(input);
        return 0;
    }

    // Ensure root is an object for subsequent operations
    // Fixed: Use bitwise check against the type flag constant (cJSON_Object)
    // The error occurred because cJSON_IsArray is a function in some versions or a macro that expands incorrectly in this context.
    // We check if it is NOT an array (i.e., it is an object) to proceed.
    // Standard cJSON types: cJSON_Object = 1, cJSON_Array = 2.
    // We want root to be an object.
    if ((root->type & 0xFF) != cJSON_Object) {
        cJSON_Delete(root);
        free(input);
        return 0;
    }

    // 2. cJSON_CreateString
    // Create a string from the input data (safe copy)
    cJSON *item1 = cJSON_CreateString(input);
    if (!item1) {
        cJSON_Delete(root);
        free(input);
        return 0;
    }

    // 3. cJSON_CreateStringReference
    // Create a reference to a static string to test reference handling
    const char *static_ref = "static_reference_string";
    cJSON *item2 = cJSON_CreateStringReference(static_ref);
    if (!item2) {
        cJSON_Delete(root);
        cJSON_Delete(item1);
        free(input);
        return 0;
    }

    // 4. cJSON_AddItemToObject (first)
    if (!cJSON_AddItemToObject(root, "key1", item1)) {
        cJSON_Delete(root);
        cJSON_Delete(item2);
        free(input);
        return 0;
    }

    // 5. cJSON_AddItemToObject (second)
    if (!cJSON_AddItemToObject(root, "key2", item2)) {
        cJSON_Delete(root);
        free(input);
        return 0;
    }

    // 6. cJSON_SetValuestring
    // Attempt to set value on item1 (which is a string)
    // We use a safe substring or the original input to avoid overflow issues
    char *new_val = (char *)malloc(Size + 1);
    if (!new_val) {
        cJSON_Delete(root);
        free(input);
        return 0;
    }
    memcpy(new_val, input, Size);
    new_val[Size] = '\0';

    cJSON *target1 = cJSON_GetObjectItem(root, "key1");
    if (target1) {
        cJSON_SetValuestring(target1, new_val);
    }

    // 7. cJSON_GetObjectItem
    cJSON *retrieved1 = cJSON_GetObjectItem(root, "key1");
    (void)retrieved1; // Suppress unused warning, logic is tested by existence

    // 8. cJSON_GetObjectItem
    cJSON *retrieved2 = cJSON_GetObjectItem(root, "key2");
    (void)retrieved2;

    // 9. cJSON_SetValuestring
    // Try to set value on item2. Note: item2 is a reference (cJSON_IsReference).
    // cJSON_SetValuestring should handle this gracefully or return NULL based on implementation.
    if (retrieved2) {
        cJSON_SetValuestring(retrieved2, "updated_reference");
    }

    // 10. cJSON_GetObjectItem
    cJSON *retrieved3 = cJSON_GetObjectItem(root, "key1");
    (void)retrieved3;

    // 11. cJSON_GetObjectItem
    cJSON *retrieved4 = cJSON_GetObjectItem(root, "key2");
    (void)retrieved4;

    // 12. cJSON_SetValuestring
    if (retrieved3) {
        cJSON_SetValuestring(retrieved3, "final_update");
    }

    // 13. cJSON_GetObjectItem
    cJSON *retrieved5 = cJSON_GetObjectItem(root, "key1");
    (void)retrieved5;

    // 14. cJSON_GetObjectItem
    cJSON *retrieved6 = cJSON_GetObjectItem(root, "key2");
    (void)retrieved6;

    // 15. cJSON_Delete
    cJSON_Delete(root);

    // Cleanup
    free(input);
    free(new_val);

    return 0;
}