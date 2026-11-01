// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for cJSON_Parse
    // Allocate a buffer with an extra byte for the null terminator
    char *buffer = (char *)malloc(Size + 1);
    if (!buffer) {
        return 0;
    }

    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    // Attempt to parse the input data as JSON
    cJSON *root = cJSON_Parse(buffer);
    free(buffer);

    // Test target functions on the parsed root
    cJSON_IsNull(root);
    cJSON_IsFalse(root);
    cJSON_IsArray(root);
    cJSON_IsInvalid(root);
    cJSON_IsTrue(root);
    cJSON_IsBool(root);

    // Test on NULL pointer explicitly
    cJSON_IsNull(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsArray(NULL);
    cJSON_IsInvalid(NULL);
    cJSON_IsTrue(NULL);
    cJSON_IsBool(NULL);

    // If parsing succeeded, traverse children to test more states
    if (root) {
        cJSON *current = root->child;
        while (current) {
            cJSON_IsNull(current);
            cJSON_IsFalse(current);
            cJSON_IsArray(current);
            cJSON_IsInvalid(current);
            cJSON_IsTrue(current);
            cJSON_IsBool(current);

            // Recursively test children if it's an object or array
            if (current->child) {
                cJSON *child = current->child;
                while (child) {
                    cJSON_IsNull(child);
                    cJSON_IsFalse(child);
                    cJSON_IsArray(child);
                    cJSON_IsInvalid(child);
                    cJSON_IsTrue(child);
                    cJSON_IsBool(child);
                    child = child->next;
                }
            }
            current = current->next;
        }
    }

    // Cleanup
    cleanup_json(root);

    return 0;
}