// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *fuzz_parse_json(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_Parse("{}");
    }
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return cJSON_Parse("{}");
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    
    if (!item) {
        return cJSON_Parse("{}");
    }
    return item;
}

static void fuzz_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = fuzz_parse_json(Data, Size);
    if (!root) {
        return 0;
    }

    // Ensure we have a valid object or array to work with
    if (root->type & cJSON_IsReference) {
        fuzz_cleanup(root);
        return 0;
    }

    // Extract key string from input for object operations
    // Ensure we have enough space for the null terminator
    size_t key_len = (Size < 255) ? Size : 255;
    char key[256];
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Test 1: cJSON_DeleteItemFromArray
    // Create a temporary array if root is not an array
    cJSON *test_array = NULL;
    if (root->type & cJSON_Array) {
        test_array = root;
    } else {
        test_array = cJSON_CreateArray();
        if (test_array) {
            cJSON_AddItemToObject(root, "_fuzz_array", test_array);
        }
    }

    if (test_array) {
        int which = (Data[0] % 100) - 50; // Range: -50 to 49
        cJSON_DeleteItemFromArray(test_array, which);
    }

    // Test 2: cJSON_DetachItemFromObjectCaseSensitive
    cJSON *detached_cs = NULL;
    if (root->type & cJSON_Object) {
        detached_cs = cJSON_DetachItemFromObjectCaseSensitive(root, key);
        if (detached_cs) {
            cJSON_Delete(detached_cs);
        }
    }

    // Test 3: cJSON_DeleteItemFromObjectCaseSensitive
    if (root->type & cJSON_Object) {
        cJSON_DeleteItemFromObjectCaseSensitive(root, key);
    }

    // Test 4: cJSON_AddArrayToObject
    cJSON *new_array = NULL;
    if (root->type & cJSON_Object) {
        new_array = cJSON_AddArrayToObject(root, key);
        // If it returns NULL, it's handled internally, no crash expected
        if (new_array) {
            // Add some items to make it non-trivial
            cJSON_AddItemToArray(new_array, cJSON_CreateNumber(123));
            cJSON_AddItemToArray(new_array, cJSON_CreateString("test"));
        }
    }

    // Test 5: cJSON_DetachItemFromObject
    cJSON *detached = NULL;
    if (root->type & cJSON_Object) {
        detached = cJSON_DetachItemFromObject(root, key);
        if (detached) {
            cJSON_Delete(detached);
        }
    }

    // Test 6: cJSON_DeleteItemFromObject
    if (root->type & cJSON_Object) {
        cJSON_DeleteItemFromObject(root, key);
    }

    // Cleanup
    fuzz_cleanup(root);
    return 0;
}