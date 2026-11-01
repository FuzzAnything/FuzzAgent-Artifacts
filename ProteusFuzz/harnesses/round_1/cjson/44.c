// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for cJSON_Parse
    // Allocate a buffer with Size + 1 to hold the null terminator
    char *input_buffer = (char *)malloc(Size + 1);
    if (!input_buffer) {
        return 0;
    }

    memcpy(input_buffer, Data, Size);
    input_buffer[Size] = '\0';

    // Create a cJSON object from the input data
    cJSON *root = cJSON_Parse(input_buffer);

    // Test cJSON_IsRaw
    if (root) {
        cJSON_IsRaw(root);
        cJSON_IsRaw(NULL);
    }

    // Test cJSON_IsString
    if (root) {
        cJSON_IsString(root);
        cJSON_IsString(NULL);
    }

    // Test cJSON_IsNull
    if (root) {
        cJSON_IsNull(root);
        cJSON_IsNull(NULL);
    }

    // Test cJSON_IsNumber
    if (root) {
        cJSON_IsNumber(root);
        cJSON_IsNumber(NULL);
    }

    // Test cJSON_IsTrue
    if (root) {
        cJSON_IsTrue(root);
        cJSON_IsTrue(NULL);
    }

    // Test cJSON_Compare with various scenarios
    cJSON *root2 = NULL;
    if (root) {
        root2 = cJSON_Parse(input_buffer);
    }

    if (root && root2) {
        cJSON_Compare(root, root2, 1); // Case sensitive
        cJSON_Compare(root, root2, 0); // Case insensitive
        cJSON_Compare(root, root, 1);  // Same pointer
        cJSON_Compare(root, root, 0);  // Same pointer
        cJSON_Compare(root, NULL, 1);  // One NULL
        cJSON_Compare(NULL, root2, 0); // Other NULL
        cJSON_Compare(NULL, NULL, 1);  // Both NULL
    } else if (root) {
        cJSON_Compare(root, NULL, 1);
        cJSON_Compare(NULL, root, 0);
    } else if (root2) {
        cJSON_Compare(NULL, root2, 1);
        cJSON_Compare(root2, NULL, 0);
    }

    // Cleanup
    free(input_buffer);
    cleanup_cjson(root);
    cleanup_cjson(root2);

    return 0;
}