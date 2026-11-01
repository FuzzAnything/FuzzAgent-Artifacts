// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: Parse JSON string and check types
    // Ensure null termination for string parsing
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (root != NULL) {
        // Call target API functions on the parsed root
        cJSON_IsArray(root);
        cJSON_IsInvalid(root);
        cJSON_IsNull(root);
        cJSON_IsTrue(root);
        cJSON_IsFalse(root);
        cJSON_IsBool(root);

        // Recursively check children to explore more states
        cJSON *current = root->child;
        while (current != NULL) {
            cJSON_IsArray(current);
            cJSON_IsInvalid(current);
            cJSON_IsNull(current);
            cJSON_IsTrue(current);
            cJSON_IsFalse(current);
            cJSON_IsBool(current);
            current = current->next;
        }
        
        cleanup_cjson(root);
    }

    // Test 2: Directly manipulate cJSON structure to test edge cases
    // Create a raw cJSON item to test type checking without parsing
    cJSON *raw_item = cJSON_CreateNull();
    if (raw_item != NULL) {
        cJSON_IsArray(raw_item);
        cJSON_IsInvalid(raw_item);
        cJSON_IsNull(raw_item);
        cJSON_IsTrue(raw_item);
        cJSON_IsFalse(raw_item);
        cJSON_IsBool(raw_item);
        cleanup_cjson(raw_item);
    }

    raw_item = cJSON_CreateTrue();
    if (raw_item != NULL) {
        cJSON_IsArray(raw_item);
        cJSON_IsInvalid(raw_item);
        cJSON_IsNull(raw_item);
        cJSON_IsTrue(raw_item);
        cJSON_IsFalse(raw_item);
        cJSON_IsBool(raw_item);
        cleanup_cjson(raw_item);
    }

    raw_item = cJSON_CreateFalse();
    if (raw_item != NULL) {
        cJSON_IsArray(raw_item);
        cJSON_IsInvalid(raw_item);
        cJSON_IsNull(raw_item);
        cJSON_IsTrue(raw_item);
        cJSON_IsFalse(raw_item);
        cJSON_IsBool(raw_item);
        cleanup_cjson(raw_item);
    }

    raw_item = cJSON_CreateNumber(123.45);
    if (raw_item != NULL) {
        cJSON_IsArray(raw_item);
        cJSON_IsInvalid(raw_item);
        cJSON_IsNull(raw_item);
        cJSON_IsTrue(raw_item);
        cJSON_IsFalse(raw_item);
        cJSON_IsBool(raw_item);
        cleanup_cjson(raw_item);
    }

    raw_item = cJSON_CreateString("test");
    if (raw_item != NULL) {
        cJSON_IsArray(raw_item);
        cJSON_IsInvalid(raw_item);
        cJSON_IsNull(raw_item);
        cJSON_IsTrue(raw_item);
        cJSON_IsFalse(raw_item);
        cJSON_IsBool(raw_item);
        cleanup_cjson(raw_item);
    }

    raw_item = cJSON_CreateArray();
    if (raw_item != NULL) {
        cJSON_IsArray(raw_item);
        cJSON_IsInvalid(raw_item);
        cJSON_IsNull(raw_item);
        cJSON_IsTrue(raw_item);
        cJSON_IsFalse(raw_item);
        cJSON_IsBool(raw_item);
        cleanup_cjson(raw_item);
    }

    // Test 3: Test with NULL pointer (should return false for all except potentially specific logic)
    cJSON_IsArray(NULL);
    cJSON_IsInvalid(NULL);
    cJSON_IsNull(NULL);
    cJSON_IsTrue(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsBool(NULL);

    // Test 4: Manually construct an invalid item to test cJSON_IsInvalid
    cJSON *invalid_item = cJSON_CreateNull();
    if (invalid_item != NULL) {
        // Manually set type to Invalid to test the check
        invalid_item->type = cJSON_Invalid;
        cJSON_IsInvalid(invalid_item);
        cJSON_IsNull(invalid_item);
        cleanup_cjson(invalid_item);
    }

    return 0;
}