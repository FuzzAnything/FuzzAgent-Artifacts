// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static cJSON *create_random_item(const uint8_t *data, size_t size) {
    if (size < 1) {
        return cJSON_CreateNull();
    }

    uint8_t type_byte = data[0] % 10;
    cJSON *item = NULL;

    switch (type_byte) {
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
            item = cJSON_CreateNumber(0.0);
            if (size >= 4) {
                int32_t val;
                // Ensure we have enough bytes to read safely
                if (size >= 1 + sizeof(int32_t)) {
                    memcpy(&val, data + 1, sizeof(int32_t));
                    cJSON_SetNumberValue(item, (double)val);
                }
            }
            break;
        case 4:
            item = cJSON_CreateString("fuzz_string");
            break;
        case 5:
            item = cJSON_CreateArray();
            break;
        case 6:
            item = cJSON_CreateObject();
            break;
        default:
            item = cJSON_CreateNull();
            break;
    }

    return item;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a root array to manipulate
    cJSON *root_array = cJSON_CreateArray();
    if (!root_array) {
        return 0;
    }

    // Populate array with random items to test array operations
    // Limit the number of items to prevent excessive memory usage or stack overflow
    size_t num_items = (Size > 100) ? 100 : Size;
    for (size_t i = 0; i < num_items; i++) {
        cJSON *item = create_random_item(Data + i, Size - i);
        if (item) {
            cJSON_AddItemToArray(root_array, item);
        }
    }

    // Test 1: cJSON_GetArraySize
    // Safe to call on valid array, NULL, or empty array
    int size1 = cJSON_GetArraySize(root_array);
    (void)size1; // Use result to avoid unused warning

    // Test 2: cJSON_GetArrayItem
    // Test with various indices derived from input
    if (Size >= 4) {
        int32_t index_val;
        memcpy(&index_val, Data, sizeof(int32_t));
        cJSON *item = cJSON_GetArrayItem(root_array, index_val);
        if (item) {
            // Item retrieved, no immediate action needed, just traversal
        }
    }

    // Test 3: cJSON_InsertItemInArray
    // Create a new item to insert
    cJSON *new_item = create_random_item(Data, Size);
    if (new_item) {
        int32_t insert_idx = 0;
        if (Size >= 4) {
            memcpy(&insert_idx, Data, sizeof(int32_t));
            cJSON_InsertItemInArray(root_array, insert_idx, new_item);
        } else {
            cJSON_InsertItemInArray(root_array, 0, new_item);
        }
    }

    // Test 4: cJSON_ReplaceItemInArray
    cJSON *replace_item = create_random_item(Data, Size);
    if (replace_item && Size >= 4) {
        int32_t replace_idx;
        memcpy(&replace_idx, Data, sizeof(int32_t));
        cJSON_ReplaceItemInArray(root_array, replace_idx, replace_item);
    }

    // Test 5: cJSON_AddItemReferenceToArray
    // Add a reference to an existing item in the array (if exists)
    if (cJSON_GetArraySize(root_array) > 0) {
        cJSON *first_item = cJSON_GetArrayItem(root_array, 0);
        if (first_item) {
            cJSON_AddItemReferenceToArray(root_array, first_item);
        }
    }

    // Test 6: cJSON_ReplaceItemViaPointer
    // Need a parent, an item within it, and a replacement
    if (cJSON_GetArraySize(root_array) > 0) {
        cJSON *target_item = cJSON_GetArrayItem(root_array, 0);
        cJSON *replacement_item = create_random_item(Data, Size);
        if (target_item && replacement_item) {
            cJSON_ReplaceItemViaPointer(root_array, target_item, replacement_item);
        }
    }

    // Cleanup
    cleanup_cjson(root_array);

    return 0;
}