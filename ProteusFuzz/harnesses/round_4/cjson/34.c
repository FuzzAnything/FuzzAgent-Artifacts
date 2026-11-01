// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = NULL;
    cJSON *item = NULL;

    // Ensure the input is null-terminated for cJSON_Parse
    // We create a temporary buffer to safely hold the input with a null terminator
    char *safe_input = (char *)malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }

    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    // Attempt to parse the input as JSON
    root = cJSON_Parse(safe_input);

    // Test functions on the parsed root (if valid)
    if (root) {
        cJSON_IsTrue(root);
        cJSON_IsNull(root);
        cJSON_IsFalse(root);
        cJSON_IsInvalid(root);
        cJSON_IsBool(root);
        cJSON_IsArray(root);

        // Traverse children to test more states
        item = root->child;
        while (item) {
            cJSON_IsTrue(item);
            cJSON_IsNull(item);
            cJSON_IsFalse(item);
            cJSON_IsInvalid(item);
            cJSON_IsBool(item);
            cJSON_IsArray(item);
            item = item->next;
        }

        // Test on a specific child if it exists
        if (root->child) {
            cJSON_IsTrue(root->child);
            cJSON_IsNull(root->child);
            cJSON_IsFalse(root->child);
            cJSON_IsInvalid(root->child);
            cJSON_IsBool(root->child);
            cJSON_IsArray(root->child);
        }
    }

    // Test functions with NULL pointer explicitly
    cJSON_IsTrue(NULL);
    cJSON_IsNull(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsInvalid(NULL);
    cJSON_IsBool(NULL);
    cJSON_IsArray(NULL);

    // Test functions on manually constructed invalid/edge case nodes
    // Create a node with Invalid type
    cJSON *invalid_node = cJSON_CreateNull();
    if (invalid_node) {
        invalid_node->type = cJSON_Invalid;
        cJSON_IsInvalid(invalid_node);
        cJSON_IsNull(invalid_node);
        cJSON_IsBool(invalid_node);
        cJSON_IsArray(invalid_node);
        cJSON_IsTrue(invalid_node);
        cJSON_IsFalse(invalid_node);
        cleanup_cjson(invalid_node);
    }

    // Create a node with True type
    cJSON *true_node = cJSON_CreateTrue();
    if (true_node) {
        cJSON_IsTrue(true_node);
        cJSON_IsBool(true_node);
        cJSON_IsNull(true_node);
        cJSON_IsFalse(true_node);
        cJSON_IsInvalid(true_node);
        cJSON_IsArray(true_node);
        cleanup_cjson(true_node);
    }

    // Create a node with False type
    cJSON *false_node = cJSON_CreateFalse();
    if (false_node) {
        cJSON_IsFalse(false_node);
        cJSON_IsBool(false_node);
        cJSON_IsTrue(false_node);
        cJSON_IsNull(false_node);
        cJSON_IsInvalid(false_node);
        cJSON_IsArray(false_node);
        cleanup_cjson(false_node);
    }

    // Create a Null node
    cJSON *null_node = cJSON_CreateNull();
    if (null_node) {
        cJSON_IsNull(null_node);
        cJSON_IsBool(null_node);
        cJSON_IsTrue(null_node);
        cJSON_IsFalse(null_node);
        cJSON_IsInvalid(null_node);
        cJSON_IsArray(null_node);
        cleanup_cjson(null_node);
    }

    // Create an Array node
    cJSON *array_node = cJSON_CreateArray();
    if (array_node) {
        cJSON_IsArray(array_node);
        cJSON_IsBool(array_node);
        cJSON_IsTrue(array_node);
        cJSON_IsFalse(array_node);
        cJSON_IsNull(array_node);
        cJSON_IsInvalid(array_node);
        cleanup_cjson(array_node);
    }

    // Cleanup root if it was parsed
    if (root) {
        cJSON_Delete(root);
    }

    free(safe_input);

    return 0;
}