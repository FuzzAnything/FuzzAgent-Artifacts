#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

/* Helper to create a deeply nested structure to trigger depth limits */
static cJSON* create_deep_structure(int depth) {
    cJSON *root = cJSON_CreateArray();
    if (!root) return NULL;

    cJSON *current = root;
    for (int i = 0; i < depth; i++) {
        cJSON *child = cJSON_CreateArray();
        if (!child) {
            cJSON_Delete(root);
            return NULL;
        }
        cJSON_AddItemToArray(current, child);
        current = child;
    }
    return root;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    cJSON *item = NULL;
    cJSON *duplicate = NULL;
    cJSON_bool recurse = (Size > 0) ? (Data[0] % 2) : 1;

    if (Size == 0) {
        /* Case 1: NULL input to trigger line 2797 */
        duplicate = cJSON_Duplicate(NULL, recurse);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        return 0;
    }

    /* Use input data to determine structure type and depth */
    int mode = Data[0] % 4;
    int depth = (Size > 1) ? (Data[1] % 10) + 1 : 1;

    if (mode == 0) {
        /* Case 2: Deep nested structure to trigger line 2836 (depth limit) and 2840 */
        /* We create a structure deeper than CJSON_CIRCULAR_LIMIT (usually 1000) if possible,
           or at least deep enough to exercise the recursion logic. */
        int target_depth = (Size > 2) ? (Data[2] % 2000) : 100; 
        if (target_depth > 2000) target_depth = 2000; // Cap to prevent OOM in driver setup
        
        item = create_deep_structure(target_depth);
        if (!item) return 0;

        /* Attempt duplicate with recurse=true to hit depth check */
        duplicate = cJSON_Duplicate(item, 1);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);

    } else if (mode == 1) {
        /* Case 3: Object with strings to trigger string duplication paths (2814, 2822) */
        item = cJSON_CreateObject();
        if (!item) return 0;

        cJSON_AddStringToObject(item, "key1", "value1");
        cJSON_AddStringToObject(item, "key2", "value2");
        
        /* Add a child to ensure line 2859 is hit on success */
        cJSON *child = cJSON_CreateNumber(123);
        if (child) {
            cJSON_AddItemToObject(item, "child", child);
        }

        duplicate = cJSON_Duplicate(item, 1);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);

    } else if (mode == 2) {
        /* Case 4: Array with many items to trigger child loop logic */
        item = cJSON_CreateArray();
        if (!item) return 0;

        for (int i = 0; i < 10; i++) {
            cJSON_AddItemToArray(item, cJSON_CreateNumber(i));
        }

        duplicate = cJSON_Duplicate(item, 1);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);

    } else {
        /* Case 5: Simple number (shallow) */
        item = cJSON_CreateNumber(42);
        if (!item) return 0;
        
        duplicate = cJSON_Duplicate(item, 0); // Non-recursive
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);
    }

    return 0;
}