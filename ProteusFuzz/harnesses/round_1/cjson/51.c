#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/* Helper to create a deeply nested array to test depth limits */
static cJSON *create_deeply_nested_array(size_t depth) {
    cJSON *root = cJSON_CreateArray();
    if (!root) return NULL;

    cJSON *current = root;
    cJSON *next = NULL;

    for (size_t i = 0; i < depth; i++) {
        next = cJSON_CreateArray();
        if (!next) {
            /* Clean up on failure */
            cJSON_Delete(root);
            return NULL;
        }
        /* Add the new array as a child of the current one */
        cJSON_AddItemToArray(current, next);
        current = next;
    }
    return root;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    cJSON *item = NULL;
    cJSON *duplicate = NULL;
    cJSON_bool recurse = 1;

    if (Size == 0) {
        /* Test NULL input path (Line 2797) */
        duplicate = cJSON_Duplicate(NULL, recurse);
        if (duplicate != NULL) {
            cJSON_Delete(duplicate);
        }
        return 0;
    }

    /* Use input data to control behavior */
    uint8_t mode = Data[0] % 4;

    if (mode == 0) {
        /* Case 0: Test with a simple Object (triggers string duplication lines 2814, 2821-2822) */
        item = cJSON_CreateObject();
        if (!item) return 0;

        cJSON_AddStringToObject(item, "key1", "value1");
        cJSON_AddNumberToObject(item, "key2", 12345);
        
        /* Add a string value item to trigger valuestring duplication (Line 2814) */
        cJSON_AddStringToObject(item, "key3", "value3");

        duplicate = cJSON_Duplicate(item, recurse);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);

    } else if (mode == 1) {
        /* Case 1: Test with an Array containing items (triggers child processing, Line 2859) */
        item = cJSON_CreateArray();
        if (!item) return 0;

        cJSON_AddItemToArray(item, cJSON_CreateString("test"));
        cJSON_AddItemToArray(item, cJSON_CreateNumber(99));
        cJSON_AddItemToArray(item, cJSON_CreateObject());

        duplicate = cJSON_Duplicate(item, recurse);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);

    } else if (mode == 2) {
        /* Case 2: Test deep nesting to trigger depth limit (Line 2836) and failure paths (2840, 2867) */
        /* Use input to determine depth, cap it to avoid infinite loops but try to hit limit */
        size_t depth = 0;
        if (Size > 1) {
            depth = (size_t)(Data[1]) * 100; 
            if (depth < 100) depth = 100; /* Minimum depth to be interesting */
            if (depth > 1500) depth = 1500; /* Cap to prevent OOM before depth limit */
        } else {
            depth = 1000;
        }

        item = create_deeply_nested_array(depth);
        if (item) {
            duplicate = cJSON_Duplicate(item, recurse);
            if (duplicate) {
                cJSON_Delete(duplicate);
            }
            cJSON_Delete(item);
        }

    } else {
        /* Case 3: Mixed complex structure */
        item = cJSON_CreateObject();
        if (!item) return 0;

        cJSON *arr = cJSON_CreateArray();
        if (arr) {
            cJSON_AddItemToArray(arr, cJSON_CreateString("nested"));
            cJSON_AddItemToArray(arr, cJSON_CreateNumber(42));
            cJSON_AddItemToObject(item, "array_key", arr);
        }

        cJSON_AddStringToObject(item, "string_key", "test_value");

        duplicate = cJSON_Duplicate(item, recurse);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);
    }

    return 0;
}