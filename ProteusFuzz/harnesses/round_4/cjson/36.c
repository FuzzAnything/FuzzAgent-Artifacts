// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0 || Data == NULL) {
        return 0;
    }

    cJSON *root = NULL;
    cJSON *item = NULL;
    cJSON_bool result = 0;

    // Allocate a buffer for the JSON string
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // Parse the JSON string
    root = cJSON_Parse(json_str);
    free(json_str);

    // Test with NULL input for all functions
    result = cJSON_IsTrue(NULL);
    result = cJSON_IsRaw(NULL);
    result = cJSON_IsNull(NULL);
    result = cJSON_IsFalse(NULL);
    result = cJSON_IsInvalid(NULL);
    result = cJSON_IsBool(NULL);

    if (root != NULL) {
        // Traverse the JSON tree and test each node
        cJSON *current = root;
        while (current != NULL) {
            // Test all type check functions on the current node
            result = cJSON_IsTrue(current);
            result = cJSON_IsRaw(current);
            result = cJSON_IsNull(current);
            result = cJSON_IsFalse(current);
            result = cJSON_IsInvalid(current);
            result = cJSON_IsBool(current);

            // Move to child if exists, otherwise next
            if (current->child != NULL) {
                current = current->child;
            } else if (current->next != NULL) {
                current = current->next;
            } else {
                // Walk up the tree to find next sibling
                cJSON *parent = current;
                while (parent != NULL) {
                    if (parent->next != NULL) {
                        current = parent->next;
                        break;
                    }
                    if (parent == root) {
                        current = NULL;
                        break;
                    }
                    parent = parent->prev;
                }
            }
        }

        // Cleanup parsed JSON
        cJSON_Delete(root);
    }

    // Additional test: Manually construct a cJSON item to test edge cases
    item = cJSON_CreateNull();
    if (item != NULL) {
        result = cJSON_IsTrue(item);
        result = cJSON_IsRaw(item);
        result = cJSON_IsNull(item);
        result = cJSON_IsFalse(item);
        result = cJSON_IsInvalid(item);
        result = cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateTrue();
    if (item != NULL) {
        result = cJSON_IsTrue(item);
        result = cJSON_IsRaw(item);
        result = cJSON_IsNull(item);
        result = cJSON_IsFalse(item);
        result = cJSON_IsInvalid(item);
        result = cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateFalse();
    if (item != NULL) {
        result = cJSON_IsTrue(item);
        result = cJSON_IsRaw(item);
        result = cJSON_IsNull(item);
        result = cJSON_IsFalse(item);
        result = cJSON_IsInvalid(item);
        result = cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateString("test");
    if (item != NULL) {
        result = cJSON_IsTrue(item);
        result = cJSON_IsRaw(item);
        result = cJSON_IsNull(item);
        result = cJSON_IsFalse(item);
        result = cJSON_IsInvalid(item);
        result = cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateNumber(42);
    if (item != NULL) {
        result = cJSON_IsTrue(item);
        result = cJSON_IsRaw(item);
        result = cJSON_IsNull(item);
        result = cJSON_IsFalse(item);
        result = cJSON_IsInvalid(item);
        result = cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateRaw("raw");
    if (item != NULL) {
        result = cJSON_IsTrue(item);
        result = cJSON_IsRaw(item);
        result = cJSON_IsNull(item);
        result = cJSON_IsFalse(item);
        result = cJSON_IsInvalid(item);
        result = cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    // Manually create an invalid item by modifying type
    item = cJSON_CreateString("invalid_test");
    if (item != NULL) {
        item->type = cJSON_Invalid;
        result = cJSON_IsTrue(item);
        result = cJSON_IsRaw(item);
        result = cJSON_IsNull(item);
        result = cJSON_IsFalse(item);
        result = cJSON_IsInvalid(item);
        result = cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    return 0;
}