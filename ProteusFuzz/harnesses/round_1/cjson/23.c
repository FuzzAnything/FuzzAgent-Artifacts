// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input has at least a null terminator if treated as string
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Split input into two parts for key and raw value if needed
    size_t split_idx = Size > 1 ? (Size / 2) : 1;
    char *key = (char *)malloc(split_idx + 1);
    char *raw_val = (char *)malloc(Size - split_idx + 1);

    if (!key || !raw_val) {
        free(input_str);
        free(key);
        free(raw_val);
        return 0;
    }

    memcpy(key, input_str, split_idx);
    key[split_idx] = '\0';
    memcpy(raw_val, input_str + split_idx, Size - split_idx);
    raw_val[Size - split_idx] = '\0';

    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        free(input_str);
        free(key);
        free(raw_val);
        return 0;
    }

    // Test 1: cJSON_CreateRaw
    cJSON *raw_item = cJSON_CreateRaw(raw_val);
    if (raw_item) {
        // Add to object
        cJSON_AddItemToObject(obj, "raw_key", raw_item);
        // Try to delete it immediately to test DeleteItemFromObject
        cJSON_DeleteItemFromObject(obj, "raw_key");
        // raw_item is now freed by DeleteItemFromObject, do not free again
    }

    // Test 2: cJSON_AddRawToObject
    cJSON *added_raw = cJSON_AddRawToObject(obj, key, raw_val);
    // added_raw is owned by obj, do not free

    // Test 3: cJSON_AddItemToObjectCS with a literal key
    cJSON *literal_item = cJSON_CreateRaw(raw_val);
    if (literal_item) {
        cJSON_AddItemToObjectCS(obj, "literal_key", literal_item);
        // literal_item is now owned by obj
    }

    // Test 4: cJSON_AddItemToObject with dynamic key
    cJSON *dynamic_item = cJSON_CreateRaw(raw_val);
    if (dynamic_item) {
        cJSON_AddItemToObject(obj, key, dynamic_item);
        // dynamic_item is now owned by obj
    }

    // Test 5: Delete an item that might not exist
    cJSON_DeleteItemFromObject(obj, "nonexistent_key");

    // Test 6: Delete an existing item
    cJSON_DeleteItemFromObject(obj, key);

    // Cleanup
    safe_cleanup(obj);
    free(input_str);
    free(key);
    free(raw_val);

    return 0;
}