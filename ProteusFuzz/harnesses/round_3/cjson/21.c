// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_cjson(cJSON **item) {
    if (*item != NULL) {
        cJSON_Delete(*item);
        *item = NULL;
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = NULL;
    cJSON *added_item = NULL;

    // Step 1: Create an object
    obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Step 2: Add a boolean true to the object
    // Use the fuzzed data as the key name, ensuring it's null-terminated
    char *key = (char *)malloc(Size + 1);
    if (key == NULL) {
        cleanup_cjson(&obj);
        return 0;
    }
    memcpy(key, Data, Size);
    key[Size] = '\0';

    added_item = cJSON_AddTrueToObject(obj, key);
    // Note: cJSON_AddTrueToObject handles its own cleanup on failure, 
    // so we don't need to manually delete added_item if it returns NULL.
    // If it returns non-NULL, the item is owned by 'obj', so we don't delete it separately.

    free(key);

    // Step 3: Delete the object (and all children)
    if (obj != NULL) {
        cJSON_Delete(obj);
        obj = NULL;
    }

    return 0;
}