// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_json_tree(cJSON *item) {
    if (item == NULL) {
        return;
    }
    cJSON_Delete(item);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a base array to append items to
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Use input data to drive logic diversity
    uint8_t op = Data[0] % 5;

    cJSON *item = NULL;

    switch (op) {
        case 0:
            item = cJSON_CreateNull();
            break;
        case 1:
            item = cJSON_CreateFalse();
            break;
        case 2:
            item = cJSON_CreateTrue();
            break;
        case 3:
            // Create another array as a nested item
            item = cJSON_CreateArray();
            break;
        case 4:
            // Create a null item specifically to test type
            item = cJSON_CreateNull();
            break;
    }

    if (item != NULL) {
        // Attempt to add the item to the array
        cJSON_AddItemToArray(array, item);
    }

    // Test cJSON_free directly on a newly created item to ensure it handles standalone nodes
    cJSON *free_test = cJSON_CreateTrue();
    if (free_test != NULL) {
        cJSON_free(free_test);
    }

    // Cleanup the main array and its children
    cleanup_json_tree(array);

    return 0;
}